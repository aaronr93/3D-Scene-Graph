/*
Main - starts everything
Author:		Aaron Rosenberger
Date due:	April 1, 2016
File:		main.cpp
*/

#include "SceneGraph.h"

int main(int argc, char* argv[])
{
	ifstream fin;
	SceneGraph* sceneGraph = SceneGraph::factory(fin, "scene2.txt");
	sceneGraph->printGraph();
	return 0;
}