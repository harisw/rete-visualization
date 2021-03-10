#include "CQUtil.h"
#include "curve_bezier.h"
#include "curve_spline.h"

//==========================================================================================================

double CQUtil::distance(POINT_GEN pt1, POINT_GEN pt2)
{
	return sqrt(pow((double)pt2.x - (double)pt1.x, 2.0) + pow((double)pt2.y - (double)pt1.y, 2.0));
}

double CQUtil::distance(vector<POINT_GEN>& path)
{
	double dist = 0.0;
	int size = (int)path.size();
	for (int i = 0; i < size - 1; i++)
	{
		dist += distance(path[i], path[i + 1]);
	}
	return dist;
}
// sp-ep 라인을 100으로 해서 sp에서 경로를 따라서 percent만큼 떨어진 거리의 좌표를 리턴한다.
POINT_GEN CQUtil::getOffsetPoint(POINT_GEN sp, POINT_GEN ep, int percent)
{
	int dx = (int)((double)(ep.x - sp.x) * (double)percent / 100.0);
	int dy = (int)((double)(ep.y - sp.y) * (double)percent / 100.0);

	POINT_GEN ret = { sp.x + dx, sp.y + dy };
	return ret;
}
// path의 시작점에서 경로를 따라서 dist만큼 떨어진 거리의 좌표를 리턴한다.
POINT_GEN CQUtil::getOffsetPoint(vector<POINT_GEN>& path, double dist)
{
	POINT_GEN ret = { 0,0 };
	double d = 0.0;
	int size = (int)path.size();
	for (int i = 0; i < size - 1; i++)
	{
		d = distance(path[i], path[i + 1]);
		if (dist < d)
		{
			double rate = dist / d;
			double dx = path[i + 1].x - path[i].x;
			double dy = path[i + 1].y - path[i].y;
			double tdx = dx * rate;
			double tdy = dy * rate;

			ret = { path[i].x + (int)tdx, path[i].y + (int)tdy };
			return ret;
		}
		dist -= d;
	}
	if (-0.000001 < dist && dist < 0.000001)
		return path.back();
	return ret;
}
// i와 j사이의 정수 랜덤값을 리턴한다.
long CQUtil::getRandomLong(long i, long j)
{
	int min = i;
	int max = j;
	if (i == j)
	{
		min = i;
		max = min + 1;
	}
	else if (j < i)
	{
		min = j;
		max = i;
	}
	int d = (max - min) + 1;	// 4 = (5 - 2) + 1
	int r = rand() % d;			// 0, 1, 2, 3
	return (min + r);			// 2, 3, 4, 5
}
// 점 pt에서 r만큼 떨어져서 a만큼 회전한 점의 좌표를 리턴. 3시가 0도, 반시계방향으로 증가
POINT_GEN CQUtil::getPoint(POINT_GEN& pt, double radius, double angle)
{
	double rad = DTR(angle);
	int x = (int)(radius * cos(rad));
	int y = (int)(radius * sin(rad));

	POINT_GEN ret = { pt.x + x, pt.y + y };
	return ret;
}
//==========================================================================================================
// path는 입력된 2개 이상읜 기준좌표이고 이 좌표들로 직선을 그려서 nTimeStamp로 분할한 좌표를
// ptArray에 담아서 리턴한다.
void CQUtil::generateLinePath(vector<POINT_GEN>& ptArray, int nTimeStamp, vector<POINT_GEN>& path)
{
	int size = (int)path.size();
	// 이동 경로점 배열 path보다 nTimeStamp가 작다면 그냥 path를 설정하고 리턴
	if (size <= 1 || nTimeStamp <= size)
	{
		ptArray = path;
		return;
	}
	double totdist = distance(path);
	double offdist = totdist / (double)(nTimeStamp - 1);

	ptArray.push_back(path.front());
	for (int i = 1; i < nTimeStamp - 1; i++)
	{
		double dist = offdist * (double)i;
		ptArray.push_back(getOffsetPoint(path, dist));
	}
	ptArray.push_back(path.back());
}
// path는 입력된 3개 이상의 기준좌표이고 이 좌표들로 베지어곡선을 그려서 nTimeStamp로 분할한 좌표를
// ptArray에 담아서 리턴한다.
void CQUtil::generateBezierCurvePath(vector<POINT_GEN>& ptArray, int nTimeStamp, vector<POINT_GEN>& path)
{
	BezierCurve	bc;
	ptArray.resize(nTimeStamp);
	bc.Bezier2D(path, nTimeStamp, ptArray);
}
void CQUtil::generateSplineCurvePath(vector<POINT_GEN>& ptArray, int nTimeStamp, vector<POINT_GEN>& path)
{
	//vector<POINT_GEN, POINT_GEN> pts;
	vector<POINT_GEN> pts;
	int size = (int)path.size();
	if (size < 2)	return;
	for (auto t : path)
		pts.push_back(t);
	
	Spline sp(pts, size);
	sp.Generate();

	ptArray.clear();
	ptArray.resize(nTimeStamp);

	int cnt = 0;
	POINT_GEN* tempPTArray = new POINT_GEN[nTimeStamp];
	sp.GetCurve(tempPTArray, cnt, nTimeStamp);
	copy(tempPTArray, tempPTArray + nTimeStamp, ptArray.begin());
	
}

// 기준경로를 따라 저장된 객체의 좌표를 랜덤 경로가 되도록 좌표를 재생성한다.
void CQUtil::generateRandomPath(vector<POINT_GEN>& ptArray)
{
	size_t size = ptArray.size();
	if (size < 2)	return;

	vector<POINT_GEN> tArray;
	// 기준 경로가 일정한 간격으로 생성되어 있기 때문에 이를 불규칙 간격이 되도록 변경한다.
	// 이때 시작점과 끝점을 그냥 둔다.
	for (size_t i = 1; i < size - 1; i++)
	{
		//ptArray[i-1]에서 ptArray[i+1]까지를 총 200등분한 뒤 1~199사이의 랜덤값을 생성해서
		// 1~99이면 ptArray[i-1]~pt[i]까지 100등분한 위치
		// 101~199면 pt[i]에서 ptArray[i+1]까지 100등분한 위치를 설정한다.

		int t = getRandomLong(1, 199);
		if (t < 100)		getOffsetPoint(ptArray[i - 1], ptArray[i], t);
		else if (100 < t)	getOffsetPoint(ptArray[i], ptArray[i+1], t-100);
		else				tArray.push_back(ptArray[i]);
	}

	vector<POINT_GEN> tArray1;
	POINT_GEN pt;
	double dist, radius, angle;
	for (size_t i = 0; i < size; i++)
	{
		pt = ptArray[i];

		// 현재(i) 점에서 다음(i+1) 점까지의 거리의 2/3를 반지름으로 원을 그리고
		// 그 원 안에 속하는 임의의 점을 현재(i) 점의 새로운 점으로 선정
		if (i < size - 1)
		{
			dist = distance(pt, ptArray[i + 1]);
			radius = (double)getRandomLong(0L, (long)dist);	// 원의 반지름값을 랜덤으로 선택
			angle = getRandomLong(0, 359);					// 방위각을 랜덤으로 선택
		}
		tArray1.push_back(getPoint(pt, radius, angle));
	}
	ptArray = tArray1;
}
//==========================================================================================================
