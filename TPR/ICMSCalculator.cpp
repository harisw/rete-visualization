// CICMSCalculator.cpp : implementation file
//
// ��õȯ 2006.02.22 ����ö å�� �ڵ�� ��ü
//#include "stdafx.h"
#include "ICMSCalculator.h"

#include <math.h>


//***************************************************************************
// ���ϸ�        : CICMSCalculator.cpp
// ���� �ۼ���   : 2006/02/22
// ������ ������ : 2006/02/22
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ����ö
// ������ ������ : SW�� SW2�׷�/PKX SW ����ö
// �Լ���        : CICMSCalculator
// ����          : ������
// �Է°�        : �ش���� ����
// ���ϰ�        : �ش���� ����
// ���          :
//***************************************************************************
CICMSCalculator::CICMSCalculator()
{
	//���� ����.
}

//***************************************************************************
// ���ϸ�        : CICMSCalculator.cpp
// ���� �ۼ���   : 2006/02/22
// ������ ������ : 2006/02/22
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ����ö
// ������ ������ : SW�� SW2�׷�/PKX SW ����ö
// �Լ���        : ~CICMSCalculator
// ����          : �Ҹ���
// �Է°�        : �ش���� ����
// ���ϰ�        : �ش���� ����
// ���          :
//***************************************************************************
CICMSCalculator::~CICMSCalculator()
{
	//���� ����.
}

//����ö ����+
//����Ѱ� �Ӱ谪 ���� 
#define DISTANCE_TOLERANCE 0.000005 // 1cm

//�ݿø� ó�� ǥ��ȭ  
//***************************************************************************
// ���ϸ�        : CICMSCalculator.cpp
// ���� �ۼ���   : 2006/02/22
// ������ ������ : 2006/02/22
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ����ö
// ������ ������ : SW�� SW2�׷�/PKX SW ����ö
// �Լ���        : round
// ����          : �ݿø� ó��
// �Է°�        : [IN] dValue - �ݿø� ���
// �Է°�        : [IN] pres - ��Ȯ��
// ���ϰ�        : double - �ݿø� ���
// ���          :
//***************************************************************************
static double round(double dValue, int pres)
{
	if (pres <0)
	{
		return dValue;
	}

	double dblTemp = dValue;
	double dblFactor = 1.;
	dblFactor *= ::pow(10., pres);

	dblTemp *= dblFactor;

	dblTemp = (dblTemp > 0) ? floor(dblTemp+0.5) : ceil(dblTemp-0.5);

	dblTemp /= dblFactor;
	return dblTemp;
}
//����ö ����-


