/*
Class for creating and manipulating a scene graph
Author:		Aaron Rosenberger
Date due:	April 1, 2016
File:		SceneGraph.h
*/

#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include "Object.h"
using std::ifstream;
using std::string;

class SceneGraph
{
public:

	// Method for parsing a config file. 
	static SceneGraph* factory(ifstream&, const string& = "");
private:
	int x_size, z_size;

	Object* objects;
};