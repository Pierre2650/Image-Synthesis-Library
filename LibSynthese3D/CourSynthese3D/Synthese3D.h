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

		Vector3();
		Vector3(const float x, const float y, const float z);
		Vector3(const Vector3& B);

		float Magnitude() const;
		Vector3 Normalized () const;

		static float Distance(const Vector3 A, const Vector3 B);
		static bool AlmostEqual(const Vector3 A, const Vector3 B);
		static bool IsNaN(const Vector3 A);
		static float Angle(const Vector3 A, const Vector3 B);

		static float Dot(const Vector3 A, const Vector3 B);
		static Vector3 Cross(const Vector3 A, const Vector3 B);

		Vector3 operator+(const Vector3& other) const;
		Vector3 operator-(const Vector3& other) const;
		Vector3 operator-() const; 
		Vector3 operator*(const float& other) const;
		Vector3 operator/(const float& other) const;
		friend Vector3 operator*(const float& scalar, const Vector3& vec);
		friend Vector3 operator/(const float& scalar, const Vector3& vec);
		Vector3& operator+=(const Vector3& other);
		

	
};

std::ostream& operator<<(std::ostream& os, const Vector3& v);



class Color : public Vector3
{
	//values betweens 1 and 0 
    public:
		static const Color Red;
		static const Color Blue;
		static const Color Green;
		static const Color Black;
		static const Color White;

		float Transparency;
		// Enumeration useful colors
		Color();
		Color(Vector3 A);
		Color(float x, float y, float z);
		Color(float x, float y, float z, float t);

	public:
		void Clamp();

		Color operator+(const Color& other) const;
		Color operator-(const Color& other) const;
		Color operator-() const;
		Color operator-(const float& other) const;

		Color operator*(const Color& other) const;
		Color operator*(const float& other) const;
		friend Color operator*(const float& scalar, const Color& vec);


		Color operator/(const float& other) const;

};

std::ostream& operator<<(std::ostream& os, const Color& A);

class Ray {

	public:
		Vector3 origin;
		Vector3 direction;

		Ray();
		Ray(Vector3 origin, Vector3 direction);

};