//***************************************************************************
// ���ϸ�        : CICMSCalculator.cpp
// ���� �ۼ���   : 2006/02/22
// ������ ������ : 2006/02/22
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ����ö
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : GetBearingNDistance
// ����          : ���浵 ���� ���� �Ÿ� ������ ��ȯ
// �Է°�        : [IN] dLat_1 : ���� ���� (���� : deg)
//				   [IN] dLon_1 : �ð� �浵 (���� : deg)
//				   [IN] dLat_2 : �� ���� (���� : deg)
//				   [IN] dLon_2 : �� �浵 (���� : deg)
//				   [OUT] dBearing : ������ġ������ ����ġ ������ (���� : deg)
//				   [OUT] dDistance : ������ġ���� ����ġ���� �Ÿ� (���� : m)
// ���ϰ�        : void
// ���          : ��°��� Nautical mile���� m�� ����
//***************************************************************************
void CICMSCalculator::GetBearingNDistance(double dLat_1, double dLon_1, double dLat_2, double dLon_2, double *dBearing, double *dDistance)
{
	/**
	�浵�� 0 ~ 360�� ������ ��ȯ�Ѵ�.
	������ -180 ~ 180�� ������ ��ȯ�Ѵ�.
	dDiffX = �� �浵 ����
	dDiffY = �� ���� ����
	if(�Էµ� ������ ������)
	{
		dCourse = dDiffX > 0.0 ? 90.0 : 270.0;
		dPartial = WGS84_FF_EQ * Sin(dLat_2);
		dDist = RADIUS_G_NM * dDiffX * Cos(dLat_2) / sqrt(1 - dPartial * dPartial);
	}
	else if(�Էµ� �浵�� ������)
	{
		delta_m = meridional_dist(dLat_2) - meridional_dist(dLat_1);
		dDist =	fabs(delta_m);
		dCourse = dDiffY > 0.0 ? 0.0: 180.0;
	}
	else
	{
		delta_M = meridional_part(dLat_2) - meridional_part(dLat_1);
		delta_m = meridional_dist(dLat_2) - meridional_dist(dLat_1);
		dCourse	= Atan2(dDiffX, delta_M);
		dDist =	delta_m/Cos(dCourse);
	}

	dDist�� ����� �ٲ۴�.

	dDiffX	= dLon_2 - dLon_1;
	dPartial = Sin(dLat_2)*Sin(dLat_1)+Cos(dLat_2)*Cos(dLat_1)*Cos(dDiffX);
	g_distance = 60.0 * Acos(dPartial);

	dPartial = Cos(dLat_1)*Tan(dLat_2) - Sin(dLat_1)*Cos(dDiffX);
	angle =	fabs(Atan(Sin(dDiffX)/dPartial));

	dCourse�� 0 ~ 180�� ������ �ٲ۴�.

	*dDistance �� dDist * NM2M�� �Է��Ѵ�
	*dBearing �� dCourse ���� �Է��Ѵ�.
	**/

	double delta_m = 0., delta_M = 0.;
	double dDiffX = 0., dDiffY = 0.;
	double dPartial = 0., dCourse = 0., dDist = 0.;
	//calculate great circle distance and initial course
	double g_distance = 0., g_course = 0., angle = 0.;

	if (dLon_1 < 0.)
	{
		dLon_1 += 360.0;
	}

	if (dLon_2 < 0.)
	{
		dLon_2 += 360.0;
	}

	dDiffX = dLon_2 - dLon_1;
	if (dDiffX > 180.0) 
	{
		dDiffX -= 360.0;
	}

	if (dDiffX < -180.0)
	{
		dDiffX += 360.0;
	}

	dDiffY = dLat_2 - dLat_1;

//����ö ����+
	// �������� �ε��Ҽ��� ���������� �ǹ��ִ� ���ؿ���
	// �ݿø��ؼ� ���� ���̶�� �Ǵ��ؾ� ��..
	dDiffY = round(dDiffY, 5);
//����ö ����-

//����ö ����+
//	if(fabs(dLat_2 - dLat_1) < 0.0000001)
	if(fabs(dLat_2 - dLat_1) < DISTANCE_TOLERANCE)
//����ö ����-
	{
		dCourse = dDiffX > 0.0 ? 90.0 : 270.0;
		dPartial = WGS84_FF_EQ * Sin(dLat_2);
		dDist = (RADIUS_G_NM * dDiffX * Cos(dLat_2)) / sqrt(1 - (dPartial * dPartial));
	}
//����ö ����+
//	else if(fabs(dLon_2 - dLon_1) < 0.0000001)
	else if(fabs(dLon_2 - dLon_1) < DISTANCE_TOLERANCE)
//����ö ����-
	{
		delta_m = meridional_dist(dLat_2) - meridional_dist(dLat_1);
		dDist =	fabs(delta_m);
		dCourse = dDiffY > 0.0 ? 0.0: 180.0;
	}
	else
	{
		delta_M = meridional_part(dLat_2) - meridional_part(dLat_1);
		delta_m = meridional_dist(dLat_2) - meridional_dist(dLat_1);
		dCourse	= Atan2(dDiffX, delta_M);
		if(dCourse<0.0)
		{
			dCourse += 360;
		}
		dDist =	delta_m/Cos(dCourse);
	}

//����ö ����+
	dDist = round(dDist, 5);
//����ö ����-

	if (dDist < 0)
	{
		dDist *= -1;
	}

//����ö ����+
	if (dDist < DISTANCE_TOLERANCE)		dDist = 0;	
//����ö ����-

	dDiffX	= dLon_2 - dLon_1;
	dPartial = Sin(dLat_2)*Sin(dLat_1)+Cos(dLat_2)*Cos(dLat_1)*Cos(dDiffX);
	g_distance = 60.0 * Acos(dPartial);

	dPartial = Cos(dLat_1)*Tan(dLat_2) - Sin(dLat_1)*Cos(dDiffX);
	angle =	fabs(Atan(Sin(dDiffX)/dPartial));

	if(dCourse > 180)
	{
		g_course = 180.0 + angle;
	}
	else
	{
		g_course = 180.0 - angle;
	}


//����ö ����+
// m�� ��ȯ�ϴ°��� �Ϲ������� ?
//����ö ����-
//	*dDistance = dDist;
	*dDistance = dDist * NM2M;	//Nautical mile���� m�� ���� ��õȯ	2006.02.22 
	*dBearing = dCourse;
}

