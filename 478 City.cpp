// 478 City

// Nick Bojanowski
// #23947054
// EECE 478

#include "478 City.h"

///// INITIAL CONDITIONS /////
//////////////////////////////

void init()
{
	glClearColor(0, 0, 0, 255);		// Background to Black
	glColor3ub(255, 255, 255);		// Initial Colour to White
	
	glEnable(GL_LIGHTING);			// Test Lights
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);

	glEnable(GL_TEXTURE_2D);		// Enable 2D Texturing

	glFrontFace(GL_CCW);			// Back Face Culling enabled by default

	cout << "Attempting to load " << inPath << endl << endl;

	// Hardcoded inPath for model building
	//inSimple.Load("D:/My Documents/Dropbox/School/EECE 478/Assignments/Assignment 2/EarthOceanSciences/EarthOceanSciences.model");
	//inSimple.Load("EarthOceanSciences/EarthOceanSciences.model");
	inSimple.Load(inPath);
	//inCity.loadCity("StormTown.city");

	transx = 0.0;					// Initial Camera Positions
	transy = 0.6;
	transz = 10.0;
		
	rotx = 0.0;						// Initial Camera Rotation
	roty = 0.0;
	rotz = 0.0;

	transObx = 0.0;					// Initialize Object Control Positioning
	transOby = 0.0;
	transObz = -3.0;

	rotObx = 0.0;					// Initial Object Control Rotation
	rotOby = 0.0;
	rotObz = 0.0;

	strafeSpeed = 0.2;				// Camera/Cameraman Movement Speed
	walkSpeed= 0.2;
	levSpeed = 0.2;		

	lookSpeedx = 2.0;				// Camera/Cameraman Rotation Speed
	lookSpeedy = 2.0;

	transSpeedx = 0.05;				// Object Movement Speed
	transSpeedy = 0.05;
	transSpeedz = 0.05;

	rotSpeedx = 0.5;				// Object Rotation Speed
	rotSpeedy = 0.5;
	rotSpeedz = 0.5;
}

// Enters a 2D Perspective.
// Called in Init
// Used for 2D Text Printing
void enter2D()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);	// Ortho Projection

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Enters a 3d Perspective
// Used to return to 3D after 2D operations
void enter3D()
{
	double aspect;
	aspect = screenWidth/screenHeight;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60, (GLdouble)aspect, 1.0, 100.0);	// Perspective Projection, FOV 60, 100 distance

	glMatrixMode(GL_MODELVIEW);
}


// Uses glutBitmapCharacter to display an entire String
// Input: x, y, font, string, colour (0-255)
void printBitmapString(float x, float y, void *font, char *string, int red, int green, int blue) 
{  
	char *ch;

	glColor3ub(red, green, blue);			// text colour

	glRasterPos2f(x,y);						// x, y co-ordinate position for text

	for (ch=string; *ch != '\0'; ch++)		// iterates through input String
	{
		glutBitmapCharacter(font, *ch);		// print character after character
	}
}


// Converts a Degree into a Radian
// Used for Trig functions
double degToRad (double deg)
{
	double rad = ((deg / 180) * PI);
	return rad;
}

// Use transformation matrixes to control the camera
// Should be called in Display() before object placement
void cameraControl()
{
	glRotatef(rotx, 1.0, 0.0, 0.0);				// rotx
	glRotatef(roty, 0.0, 1.0, 0.0);				// roty
	glRotatef(rotz, 0.0, 0.0, 1.0);				// rotz

	glTranslated(-transx, -transy, -transz);	// inverse properties, due to camera control instead of object
}


// Use transformation matrixes to control the specified object
// Called by the object that will be controlled
void objectControl()
{
	glTranslated(transObx, transOby, transObz);

	glRotatef(rotObx, 1.0, 0.0, 0.0);				// rotx for Object
	glRotatef(rotOby, 0.0, 1.0, 0.0);				// roty for Object
	glRotatef(rotObz, 0.0, 0.0, 1.0);				// rotz for Object
}


