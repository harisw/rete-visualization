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
// sp-ep ������ 100���� �ؼ� sp���� ��θ� ���� percent��ŭ ������ �Ÿ��� ��ǥ�� �����Ѵ�.
POINT_GEN CQUtil::getOffsetPoint(POINT_GEN sp, POINT_GEN ep, int percent)
{
	int dx = (int)((double)(ep.x - sp.x) * (double)percent / 100.0);
	int dy = (int)((double)(ep.y - sp.y) * (double)percent / 100.0);

	POINT_GEN ret = { sp.x + dx, sp.y + dy };
	return ret;
}
// path�� ���������� ��θ� ���� dist��ŭ ������ �Ÿ��� ��ǥ�� �����Ѵ�.
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
// i�� j������ ���� �������� �����Ѵ�.
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
// �� pt���� r��ŭ �������� a��ŭ ȸ���� ���� ��ǥ�� ����. 3�ð� 0��, �ݽð�������� ����
POINT_GEN CQUtil::getPoint(POINT_GEN& pt, double radius, double angle)
{
	double rad = DTR(angle);
	int x = (int)(radius * cos(rad));
	int y = (int)(radius * sin(rad));

	POINT_GEN ret = { pt.x + x, pt.y + y };
	return ret;
}
//==========================================================================================================
// path�� �Էµ� 2�� �̻��� ������ǥ�̰� �� ��ǥ��� ������ �׷��� nTimeStamp�� ������ ��ǥ��
// ptArray�� ��Ƽ� �����Ѵ�.
void CQUtil::generateLinePath(vector<POINT_GEN>& ptArray, int nTimeStamp, vector<POINT_GEN>& path)
{
	int size = (int)path.size();
	// �̵� ����� �迭 path���� nTimeStamp�� �۴ٸ� �׳� path�� �����ϰ� ����
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
// path�� �Էµ� 3�� �̻��� ������ǥ�̰� �� ��ǥ��� �������� �׷��� nTimeStamp�� ������ ��ǥ��
// ptArray�� ��Ƽ� �����Ѵ�.
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

// ���ذ�θ� ���� ����� ��ü�� ��ǥ�� ���� ��ΰ� �ǵ��� ��ǥ�� ������Ѵ�.
void CQUtil::generateRandomPath(vector<POINT_GEN>& ptArray)
{
	size_t size = ptArray.size();
	if (size < 2)	return;

	vector<POINT_GEN> tArray;
	// ���� ��ΰ� ������ �������� �����Ǿ� �ֱ� ������ �̸� �ұ�Ģ ������ �ǵ��� �����Ѵ�.
	// �̶� �������� ������ �׳� �д�.
	for (size_t i = 1; i < size - 1; i++)
	{
		//ptArray[i-1]���� ptArray[i+1]������ �� 200����� �� 1~199������ �������� �����ؼ�
		// 1~99�̸� ptArray[i-1]~pt[i]���� 100����� ��ġ
		// 101~199�� pt[i]���� ptArray[i+1]���� 100����� ��ġ�� �����Ѵ�.

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

		// ����(i) ������ ����(i+1) �������� �Ÿ��� 2/3�� ���������� ���� �׸���
		// �� �� �ȿ� ���ϴ� ������ ���� ����(i) ���� ���ο� ������ ����
		if (i < size - 1)
		{
			dist = distance(pt, ptArray[i + 1]);
			radius = (double)getRandomLong(0L, (long)dist);	// ���� ���������� �������� ����
			angle = getRandomLong(0, 359);					// �������� �������� ����
		}
		tArray1.push_back(getPoint(pt, radius, angle));
	}
	ptArray = tArray1;
}
//==========================================================================================================