//***************************************************************************
// ���ϸ�        : CICMSCalculator.cpp
// ���� �ۼ���   : 2006/02/22
// ������ ������ : 2006/02/22
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ����ö
// ������ ������ : SW�� SW2�׷�/PKX SW ����ö
// �Լ���        : GetBearing
// ����          : ���浵 ���� ���� ������ ��ȯ
// �Է°�        : [IN] dLat_1 : ���� ���� (���� : deg)
//				   [IN] dLon_1 : �ð� �浵 (���� : deg)
//				   [IN] dLat_2 : �� ���� (���� : deg)
//				   [IN] dLon_2 : �� �浵 (���� : deg)
// ���ϰ�        : ������ġ������ ����ġ ������ (���� : deg)
// ���          :
//***************************************************************************
double CICMSCalculator::GetBearing(double dLat_1, double dLon_1, double dLat_2, double dLon_2)
{
	/**
	GetBearingNDistance() �Լ��� ȣ���Ѵ�.
	���� �������� ��ȯ�Ѵ�.
	**/
	double dBearing;
	double dDistance;
	GetBearingNDistance(dLat_1, dLon_1, dLat_2, dLon_2, &dBearing, &dDistance);

	return dBearing;
}

//***************************************************************************
// ���ϸ�        : CICMSCalculator.cpp
// ���� �ۼ���   : 2006/02/22
// ������ ������ : 2006/02/22
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ����ö
// ������ ������ : SW�� SW2�׷�/PKX SW ����ö
// �Լ���        : GetDistance
// ����          : ���浵 ���� ���� ������ ��ȯ
// �Է°�        : [IN] dLat_1 : ���� ���� (���� : deg)
//				   [IN] dLon_1 : �ð� �浵 (���� : deg)
//				   [IN] dLat_2 : �� ���� (���� : deg)
//				   [IN] dLon_2 : �� �浵 (���� : deg)
// ���ϰ�        : ������ġ���� ����ġ���� �Ÿ� (���� : m)
// ���          :
//***************************************************************************
double CICMSCalculator::GetDistance(double dLat_1, double dLon_1, double dLat_2, double dLon_2)
{
	/**
	GetBearingNDistance() �Լ��� ȣ���Ѵ�.
	���� �Ÿ��� ��ȯ�Ѵ�.
	**/
	double dBearing;
	double dDistance;
	GetBearingNDistance(dLat_1, dLon_1, dLat_2, dLon_2, &dBearing, &dDistance);

	return dDistance;
}

