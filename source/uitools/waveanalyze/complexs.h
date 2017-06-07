/**
@file complex.h
@brief 复数计算头文件
@author lujiashun 
@version 1.0.0
@date 2016-10-23
*/

#ifndef COMPLEXS_H
#define COMPLEXS_H

#include <math.h>

class Complex
{
public:
	Complex() : Re(0), Im(0) {}
	Complex(double re, double im) : Re(re), Im(im){}
	double Re;
	double Im;
};

Complex complex_add(const Complex& c1, const Complex& c2);	// c1+c2
Complex complex_sub(const Complex& c1, const Complex& c2);	// c1-c2
Complex complex_mul(const Complex& c1, const Complex& c2);	// c1*c2
Complex complex_div(const Complex& c1, const Complex& c2);	// c1/c2
Complex complex_slc(double scale, const Complex& c);		// scale * c
double  complex_mag(const Complex& c);	// magnitude
double  complex_phase(const Complex& c);	// phase,(in radians, between -pi and pi)

#endif