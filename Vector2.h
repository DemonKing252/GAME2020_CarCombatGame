#pragma once
#ifndef __VECTOR2__
#define __VECTOR2__
#include <math.h>
#include "SDL.h"
#include <iostream>
using namespace std;
class Vector2
{
public:
	double x;
	double y;

	Vector2();
	Vector2(double, double);
	~Vector2();
	
	
	void operator=(Vector2 vector);
	void operator*=(Vector2 vector);
	void operator+=(Vector2 vector);
	void operator-=(Vector2 vector);
	// Math Operations
	void Normalize();

	// Mainly for debugging purposes
	friend ostream& operator<<(ostream& stream, Vector2& v);
};
class VectorMath
{
public:
	static double RadToDegree;
	static double DegreeToRad;
	static double DeltaX(double);
	static double DeltaY(double);
	static double Angle(double, double, double, double);
	static double Magnitude(double, double, double, double);
};
#endif // !__VECTOR2__