//***************************************************************************
// ���ϸ�        : CICMSCalculator.cpp
// ���� �ۼ���   : 2006/02/22
// ������ ������ : 2006/02/22
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ����ö
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : ChangeBearingNRangeToLatLong
// ����          : ������ �� �Ÿ� ���� ���浵�� ��ȯ
// �Է°�        : [IN] dLat_1 : ���� ���� (���� : deg)
//				   [IN] dLon_1 : �ð� �浵 (���� : deg)
//				   [IN] dBearing : ������ġ������ ����ġ ������ (���� : deg)
//				   [IN] dDistance : ������ġ���� ����ġ���� �Ÿ� (���� : m)
//				   [OUT] dLat_2 : �� ���� (���� : deg)
//				   [OUT] dLon_2 : �� �浵 (���� : deg)
// ���ϰ�        : void
// ���          :
//***************************************************************************
void   CICMSCalculator::ChangeBearingNRangeToLatLong(double dLat_1, double dLon_1, double dBearing, double dDistance, double* dLat_2, double* dLon_2)
{

	/**
	dDistance�� Nautical mile�� ��ȯ �Ѵ�.

	dCourse �� dBearing�� 0 ~ 360�� ������ ��ȯ�Ͽ� �Է��Ѵ�.
	dDist �� dDistance�� �Է��Ѵ�.
	dPtval_X �� dLon_1�� �Է��Ѵ�.

	dCourse�� 0���� 360�� ������ ��ȯ�Ѵ�.

	if(dCourse �� 0�� �̰ų� 180�� �̸�)
	{
		dPtval_Y = recurs(dLat_1, dCourse, dDist);
	}
	else if(dCourse�� 90�� �̰ų� 270�� �̸�)
	{
		dpartial	=	WGS84_FF_EQ * Sin(dLat_1);
		dDiffValue	=	dDist* sqrt(1 - dpartial * dpartial) / (RADIUS_G_NM * Cos(dLat_1));
		dPtval_X += (dCourse==90.0) ? dDiffValue : -dDiffValue;
		dPtval_Y = dLat_1;
	}
	else
	{
		dPtval_Y = recurs(dLat_1, dCourse, dDist);
		delta_M = meridional_part(dPtval_Y) - meridional_part(dLat_1);
		dDiffValue	= fabs(delta_M*Tan(dCourse));
		dPtval_X +=	(dCourse< 180.0) ? dDiffValue : -dDiffValue;
	}

	*dLon_2 �� dPtval_X�� �Է��Ѵ�.
	*dLat_2 �� dPtval_Y�� �Է��Ѵ�.
	**/

//����ö ����+
	dDistance = round(dDistance, 5);
	dBearing = round(dBearing, 5);
	
	*dLat_2 = dLat_1;
	*dLon_2 = dLon_1;

	if (dDistance < DISTANCE_TOLERANCE)
	{
		return;// dPtFrom;
	}
//�� ������ �Ÿ��� ���� ������ ���� ������ �������� ���� ����� �߻��� 
//1cm ���� ���� ���� 
//����ö ����-

	dDistance = dDistance * M2NM;	//m�� Nautical mile�� ��ȯ // ��õȯ �߰� 2006.02.22

	double dPtval_X = 0;
	double dPtval_Y = 0;


	double dDiffValue = 0., dCourse = 0.;
	double dpartial = 0., dDist = 0.;
	double delta_M = 0.;

	int C = 0;

	dCourse = dBearing;
	dDist = dDistance;
	dPtval_X = dLon_1;

	if( (dCourse < 0.0) || (dCourse >= 360.0))
	{
		C =	(int)dCourse;
		dCourse	= C % 360 + dCourse - C;
		if(dCourse < 0.0)
		{
			dCourse+=360.0;
		}
	}

	if( DOUBLE_EQUAL(dCourse, 0.0) || DOUBLE_EQUAL(dCourse, 180.0))
	{
		dPtval_Y = recurs(dLat_1, dCourse, dDist);
	}
	else if( DOUBLE_EQUAL(dCourse, 90.0) || DOUBLE_EQUAL(dCourse, 270.0))
	{
		dpartial	=	WGS84_FF_EQ * Sin(dLat_1);

//		if(fabs(Cos(dLat_1)) < 0.0000000001)
//����ö ����+
//1cm ���� ���� ���� 
//����ö ����-
		if(fabs(Cos(dLat_1)) < 0.00000001)
		{
			dDiffValue	=	dDist* sqrt(1 - dpartial * dpartial);
		}
		else
		{
			dDiffValue	=	dDist* sqrt(1 - dpartial * dpartial) / (RADIUS_G_NM * Cos(dLat_1));
		}

		dPtval_X += (DOUBLE_EQUAL(dCourse, 90.0)) ? dDiffValue : -dDiffValue;
		dPtval_Y = dLat_1;
	}
	else
	{
		dPtval_Y = recurs(dLat_1, dCourse, dDist);
		delta_M = meridional_part(dPtval_Y) - meridional_part(dLat_1);
		dDiffValue	= fabs(delta_M*Tan(dCourse));
		dPtval_X +=	(dCourse< 180.0) ? dDiffValue : -dDiffValue;
	}

	*dLon_2 = dPtval_X;
	*dLat_2 = dPtval_Y;
//	return dPtval;
}

