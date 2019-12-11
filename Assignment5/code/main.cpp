#include <stdlib.h>
#include "GL/glut.h"

float fTranslate;
float fRotate;
float fScale = 1.0f;	// set inital scale value to 1.0f

bool bPersp = false;  // Judge whether it is a perspective projection or a orthographic projection
bool bAnim = false;  // Determine if the teapot and table rotate
bool bWire = false;  // Determine if the drawing mode is linear or filled

int wHeight = 0;
int wWidth = 0;

bool light_color = true;
GLfloat color[] = { 1.0 , 1.0 , 1.0 , 1.0 };  // Define the color of light

// The position of ambient light
GLfloat ambient_x = 0.0f;
GLfloat ambient_y = 0.0f;
GLfloat ambient_z = 0.0f;

//The direction and angle of  spot light
GLfloat spotdir_x = 0.0f;
GLfloat spotdir_y = 0.0f;
GLfloat spotdir_z = 0.0f;
GLfloat spot_angle = 5.0f;

void Draw_Leg()  // Draw a leg
{
	glScalef(1, 1, 3);  // Stretch the model three times in the z direction
	glutSolidCube(1.0);  // Draw a cube with a side length of one
}

void Draw_Table() // Draw a table with RGB colors
{
	GLfloat mat_specular[] = { 0.6f , 0.6f , 0.6f , 1.0f };
	GLfloat mat_diffuse[] = { 0.85f , 0.65f , 0.2f , 1.0f };  // This color is gold

	// The color of table
	GLfloat color0[] = { 1.0f , 0.0f , 0.0f };
	GLfloat color1[] = { 0.0f , 1.0f , 0.0f };
	GLfloat color2[] = { 1.0f , 1.0f , 0.0f };
	GLfloat color3[] = { 0.0f , 1.0f , 1.0f };
	GLfloat color4[] = { 0.0f , 0.0f , 1.0f };

	// The teapot
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);  // Set the specular color
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 50);  // Set specular reflection index
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);  // Set the ambient and diffuse property
	glTranslatef(0, 0, 4 + 1);
	glRotatef(90, 1, 0, 0);
	glutSolidTeapot(1);
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color0);  // Set the specular property
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color0);  // Set the ambient and diffuse property
	glTranslatef(0, 0, 3.5);
	glScalef(5, 4, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color1);  // Set the specular property
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color1);  // Set the ambient and diffuse property
	glTranslatef(1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color2);  // Set the specular property
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color2);  // Set the ambient and diffuse property
	glTranslatef(-1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color3);  // Set the specular property
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color3);  // Set the ambient and diffuse property
	glTranslatef(1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color4);  // Set the specular property
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color4);  // Set the ambient and diffuse property
	glTranslatef(-1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

}


void updateView(int width, int height)
{
	glViewport(0, 0, width, height);						// Reset the current viewport

	glMatrixMode(GL_PROJECTION);						// Select the projection matrix
	glLoadIdentity();									// Reset the projection matrix

	float whRatio = (GLfloat)width / (GLfloat)height;  // Set the display scale
	if (bPersp) {
		gluPerspective(45.0f, whRatio, 0.1f, 100.0f);  // Perspective mode, the parameters of the function are angle of view, aspect ratio, near, far
		//glFrustum(-3, 3, -3, 3, 3,100);
	}
	else {
		glOrtho(-3, 3, -3, 3, -100, 100);
	}

	glMatrixMode(GL_MODELVIEW);							// Select the modelview matrix
}

void reshape(int width, int height)
{
	if (height == 0)										// Prevent A divide by zero 
	{
		height = 1;										// Make Height Equal One
	}

	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth);
}

void idle()
{
	glutPostRedisplay();  // Call the current drawing function
}

