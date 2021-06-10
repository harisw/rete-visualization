#pragma once


//���� define �� �̿�
#define ENV_AIR			1// ����
#define ENV_SURFACE		2// ����
#define ENV_LAND		3// ����
#define ENV_SUBSURFACE	4// ����
#define ENV_UNKNOWN		5// ��Ȯ��

#define PI 				3.1415926535897932384626	//�� 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define DOUBLE_EQUAL(a,b) ((((a)<(b))?(b)-(a):(a)-(b)) < (double)0.000000000000001)   // TRUE : equal,  FALSE : not equal//130225 ����ȯ �߰�

#define RAD2MIL			1018.59163578813			//radian���� mil�� ��ȯ �ϴ� ���
#define RAD2DEG			(180.0/PI)					//radian���� degree�� ��ȯ �ϴ� ���
#define RAD2MIN 		(10800./PI)					//radina���� mimnute�� ��ȯ �ϴ� ���
#define MIL2DEG			0.05625						//mil���� degree������ ��ȯ �ϴ� ���
#define DEG2MIL			17.777777777777				//degree���� mil������ ��ȯ �ϴ� ���

#define KPH2MS			0.27777777					//m/hour�� m/sec�� ��ȯ �ϴ� ���

#define DEG2RAD			(PI/180.0)					//degree���� radian������ ��ȯ �ϴ� ���	
#define MIL2RAD			0.000981747705				//mil���� radian������ ��ȯ �ϴ� ���

#define MIN2DEG			(1./60.)					//minute���� degree������ ��ȯ �ϴ� ���
#define	DEG2MIN			60.							//degree���� minute������ ��ȯ �ϴ� ��� 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define POUND2KG		(0.453592)					//pound�� kg�� ��ȯ �ϴ� ��� 
#define SQ2KG			(0.13)						//square�� kg�� ��ȯ �ϴ� ��� (ź�� ���)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MBARTONM2		(100.)						//mbar�� newton/m^2�� ��ȯ �ϴ� ���
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define KT2MS			(1852./3600.)				//kts�� m/s�� ��ȯ �ϴ� ��� 
#define MS2KT			(3600./1852.)				//m/s�� kts�� ��ȯ �ϴ� ���
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// 101126 �ֺ�ö DM(Data Mile)     = 1828.8  meter  //
//               NM(Nautical Mile) = 1852    meter  //
//				 MILE(Mile)        = 1609.34 meter  //
//                                                  //
//          **** ���� ȥ�� ���� ****                //
//                                                  //
//////////////////////////////////////////////////////

// 101206 ���ȯ
// ������� : 1DM(Data Mile) - ���̴ٳ� ���������͸�ũ������ �ý��ۿ��� �ַ� ���Ǵ� ����, 6000 feet�� ���ϸ�, 1.8288 km���� ���
//            MILE - 1DM * 0.88, �� 1609.3m   

#define DmToM 1828.8								// DM -> m								//ticket:1320 101126 �迵��
#define MToDM 1/1828.8								// m -> DM								//ticket:1320 101126 �迵��					
#define DmToNM 1828.8/1852;							// DM -> NM								//ticket:1320 101126 �迵�� //PKX-B ticket:133 140225 �迵��
#define NMToDM 1852/1828.8;							// NM -> DM								//ticket:1320 101126 �迵�� //PKX-B ticket:133 140225 �迵��

#define INCH2METER		(0.0254)					//inch�� m�� ��ȯ �ϴ� ���
#define NM2M			1852.						//nautical mile�� m�� ��ȯ �ϴ� ��� 
#define M2NM			(1./1852.)					//m�� nautical mile�� ��ȯ �ϴ� ���

#define FT2M 0.3048									// feet  -> meter						//ticket:1272 101123 ������
#define M2FT  3.28084								// meter -> feet						//ticket:1272 101123 ������

#define YD2M			0.91440183					//yard�� m�� ��ȯ �ϴ� ���
#define M2YD			(1./YD2M)					//m�� yard�� ��ȯ �ϴ� ���				// ticket:1049 20101022 ������, ����� 		

#define	KNOT2MPS		0.51444444444444			//knot�� m/sec������ ��ȯ �ϴ� ���
#define MPS2KNOT		1.9438444924406				//m/sec�� knot������ ��ȯ �ϴ� ���
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define TORAD(d)	((d)/57.2957795131)
#define TODEG(r)	((r)*57.2957795131)