//***************************************************************************
// ���ϸ�        : CICMSCalculator.cpp
// ���� �ۼ���   : 2006/02/22
// ������ ������ : 2006/02/22
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ����ö
// ������ ������ : SW�� SW2�׷�/PKX SW ����ö
// �Լ���        : recurs
// ����          : ���� #3
// �Է°�        : [IN] FromLatt : ���� ���� (���� : deg)
//				 : [IN] dBearing : ������ (���� : deg)
//				 : [IN] dDistance : �Ÿ� (���� : NM)
// ���ϰ�        : recurs() �Լ� ��� ���
// ���          :
//***************************************************************************
double CICMSCalculator::recurs(double FromLatt, double dBearing, double dDistance)
{
	/**
	dfrom = meridional_dist(FromLatt);
	dguess = dfrom + dDistance * Cos(dBearing);
	dlat = FromLatt + (dguess - dfrom) / 60.0;
	for(iteration = 0; iteration < 10; iteration++)
	{
		dpartial = meridional_dist(dlat);
		dlat = dlat + (dguess - dpartial) / 60.0;
	}
	return dlat;
	**/

	register int iteration;
	double dfrom = 0., dguess = 0., dpartial = 0., dlat = 0.;

	dfrom = meridional_dist(FromLatt);
	dguess = dfrom + dDistance * Cos(dBearing);
	dlat = FromLatt + (dguess - dfrom) / 60.0;
	for(iteration = 0; iteration < 10; iteration++)
	{
		dpartial = meridional_dist(dlat);
		dlat = dlat + (dguess - dpartial) / 60.0;
	}
	return dlat;
}

//***************************************************************************
// ���ϸ�        : CICMSCalculator.cpp
// ���� �ۼ���   : 2006/02/22
// ������ ������ : 2006/02/22
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ����ö
// ������ ������ : SW�� SW2�׷�/PKX SW ����ö
// �Լ���        : meridional_dist
// ����          : ���� #1
// �Է°�        : [IN] lat - ���� (���� : deg)
// ���ϰ�        : meridional_dist() �Լ� ��� ���
// ���          :
//***************************************************************************
double CICMSCalculator::meridional_dist(double lat)
{
	/**
	e2 = WGS84_FF_EQ * WGS84_FF_EQ;
	e4 = e2 * e2;
	e6 = e4 * e2;
	double v1 = PI/180.0 * (1.0 - e2 / 4.0 - 3.0 * e4 / 64.0 - 5.0 * e6 / 256.0);
	double v2 = 3.0 /8.0 * (e2 - e4 / 4.0 + 15.0 * e6 / 128.0);
	double v3 = 15.0 / 256.0 * (e4 + 3.0 * e6 / 4.0);
	double v4 = RADIUS_G / 1852.0;
	retrun v4 * (v1 * lat - v2 * Sin(2.0 * lat) + v3 * Sin(4.0 * lat));
	**/

	double md, e2, e4, e6;

	e2 = WGS84_FF_EQ * WGS84_FF_EQ;
	e4 = e2 * e2;
	e6 = e4 * e2;
	double v1 = PI/180.0 * (1.0 - e2 / 4.0 - 3.0 * e4 / 64.0 - 5.0 * e6 / 256.0);
	double v2 = 3.0 /8.0 * (e2 - e4 / 4.0 + 15.0 * e6 / 128.0);
	double v3 = 15.0 / 256.0 * (e4 + 3.0 * e6 / 4.0);
	double v4 = RADIUS_G / 1852.0;
	md = v4 * (v1 * lat - v2 * Sin(2.0 * lat) + v3 * Sin(4.0 * lat));

	return(md);
}

//***************************************************************************
// ���ϸ�        : CICMSCalculator.cpp
// ���� �ۼ���   : 2006/02/22
// ������ ������ : 2006/02/22
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ����ö
// ������ ������ : SW�� SW2�׷�/PKX SW ����ö
// �Լ���        : meridional_part
// ����          : ���� #2
// �Է°�        : [IN] lat - ���� (���� : deg)
// ���ϰ�        : meridional_part() �Լ� ��� ���
// ���          :
//***************************************************************************
double CICMSCalculator::meridional_part(double lat)
{
	/**
	return TODEG(log(Tan(45.0 + lat / 2.0)) - 0.5 * WGS84_FF_EQ * log((1 + WGS84_FF_EQ * Sin(lat)) / (1 - WGS84_FF_EQ * Sin(lat))));
	**/

	double mp, part;

	part = WGS84_FF_EQ * Sin(lat);
	mp = TODEG(log(Tan(45.0 + lat / 2.0))
	     - 0.5 * WGS84_FF_EQ * log((1 + part) / (1 - part)));
	return(mp);
}

