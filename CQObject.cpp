#include "CQObject.h"
#include "CQUtil.h"

//================================================================================================
//
// 탐지대상 이동객체 클래스
//
//================================================================================================
// 기준 경로를 이동 경로로 복사해 준다.
void CQObject::copyBasePath(vector<POINT_GEN>& path)
{
	m_ptArray = path;
	return;
}

void CQObject::saveData(FILE* fp)
{
	fprintf(fp, "%d\n", (int)m_ptArray.size());
	for (auto pt : m_ptArray)
	{
		fprintf(fp, "%d,%d\n", pt.x, pt.y);
	}
}

// CQObjectSet에서 제공해주는 기본 경로를 따라 객체의 이동경로를 생성한다.
// 설정된 속도 타입에 따라 점의 개수는 다르게 설정된다. 최대 nTimeStamp만큼 생성
// nTimeStamp는 기준경로(path)의 점 개수와 동일함.
void CQObject::generateObjectPath(vector<POINT_GEN>& path)
{
	m_ptArray = path;
	CQUtil::generateRandomPath(m_ptArray);
}
//==============================================================================================
//
// 이동 객체 집합 클래스
//
//==============================================================================================
CQObjectSet::CQObjectSet(int objcount)
	: m_nObjectCount(objcount)
{
}
CQObjectSet::~CQObjectSet()
{
	int size = (int)m_objArray.size();
	for (int i = 0; i < size; i++)
		delete m_objArray[i];
	m_objArray.clear();
}
// 디버깅용 경로 출력에 사용하는 함수 ==> PathTest에서 그려볼 수 있음
void CQObjectSet::logging()
{
	FILE* fp = NULL;
	errno_t err = fopen_s(&fp, "objectset.txt", "wt");
	if (!fp)	return;
	for (auto obj : m_objArray)
		obj->saveData(fp);
	fclose(fp);
}
vector<pair<int, int>> CQObjectSet::getRawPointArray()
{
	vector<pair<int, int>> res;

	for (auto pt : m_ptArray) {
		res.push_back( {pt.x, pt.y} );
	}

	return res;
}
vector<pair<float, float>> CQObjectSet::getSingleObjectTrajectory(int obj_id)
{
	vector<pair<float, float>> result;
	for (auto arr : m_objArray[obj_id]->m_ptArray) {
		result.push_back({arr.x, arr.y});
	}

	return result;
}
//========================================================================================================================
//
// 기준 경로를 따라 객체별로 좌표를 설정해준다.
//
void CQObjectSet::generateObjectPath(int nTimeStamp, vector<POINT_GEN>& path, CURVETYPE ctype)
{
	// 사용자가 입력한 점들로 개체들의 이동 기준 경로를 설정한다.(m_ptArray)
	int size = (int)path.size();
	if (size < 0)	return;

	m_ptArray.clear();
	if (size == 1)
	{
		m_ptArray.push_back(path[0]);
	}
	else if (size == 2 || (2 < size && ctype == CURVETYPE::LINE))
	{
		CQUtil::generateLinePath(m_ptArray, nTimeStamp, path);
	}
	else
	{
		if (ctype == CURVETYPE::BEZIER)	CQUtil::generateBezierCurvePath(m_ptArray, nTimeStamp, path);
		else							CQUtil::generateSplineCurvePath(m_ptArray, nTimeStamp, path);
	}

	for (int j = 0; j < m_nObjectCount; j++)
	{
		CQObject* pObj = new CQObject;
		pObj->generateObjectPath(m_ptArray);
		m_objArray.push_back(pObj);
	}
	logging();
}
//==========================================================================================================================
//
// CQ영역의 MBR과 이동 객체의 이동 기본 좌표(2, 3개)를 입력해서 자동으로 이동객체를 생성하고 경로를 설정해서 리턴한다.
//
// objcount		: 이동객체 개수
// nTimeStamp	: 경로상의 객체의 이동 정점 개수를 지정한다. (100이 입력되면 전체 경로상에 100개의 정점을 생성한다.)
// strCurveType	: 경로 생성시 생성 타입
//				enum CURVETYPE {
//					LINE = 0,
//					BEZIER,
//					CURVE
//				};
//
CQObjectSet* generateObjects(vector<POINT_GEN>& path, int objcount, int nTimeStamp, CURVETYPE ctype)
{
	CQObjectSet * ret = new CQObjectSet(objcount);

	ret->generateObjectPath(nTimeStamp, path, ctype);

	return ret;
}
//==========================================================================================================================
//
// CQ영역의 MBR과 이동 객체의 패턴을 입력해서 자동으로 이동객체를 생성하고 경로를 설정해서 리턴한다.
//
// cqmbr		: CQ영역의 mbr
// objcount		: 이동객체 개수
// pp			: 경로 생성 패턴 (Out_In_Out, Out_Out, Out_In)
// startflag	: 경로 시작지점 플래그
// endflag		: 경로 종료지점 플래그
// nTimeStamp	: 경로상의 객체의 이동 정점 개수를 지정한다. (100이 입력되면 전체 경로상에 100개의 정점을 생성한다.)
//
CQObjectSet* generateObjects(MBR& cqmbr, int objcount, PATHPATTERN pp, int start_flag, int end_flag, int nTimeStamp, CURVETYPE ctype)
{
	// 주어진 조건으로 경로 데이터를 자동으로 생성한다.
	POINT_GEN sp;	// 경로 시작점
	POINT_GEN via;	// 경로 경유점
	POINT_GEN ep;	// 경로 종료점
	//========================================================================================
	//
	// 등록된 CQ가 있으면 y축으로는 100, x축으로는 200의 여유값을 가지도록 설정한다.
	//
	//-----------------------------------------------------------------------------------
	// Start 설정 (일단 out에서 시작하도록 설정)
	if (start_flag & CQLEFT)	sp.x = cqmbr.minx - 200;
	else						sp.x = cqmbr.maxx + 200;
	if (start_flag & CQDOWN)	sp.y = cqmbr.maxy + 100;
	else						sp.y = cqmbr.miny - 100;

	// 시작점을 50 픽셀 랜덤으로 변경한다.
	sp.x += (rand() % 101 - 50);
	sp.y += (rand() % 101 - 50);
	//-----------------------------------------------------------------------------------
	// End 설정
	if (pp == PATHPATTERN::Out_In)
	{
		ep.x = (cqmbr.minx + cqmbr.maxx) / 2;
		ep.y = (cqmbr.miny + cqmbr.maxy) / 2;
	}
	else
	{
		if (end_flag & CQLEFT)	ep.x = cqmbr.minx - 200;
		else					ep.x = cqmbr.maxx + 200;
		if (end_flag & CQDOWN)	ep.y = cqmbr.maxy + 100;
		else					ep.y = cqmbr.miny - 100;
	}

	// 끝점을 50 픽셀 랜덤으로 변경한다.
	ep.x += (rand() % 101 - 50);
	ep.y += (rand() % 101 - 50);
	//-----------------------------------------------------------------------------------
	// Via 설정
	if (pp == PATHPATTERN::Out_In_Out)
	{
		via.x = (cqmbr.minx + cqmbr.maxx) / 2;
		via.y = (cqmbr.miny + cqmbr.maxy) / 2;
	}
	else
	{
		// 기본적으로 sp-ep의 중간점으로 설정한다.
		via.x = (ep.x + sp.x) / 2;
		via.y = (ep.y + sp.y) / 2;

		if (pp == PATHPATTERN::Out_In)
		{
			// 경로의 중간위치를 50~100 필셀 아래로 휘도록 변경한다. (직선이 안되도록)
			via.y += (rand() % 51 + 50);
		}
		else
		{
			// 중앙으로 향하도록 50~100 픽셀 안쪽으로 휘도록 변경한다. (직선이 안되도록)
			// CQ보다 왼쪽이면
			if (cqmbr.minx < via.x)	via.x += (rand() % 51 + 50);
			else					via.x -= (rand() % 51 + 50);

			// CQ보다 위쪽이면
			if (via.y < cqmbr.miny)	via.y += (rand() % 51 + 50);
			else					via.y -= (rand() % 51 + 50);
		}
	}

	vector<POINT_GEN> path;
	path.push_back(sp);
	path.push_back(via);
	path.push_back(ep);

	return generateObjects(path, objcount, nTimeStamp, ctype);
}