#define Sin(x)			sin(TORAD(x))
#define Cos(x)			cos(TORAD(x))
#define Tan(x)			tan(TORAD(x))
#define Asin(x)			TODEG(asin(x))
#define Acos(x)			TODEG(acos(x))
#define Atan(x)			TODEG(atan(x))
#define Atan2(x1,x2)	TODEG(atan2(x1,x2))

#define RADIUS_G 6378137.
#define RADIUS_G_NM (RADIUS_G*PI/180.0/1852.)

#define WGS84_FF	(1.0/298.257223563)
#define WGS84_FF_EQ	(sqrt(2.0*WGS84_FF-WGS84_FF*WGS84_FF))
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// - FFX�� ���� TM�� ���(default)
#include "math.h"



//***************************************************************************
// ���ϸ�        : ICMSCalculator.h
// ������Ʈ��    : ICMS ���� ��� ���
// ���� �ۼ���   : 2009/01/03
// ������ ������ : 2009/01/03
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ 
// Ŭ������      : CICMSCalculator
// Ŭ���� ����   : ICMS���� ����ϴ� ��ǥ��� ���� 
// ���          : 
//***************************************************************************
class CICMSCalculator
{
// Construction
public:
	CICMSCalculator();																	// ������
	virtual ~CICMSCalculator();															// �Ҹ���

	static double GetBearing(double dLat_1, double dLon_1, double dLat_2, double dLon_2);		// 2�� ���浵�� ���� ���ϴ� �Լ�
	static double GetDistance(double dLat_1, double dLon_1, double dLat_2, double dLon_2);		// 2�� ���浵�� �Ÿ� ���ϴ� �Լ�
	static void GetBearingNDistance(double dLat_1, double dLon_1, double dLat_2, double dLon_2, double* dBearing, double* dDistance); // 2���� ���浵�� ����, �Ÿ� ���ϴ� �Լ�
	static void ChangeBearingNRangeToLatLong(double dLat_1, double dLon_1, double dBearing, double dDistance, double* dLat_2, double* dLon_2); // ���� ���浵�κ��� ����, �Ÿ��� �ش��ϴ� ���浵 ���ϴ� �Լ�
	static void GetXYPos(double i_dblOwnShipLongitude, double i_dblOwnShipLatitude, double i_dblTargetLongitude, double i_dblTargetLatitude, double * i_pdblTargetPosX, double * i_pdblTargetPosY); // ���浵�κ��� X,Y,Z���ϴ� �Լ�

	static double GetSpeedFromVelocity(double fXVelocity, double fYVelocity, double fZVelocity);	// �ӵ��������κ��� X,Y,Z�� �ӵ� ���ϱ�
	static double GetRangeFromPosition(double fXRelPosition, double fYRelPosition, double fZRelPosition); // ��ġ�����κ��� �Ÿ� ���ϱ�
	static double GetElevation(double fXRelPosition, double fYRelPosition, double fZRelPosition);	// ��ġ�����κ��� �� ���ϱ�
	static double GetCourseFromVelocity(double fXVelocity, double fYVelocity, double fZVelocity);	// �ӵ��������� ħ�� ���ϱ�
	static double GetBearingFromPosition(double fXRelPosition, double fYRelPosition);				// ��ġ������ ���� ���ϱ�
	static void Sphere2Orthogonal(double i_dblBearing, double i_dblRange, double i_dblElevation, double * i_pdblPosX, double * i_pdblPosY, double * i_pdblPosZ); // ����ǥ�� ������ǥ�� ��ȯ
	static void Orthogonal2Sphere(double i_dblPosX, double i_dblPosY, double i_dblPosZ, double * i_pdblBearing, double * i_pdblRange, double * i_pdblElevation); // ���� ��ǥ�� ����ǥ�� ��ȯ

private:
	static double	meridional_dist(double lat);												// meridional_dist
	static double	meridional_part(double lat);												// meridional_part
	static double recurs(double FromLatt, double dBearing, double dDistance);					// recurs

public:
	static double GetTimeDiff(unsigned int uiEndTimeSec, unsigned int uiEndTimeMicroSec, unsigned int uiStartTimeSec, unsigned int uiStartTimeMicroSec);
};