float eye[] = { 0, 0, 8 };  // The place of the camera
float center[] = { 0, 0, 0 };  // The place of the object

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
		eye[0] += 0.2f;
		center[0] += 0.2f;
		break;
	}
	case 'd': {// Move right
		eye[0] -= 0.2f;
		center[0] -= 0.2f;
		break;
	}
	case 'w': {// Move up
		eye[1] -= 0.2f;
		center[1] -= 0.2f;
		break;
	}
	case 's': {// Move down
		eye[1] += 0.2f;
		center[1] += 0.2f;
		break;
	}
	case 'z': {// Move forward
		eye[2] -= 0.2f;
		center[2] -= 0.2f;
		break;
	}
	case 'c': {// Move backwards
		eye[2] += 0.2f;
		center[2] += 0.2f;
		break;
	}
	case 'j': {// Ambient light moves left
		ambient_x = ambient_x - 0.2f;
		break;
	}
	case 'l': {// Ambient light moves right
		ambient_x = ambient_x + 0.2f;
		break;
	}
	case 'i': {// Ambient light moves up
		ambient_y = ambient_y + 0.2f;
		break;
	}
	case 'k': {// Ambient light moves down
		ambient_y = ambient_y - 0.2f;
		break;
	}
	case 'r': {// Ambient light moves forward
		ambient_z = ambient_z + 0.2f;
		break;
	}
	case 'y': {// Ambient light moves backwards
		ambient_z = ambient_z - 0.2f;
		break;
	}
	case 'x': {// Ambient light color switching
		light_color = !light_color;
		break;
	}
	case 'f': {// Spot light moves left
		spotdir_x = spotdir_x - 0.05f;
		break;
	}
	case 'h': {// Spot light moves right
		spotdir_x = spotdir_x + 0.05f;
		break;
	}
	case 't': {// Spot light moves up
		spotdir_y = spotdir_y - 0.05f;
		break;
	}
	case 'g': {// Spot light moves down
		spotdir_y = spotdir_y + 0.05f;
		break;
	}
	case 'v': {// Spot light moves forward
		spotdir_z = spotdir_z + 0.05f;
		break;
	}
	case 'b': {// Spot light moves backwards
		spotdir_z = spotdir_z - 0.05f;
		break;
	}
	case 'n': {// Spotlight angle becomes larger
		if (spot_angle <= 89.0f)
			spot_angle = spot_angle + 0.2f;
		break;
	}
	case 'm': {// Spotlight angle becomes smaller
		if (spot_angle >= 1.0f)
			spot_angle = spot_angle - 0.2f;
		break;
	}
	}

	updateView(wHeight, wWidth);
}


void redraw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear color cache and depth cache
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

	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };  // The color of white
	GLfloat ambient_pos[] = { 5 + ambient_x , 5 + ambient_y , 5 + ambient_z , 1 };  // The position of ambient light
	GLfloat spot_pos[] = { 0.0f , 5.0f , 0.0f , 1.0f };  // The position of spot light
	GLfloat spot_direction[] = { 0.0f + spotdir_x , -1.0f + spotdir_y , 0.0f + spotdir_z };  // The direction of spot light

	if (light_color) {
		color[0] = 1.0f, color[1] = 1.0f, color[2] = 1.0f, color[3] = 1.0f;  // The color of white
	}
	else {
		color[0] = 0.0f, color[1] = 1.0f, color[2] = 0.0f, color[3] = 1.0f;  // Another color
	}

	glLightfv(GL_LIGHT0, GL_POSITION, ambient_pos);  // Set the illumination position of the 0th light source
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);  // Set the specular lighting color
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);  // Set the diffuse lighting color
	glLightfv(GL_LIGHT0, GL_AMBIENT, color);  // Set the illumination color after the multiple reflection of the No. 0 light source (ambient light color)
	glEnable(GL_LIGHT0);  // Turn on the 0th light source

	glLightfv(GL_LIGHT1, GL_AMBIENT, color);  // Set ambient light composition
	glLightfv(GL_LIGHT1, GL_SPECULAR, white);  // Set specular light composition
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white);  // Set diffuse light composition

	glLightfv(GL_LIGHT1, GL_POSITION, spot_pos);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, spot_angle);  // Cut angle
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);  // Light source direction
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.);  // Aggregation
	glEnable(GL_LIGHT1);  // Turn on the first light source
	

	//	glTranslatef(0.0f, 0.0f,-6.0f);			// Place the triangle at Center
	glRotatef(fRotate, 0, 1.0f, 0);  // Rotate around Y axis
	glRotatef(-90, 1, 0, 0);  // Make the table facing the camera
	glScalef(0.2, 0.2, 0.2);  // Scale to make the object appear in the window at the appropriate size
	Draw_Table();  // Draw Scene

	if (bAnim) fRotate += 0.5f;  // Rotation factor change
	glutSwapBuffers();  // Swap buffer
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);  // Initialize the glut library
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);  // Specify the window display mode that the function glutCreateWindow will create. RGB mode Double buffering
	glutInitWindowSize(480, 480);  // Set the window position, which is the position of the top left corner of the window relative to the entire screen
	int windowHandle = glutCreateWindow("Simple GLUT App");  // Set the window title

	glutDisplayFunc(redraw);  // Register a draw callback function that specifies the function to call when the window content needs to be redrawn
	glutReshapeFunc(reshape); // The callback function when the registration window size changes.
	glutKeyboardFunc(key);  // Register key callback function
	glutIdleFunc(idle);  // Register global callback function : call when idle

	glutMainLoop();  // Glut event processing loop
	return 0;
}


