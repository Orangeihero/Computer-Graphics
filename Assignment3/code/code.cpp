#include <stdlib.h>
#include "GL/glut.h"


float fTranslate;
float fRotate = 0.0f;  // Set initial rotation value to 0.0f
float fScale = 1.0f;  // Set inital scale value to 1.0f

bool bPersp = false;  // Judging whether it is a perspective projection or a orthographic projection
bool bAnim = false;  // Determine if the teapot and table rotate
bool bWire = false;  // Determine if the drawing mode is linear or filled

int wHeight = 0;
int wWidth = 0;

float tRotate = 0.0f;  // the rotation value of the teapot
bool tpRotate = false;  //  Determine if the teapot rotate

void Draw_Leg() // Draw the leg
{
	glScalef(1, 1, 3);  // Stretch the model three times in the z direction
	glutSolidCube(1.0);  // Draw a cube with a side length of one
}

void Draw_Scene(float place[])
{
	glPushMatrix();
	glTranslatef(place[0], place[1], place[2]);  // Move the teapot to its location
	glRotatef(90, 1, 0, 0); //Put the kettle body facing the camera
	glRotatef(tRotate, 0, 1, 0);  // Control the rotation of the teapot							
	glutSolidTeapot(1);  // Draw a teapot
	glPopMatrix();

	// Draw the cube
	glPushMatrix();
	glTranslatef(0, 0, 3.5);
	glScalef(5, 4, 1);  // Stretch the model five times in the x direction and four times in the y direction
	glutSolidCube(1.0);  // Draw a cube with a side length of one
	glPopMatrix();

	// Draw four legs
	glPushMatrix();
	glTranslatef(1.5, 1, 1.5); // Move the leg to its position
	Draw_Leg();  // Draw a leg
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 1, 1.5);  // Move the leg to its position
	Draw_Leg();  // Draw a leg
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, -1, 1.5);  // Move the leg to its position
	Draw_Leg();  // Draw a leg
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, -1, 1.5);  // Move the leg to its position
	Draw_Leg();  // Draw a leg
	glPopMatrix();

}

void updateView(int width, int height)
{
	glViewport(0, 0, width, height);  // Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);  // Select The Projection Matrix
	glLoadIdentity();  // Reset The Projection Matrix

	float whRatio = (GLfloat)width / (GLfloat)height;  // Set the display scale

	if (bPersp) {
		gluPerspective(45, whRatio, 1, 100);  // Perspective mode, the parameters of the function are angle of view, aspect ratio, near, far
	}
	else
		glOrtho(-3, 3, -3, 3, -100, 100);

	glMatrixMode(GL_MODELVIEW);  // Select The Modelview Matrix
}

void reshape(int width, int height)
{
	if (height == 0)  // Prevent A Divide By Zero By
	{
		height = 1;  // Making Height Equal One
	}

	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth);  // Update perspective
}

void idle()
{
	glutPostRedisplay();  // Call the current drawing function
}

float eye[] = { 0, 0, 8 };  // The place of the camera
float center[] = { 0, 0, 0 };  // The place of the object
float place[] = { 0 , 0 , 5 };  //The place of the teapot

