// Synthese3D.cpp : Définit les fonctions de la bibliothèque statique.
//
#include "pch.h"
#include "framework.h"
#include "Synthese3D.h"


// Vector3 Class

/// Var
Vector3 Vector3::Zero(0, 0, 0);
Vector3 Vector3::One(1, 1, 1);

///Construct
Vector3::Vector3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

///Methods
Vector3 Vector3::operator+(Vector3 other) {
	return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator-(Vector3 other) {
	return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::Dot(Vector3 A, Vector3 B) {

}

Vector3 Vector3::Cross(Vector3 A, Vector3 B) {

}

//Point Class

float Point::Distance(Point A, Point B) {

}


//Direction Class

Direction::Direction()
{
}

Direction::~Direction()
{
}
	 