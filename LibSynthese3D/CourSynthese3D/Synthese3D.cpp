// Synthese3D.cpp : Définit les fonctions de la bibliothèque statique.
//
#include "pch.h"
#include "framework.h"
#include "Synthese3D.h"
#include <iostream>

#pragma region Vector3_Class

/// ------------- Var -------------
Vector3 Vector3::Zero(0, 0, 0);
Vector3 Vector3::One(1, 1, 1);

/// ------------- Construct -------------
Vector3::Vector3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3::Vector3(const Vector3& B) {
	this->x = B.x;
	this->y = B.y;
	this->z = B.z;
}

/// ------------- Methods -------------

float Vector3::Magnitude() {
	return std::sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
}

Vector3 Vector3::Normalized() {
	float mag = this->Magnitude();
	
	if (mag == 0) { return this->Zero; }

	return Vector3(this->x / mag, this->y / mag, this->z / mag);

}


//// ------------- Static -------------

bool Vector3::Equals(Vector3 A, Vector3 B) {
	if (A.x != B.x || std::abs(A.x - B.x) > 0.00001) { return false; }
	if (A.y != B.y || std::abs(A.y - B.y) > 0.00001) { return false; }
	if (A.z != B.z || std::abs(A.z - B.z) > 0.00001) { return false; }

	return true;
}

float Vector3::Angle(Vector3 A, Vector3 B) {
	// In radians
	float magA = A.Magnitude(), magB = B.Magnitude();
	float dot = Vector3::Dot(A, B);
		
	return  dot / (magA * magB);
}

float Vector3::Dot(Vector3 A, Vector3 B) {
	
	return A.x * B.x + A.y * B.y + A.z * B.z;

}

float Vector3::Dot(Vector3 A, Vector3 B, float deg) {

	// angle in degrees ;
	float rad;
	if (deg == 90) { return 0; }
	else if (deg == 0) { deg = 1; }
	else if (deg == 180) { deg = -1; }
	else { deg = std::cos(deg); }

	float magA = A.Magnitude(), magB = B.Magnitude();
	return magA * magB * deg;
}

float Vector3::Dot(Vector3 A, Vector3 B, float rad) {

	// angle in radiant  ==  cosTeta;

	float magA = A.Magnitude(), magB = B.Magnitude();
	return magA * magB * rad;
}

Vector3 Vector3::Cross(Vector3 A, Vector3 B) {
	float Cx = A.y * B.z - A.z * B.y;
	float Cy = A.z * B.x - A.x * B.z;
	float Cz = A.x * B.y - A.y * B.x;

	return Vector3(Cx, Cy, Cz);
}

//// ------------- Overrides -------------

Vector3 Vector3::operator+(Vector3& other) {
	return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator-(Vector3& other) {
	return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator*(float& other) {
	return  Vector3(x * other, y * other, z * other);
}

Vector3 Vector3::operator/(float& other) {
	return  Vector3(x / other, y / other, z / other);
}

std::ostream& operator<<(std::ostream& os, const Vector3& A) {
	os << "(" << A.x << ", " << A.y << ", " << A.z << ")";
	return os;
}


#pragma endregion



#pragma region Point_Class

Point::Point(float x, float y, float z):Vector3(x,y,z){}

//// ------------- Statcic -------------


float Point::Distance(Point A, Point B) {
	//Eucledian
	return std::sqrt((B.x - A.x)* (B.x - A.x) + (B.y - A.y)* (B.y - A.y) + (B.z - A.z)* (B.z - A.z));
}

//// ------------- Overrides -------------

bool Point::operator==(Point& B) {
	if (x != B.x || std::abs(x - B.x) > 0.00001) { return false; }
	if (y != B.y || std::abs(y - B.y) > 0.00001) { return false; }
	if (z != B.z || std::abs(z - B.z) > 0.00001) { return false; }

	return true;
}

#pragma endregion

#pragma region Direction_Class

Direction::Direction(Point A) : Vector3(Init()) {}


Vector3 Direction::Init() {
	//Vector3 result = A / A.Magnitude();
	return Vector3::Zero;
}


Point Direction::GetPoint() {
	return this->A;
}



#pragma endregion


#pragma region Color_Class

Color::Color(float x, float y, float z) :Vector3(x, y, z) {}

//// ------------- Overrides -------------

bool Color::operator==(Color& B) {
	if (x != B.x || std::abs(x - B.x) > 0.00001) { return false; }
	if (y != B.y || std::abs(y - B.y) > 0.00001) { return false; }
	if (z != B.z || std::abs(z - B.z) > 0.00001) { return false; }

	return true;
}


#pragma endregion

	 