#include "simpleModel.h"

int simpleModel::Load(char *inFile)
{
	fullPath = inFile;			// Split the path string to specify the model folder
	modelPath = fullPath.substr(0, (fullPath.find_last_of("/")+1));

	fstream modelFile;			// Start File Streaming
	modelFile.open(inFile);	

	if (modelFile.is_open())    // If File Opened correctly, begin passing through it
	{
		char currentLine[255];	// Char array for containing the contents of a line of text
		
		while(modelFile.good())
		{
			modelFile.getline(currentLine, 255);	// Get the first line of the file
			
			if(strlen(currentLine) == 0)			// If the line is empty, don't parse it
			{
				continue;
			}

			// Scan the first 3 characters from the line, ignoring whitespace
			// Use the characters to determine when to begin parsing which pieces of data
			sscanf(currentLine, " %c %c %c", &lineChar1, &lineChar2, &lineChar3);

			// If at <textures> begin Parsing Textures
			if ((lineChar1 == '<') && (lineChar2 == 't') && (lineChar3 == 'e'))
			{
				modelFile.getline(currentLine, 255);	// Go to next line after the heading
				sscanf(currentLine, " %c %c", &lineChar1, &lineChar2);

				// Parse lines while in the textures section
				// Stopping at </>
				while (lineChar2 != '/')
				{
					// If a line is empty, don't parse it and move to the next
					if((strlen(currentLine) == 0))     
					{
						modelFile.getline(currentLine, 255);
						continue;
					}

					texArray.push_back(currentLine);

					modelFile.getline(currentLine, 255);
					sscanf(currentLine, " %c %c", &lineChar1, &lineChar2);
				}

				numTex = texArray.size();
			}
			
			// If at <vertices> begin Parsing Vertices
			if ((lineChar1 == '<') && (lineChar2 == 'v'))
			{
				modelFile.getline(currentLine, 255);	// Go to next line after the heading
				
				// Parse lines while in the vertices section
				// Stopping at </>
				while (sscanf(currentLine, "%f %f %f", &vFloat1, &vFloat2, &vFloat3) != 0)
				{
					// If a line is empty, don't parse it and move to the next
					if((strlen(currentLine) == 0))     
					{
						modelFile.getline(currentLine, 255);
						continue;
					}

					vertices.push_back(Vector());       // Add new Vertex

					vertices.back().x = vFloat1;		// Place Points into Vertices Vector
					vertices.back().y = vFloat2;
					vertices.back().z = vFloat3;
					
					modelFile.getline(currentLine, 255);
				}
			}
			
			// If at <normals> begin Parsing Vertices
			if ((lineChar1 == '<') && (lineChar2 == 'n'))
			{
				modelFile.getline(currentLine, 255);	// Go to next line after the heading
				
				// Parse lines while in the vertices section
				// Stopping at </>
				while (sscanf(currentLine, "%f %f %f", &vFloat1, &vFloat2, &vFloat3) != 0)
				{
					// If a line is empty, don't parse it and move to the next
					if((strlen(currentLine) == 0))     
					{
						modelFile.getline(currentLine, 255);
						continue;
					}

					normals.push_back(Vector());       // Add new Vertex

					normals.back().x = vFloat1;		// Place Points into Vertices Vector
					normals.back().y = vFloat2;
					normals.back().z = vFloat3;
					
					modelFile.getline(currentLine, 255);
				}
			}

			// If at <traingles> being parsing Triangle Section
			if ((lineChar1 == '<') && (lineChar2 == 't') && (lineChar3 == 'r'))
			{
				modelFile.getline(currentLine, 255);		// Go to next line after heading
				
				// parse lines while in the Triangle section, stopping at </>
				while (sscanf(currentLine, " %d %d %d %d %d %f %f %f %f %f %f", &vecInd1, &vecInd2, &vecInd3, &normInd, &texInd, &tex1, &tex2, &tex3, &tex4, &tex5, &tex6) != 0)
				{

					// If a line is empty, don't parse it and move to the next
					if((strlen(currentLine) == 0))
					{
						modelFile.getline(currentLine, 255);
						continue;
					}

					triangles.push_back(Triangle());        // Add new Triangle

					triangles.back().vert1 = vecInd1;		// Place vertices from index points in the Triangle vector
					triangles.back().vert2 = vecInd2;
					triangles.back().vert3 = vecInd3;

					triangles.back().norm1 = normInd;		// Place normals from the index point in the Triangle vector
					triangles.back().norm2 = normInd;
					triangles.back().norm3 = normInd;
					
					triangles.back().tex = texInd;			// Store Texture info in the Traingle Vector
					triangles.back().tex1x = tex1;
					triangles.back().tex1y = tex2;
					triangles.back().tex2x = tex3;
					triangles.back().tex2y = tex4;
					triangles.back().tex3x = tex5;
					triangles.back().tex3y = tex6;

					modelFile.getline(currentLine, 255);
				}
			}
		}
		modelFile.close();
	}

	else
	{
		cout << "Unable to open .model file.";
		return 0;
	}

	return 1;
}


// Loads texture data from a file, sets up the texture, and returns a single GLuint
GLuint simpleModel::loadTexture(string inString, int width, int height)
{
	// Input String Processing
	// Remove blank space and append texture file name to folder path
	inString.erase( remove_if( inString.begin(), inString.end(), isspace), inString.end());
	texFile = modelPath + inString;

	// Load .ppm data
	ppmData = PPM::Read(texFile, width, height);

	// Generate Texture Name
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Setup Texture
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ); //set texture environment parameters
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ppmData);

	// Clear Data
	free(ppmData);

	return texture;
}


// Draws the Model on the screen using the parsed Vector and Triangle information
void simpleModel::Create()
{
	// Create an array, size = the number of textures, to hold texture data
	textureNames = new GLuint[numTex];
	
	// Fill an array of GLuints using parsed data and individual loading method
	for (int i = 0; i < numTex; i++)
	{
		textureNames[i] =  loadTexture(texArray[i], 512, 512);
	}

	for (int i = 0; i < triangles.size(); i++)
	{
		// Bind Texture per triangle Tex index
		glBindTexture(GL_TEXTURE_2D, textureNames[triangles[i].tex]);

		// Draw Triangle and Normals using indices
		glBegin(GL_TRIANGLES);

		glTexCoord2f(triangles[i].tex1x, triangles[i].tex1y);
		glNormal3f(normals[triangles[i].norm1].x, normals[triangles[i].norm1].y, normals[triangles[i].norm1].z);
		glVertex3f(vertices[triangles[i].vert1].x, vertices[triangles[i].vert1].y, vertices[triangles[i].vert1].z);

		glTexCoord2f(triangles[i].tex2x, triangles[i].tex2y);
		glNormal3f(normals[triangles[i].norm2].x, normals[triangles[i].norm2].y, normals[triangles[i].norm2].z);
		glVertex3f(vertices[triangles[i].vert2].x, vertices[triangles[i].vert2].y, vertices[triangles[i].vert2].z);

		glTexCoord2f(triangles[i].tex3x, triangles[i].tex3y);
		glNormal3f(normals[triangles[i].norm3].x, normals[triangles[i].norm3].y, normals[triangles[i].norm3].z);
		glVertex3f(vertices[triangles[i].vert3].x, vertices[triangles[i].vert3].y, vertices[triangles[i].vert3].z);

		glEnd();

		// Unbind Texture
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Clear Texture Memory
	glDeleteTextures(numTex, textureNames);

}