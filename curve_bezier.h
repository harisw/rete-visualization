#ifndef _CURVE_BEZIER_H_
#define _CURVE_BEZIER_H_

#include "CQObject.h"

class BezierCurve
{
private:
	double FactorialLookup[33];

	// just check if n is appropriate, then return the result
	double factorial(int n)
	{
		if (n < 0 || 32 < n) return 1.0;
		return FactorialLookup[n]; /* returns the value n! as a SUMORealing point number */
	}

	// create lookup table for fast factorial calculation
	void CreateFactorialTable()
	{
		// fill untill n=32. The rest is too high to represent
		FactorialLookup[0] = 1.0;
		FactorialLookup[1] = 1.0;
		FactorialLookup[2] = 2.0;
		FactorialLookup[3] = 6.0;
		FactorialLookup[4] = 24.0;
		FactorialLookup[5] = 120.0;
		FactorialLookup[6] = 720.0;
		FactorialLookup[7] = 5040.0;
		FactorialLookup[8] = 40320.0;
		FactorialLookup[9] = 362880.0;
		FactorialLookup[10] = 3628800.0;
		FactorialLookup[11] = 39916800.0;
		FactorialLookup[12] = 479001600.0;
		FactorialLookup[13] = 6227020800.0;
		FactorialLookup[14] = 87178291200.0;
		FactorialLookup[15] = 1307674368000.0;
		FactorialLookup[16] = 20922789888000.0;
		FactorialLookup[17] = 355687428096000.0;
		FactorialLookup[18] = 6402373705728000.0;
		FactorialLookup[19] = 121645100408832000.0;
		FactorialLookup[20] = 2432902008176640000.0;
		FactorialLookup[21] = 51090942171709440000.0;
		FactorialLookup[22] = 1124000727777607680000.0;
		FactorialLookup[23] = 25852016738884976640000.0;
		FactorialLookup[24] = 620448401733239439360000.0;
		FactorialLookup[25] = 15511210043330985984000000.0;
		FactorialLookup[26] = 403291461126605635584000000.0;
		FactorialLookup[27] = 10888869450418352160768000000.0;
		FactorialLookup[28] = 304888344611713860501504000000.0;
		FactorialLookup[29] = 8841761993739701954543616000000.0;
		FactorialLookup[30] = 265252859812191058636308480000000.0;
		FactorialLookup[31] = 8222838654177922817725562880000000.0;
		FactorialLookup[32] = 263130836933693530167218012160000000.0;
	}

	double Ni(int n, int i)
	{
		double ni;
		double a1 = factorial(n);
		double a2 = factorial(i);
		double a3 = factorial(n - i);
		ni = a1 / (a2 * a3);
		return ni;
	}

	// Calculate Bernstein basis
	double Bernstein(int n, int i, double t)
	{
		double basis;
		double ti; /* t^i */
		double tni; /* (1 - t)^i */

		/* Prevent problems with pow */

		if (t == 0.0 && i == 0)
			ti = 1.0;
		else
			ti = pow(t, i);

		if (n == i && t == 1.0)
			tni = 1.0;
		else
			tni = pow((1 - t), (n - i));

		//Bernstein basis
		basis = Ni(n, i) * ti * tni;
		return basis;
	}

public:
	BezierCurve()
	{
		CreateFactorialTable();
	}

	void Bezier2D(vector<POINT_GEN>& b, int cpts, vector<POINT_GEN>& p)
	{
		int npts = (int)b.size();
		int icount, jcount;
		double step, t;

		// Calculate points on curve

		icount = 0;
		t = 0;
		step = (double)1.0 / (cpts - 1);

		for (int i1 = 0; i1 != cpts; i1++)
		{
			if ((1.0 - t) < 5e-6)
				t = 1.0;

			jcount = 0;

			p[icount].x = 0;
			p[icount].y = 0;
			for (int i = 0; i != npts; i++)
			{
				double basis = Bernstein(npts - 1, i, t);
				p[icount].x += (int)((basis * b[jcount].x));
				p[icount].y += (int)((basis * b[jcount].y));
				jcount = jcount++;
			}
			icount++;
			t += step;
		}
	}
};

#endif