#include "Vector2.h"
double VectorMath::RadToDegree = 180.0f / M_PI;
double VectorMath::DegreeToRad = M_PI / 180.0f;


Vector2::Vector2()
{
	x = y = 0.0f;
}

Vector2::Vector2(double p_dx, double p_dy)
{
	x = p_dx;
	y = p_dy;
}


Vector2::~Vector2()
{
}

double VectorMath::DeltaX(double p_dAngle)
{
	return cos((p_dAngle - 90) * VectorMath::DegreeToRad);
}

double VectorMath::DeltaY(double p_dAngle)
{
	return sin((p_dAngle - 90) * VectorMath::DegreeToRad);
}

double VectorMath::Angle(double p_dX1, double p_dY1, double p_dX2, double p_dY2)
{
	return (atan2(p_dY2 - p_dY1, p_dX2 - p_dX1)) * VectorMath::RadToDegree + 90;
}

double VectorMath::Magnitude(double p_dX1, double p_dY1, double p_dX2, double p_dY2)
{
	return sqrt(pow(p_dX2 - p_dX1, 2) + pow(p_dY2 - p_dY1, 2));
}

void Vector2::operator=(Vector2 vector)
{
	x = vector.x;
	y = vector.y;
}

void Vector2::operator*=(Vector2 vector)
{
	x *= vector.x;
	y *= vector.y;
}

void Vector2::operator+=(Vector2 vector)
{
	x *= vector.x;
	y *= vector.y;
}

void Vector2::operator-=(Vector2 vector)
{
	x *= vector.x;
	y *= vector.y;
}

void Vector2::Normalize()
{
	double magnitude = sqrt(x * x + y * y);
	x /= magnitude;
	y /= magnitude;
}

ostream& operator<<(ostream& stream, Vector2& v)
{
	stream << v.x << " " << v.y;
	return stream;
}