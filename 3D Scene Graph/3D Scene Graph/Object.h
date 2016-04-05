/*
Struct for scene objects
Author:		Aaron Rosenberger
Date due:	April 1, 2016
File:		Object.h
*/

#pragma once

#include <iostream>
#include <string>
#include "glm\gtx\transform.hpp"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::translate;
using glm::rotate;
using glm::scale;
using std::string;
using std::cout;
using std::endl;

#define NO_TRANSLATE 0
#define NO_ROTATE 0
#define NO_SCALE 1

struct Geometry
{
	string geoType;

	Geometry()
	{
		geoType = "Cube";
	}

	Geometry(const string& geo)
	{
		geoType = geo;
	}
};

struct Transformation
{
	int x_index, z_index;
	int stack;	// y_index
	float rotation;
	float x_scale, y_scale, z_scale;

	Geometry* geo;

	Transformation()
	{
		geo = nullptr;
		x_index = NO_TRANSLATE;
		z_index = NO_TRANSLATE;
		stack = 0;
		rotation = NO_ROTATE;
		x_scale = NO_SCALE;
		y_scale = NO_SCALE;
		z_scale = NO_SCALE;
	}

	~Transformation() {}

	mat4 translationMatrix();
	mat4 rotationMatrix();
	mat4 scaleMatrix();
	void print();
	bool isEmpty();
};

mat4 Transformation::translationMatrix()
{
	if (x_index == NO_TRANSLATE && z_index == NO_TRANSLATE) {
		return mat4();	// Identity matrix
	}
	return translate(vec3(x_index, 0, z_index));
}

mat4 Transformation::rotationMatrix()
{
	if (rotation == NO_ROTATE) {
		return mat4();
	}
	return rotate(rotation, vec3(0,1,0));
}

mat4 Transformation::scaleMatrix()
{
	if (x_scale == NO_SCALE && y_scale == NO_SCALE && z_scale == NO_SCALE) {
		return mat4();
	}
	return scale(vec3(x_scale, y_scale, z_scale));
}

void Transformation::print()
{
	//if (geo != nullptr) cout << geo->geoType << endl;
	cout << x_index << " " << z_index << " stack: " << stack << endl;
	cout << rotation << endl;
	cout << x_scale << " " << y_scale << " " << z_scale << endl;
}

bool Transformation::isEmpty()
{
	return (geo == nullptr &&
		x_index == 0 && z_index == 0 &&
		rotation == 0 && 
		x_scale == 1 && y_scale == 1 && z_scale == 1);
}