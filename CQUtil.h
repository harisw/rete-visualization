#ifndef _CQUtil_H_
#define _CQUtil_H_

#include "CQObject.h"

class CQUtil
{
private:
	//static POINT Bezier(vector<POINT>& p, int n, double mu);

	// sp-ep 라인을 100으로 해서 sp에서 경로를 따라서 percent만큼 떨어진 거리의 좌표를 리턴한다.
	static POINT_GEN getOffsetPoint(POINT_GEN sp, POINT_GEN ep, int percent);

	// path의 시작점에서 경로를 따라서 dist만큼 떨어진 거리의 좌표를 리턴한다.
	static POINT_GEN getOffsetPoint(vector<POINT_GEN>& path, double dist);

	// i과 j사이의 정수 랜덤값을 리턴한다.
	static long		getRandomLong(long i, long j);

	// 점 pt에서 r만큼 떨어져서 a만큼 회전한 점의 좌표를 리턴
	// 3시가 0도, 반시계방향으로 증가
	static POINT_GEN getPoint(POINT_GEN& pt, double radius, double angle);

public:
	// 직선경로 생성 함수, nTimeStamp는 path를 분할해서 ptArray에 넣을 좌표 개수를 의미한다.
	// 만약 nTimeStamp가 10이면 제일 앞, 뒤는 빼고 8개의 좌표를 생성해서 넣게 된다.
	static void generateLinePath(vector<POINT_GEN>& ptArray, int nTimeStamp, vector<POINT_GEN>& path);

	// 곡선경로 생성 함수
	static void generateBezierCurvePath(vector<POINT_GEN>& ptArray, int nTimeStamp, vector<POINT_GEN>& path);
	static void generateSplineCurvePath(vector<POINT_GEN>& ptArray, int nTimeStamp, vector<POINT_GEN>& path);

	// 기준경로를 따라 저장된 좌표를 랜덤 경로가 되도록 좌표를 재생성한다.
	static void generateRandomPath(vector<POINT_GEN>& ptArray);

	static double distance(POINT_GEN pt1, POINT_GEN pt2);
	static double distance(vector<POINT_GEN>& path);
};

#endif 