//***************************************************************************
// ���ϸ�        : CICMSCalculator.cpp
// ���� �ۼ���   : 2006/02/22
// ������ ������ : 2006/02/22
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ����ö
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : GetXYPos
// ����          : ���浵 ���� X,Y ������ ��ȯ
// �Է°�        : [IN] i_dbl
// Longitude : �ð� �浵 (���� : deg)
//				   [IN] i_dblOwnShipLatitude : ���� ���� (���� : deg)
//				   [IN] i_dblTargetLongitude : �� �浵 (���� : deg)
//				   [IN] i_dblTargetLatitude : �� ���� (���� : deg)
//				   [OUT] i_pdblTargetPosX : ������ġ���� ����ġ���� X(����)�Ÿ� (���� : m)
//				   [OUT] i_pdblTargetPosY : ������ġ���� ����ġ���� Y(����)�Ÿ� (���� : m)
// ���ϰ�        : void
// ���          :
//***************************************************************************
void CICMSCalculator::GetXYPos(double i_dblOwnShipLongitude, double i_dblOwnShipLatitude, double i_dblTargetLongitude, double i_dblTargetLatitude, double * i_pdblTargetPosX, double * i_pdblTargetPosY)
{
	/**
	GetBearingNDistance() �Լ��� ȣ���Ѵ�.
	*i_pdblTargetPosX = ���� �Ÿ� * Sin(���� ����)
	*i_pdblTargetPosY = ���� �Ÿ� * Cos(���� ����)
	**/
	double dblBearing,dblDistance;

	GetBearingNDistance(i_dblOwnShipLatitude,i_dblOwnShipLongitude, i_dblTargetLatitude, i_dblTargetLongitude, &dblBearing, &dblDistance);

	*i_pdblTargetPosX = dblDistance * Sin(dblBearing);
	*i_pdblTargetPosY = dblDistance * Cos(dblBearing);

}

//*********************************************************************************************
// ���ϸ�		: CICMSCalculator.cpp
// ���� �ۼ���	: 2005/05/09
// ������ ������: 2005/05/09
// ���� �ۼ���	: �ý����� SW�׷�/PKX SW �賲��
// ������ ������: �ý����� SW�׷�/PKX SW �賲��
// �Լ���		: GetSpeedFromEachVelocity
// ����			: ������ �ӷ������κ��� �ӵ����� ����
// �Է°�		: X, Y, Z�� �ӷ�
// ���ϰ�		: �ӵ�
// ���			:
//*********************************************************************************************
double CICMSCalculator::GetSpeedFromVelocity(double fXVelocity, double fYVelocity, double fZVelocity)
{
	/**
	double dblSpeed;
	dblSpeed = GetRangeFromPosition(fXVelocity, fYVelocity, fZVelocity);
	return (double)dblSpeed;
	**/
	double dblSpeed;
	dblSpeed = GetRangeFromPosition(fXVelocity, fYVelocity, fZVelocity);
	return (double)dblSpeed;
}


//*********************************************************************************************
// ���ϸ�		: CICMSCalculator.cpp
// ���� �ۼ���	: 2005/05/09
// ������ ������: 2005/05/09
// ���� �ۼ���	: �ý����� SW�׷�/PKX SW �賲��
// ������ ������: �ý����� SW�׷�/PKX SW �賲��
// �Լ���		: GetRangeFromPosition
// ����			: ��ġ�����κ��� ǥ���� Range���� ����
// �Է°�		: X, Y, Z ��ġ ��
// ���ϰ�		: �Ÿ�
// ���			:
//*********************************************************************************************
double CICMSCalculator::GetRangeFromPosition(double fXRelPosition, double fYRelPosition, double fZRelPosition)
{
	/**
	double dblRange;

	dblRange = (fXRelPosition * fXRelPosition + fYRelPosition * fYRelPosition + fZRelPosition * fZRelPosition);
	dblRange = sqrt(dblRange);

	return (double) dblRange;
	**/
	double dblRange;

	dblRange = (fXRelPosition * fXRelPosition + fYRelPosition * fYRelPosition + fZRelPosition * fZRelPosition);
	dblRange = sqrt(dblRange);

	return (double) dblRange;
}

