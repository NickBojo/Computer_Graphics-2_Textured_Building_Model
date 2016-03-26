// Header file for the Main Program
// Constains basic includes, variable, constant, and runction declarations

#include "simpleModel.h"
#include "cityFile.h"
#include <GL/glut.h>
#include <Windows.h>

// Constant Declaration
#define PI 3.1415926535897932384626433832795	// PI definition for Trig
const int screenWidth = 800;					// Screen Setup Constants
const int screenHeight = 800;

// Camera Position Variables
double rotx, roty, rotz;
double transx, transy, transz;

// Object Position Variables
double rotObx, rotOby, rotObz;
double transObx, transOby, transObz;

// Speed of Movement Variables
double strafeSpeed, walkSpeed, levSpeed;		// Camera Movement
double lookSpeedx, lookSpeedy;					// Camera Rotation
double transSpeedx, transSpeedy, transSpeedz;	// Object Movement
double rotSpeedx, rotSpeedy, rotSpeedz;			// Object Rotation
 
// Mouse Button States
bool leftButton = false;
bool rightButton = false;

// Hide the Objects Booleans
bool hideFloor;
bool hideTeapot;

// Function Declaration
void printBitmapString(float x, float y, void *font, char *string, int red, int green, int blue);
void enter2D();
void enter3D();
void reshape(int width, int height);

// Model Directory, filled with exec argument
char* inPath;

// Imported Model Variables
simpleModel inSimple;

// Imported City Variable
cityFile inCity;