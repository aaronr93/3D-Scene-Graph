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
vec4* getCube();
vec4* getNewCube(vec4*);

class Node
{
public:
	Node() {}
	~Node() {}
	void traverse(mat4 matrix);
	void printGraph();
	void addChild(Transformation);
private:
	Transformation transformation;
	vector<Node> children;
	mat4 transform;
	vec4* cube;
	vec4* finishedCube;

	void addNewChild(Transformation);
	void moveUnsharedTransformations(TransformationType);
	void findLeaves();
};

void Node::traverse(mat4 matrix)
{
	mat4 t = transformation.translationMatrix();
	mat4 r = transformation.rotationMatrix();
	mat4 s = transformation.scaleMatrix();
	transform *= matrix * t * r * s;
	for (int i = 0; i < children.size(); i++) {
		children[i].traverse(transform);
		cube = getCube();
		for (int j = 0; j < 24; j++) {
			cube[j] = transform * cube[j];
			//cout << "(" << cube[j].x << "," << cube[j].y << "," << cube[j].z << ")\n";
		}
		finishedCube = getNewCube(cube);
		cout << "** " << "Cube"/*transformation.geo->geoType*/ << " **\n";
		for (int j = 0; j < 8; j++) {
			cout << "Point" << j << ": (" << finishedCube[j].x << ", " << finishedCube[j].y << ", " << finishedCube[j].z << ")\n";
		}
		cout << "**********\n\n";
	}
}

void Node::printGraph()
{
	transformation.print();
	cout << "==================\n\t||\n\t\\/\n";
	if (children.empty()) return;
	for (int i = 0; i < children.size(); i++) {
		cout << "Iteration : " << i << endl;
		children[i].printGraph();
		cout << "==================\n";
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
			toAdd.x_index = NO_TRANSLATE;
			toAdd.z_index = NO_TRANSLATE;
			toAdd.stack = 1;	// Stack the cube on the cube below it
			children[i].addChild(toAdd);	
			return;

		} else if (matches(ROTATE, toAdd, children[i].transformation)) {
			// The geometry shares a rotation; add child

			logMessage("Transformation toAdd's ROTATE already exists; adding child.");

			moveUnsharedTransformations(ROTATE);
			toAdd.rotation = NO_ROTATE;
			children[i].addChild(toAdd);
			return;

		} else if (matches(SCALE, toAdd, children[i].transformation)) {
			// The geometry shares a scale; add child

			logMessage("Transformation toAdd's SCALE already exists; adding child.");

			moveUnsharedTransformations(SCALE);
			toAdd.x_scale = NO_SCALE;
			toAdd.y_scale = NO_SCALE;
			toAdd.z_scale = NO_SCALE;
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
		return (a.x_index != NO_TRANSLATE) && (b.x_index != NO_TRANSLATE) && 
			(a.z_index != NO_TRANSLATE) && (b.z_index != NO_TRANSLATE) &&
			(a.x_index == b.x_index) && (a.z_index == b.z_index);
		break;
	case ROTATE:
		return (a.rotation != NO_ROTATE && b.rotation != NO_ROTATE) &&
			(a.rotation == b.rotation);
		break;
	case SCALE:
		return (a.x_scale != NO_SCALE) && (b.x_scale != NO_SCALE) &&
			(a.y_scale != NO_SCALE) && (b.y_scale != NO_SCALE) &&
			(a.z_scale != NO_SCALE) && (b.z_scale != NO_SCALE) &&
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

		transformation.rotation = NO_ROTATE;
		transformation.x_scale = NO_SCALE;
		transformation.y_scale = NO_SCALE;
		transformation.z_scale = NO_SCALE;

		addNewChild(transfer);

		break;
	case ROTATE:

		transfer.x_index = transformation.x_index;
		transfer.z_index = transformation.z_index;
		transfer.x_scale = transformation.x_scale;
		transfer.y_scale = transformation.y_scale;
		transfer.z_scale = transformation.z_scale;

		transformation.x_index = NO_TRANSLATE;
		transformation.z_index = NO_TRANSLATE;
		transformation.x_scale = NO_SCALE;
		transformation.y_scale = NO_SCALE;
		transformation.z_scale = NO_SCALE;

		addNewChild(transfer);

		break;
	case SCALE:

		transfer.x_index = transformation.x_index;
		transfer.z_index = transformation.z_index;
		transfer.rotation = transformation.rotation;
		
		transformation.x_index = NO_TRANSLATE;
		transformation.z_index = NO_TRANSLATE;
		transformation.rotation = NO_ROTATE;

		addNewChild(transfer);

		break;
	}
}

void Node::findLeaves()
{
	if (children.empty()) {
		transformation.geo = new Geometry;
	}
	for (int i = 0; i < children.size(); i++) {
		children[i].findLeaves();
	}
}

void logMessage(const string& message)
{
	cout << "[scene] " << message << endl;
}

vec4* getCube()
{
	vec4* points = new vec4[24];

	//front face
	points[0] = vec4( 0.75f,  0.75f, 0.75f, 1.0f);
	points[1] = vec4(-0.75f,  0.75f, 0.75f, 1.0f);
	points[2] = vec4(-0.75f, -0.75f, 0.75f, 1.0f);
	points[3] = vec4( 0.75f, -0.75f, 0.75f, 1.0f);

	//back face
	points[4] = vec4( 0.75f,  0.75f, -0.75f, 1.0f);
	points[5] = vec4( 0.75f, -0.75f, -0.75f, 1.0f);
	points[6] = vec4(-0.75f, -0.75f, -0.75f, 1.0f);
	points[7] = vec4(-0.75f,  0.75f, -0.75f, 1.0f);

	//right face
	points[8] =  vec4(0.75f,  0.75f, -0.75f, 1.0f);
	points[9] =  vec4(0.75f,  0.75f,  0.75f, 1.0f);
	points[10] = vec4(0.75f, -0.75f,  0.75f, 1.0f);
	points[11] = vec4(0.75f, -0.75f, -0.75f, 1.0f);

	//left face
	points[12] = vec4(-0.75f,  0.75f,  0.75f, 1.0f);
	points[13] = vec4(-0.75f,  0.75f, -0.75f, 1.0f);
	points[14] = vec4(-0.75f, -0.75f, -0.75f, 1.0f);
	points[15] = vec4(-0.75f, -0.75f,  0.75f, 1.0f);

	//top face
	points[16] = vec4( 0.75f, 0.75f, -0.75f, 1.0f);
	points[17] = vec4(-0.75f, 0.75f, -0.75f, 1.0f);
	points[18] = vec4(-0.75f, 0.75f,  0.75f, 1.0f);
	points[19] = vec4( 0.75f, 0.75f,  0.75f, 1.0f);

	//bottom face
	points[20] = vec4( 0.75f, -0.75f,  0.75f, 1.0f);
	points[21] = vec4(-0.75f, -0.75f,  0.75f, 1.0f);
	points[22] = vec4(-0.75f, -0.75f, -0.75f, 1.0f);
	points[23] = vec4( 0.75f, -0.75f, -0.75f, 1.0f);

	return points;
}

vec4* getNewCube(vec4* cube)
{
	vec4* newCube = new vec4[8];

	newCube[0] = cube[0];
	newCube[1] = cube[3];
	newCube[2] = cube[1];
	newCube[3] = cube[2];
	newCube[4] = cube[4];
	newCube[5] = cube[5];
	newCube[6] = cube[7];
	newCube[7] = cube[6];

	return newCube;
}