#ifndef SIMPLEMODEL_H
#define SIMPLEMODEL_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <algorithm>
#include <GL/glut.h>
#include "PPM.hpp"

using namespace std;

typedef struct		// Vector Structure, for x, y, z vertices
{
	float x;
	float y;
	float z;
}
Vector;

typedef struct		// Triangle Structure, for 3 vertices combined
{
	int vert1; 
	int vert2; 
	int vert3;

	int norm1;
	int norm2;
	int norm3;

	int tex;

	float tex1x;
	float tex1y;
	float tex2x;
	float tex2y;
	float tex3x;
	float tex3y;
}
Triangle;

class simpleModel
{
	public:

		int simpleModel::Load(char *inFile);	// Method to load the .model file
		void simpleModel::Create();				// Outputs the model to the screen
		GLuint simpleModel::loadTexture(string inString, int width, int height);		// Sets up Textures

		vector<Vector> vertices;				// Vector Variables
		vector<Triangle> triangles;
		vector<Vector> normals;
		vector<string> texArray;

		int numTex;								// stores the number of textures before making the number constant
		GLuint texture;							// Texture
		GLuint *textureNames;					// Array for Textures

		string fullPath;						// the full input path of the model file: "path/model.model"
		string modelPath;						// the path, minus the filename: "path/"
		string base;							// loadTexture input String Processing
		string texFile;							// loadTexture input String Processing
		unsigned char * ppmData;				// loadTexture parsed .ppm file data

		char lineChar1, lineChar2, lineChar3;	// Used to find sections while parsing lines

		float vFloat1, vFloat2, vFloat3;		// Used to temporarily store Vertex Data

		int vecInd1, vecInd2, vecInd3;			// Used to temporarily store Triangle Vertex Indices

		int normInd;							// Used to temporarily store the Normal Index
		int texInd;								// Used to temporarily store the Texture Index

		float tex1;								// Used to temporarily store the Texture Coords
		float tex2;
		float tex3;
		float tex4;
		float tex5;
		float tex6;

};


#endif