#pragma once
#include <iostream>

class Vector3 {
	public:
		float x;
		float y;
		float z;

		static const Vector3 Zero;
		static const Vector3 One;
		static const Vector3 NaN;

		Vector3(const float x, const float y, const float z);
		Vector3(const Vector3& B);

		float Magnitude() const;
		Vector3 Normalized () const;

		static bool AlmostEqual(const Vector3 A, const Vector3 B);
		static bool IsNaN(const Vector3 A);
		static float Angle(const Vector3 A, const Vector3 B);

		static float Dot(const Vector3 A, const Vector3 B);
		static Vector3 Cross(const Vector3 A, const Vector3 B);

		Vector3 operator+(const Vector3& other) const;
		Vector3 operator-(const Vector3& other) const;
		Vector3 operator*(const float& other) const;
		Vector3 operator/(const float& other) const;
		

	
};

std::ostream& operator<<(std::ostream& os, const Vector3& v);

class Point : public Vector3
{
	public:
		Point(const float x, const float y, const float z);

		static float Distance(const Point A, const Point B);


};

class Direction : public Vector3
{
	public:
		Direction(const Point A);
		Direction(const Direction& B);

		Point GetPoint() const;


	private:
		Point P ;
		Vector3 Init(const Point A);




};

std::ostream& operator<<(std::ostream& os, const Direction& v);

class Color : public Vector3
{
	//values betweens 1 and 0 
    public:
		float Transparency;
		// Enumeration useful colors

		Color(float x, float y, float z, float t);

};

std::ostream& operator<<(std::ostream& os, const Color& A);
