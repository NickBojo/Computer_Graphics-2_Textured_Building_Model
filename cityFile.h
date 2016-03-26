#ifndef CITYFILE_H
#define CITYFILE_H

#include "simpleModel.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <GL/glut.h>

using namespace std;

typedef struct		// Vector Structure, for each building in the city
{
	char* buildingPath;	// Building path

	float Sx;				// Scaling
	float Sy;
	float Sz;

	float Rx;				// Rotation
	float Ry;
	float Rz;

	float Ox;				// Translation from Model to Origin
	float Oy;
	float Oz;

	float Tx;				// Translation from Model to World
	float Ty;
	float Tz;
}
Building;

class cityFile
{
	public:

		int cityFile::loadCity(char *inCity);		// Load and Parse the .city file
		void cityFile::prepModels();				// Load and Prepare the Model Files from .city specs
		void cityFile::createCity();				// Generate the City
	
		vector<Building> buildings;					// Building Vector Variable
		vector<simpleModel> buildingVector;

		int modelTotal;								// Total amount of models used

		int tempPathLength;
		float tempSx;
		float tempSy;
		float tempSz;
		float tempRx;
		float tempRy;
		float tempRz;
		float tempOx;
		float tempOy;
		float tempOz;
		float tempTx;
		float tempTy;
		float tempTz;
		int tempModelTotal;

		simpleModel *buildingArray;				// Variables for the creation of models
		simpleModel newModel;
};

#endif