// Draws a basic floor-grid
// Used to calibrate movement, and provide a reference point
void floorGrid()
{	
	glBegin(GL_LINES);
	for (GLfloat n = -10.0; n<= 10.0; n += 0.25)
	{
		glVertex3f(n, 0, 10.0); 
		glVertex3f(n, 0, -10.0);
		glVertex3f(10.0, 0, n); 
		glVertex3f(-10.0, 0, n);

	}
	glEnd();
}


// Mouse button states
// Sets global variables to True when mouse buttons are pressed
// Swaps global variable for Right button when pressed
void mouseButton (int button, int state, int x, int y)
{
	// Left
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
			leftButton = true;
	}
	else	leftButton = false;

	// Right
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
			rightButton = !rightButton;
	}
}


// Mouse Motion, no Button
// Moving the mouse with no buttons held will rotate an object
// Pressing the Left Mouse Button enters Translates or Rotates using the z-axis
// Pressing the Right Mouse Button switches between Rotation and Translation mode
void mouseMove(int x, int y)
{
	int cursorx = screenWidth / 2;		// Center of Screen
	int cursory = screenHeight / 2;

	int changex = x - cursorx;			// Determine how far the cursor has moved
	int changey = y - cursory;

	if (x == cursorx && y == cursory)	// If cursor is centered, no need to recenter
	{
		return;
	}

	// Rotation Mode
	// X and Y Rotation
	if (!rightButton && !leftButton)
	{
		rotObx += (double)changey * rotSpeedx;
		rotOby -= (double)changex * rotSpeedy;
	}

	// Z Rotation
	if (!rightButton && leftButton)
	{
		rotObz -= (double)changex  * rotSpeedz;
	}
	
	//Translation Mode
	// X and Y Translation
	if (rightButton && !leftButton)
	{
		transObx += cos(degToRad(-roty)) * (double)changex * transSpeedx;	// Trig added to make translation relative to camera
		transObz -= sin(degToRad(-roty)) * (double)changex * transSpeedx;

		transOby -= cos(degToRad(-rotx)) * (double)changey * transSpeedy;	// Trig added to make translation relative to camera
		transObz += sin(degToRad(-rotx)) * (double)changey * transSpeedy;
	}

	// Z Translation
	if (rightButton && leftButton)
	{
		transObx += sin(degToRad(-roty)) * (double)changey * transSpeedz;	// Trig added to make translation relative to camera
		transOby -= sin(degToRad(-rotx)) * (double)changey * transSpeedz;
		transObz += cos(degToRad(-roty)) * (double)changey * transSpeedz;
	}

	glutWarpPointer(cursorx, cursory);	// Center the cursor
	glutSetCursor(GLUT_CURSOR_NONE);
}


// WASD keys control the movement of the camera
// Similar to FPS game controls, with fly enabled
// Forward, Back, Strafe, Up, and Down
// Exit with Esc key
void keyPress (unsigned char key, int x, int y)
{
	// Move Forward
	if (key == 'w')
	{
		transx += sin(degToRad(roty)) * walkSpeed;
		transy -= sin(degToRad(rotx)) * walkSpeed;
		transz -= cos(degToRad(roty)) * walkSpeed;
	}

	// Move Back
	if (key == 's')
	{
		transx -= sin(degToRad(roty)) * walkSpeed;
		transy += sin(degToRad(rotx)) * walkSpeed;
		transz += cos(degToRad(roty)) * walkSpeed;
	}

	// Strafe Right
	if (key == 'd')
	{
		transx += cos(degToRad(roty)) * strafeSpeed;
		transz += sin(degToRad(roty)) * strafeSpeed;
	}

	// Strafe Left
	if (key == 'a')
	{
		transx -= cos(degToRad(roty)) * strafeSpeed;
		transz -= sin(degToRad(roty)) * strafeSpeed;
	}

	// Levitate Up
	if (key == 'e')
	{
		transy += levSpeed;
	}

	// Levitate Down
	if (key == 'c')
	{
		transy -= levSpeed;
	}

	// Hide the Floor Grid
	if (key == 'h')
	{
		hideFloor = !hideFloor;
	}

	// Hide the Teapot
	if (key == 'j')
	{
		hideTeapot =!hideTeapot;
	}

	// Exit Program with Esc
	if (key == 27)
	{
		exit(0);
	}
}


