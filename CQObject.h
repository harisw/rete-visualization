#ifndef _CQOBJECT_H_
#define _CQOBJECT_H_

#include <vector>
#include <string>

using namespace std;

#define PI		3.141592
#define DTR(deg) ((double)(deg)*PI/180.0)
#define RTD(rad) ((double)(rad)*180.0/PI)

#define CQLEFT	0x0001
#define CQRIGHT	0x0002
#define CQUP	0x0004
#define CQDOWN	0x0008

typedef struct _POINT
{
	int x, y;
}POINT_GEN;

typedef struct _MBR
{
	int minx, miny;	// left, top
	int maxx, maxy;	// right, bottom
}MBR;

enum class PATHPATTERN
{
	Out_In_Out = 0,		// CQ영역 바깥에서 시작해서 CQ영역 안으로 들어온뒤 다시 바깥으로 나가는 경로 패턴
	Out_Out,			// CQ영역 바깥에서 바깥으로 이동하는 경로 패턴
	Out_In				// CQ영역 바깥에서 안쪽으로 이동하는 경로 패턴
};

enum class CURVETYPE {
	LINE = 0,
	BEZIER,
	CURVE
};
//================================================================================================
//
// 탐지대상 이동객체 클래스
//
//================================================================================================
class CQObject
{
private:
	vector<POINT_GEN> m_ptArray;		// 각 객체의 이동 경로 배열 ==> nTimeStamp와 같을 필요는 없다.
									// 속도가 빠르게 설정되면 개수가 적을테고 반대면 더 많게 설정됨.
	void saveData(FILE* fp);

public:
	CQObject() {}

	void copyBasePath(vector<POINT_GEN>& path);
	void generateObjectPath(vector<POINT_GEN>& path);

	friend class CQObjectSet;
};
//================================================================================================
//
// 탐지대상 이동객체 집합 클래스
//
//================================================================================================
class CQObjectSet
{
private:
	vector<POINT_GEN> m_selptArray;			// 경로 생성을 위한 기본 좌표 / Default coordinates for path generation
	vector<POINT_GEN> m_ptArray;			// 이동 객체의 기본 이동좌표 배열 / Arrange the default move coordinates for a move object
	vector<CQObject*>	m_objArray;		// 경로를 따라 이동할 객체 배열 --> 각각의 객체들은 경로를 따로 가진다. / Array of objects to be moved along a path --> Each object has a separate path.
	int m_nObjectCount;					// 객체 수

	void logging();

public:
	CQObjectSet(int objcount);
	~CQObjectSet();

	//fetch the generated waypoint based on the object
	vector<pair<int, int>> getRawPointArray();
	vector<pair<float, float>> getSingleObjectTrajectory(int obj_id);

	void generateObjectPath(int nTimeStamp, vector<POINT_GEN>& path, CURVETYPE ctype);
};
//========================================================================================
//
// 사용자가 경로 좌표를 직접 입력해서 객체 경로를 산출하는 경우
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
CQObjectSet* generateObjects(vector<POINT_GEN>& path, int objcount, int nTimeStamp, CURVETYPE ctype);
//========================================================================================
//
// CQ의 MBR과 객체의 경로 패턴을 지정해서 객체 경로를 산출하는 경우
//
// CQ영역의 MBR과 이동 객체의 패턴을 입력해서 자동으로 이동객체를 생성하고 경로를 설정해서 리턴한다.
//
// cqmbr	: CQ영역의 mbr
// objcount	: 이동객체 개수
// pp		: 경로 생성 패턴 (Out_In_Out, Out_Out, Out_In)
// startflag: 경로 시작지점 플래그
// endflag	: 경로 종료지점 플래그
//
CQObjectSet* generateObjects(MBR& cqmbr, int objcount, PATHPATTERN pp, int start_flag, int end_flag, int nTimeStamp, CURVETYPE ctype);
#endif