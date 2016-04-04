/*
Class for nodes in Scene Graph
Author:		Aaron Rosenberger
Date due:	April 1, 2016
File:		Node.h
*/

#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include "Object.h"
#include "glm\gtx\transform.hpp"
#include "glm\glm.hpp"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using std::string;
using std::vector;
using std::cout;
using std::endl;

enum TransformationType { TRANSLATE, ROTATE, SCALE };

bool matches(TransformationType, Transformation, Transformation);
void logMessage(const string&);

class Node
{
public:
	Node();
	~Node();
	void traverse(mat4 matrix);
	void printGraph();
	void addChild(Transformation);
private:
	Transformation transformation;
	vector<Node> children;

	void addNewChild(Transformation);
	void moveUnsharedTransformations(TransformationType);
};

Node::Node()
{}

Node::~Node()
{}

void Node::traverse(mat4 matrix)
{
	
}

void Node::printGraph()
{
	transformation.print();
	cout << "\n\t||\n\t\\/\n\n";
	if (children.empty()) return;
	for (int i = 0; i < children.size(); i++) {
		cout << "Iteration : " << i << endl;
		children[i].printGraph();
		cout << "\n==================\n\n";
	}
}

void Node::addChild(Transformation toAdd)
{
	if (transformation.isEmpty()) {
		// We're at the head (main scene), so the transformation
		//   must be the translation of the scene's origin.
		transformation = toAdd;
		return;
	}

	if (children.empty()) {
		logMessage("No children exist for the current node; adding one.");
		addNewChild(toAdd);
		return;
	}

	for (int i = 0; i < children.size(); ++i) {
		// For each child of the current node, check if we need to
		//   recursively add a child to a child, or just add a child

		if (matches(TRANSLATE, toAdd, children[i].transformation)) {
			// The geometry is stacked; add child, change its Y

			logMessage("Transformation toAdd's TRANSLATE already exists; adding child.");

			moveUnsharedTransformations(TRANSLATE);
			toAdd.x_index = 0;
			toAdd.z_index = 0;
			toAdd.stack = 1;	// Stack the cube on the cube below it
			children[i].addChild(toAdd);	
			return;

		} else if (matches(ROTATE, toAdd, children[i].transformation)) {
			// The geometry shares a rotation; add child

			logMessage("Transformation toAdd's ROTATE already exists; adding child.");

			moveUnsharedTransformations(ROTATE);
			toAdd.rotation = 0;
			children[i].addChild(toAdd);
			return;

		} else if (matches(SCALE, toAdd, children[i].transformation)) {
			// The geometry shares a scale; add child

			logMessage("Transformation toAdd's SCALE already exists; adding child.");

			moveUnsharedTransformations(SCALE);
			toAdd.x_scale = 0;
			toAdd.y_scale = 0;
			toAdd.z_scale = 0;
			children[i].addChild(toAdd);
			return;

		}

	}

	// If this code is reached, then another child needs to be added to the current node.
	logMessage("This transformation is unique to the current node's children; appending child to current node.");
	addNewChild(toAdd);
	return;

}

bool matches(TransformationType type, Transformation a, Transformation b)
{
	switch (type) {
	case TRANSLATE:
		return (a.x_index != 0) && (b.x_index != 0) && (a.z_index != 0) && (b.z_index != 0) &&
			(a.x_index == b.x_index) && (a.z_index == b.z_index);
		break;
	case ROTATE:
		return (a.rotation != 0 && b.rotation != 0) && (a.rotation == b.rotation);
		break;
	case SCALE:
		return (a.x_scale != 1) && (b.x_scale != 1) &&
			(a.y_scale != 1) && (b.y_scale != 1) &&
			(a.z_scale != 1) && (b.z_scale != 1) &&
			(a.x_scale == b.x_scale) &&
			(a.y_scale == b.y_scale) &&
			(a.z_scale == b.z_scale);
		break;
	}
}

void Node::addNewChild(Transformation toAdd)
{
	Node child;
	child.transformation = toAdd;
	children.push_back(child);
}

// When a node gains a child, it passes off the transformations that it doesn't share with its child, to a new child.
void Node::moveUnsharedTransformations(TransformationType sharedTransformation)
{
	Transformation transfer;

	switch (sharedTransformation) {
	case TRANSLATE:

		transfer.rotation = transformation.rotation;
		transfer.x_scale = transformation.x_scale;
		transfer.y_scale = transformation.y_scale;
		transfer.z_scale = transformation.z_scale;

		transformation.rotation = 0;
		transformation.x_scale = 1;
		transformation.y_scale = 1;
		transformation.z_scale = 1;

		addNewChild(transfer);

		break;
	case ROTATE:

		transfer.x_index = transformation.x_index;
		transfer.z_index = transformation.z_index;
		transfer.x_scale = transformation.x_scale;
		transfer.y_scale = transformation.y_scale;
		transfer.z_scale = transformation.z_scale;

		transformation.x_index = 0;
		transformation.z_index = 0;
		transformation.x_scale = 1;
		transformation.y_scale = 1;
		transformation.z_scale = 1;

		addNewChild(transfer);

		break;
	case SCALE:

		transfer.x_index = transformation.x_index;
		transfer.z_index = transformation.z_index;
		transfer.rotation = transformation.rotation;
		
		transformation.x_index = 0;
		transformation.z_index = 0;
		transformation.rotation = 0;

		addNewChild(transfer);

		break;
	}
}

void logMessage(const string& message)
{
	cout << "[scene] " << message << endl;
}