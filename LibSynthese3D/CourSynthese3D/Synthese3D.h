#pragma once
#include <iostream>

class Vector3 {
	public:
		float x;
		float y;
		float z;

		static Vector3 Zero;
		static Vector3 One;

		Vector3(float x, float y, float z);
		Vector3(const Vector3& B);

		float Magnitude();
		Vector3 Normalized();

		static bool Equals(Vector3 A, Vector3 B);
		static float Angle(Vector3 A, Vector3 B);

		static float Dot(Vector3 A, Vector3 B);
		static float Dot(Vector3 A, Vector3 B, float deg);
		static float Dot(Vector3 A, Vector3 B, float rad);
		static Vector3 Cross(Vector3 A, Vector3 B);

		Vector3 operator+(Vector3& other);
		Vector3 operator-(Vector3& other);
		Vector3 operator*(float& other);
		Vector3 operator/(float& other);
		

	
};

std::ostream& operator<<(std::ostream& os, const Vector3& v);

class Point : Vector3
{
	public:
		Point(float x, float y, float z);

		static float Distance(Point A, Point B);
		bool operator==(Point& other);



};

//class Direction : Vector3
//{
//	public:
//		Direction(Point A, Point B);
//
//		Point GetPointA();
//		Point GetPointB();
//		float GetTheta();
//
//		bool operator==(Direction& other);
//
//	private:
//		Point A, B;
//		float theta;
//
//		Vector3 Init(Point A, Point B);
//
//
//
//
//};


class Color : Vector3
{
	public:

		Color(float x, float y, float z);
		bool operator==(Color& other);

};

