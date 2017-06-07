#include "complexs.h"
#include "mathutils.h"

Complex complex_add(const Complex& c1, const Complex& c2)
{
	return Complex(c1.Re + c2.Re, c1.Im + c2.Im);
}

Complex complex_sub(const Complex& c1, const Complex& c2)
{
	return Complex(c1.Re - c2.Re, c1.Im - c2.Im);
}

Complex complex_mul(const Complex& c1, const Complex& c2)
{
	return Complex(c1.Re * c2.Re - c1.Im * c2.Im, 
				   c1.Im * c2.Re + c1.Re * c2.Im);
}

Complex complex_div(const Complex& c1, const Complex& c2)
{
	double f1 = sqrt(c2.Re * c2.Re + c2.Im * c2.Im);
	f1 = f1 <= 0.00001 ? 0 : (sqrt(c1.Re * c1.Re + c1.Im * c1.Im) / f1);
	double f2 = atan2(c1.Im, c1.Re) - atan2(c2.Im, c2.Re);
	return Complex(f1 * cos(f2), f1 * sin(f2));
}

Complex complex_slc(double scale, const Complex& c)
{
	return Complex(scale * c.Re, scale * c.Im);
}

double complex_mag(const Complex& c)
{
	return sqrt(c.Re * c.Re + c.Im * c.Im);
}

double complex_phase(const Complex& c)
{
	const double c2Pi =  2 * PI;
	const double cPid2 = 0.5 * PI;
		
	if (c.Re == 0 && c.Im == 0)
		return 0;
	
	double phase;
	if (fabs(c.Re) > fabs(c.Im))
	{
		phase = atan(c.Im / c.Re);			// -45~45, 135~-135
		if (c.Re < 0)
			phase += PI;
	}
	else
	{
		phase = cPid2 - atan(c.Re / c.Im);	// 45~135,-45~-135
		if (c.Im < 0)
			phase += PI;
	}
	if (phase > PI)
		phase -= c2Pi;
	return phase;
}