// Arrow keys control the rotation of the camera
// Allows the user to 'look around'
// Rotate left, right, up, and down.
void specialKeyPress (int key, int x, int y)
{
	// Look Left
	if (key == GLUT_KEY_LEFT)
	{
		roty -= lookSpeedx;
	}

	// Look Right
	if (key == GLUT_KEY_RIGHT)
	{
		roty += lookSpeedx;
	}

	// Look Up
	if (key == GLUT_KEY_UP)
	{
		rotx -= lookSpeedy;
		if (rotx < -360)	// avoid gimbal lock
			rotx += 360;
	}

	// Look Down
	if (key == GLUT_KEY_DOWN)
	{
		rotx += lookSpeedy;
		if (rotx > 360)		// avoid gimal lock
			rotx -= 360;
	}
}


///// RESHAPE METHOD /////
//////////////////////////

void reshape(int width, int height)
{
	double aspect;
	if (height == 0) height = 1;	// divide by 0 elimination
	aspect = width/height;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);	// Set viewport to window specifications

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60, (GLdouble)aspect, 1.0, 100.0);	// Perspective Projection, FOV 60, 100 distance

	glMatrixMode(GL_MODELVIEW);
}


///// DISPLAY METHOD /////
//////////////////////////

void display()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glutSetCursor(GLUT_CURSOR_NONE);			// Hide the Cursor

	GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f}; //Color(0.2, 0.2, 0.2)
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos0[] = {4.0f, 0.0f, 8.0f, 1.0f}; //Positioned at (4, 0, 8)
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	cameraControl();							// Activates Scene Camera Control

	// Make a floor grid for a reference point
	if (!hideFloor)								// Display if Visible, 'h' key toggles
	{
	glColor3ub(255,255,255);
	floorGrid();
	}

	// Display a Wireframe Teapot
	if (!hideTeapot)							// Display if Visible, 'j' key toggles
	{
	glPushMatrix();

	glTranslated(8.0, 0.7, -4.0);				// Position Object
	glRotatef(0, 1.0, 0.0, 0.0);
	glRotatef(0, 0.0, 1.0, 0.0);
	glRotatef(0, 0.0, 0.0, 1.0);
	glColor3ub(150,150,255);
	glutSolidTeapot(1.0);						// Render Teapot

	glPopMatrix();
	}

	// Create the Model from the .model file/
	objectControl();							// Enable Mouse Control and Initial Position
	inSimple.Create();
	//inCity.createCity();

	// Print Words on the Screen
	enter2D();
		printBitmapString(0.57, -0.91, GLUT_BITMAP_TIMES_ROMAN_24, "Nick Bojanowski", 255, 0, 0);
		printBitmapString(0.72, -0.98, GLUT_BITMAP_TIMES_ROMAN_24, "#23947054", 255, 0, 0);
	enter3D();

	glutSwapBuffers();							// Refresh
}


///// MAIN METHOD ////////
//////////////////////////

int main(int argc, char** argv)
{
	// Take the First Argument for use as a file path for the imported model
	if (argc >= 2)
	{
		inPath = argv[1];
	}

	// If there are no arguments
	// This will allow the environment to load without a model
	else
	{
		inPath = "EarthOceanSciences/EarthOceanSciences.model";
	}

	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Setup Viewing Window
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("478 City");

	init();

	glutDisplayFunc(display);				// display callbacks
	glutIdleFunc(display);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyPress);				// normal keyboard callback
	glutSpecialFunc(specialKeyPress);		// special keyboard callback
	glutMouseFunc(mouseButton);				// mouse clicks callback
	glutMotionFunc(mouseMove);				// left mouse button and motion callback
	glutPassiveMotionFunc(mouseMove);		// no mouse button and motion callback

	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}

