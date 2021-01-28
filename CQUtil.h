#ifndef _CQUtil_H_
#define _CQUtil_H_

#include "CQObject.h"

class CQUtil
{
private:
	//static POINT Bezier(vector<POINT>& p, int n, double mu);

	// sp-ep ������ 100���� �ؼ� sp���� ��θ� ���� percent��ŭ ������ �Ÿ��� ��ǥ�� �����Ѵ�.
	static POINT_GEN getOffsetPoint(POINT_GEN sp, POINT_GEN ep, int percent);

	// path�� ���������� ��θ� ���� dist��ŭ ������ �Ÿ��� ��ǥ�� �����Ѵ�.
	static POINT_GEN getOffsetPoint(vector<POINT_GEN>& path, double dist);

	// i�� j������ ���� �������� �����Ѵ�.
	static long		getRandomLong(long i, long j);

	// �� pt���� r��ŭ �������� a��ŭ ȸ���� ���� ��ǥ�� ����
	// 3�ð� 0��, �ݽð�������� ����
	static POINT_GEN getPoint(POINT_GEN& pt, double radius, double angle);

public:
	// ������� ���� �Լ�, nTimeStamp�� path�� �����ؼ� ptArray�� ���� ��ǥ ������ �ǹ��Ѵ�.
	// ���� nTimeStamp�� 10�̸� ���� ��, �ڴ� ���� 8���� ��ǥ�� �����ؼ� �ְ� �ȴ�.
	static void generateLinePath(vector<POINT_GEN>& ptArray, int nTimeStamp, vector<POINT_GEN>& path);

	// ���� ���� �Լ�
	static void generateBezierCurvePath(vector<POINT_GEN>& ptArray, int nTimeStamp, vector<POINT_GEN>& path);
	static void generateSplineCurvePath(vector<POINT_GEN>& ptArray, int nTimeStamp, vector<POINT_GEN>& path);

	// ���ذ�θ� ���� ����� ��ǥ�� ���� ��ΰ� �ǵ��� ��ǥ�� ������Ѵ�.
	static void generateRandomPath(vector<POINT_GEN>& ptArray);

	static double distance(POINT_GEN pt1, POINT_GEN pt2);
	static double distance(vector<POINT_GEN>& path);
};

#endif 