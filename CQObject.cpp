#include "CQObject.h"
#include "CQUtil.h"

//================================================================================================
//
// Ž����� �̵���ü Ŭ����
//
//================================================================================================
// ���� ��θ� �̵� ��η� ������ �ش�.
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

// CQObjectSet���� �������ִ� �⺻ ��θ� ���� ��ü�� �̵���θ� �����Ѵ�.
// ������ �ӵ� Ÿ�Կ� ���� ���� ������ �ٸ��� �����ȴ�. �ִ� nTimeStamp��ŭ ����
// nTimeStamp�� ���ذ��(path)�� �� ������ ������.
void CQObject::generateObjectPath(vector<POINT_GEN>& path)
{
	m_ptArray = path;
	CQUtil::generateRandomPath(m_ptArray);
}
//==============================================================================================
//
// �̵� ��ü ���� Ŭ����
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
// ������ ��� ��¿� ����ϴ� �Լ� ==> PathTest���� �׷��� �� ����
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
// ���� ��θ� ���� ��ü���� ��ǥ�� �������ش�.
//
void CQObjectSet::generateObjectPath(int nTimeStamp, vector<POINT_GEN>& path, CURVETYPE ctype)
{
	// ����ڰ� �Է��� ����� ��ü���� �̵� ���� ��θ� �����Ѵ�.(m_ptArray)
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
// CQ������ MBR�� �̵� ��ü�� �̵� �⺻ ��ǥ(2, 3��)�� �Է��ؼ� �ڵ����� �̵���ü�� �����ϰ� ��θ� �����ؼ� �����Ѵ�.
//
// objcount		: �̵���ü ����
// nTimeStamp	: ��λ��� ��ü�� �̵� ���� ������ �����Ѵ�. (100�� �ԷµǸ� ��ü ��λ� 100���� ������ �����Ѵ�.)
// strCurveType	: ��� ������ ���� Ÿ��
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
// CQ������ MBR�� �̵� ��ü�� ������ �Է��ؼ� �ڵ����� �̵���ü�� �����ϰ� ��θ� �����ؼ� �����Ѵ�.
//
// cqmbr		: CQ������ mbr
// objcount		: �̵���ü ����
// pp			: ��� ���� ���� (Out_In_Out, Out_Out, Out_In)
// startflag	: ��� �������� �÷���
// endflag		: ��� �������� �÷���
// nTimeStamp	: ��λ��� ��ü�� �̵� ���� ������ �����Ѵ�. (100�� �ԷµǸ� ��ü ��λ� 100���� ������ �����Ѵ�.)
//
CQObjectSet* generateObjects(MBR& cqmbr, int objcount, PATHPATTERN pp, int start_flag, int end_flag, int nTimeStamp, CURVETYPE ctype)
{
	// �־��� �������� ��� �����͸� �ڵ����� �����Ѵ�.
	POINT_GEN sp;	// ��� ������
	POINT_GEN via;	// ��� ������
	POINT_GEN ep;	// ��� ������
	//========================================================================================
	//
	// ��ϵ� CQ�� ������ y�����δ� 100, x�����δ� 200�� �������� �������� �����Ѵ�.
	//
	//-----------------------------------------------------------------------------------
	// Start ���� (�ϴ� out���� �����ϵ��� ����)
	if (start_flag & CQLEFT)	sp.x = cqmbr.minx - 200;
	else						sp.x = cqmbr.maxx + 200;
	if (start_flag & CQDOWN)	sp.y = cqmbr.maxy + 100;
	else						sp.y = cqmbr.miny - 100;

	// �������� 50 �ȼ� �������� �����Ѵ�.
	sp.x += (rand() % 101 - 50);
	sp.y += (rand() % 101 - 50);
	//-----------------------------------------------------------------------------------
	// End ����
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

	// ������ 50 �ȼ� �������� �����Ѵ�.
	ep.x += (rand() % 101 - 50);
	ep.y += (rand() % 101 - 50);
	//-----------------------------------------------------------------------------------
	// Via ����
	if (pp == PATHPATTERN::Out_In_Out)
	{
		via.x = (cqmbr.minx + cqmbr.maxx) / 2;
		via.y = (cqmbr.miny + cqmbr.maxy) / 2;
	}
	else
	{
		// �⺻������ sp-ep�� �߰������� �����Ѵ�.
		via.x = (ep.x + sp.x) / 2;
		via.y = (ep.y + sp.y) / 2;

		if (pp == PATHPATTERN::Out_In)
		{
			// ����� �߰���ġ�� 50~100 �ʼ� �Ʒ��� �ֵ��� �����Ѵ�. (������ �ȵǵ���)
			via.y += (rand() % 51 + 50);
		}
		else
		{
			// �߾����� ���ϵ��� 50~100 �ȼ� �������� �ֵ��� �����Ѵ�. (������ �ȵǵ���)
			// CQ���� �����̸�
			if (cqmbr.minx < via.x)	via.x += (rand() % 51 + 50);
			else					via.x -= (rand() % 51 + 50);

			// CQ���� �����̸�
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
