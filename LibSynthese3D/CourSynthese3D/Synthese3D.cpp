// Synthese3D.cpp : Définit les fonctions de la bibliothèque statique.
//
#include "pch.h"
#include "framework.h"
#include "Synthese3D.h"
#include <cmath>
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

/// ------------- Methods -------------

float Vector3::Magnitude() {
	return sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z);
}
Vector3 Vector3::Normalized() {
	float mag = this->Magnitude();
	
	if (mag == 0) { return this->Zero };

	return Vector3(this->x / mag, this->y / mag, this->z / mag);

}


//// ------------- Static -------------

bool Vector3::Equals(Vector3 A, Vector3 B) {
	if (A.x != B.x || abs(A.x - B.x) > 0.00001) { return false; }
	if (A.y != B.y || abs(A.y - B.y) > 0.00001) { return false; }
	if (A.z != B.z || abs(A.z - B.z) > 0.00001) { return false; }

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

float Vector3::Dot(Vector3 A, Vector3 B, int angle) {

	// angle in degrees ;
	float rads;
	if (angle == 90f) { return 0; }
	else if (angle = 0f) { rads = 1; }
	else if (angle = 180f) { rads = -1; }
	else { rads = cos(angle) }

	float magA = A.Magnitude(), magB = B.Magnitude();
	return magA * magB * rads;
}

float Vector3::Dot(Vector3 A, Vector3 B, float angle) {

	// angle in radiant  ==  cosTeta;

	float magA = A.Magnitude(), magB = B.Magnitude();
	return magA * magB * angle;
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

ostream& Vector3::operator<<(ostream& os, Vector3& A) {
	std::os << "(" << A.x << ", " << A.y << ", " << A.z << ")";
	return std::os;
}


#pragma endregion


#pragma region Point_Class

//// ------------- Statcic -------------

float Point::Distance(Point A, Point B) {
	//Eucledian
	return sqrt((B.x - A.x)* (B.x - A.x) + (B.y - A.y)* (B.y - A.y) + (B.z - A.z)* (B.z - A.z));
}

//// ------------- Overrides -------------

bool Point::operator==(Point& B) {
	if (x != B.x || abs(x - B.x) > 0.00001) { return false; }
	if (y != B.y || abs(y - B.y) > 0.00001) { return false; }
	if (z != B.z || abs(z - B.z) > 0.00001) { return false; }

	return true;
}

#pragma endregion

#pragma region Direction_Class

Direction::Direction(Point A, Point B) {
	this->A = A;
	this->B = B;
}


Point Direction::GetPointA() {
	return this->A;
}
Point Direction::GetPointB() {
	return this->B;
}


#pragma endregion


#pragma region Color_Class



#pragma endregion

	 