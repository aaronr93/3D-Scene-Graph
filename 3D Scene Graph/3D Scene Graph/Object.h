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

struct Geometry
{
	string geoType;

	Geometry()
	{
		geoType = "cube";
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
		x_index = 0;
		z_index = 0;
		stack = 0;
		rotation = 0;
		x_scale = 1;
		y_scale = 1;
		z_scale = 1;
	}

	/*~Transformation()
	{
		if (geo != nullptr) delete geo;
	}*/

	mat4 translationMatrix();
	mat4 rotationMatrix();
	mat4 scaleMatrix();
	void print();
	bool isEmpty();
};

mat4 Transformation::translationMatrix()
{
	return translate(vec3(x_index, 0, z_index));
}

mat4 Transformation::rotationMatrix()
{
	return rotate(rotation, vec3(0,1,0));
}

mat4 Transformation::scaleMatrix()
{
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