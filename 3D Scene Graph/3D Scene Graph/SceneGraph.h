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
#include <vector>
#include "Object.h"
#include "Node.h"
using std::ifstream;
using std::string;

#define NO_ITEM_FLAG -1

class SceneGraph
{
public:
	SceneGraph();
	~SceneGraph();
	void printGraph();
	// Method for parsing a config file. 
	static SceneGraph* factory(ifstream&, const string& = "");
private:
	int x_size, z_size;

	Node* graph;
};

inline SceneGraph::SceneGraph()
{
	graph = new Node;
}

inline SceneGraph::~SceneGraph()
{
	delete graph;
}

void SceneGraph::printGraph()
{
	graph->printGraph();
}

SceneGraph* SceneGraph::factory(ifstream& fin, const string& fileName)
{
	bool shouldCloseFilestreamAtEnd = false;
	if (fileName != "") {
		// A filename has been specified; open a new file stream with it
		fin.open(fileName);
		shouldCloseFilestreamAtEnd = true;
	}

	// Keep track of how much more there is to parse.
	int numItems = 0;
	int currentNumItems = 0;

	// The scene graph object we parse into and finally return.
	SceneGraph* scene = new SceneGraph;

	fin >> scene->x_size;
	fin >> scene->z_size;
	fin >> numItems;

	// Translate the floor so the origin is in the center
	Transformation centerFloor;
	centerFloor.x_index = scene->x_size / 2;
	centerFloor.z_index = scene->z_size / 2;
	scene->graph->addChild(centerFloor);	// This becomes the root node

	// Parse all the objects
	while (!fin.eof() && !fin.fail() && currentNumItems < numItems) {
		cout << "REMAINING: " << numItems - currentNumItems - 1 << "\r";
		cout.flush();

		Transformation temp;

		string geo;
		fin >> geo;
		temp.geo = new Geometry(geo);

		fin >> temp.x_index;
		fin >> temp.z_index;
		fin >> temp.rotation;
		fin >> temp.x_scale;
		fin >> temp.y_scale;
		fin >> temp.z_scale;

		scene->graph->addChild(temp);

		++currentNumItems;

	}

	//Geometry* cube = new Geometry;

	if (shouldCloseFilestreamAtEnd) {
		fin.clear();
		fin.close();
	}

	return scene;
}

void cube()
{
	vec4* points = new vec4[24];
	vec3* colors = new vec3[24];

	//front face
	points[0] = vec4(0.75f, 0.75f, 0.75f, 1.0f);
	points[1] = vec4(-0.75f, 0.75f, 0.75f, 1.0f);
	points[2] = vec4(-0.75f, -0.75f, 0.75f, 1.0f);
	points[3] = vec4(0.75f, -0.75f, 0.75f, 1.0f);

	colors[0] = vec3(1.0f, 0.0f, 0.0f);
	colors[1] = vec3(1.0f, 0.0f, 0.0f);
	colors[2] = vec3(1.0f, 0.0f, 0.0f);
	colors[3] = vec3(1.0f, 0.0f, 0.0f);

	//back face
	points[4] = vec4(0.75f, 0.75f, -0.75f, 1.0f);
	points[5] = vec4(0.75f, -0.75f, -0.75f, 1.0f);
	points[6] = vec4(-0.75f, -0.75f, -0.75f, 1.0f);
	points[7] = vec4(-0.75f, 0.75f, -0.75f, 1.0f);

	colors[4] = vec3(1.0f, 0.0f, 0.0f);
	colors[5] = vec3(1.0f, 0.0f, 0.0f);
	colors[6] = vec3(1.0f, 0.0f, 0.0f);
	colors[7] = vec3(1.0f, 0.0f, 0.0f);

	//right face
	points[8] = vec4(0.75f, 0.75f, -0.75f, 1.0f);
	points[9] = vec4(0.75f, 0.75f, 0.75f, 1.0f);
	points[10] = vec4(0.75f, -0.75f, 0.75f, 1.0f);
	points[11] = vec4(0.75f, -0.75f, -0.75f, 1.0f);

	colors[8] = vec3(0.0f, 0.8f, 0.0f);
	colors[9] = vec3(0.0f, 0.8f, 0.0f);
	colors[10] = vec3(0.0f, 0.8f, 0.0f);
	colors[11] = vec3(0.0f, 0.8f, 0.0f);

	//left face
	points[12] = vec4(-0.75f, 0.75f, 0.75f, 1.0f);
	points[13] = vec4(-0.75f, 0.75f, -0.75f, 1.0f);
	points[14] = vec4(-0.75f, -0.75f, -0.75f, 1.0f);
	points[15] = vec4(-0.75f, -0.75f, 0.75f, 1.0f);

	colors[12] = vec3(0.0f, 0.8f, 0.0f);
	colors[13] = vec3(0.0f, 0.8f, 0.0f);
	colors[14] = vec3(0.0f, 0.8f, 0.0f);
	colors[15] = vec3(0.0f, 0.8f, 0.0f);

	//top face
	points[16] = vec4(0.75f, 0.75f, -0.75f, 1.0f);
	points[17] = vec4(-0.75f, 0.75f, -0.75f, 1.0f);
	points[18] = vec4(-0.75f, 0.75f, 0.75f, 1.0f);
	points[19] = vec4(0.75f, 0.75f, 0.75f, 1.0f);

	colors[16] = vec3(0.0f, 0.0f, 1.0f);
	colors[17] = vec3(0.0f, 0.0f, 1.0f);
	colors[18] = vec3(0.0f, 0.0f, 1.0f);
	colors[19] = vec3(0.0f, 0.0f, 1.0f);

	//bottom face
	points[20] = vec4(0.75f, -0.75f, 0.75f, 1.0f);
	points[21] = vec4(-0.75f, -0.75f, 0.75f, 1.0f);
	points[22] = vec4(-0.75f, -0.75f, -0.75f, 1.0f);
	points[23] = vec4(0.75f, -0.75f, -0.75f, 1.0f);

	colors[20] = vec3(0.0f, 0.0f, 1.0f);
	colors[21] = vec3(0.0f, 0.0f, 1.0f);
	colors[22] = vec3(0.0f, 0.0f, 1.0f);
	colors[23] = vec3(0.0f, 0.0f, 1.0f);

	delete[] points;
	delete[] colors;
}