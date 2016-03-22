/*
Methods for creating and manipulating a scene graph
Author:		Aaron Rosenberger
Date due:	April 1, 2016
File:		SceneGraph.cpp
*/

#include "SceneGraph.h"
using std::cout;
using std::endl;

Object* parseObject(ifstream& fin)
{
	Object* object = new Object;
	
	fin >> object->geoType;
	fin >> object->x_index;
	fin >> object->z_index;
	fin >> object->rotation;
	fin >> object->x_scale;
	fin >> object->y_scale;
	fin >> object->z_scale;

	return object;
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
	SceneGraph* sceneGraph = new SceneGraph;

	// X size
	fin >> sceneGraph->x_size;
	// Z size
	fin >> sceneGraph->z_size;
	// Number of items
	fin >> numItems;

	// Declare all the objects we're about to parse
	sceneGraph->objects = new Object[numItems];
	Object* objects = sceneGraph->objects;

	// Parse all the objects
	while (!fin.eof() && !fin.fail() && currentNumItems < numItems) {
		cout << "REMAINING: " << numItems - currentNumItems - 1 << "\r";
		cout.flush();

		objects[currentNumItems] = *parseObject(fin);
		++currentNumItems;

	}

	if (shouldCloseFilestreamAtEnd) {
		fin.clear();
		fin.close();
	}

	return sceneGraph;
}
