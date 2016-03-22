/*
Struct for a scene object
Author:		Aaron Rosenberger
Date due:	April 1, 2016
File:		Object.h
*/

#include <string>
using std::string;

struct Object
{
	string geoType;
	int x_index, z_index;
	float rotation;
	float x_scale, y_scale, z_scale;
};