void key(unsigned char k, int x, int y)
{
	switch (k)
	{
	case 27:
	case 'q': {exit(0); break; }  // exit
	case 'p': {bPersp = !bPersp; updateView(wHeight, wWidth); break; }  //Switch orthographic projection and perspective projection

	case ' ': {bAnim = !bAnim; break; }  // Switch the rotation mode
	case 'o': {bWire = !bWire; break; }  // Switch the rendering mode

	case 'a': {// Move left
		center[0] = center[0] + 0.2f; // The object moves to the left
		eye[0] = eye[0] + 0.2f;  // The viewpoint moves to the right
		break;
	}
	case 'd': {// Move right
		center[0] = center[0] - 0.2f;  //The object moves to the right
		eye[0] = eye[0] - 0.2f;  // The viewpoint moves to the left
		break;
	}
	case 'w': {// Move up
		center[1] = center[1] - 0.2f;  //The object moves up
		eye[1] = eye[1] - 0.2f;  // The viewpoint moves down
		break;
	}
	case 's': {// Move down
		center[1] = center[1] + 0.2f;  // The object moves down
		eye[1] = eye[1] + 0.2f;  // The viewpoint moves up
		break;
	}
	case 'z': {// Move forward
		center[2] = center[2] - 0.2f;  // The object moves forward
		eye[2] = eye[2] - 0.2f;  // The viewpoint moves backwards
		break;
	}
	case 'c': {// Move backwards
		center[2] = center[2] + 0.2f;  // The object moves backwards
		eye[2] = eye[2] + 0.2f;  // The viewpoint moves forward
		break;
	}

	// Teapot related operations
	case 'l': {// Move the teapot to the right
		place[0] = place[0] + 0.2f;  // The teapot moves right
		if (place[0] > 1.5f) place[0] = 1.5f;  // Prevent the teapot from going beyond the desktop
		break;
	}
	case 'j': {// Move the teapot to the left
		place[0] = place[0] - 0.2f;  // The teapot moves left
		if (place[0] < -1.5f) place[0] = -1.5f;  // Prevent the teapot from going beyond the desktop
		break;
	}
	case 'i': {// Move the teapot backwards
		place[1] = place[1] + 0.2f;  // The teapot moves backwards
		if (place[1] > 1.5f) place[1] = 1.5f;  // Prevent the teapot from going beyond the desktop
		break;
	}
	case 'k': {// Move the teapot forward
		place[1] = place[1] - 0.2f;  // The teapot moves forward
		if (place[1] < -1.5f) place[1] = -1.5f;  // Prevent the teapot from going beyond the desktop
		break;
	}
	case 'e': {// Rotate the teapot
		tpRotate = !tpRotate;
		break;
	}
	}
}


void redraw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	// The place of the camera,
	// the place of the object 
	// and the observation direction
	gluLookAt(eye[0], eye[1], eye[2],
		center[0], center[1], center[2],
		0, 1, 0);			

	if (bWire) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // Set the polygon drawing mode: front and back, line type
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // Set the polygon drawing mode: front and back, fill type
	}

	glEnable(GL_DEPTH_TEST);  // Open depth test
	glEnable(GL_LIGHTING);  // Turn on lighting mode
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };  // Define color
	GLfloat light_pos[] = { 5,5,5,1 };  // Define the source position

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);  // Set the illumination position of the zeroth source
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);  // Set the illumination color after the multiple reflection of the zeroth source
	glEnable(GL_LIGHT0);  // Turn on the zeroth light source

	glRotatef(fRotate, 0, 1.0f, 0);  // Rotate around Y axis
	glRotatef(-90, 1, 0, 0);  // Make the table facing the camera
	glScalef(0.2, 0.2, 0.2);  // Scale to make the object appear in the window at the appropriate size
	Draw_Scene(place);  // Draw Scene

	if (bAnim) fRotate += 0.5f;  // Teapot and table rotate together
	if (tpRotate) tRotate += 0.5f;  // Teapot rotate

	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);  // Initialize the glut library 
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);  // Specify the window display mode that the function glutCreateWindow will create. RGB mode Double buffering
	glutInitWindowSize(480, 480);  // Set the window position, which is the position of the top left corner of the window relative to the entire screen
	int windowHandle = glutCreateWindow("Experiment 3");  // Set the window title

	glutDisplayFunc(redraw);  // Register a draw callback function that specifies the function to call when the window content needs to be redrawn
	glutReshapeFunc(reshape); // The callback function when the registration window size changes.
	glutKeyboardFunc(key);  // Register key callback function
	glutIdleFunc(idle);  // Register global callback function : call when idle

	glutMainLoop();  // Glut event processing loop
	return 0;
}


