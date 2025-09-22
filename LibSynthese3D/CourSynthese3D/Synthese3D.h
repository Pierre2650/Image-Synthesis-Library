#pragma once

class Vector3 {
	public:
		float x;
		float y;
		float z;

		static Vector3 Zero;
		static Vector3 One;

		Vector3(float x, float y, float z);

		Vector3 operator+( Vector3 other);
		Vector3 operator-(Vector3 other);

		float Magnitude();
		Vector3 Normalized();


		Vector3 Dot(Vector3 A, Vector3 B);
		Vector3 Cross(Vector3 A, Vector3 B);
		bool Equals(Vector3 B);


		

	
};

class Point : Vector3
{
	static float Distance(Point A, Point B);



};

class Direction
{
	public:

	private:

};

class Color
{
public:
private:

};

