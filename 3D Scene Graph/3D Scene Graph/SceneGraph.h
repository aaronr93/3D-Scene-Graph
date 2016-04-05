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
	void traverse();
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

void SceneGraph::traverse()
{
	graph->traverse(mat4());
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