//*********************************************************************************************
// ���ϸ�		: CICMSCalculator.cpp
// ���� �ۼ���	: 2005/05/09
// ������ ������: 2005/05/09
// ���� �ۼ���	: �ý����� SW�׷�/PKX SW �賲��
// ������ ������: �ý����� SW�׷�/PKX SW �賲��
// �Լ���		: GetElevation
// ����			: X, Y, Z���κ��� �� ���
// �Է°�		: X, Y, Z�� ��ġ
// ���ϰ�		: �ӵ�
// ���			:
//*********************************************************************************************
double CICMSCalculator::GetElevation(double fXRelPosition, double fYRelPosition, double fZRelPosition)
{
	/**
	double fElevation;
	double fTemp;

	fTemp = fXRelPosition*fXRelPosition + fYRelPosition*fYRelPosition;
	fElevation = (double)Atan2(fZRelPosition,sqrt(fTemp));

	return fElevation;
	**/
	double fElevation;
	double fTemp;

	// ticket 2568 fTemp = fXRelPosition*fXRelPosition + fYRelPosition*fYRelPosition + fZRelPosition*fZRelPosition; ���Ŀ��� 2012.03.30
	fTemp = fXRelPosition*fXRelPosition + fYRelPosition*fYRelPosition;
	fElevation = (double)Atan2(fZRelPosition,sqrt(fTemp));

	return fElevation;
}


//*********************************************************************************************
// ���ϸ�		: CICMSCalculator.cpp
// ���� �ۼ���	: 2005/11/24
// ������ ������: 2005/11/24
// ���� �ۼ���	: �ý����� SW�׷�/PKX SW �賲��
// ������ ������: �ý����� SW�׷�/PKX SW �賲��
// �Լ���		: GetCourseFromVelocity
// ����			: ������ �ӵ������κ��� ħ�θ� ����
// �Է°�		: X, Y, Z�� �ӵ�
// ���ϰ�		: ħ��
// ���			:
//*********************************************************************************************
double CICMSCalculator::GetCourseFromVelocity(double fXVelocity, double fYVelocity, double fZVelocity)
{
	/// return GetBearingFromPosition(fXVelocity, fYVelocity);
	return GetBearingFromPosition(fXVelocity, fYVelocity);
}


//*********************************************************************************************
// ���ϸ�		: CICMSCalculator.cpp
// ���� �ۼ���	: 2005/05/09
// ������ ������: 2005/05/09
// ���� �ۼ���	: �ý����� SW�׷�/PKX SW �賲��
// ������ ������: �ý����� SW�׷�/PKX SW �賲��
// �Լ���		: GetBearingFromPosition
// ����			: ��ġ�����κ��� ǥ���� Bearing���� ����
// �Է°�		: ǥ���� X, Y�� �Ÿ�
// ���ϰ�		: �ش���� ����
// ���			: ���ϱ����� Bearing�� Return(0 - 359.9999)
//*********************************************************************************************
double CICMSCalculator::GetBearingFromPosition(double fXRelPosition, double fYRelPosition)
{
	/**
	double fBearing = 0;

	fBearing = (double)Atan2(fXRelPosition,fYRelPosition);

	if ( fBearing < 0.0)
	{
		fBearing += 360;
	}

	return fBearing;
	**/
	double fBearing = 0;

	fBearing = (double)Atan2(fXRelPosition,fYRelPosition);

	if ( fBearing < 0.0)
	{
		fBearing += 360;
	}

	return fBearing;
}

//*********************************************************************************************
// ���ϸ�		: CICMSCalculator.cpp
// ���� �ۼ���	: 2005/05/09
// ������ ������: 2005/05/09
// ���� �ۼ���	: �ý����� SW�׷�/PKX SW �賲��
// ������ ������: �ý����� SW�׷�/PKX SW �賲��
// �Լ���		: GetBearingFromPosition
// ����			: ����ǥ�� ������ǥ�� ��ȯ
// �Է°�		: [IN] i_dblBearing - ������ (���� : deg)
//                [IN] i_dblRange - �Ÿ� (���� : m)
//				  [IN] i_dblElevation - �� (���� : deg)
//				  [OUT] i_pdblPosX - X ��ġ (���� : m)
//				  [OUT] i_pdblPosY - Y ��ġ (���� : m)
//				  [OUT] i_pdblPosZ - Z ��ġ (���� : m)
// ���ϰ�		: �ش���� ����
// ���			: 
//*********************************************************************************************
void CICMSCalculator::Sphere2Orthogonal(double i_dblBearing, double i_dblRange, double i_dblElevation, double * i_pdblPosX, double * i_pdblPosY, double * i_pdblPosZ)
{
	/**
	double dblElevationRad;
	double dblBearingRad;
	dblElevationRad = i_dblElevation * DEG2RAD;
	dblBearingRad = i_dblBearing * DEG2RAD;

	if(i_pdblPosX)
	{
		* i_pdblPosX = i_dblRange * cos(dblElevationRad) * sin(dblBearingRad);
	}
	if(i_pdblPosY)
	{
		* i_pdblPosY = i_dblRange * cos(dblElevationRad) * cos(dblBearingRad);
	}
	if(i_pdblPosZ)
	{
		* i_pdblPosZ = i_dblRange * sin(dblElevationRad);
	}
	**/
	double dblElevationRad;
	double dblBearingRad;
	dblElevationRad = i_dblElevation * DEG2RAD;
	dblBearingRad = i_dblBearing * DEG2RAD;

	if(i_pdblPosX)
	{
		* i_pdblPosX = i_dblRange * cos(dblElevationRad) * sin(dblBearingRad);
	}
	if(i_pdblPosY)
	{
		* i_pdblPosY = i_dblRange * cos(dblElevationRad) * cos(dblBearingRad);
	}
	if(i_pdblPosZ)
	{
		* i_pdblPosZ = i_dblRange * sin(dblElevationRad);
	}
}

