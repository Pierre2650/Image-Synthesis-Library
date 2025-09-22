#pragma once

class Vector3 {
	public:
		float x;
		float y;
		float z;

		static Vector3 Zero;
		static Vector3 One;

		Vector3(float x, float y, float z);

		float Magnitude();
		Vector3 Normalized();

		static bool Equals(Vector3 A, Vector3 B);
		static float Angle(Vector3 A, Vector3 B);

		static float Dot(Vector3 A, Vector3 B);
		static float Dot(Vector3 A, Vector3 B, float angle);
		static Vector3 Cross(Vector3 A, Vector3 B);


		ostream& operator<<(ostream& os,  Vector3& v);
		Vector3 operator+(Vector3& other);
		Vector3 operator-(Vector3& other);
		Vector3 operator*(float& other);
		Vector3 operator/(float& other);
		

	
};

class Point : Vector3
{
	static float Distance(Point A, Point B);
	bool operator==(Point& other);



};

class Direction : Vector3
{
	public:
		Direction(Point A, Point B);

		Point GetPointA();
		Point GetPointB();
		float GetTheta();

		bool operator==(Direction& other);

	private:
		Point A, B;
		float theta;




};

class Color : Vector3
{
	public:

	bool operator==(Color& other);

};

