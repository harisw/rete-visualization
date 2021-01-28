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
	Out_In_Out = 0,		// CQ���� �ٱ����� �����ؼ� CQ���� ������ ���µ� �ٽ� �ٱ����� ������ ��� ����
	Out_Out,			// CQ���� �ٱ����� �ٱ����� �̵��ϴ� ��� ����
	Out_In				// CQ���� �ٱ����� �������� �̵��ϴ� ��� ����
};

enum class CURVETYPE {
	LINE = 0,
	BEZIER,
	CURVE
};
//================================================================================================
//
// Ž����� �̵���ü Ŭ����
//
//================================================================================================
class CQObject
{
private:
	vector<POINT_GEN> m_ptArray;		// �� ��ü�� �̵� ��� �迭 ==> nTimeStamp�� ���� �ʿ�� ����.
									// �ӵ��� ������ �����Ǹ� ������ �����װ� �ݴ�� �� ���� ������.
	void saveData(FILE* fp);

public:
	CQObject() {}

	void copyBasePath(vector<POINT_GEN>& path);
	void generateObjectPath(vector<POINT_GEN>& path);

	friend class CQObjectSet;
};
//================================================================================================
//
// Ž����� �̵���ü ���� Ŭ����
//
//================================================================================================
class CQObjectSet
{
private:
	vector<POINT_GEN> m_selptArray;			// ��� ������ ���� �⺻ ��ǥ / Default coordinates for path generation
	vector<POINT_GEN> m_ptArray;			// �̵� ��ü�� �⺻ �̵���ǥ �迭 / Arrange the default move coordinates for a move object
	vector<CQObject*>	m_objArray;		// ��θ� ���� �̵��� ��ü �迭 --> ������ ��ü���� ��θ� ���� ������. / Array of objects to be moved along a path --> Each object has a separate path.
	int m_nObjectCount;					// ��ü ��

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
// ����ڰ� ��� ��ǥ�� ���� �Է��ؼ� ��ü ��θ� �����ϴ� ���
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
CQObjectSet* generateObjects(vector<POINT_GEN>& path, int objcount, int nTimeStamp, CURVETYPE ctype);
//========================================================================================
//
// CQ�� MBR�� ��ü�� ��� ������ �����ؼ� ��ü ��θ� �����ϴ� ���
//
// CQ������ MBR�� �̵� ��ü�� ������ �Է��ؼ� �ڵ����� �̵���ü�� �����ϰ� ��θ� �����ؼ� �����Ѵ�.
//
// cqmbr	: CQ������ mbr
// objcount	: �̵���ü ����
// pp		: ��� ���� ���� (Out_In_Out, Out_Out, Out_In)
// startflag: ��� �������� �÷���
// endflag	: ��� �������� �÷���
//
CQObjectSet* generateObjects(MBR& cqmbr, int objcount, PATHPATTERN pp, int start_flag, int end_flag, int nTimeStamp, CURVETYPE ctype);
#endif