//*********************************************************************************************
// ���ϸ�		: CICMSCalculator.cpp
// ���� �ۼ���	: 2005/05/09
// ������ ������: 2005/05/09
// ���� �ۼ���	: �ý����� SW�׷�/PKX SW �賲��
// ������ ������: �ý����� SW�׷�/PKX SW �賲��
// �Լ���		: Orthogonal2Sphere
// ����			: ���� ��ǥ�� ����ǥ�� ��ȯ
// �Է°�		: [IN] i_dblPosX - X ��ġ (���� : m)
//				  [IN] i_dblPosY - Y ��ġ (���� : m)
//				  [IN] i_dblPosZ - Z ��ġ (���� : m)
//				  [OUT] i_pdblBearing - ������ (���� : deg)
//                [OUT] i_pdblRange - �Ÿ� (���� : m)
//				  [OUT] i_pdblElevation - �� (���� : deg)
// ���ϰ�		: �ش���� ����
// ���			: 
//*********************************************************************************************
void CICMSCalculator::Orthogonal2Sphere(double i_dblPosX, double i_dblPosY, double i_dblPosZ, double * i_pdblBearing, double * i_pdblRange, double * i_pdblElevation)
{
	/**
	double dblTemp;
	dblTemp = i_dblPosX * i_dblPosX + i_dblPosY * i_dblPosY;

	if(i_pdblElevation)
	{
		*i_pdblElevation = atan2(i_dblPosZ,sqrt(dblTemp)) * RAD2DEG;
	}
	if(i_pdblBearing)
	{
		*i_pdblBearing = atan2(i_dblPosX,i_dblPosY) * RAD2DEG;
	}
	if(i_pdblRange)
	{
		*i_pdblRange = sqrt(dblTemp + i_dblPosZ * i_dblPosZ);
	}
	**/
	double dblTemp;
	dblTemp = i_dblPosX * i_dblPosX + i_dblPosY * i_dblPosY;

	if(i_pdblElevation)
	{
		*i_pdblElevation = atan2(i_dblPosZ,sqrt(dblTemp)) * RAD2DEG;
	}
	if(i_pdblBearing)
	{
		*i_pdblBearing = atan2(i_dblPosX,i_dblPosY) * RAD2DEG;
	}
	if(i_pdblRange)
	{
		*i_pdblRange = sqrt(dblTemp + i_dblPosZ * i_dblPosZ);
	}
}

double CICMSCalculator::GetTimeDiff(unsigned int uiEndTimeSec, unsigned int uiEndTimeMicroSec, unsigned int uiStartTimeSec, unsigned int uiStartTimeMicroSec)
{
	//start PKXB ticket: 8 y.san.kim 20141125 [����ǥ�� DR �� Ƣ�� ����]
	int iTimeDiff = 0;
	int iTimeDiff_us = 0;
	//end PKXB ticket: 8 y.san.kim 20141125 [����ǥ�� DR �� Ƣ�� ����]
	double dblDeltaTimeGap = 0.0f;

	iTimeDiff = uiEndTimeSec - uiStartTimeSec;				// sec.
	iTimeDiff_us = uiEndTimeMicroSec - uiStartTimeMicroSec;		// micro sec.

	if (iTimeDiff_us < 0)
	{
		iTimeDiff-- ;
		iTimeDiff_us = iTimeDiff_us + 1000000;

	}
	dblDeltaTimeGap = (double) ( (double) iTimeDiff + (double)  ((iTimeDiff_us) / 1000000.0));
	return dblDeltaTimeGap;
}