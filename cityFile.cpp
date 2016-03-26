#include "cityFile.h"

int cityFile::loadCity(char *inCity)
{
	fstream inCityFile;			// Start File Streaming
	inCityFile.open(inCity);	

	if (inCityFile.is_open())    // If File Opened correctly, begin passing through it
	{
		char currentLine[255];		// Char array for containing the contents of a line of text
		while(inCityFile.good())
		{
			inCityFile.getline(currentLine, 255);	// Get the first line of the file
			
			if(strlen(currentLine) == 0)
			{
				continue;							// If the line is empty, don't parse it
			}

			if (sscanf(currentLine, " %d", &tempModelTotal) == 1)	// Single Integer refers to the total number of models
			{ 
				modelTotal = tempModelTotal;
			}

			if (sscanf(currentLine, " %*s %f %f %f %f %f %f %f %f %f %f %f %f", &tempSx, &tempSy, &tempSz, &tempRx, &tempRy, &tempRz, &tempOx, &tempOy, &tempOz, &tempTx, &tempTy, &tempTz) == 12)	// Once able to parse, begin parsing
			{  
				// Copy, Format, and Trim the line to place the model file path in a variable
				tempPathLength = strcspn(currentLine,".")+6;
				char* tempPath = new char[tempPathLength];
				tempPath[tempPathLength]='\0';
				strncpy(tempPath, currentLine, tempPathLength);

				buildings.push_back(Building());        // Add new Building to the Vector

				buildings.back().buildingPath = tempPath;

				buildings.back().Sx = tempSx;
				buildings.back().Sy = tempSy;
				buildings.back().Sz = tempSz;

				buildings.back().Rx = tempRx;
				buildings.back().Ry = tempRy;
				buildings.back().Rz = tempRz;

				buildings.back().Ox = tempOx;
				buildings.back().Oy = tempOy;
				buildings.back().Oz = tempOz;

				buildings.back().Tx = tempTx;
				buildings.back().Ty = tempTy;
				buildings.back().Tz = tempTz;
			}
		}

		inCityFile.close();
	}

	else
	{
		cout << "Unable to open .city file.";
		return 0;
	}

	prepModels();			// After parsing the city file, load the model data into memory

	return 1;
}

// Load the Model Data for all the Buildings specified in the City File
void cityFile::prepModels()
{
	buildingVector.resize(modelTotal);

	for (int i = 0; i < modelTotal; i++)
	{ 
		buildingVector[i].Load(buildings[i].buildingPath);
	}
}

// Using preloaded model data for the buildings, draw the models
void cityFile::createCity()
{
	for (int j = 0; j < modelTotal; j++)
	{ 
		glPushMatrix();
		
		glTranslatef(buildings[j].Tx, buildings[j].Ty, buildings[j].Tz);
		glScalef(buildings[j].Sx, buildings[j].Sy, buildings[j].Sz);
		glRotatef(buildings[j].Rz, 0, 0, 1);
		glRotatef(buildings[j].Ry, 0, 1, 0);
		glRotatef(buildings[j].Rx, 1, 0, 0);
		glTranslatef(buildings[j].Ox, buildings[j].Oy, buildings[j].Oz);

		buildingVector[j].Create();

		glPopMatrix();
	}
}