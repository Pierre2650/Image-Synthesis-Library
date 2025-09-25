// Synthese3D.cpp : Définit les fonctions de la bibliothèque statique.
//
#include "pch.h"
#include "framework.h"
#include "Synthese3D.h"
#include <iostream>

#pragma region Vector3_Class

/// ------------- Var -------------
Vector3 const Vector3::Zero(0, 0, 0);
Vector3 const Vector3::One(1, 1, 1);
Vector3 const Vector3::NaN(NAN, NAN, NAN);

/// ------------- Construct -------------
Vector3::Vector3(const float x, const float y, const float z) {
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

float Vector3::Magnitude() const {
	return std::sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
}

Vector3 Vector3::Normalized() const {
	float mag = this->Magnitude();
	
	if (mag == 0) { return Vector3::NaN; }

	return Vector3(this->x / mag, this->y / mag, this->z / mag);

}


//// ------------- Static -------------

bool Vector3::AlmostEqual(const Vector3 A, const Vector3 B) {

	if (Vector3::IsNaN(A) || Vector3::IsNaN(A)) { return false;  }
	if (std::abs(A.x - B.x) > 0.00001) { return false; }
	if (std::abs(A.y - B.y) > 0.00001) { return false; }
	if (std::abs(A.z - B.z) > 0.00001) { return false; }

	return true;
}

bool Vector3::IsNaN(const Vector3 A) {
	if (std::isnan(A.x) && std::isnan(A.y) && std::isnan(A.y)) { return true; }
	else { return false;}
}

float Vector3::Angle(const Vector3 A, const Vector3 B) {
	// In radians
	float magA = A.Magnitude(), magB = B.Magnitude();
	float dot = Vector3::Dot(A, B);
		
	return  dot / (magA * magB);
}

float Vector3::Dot(const Vector3 A, const Vector3 B) {
	
	return A.x * B.x + A.y * B.y + A.z * B.z;

}


Vector3 Vector3::Cross(const Vector3 A, const Vector3 B) {
	float Cx = A.y * B.z - A.z * B.y;
	float Cy = A.z * B.x - A.x * B.z;
	float Cz = A.x * B.y - A.y * B.x;

	return Vector3(Cx, Cy, Cz);
}

//// ------------- Overrides -------------

Vector3 Vector3::operator+(const Vector3& other) const {
	return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator-(const Vector3& other) const {
	return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator*(const float& other) const {
	return  Vector3(x * other, y * other, z * other);
}

Vector3 Vector3::operator/(const float& other) const {
	return  Vector3(x / other, y / other, z / other);
}

std::ostream& operator<<(std::ostream& os, const Vector3& A) {
	os << "(" << A.x << ", " << A.y << ", " << A.z << ")";
	return os;
}


#pragma endregion



#pragma region Point_Class

Point::Point(const float x, const float y, const float z):Vector3(x,y,z){}

//// ------------- Statcic -------------


float Point::Distance(const Point A, const Point B) {
	//Eucledian
	return std::sqrt((B.x - A.x)* (B.x - A.x) + (B.y - A.y)* (B.y - A.y) + (B.z - A.z)* (B.z - A.z));
}

//// ------------- Overrides -------------


#pragma endregion

#pragma region Direction_Class

Direction::Direction(const Point A) :  Vector3(Init(A)), P(A) {}

Direction::Direction(const Direction& B) : Vector3(B), P(B.GetPoint()){}


Vector3 Direction::Init(const Point A) {
	Vector3 result = A / A.Magnitude();
	return result;
}


Point Direction::GetPoint() const {
	return this->P;
}


std::ostream& operator<<(std::ostream& os, const Direction& A) {
	os << "(" << A.x << ", " << A.y << ", " << A.z << ")" << " Point: " << A.GetPoint();
	return os;
}



#pragma endregion


#pragma region Color_Class

Color::Color(float x, float y, float z,  float t) : Vector3(x, y, z) {
	this->Transparency = t;
}

//// ------------- Overrides -------------

std::ostream& operator<<(std::ostream& os, const Color& A) {
	os << "(" << A.x << ", " << A.y << ", " << A.z << ", " << A.Transparency << ")";
	return os;
}

#pragma endregion

	 