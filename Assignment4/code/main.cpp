l'l'l'l'l'l'l'l'l'l'l'l'l#include <stdlib.h>
#include "GL/glut.h"
#include <stdio.h>
#include <string.h>

#include "stanford_bunny.h"

float eye[] = { 0, 4, 6 }; //  the place of the eyes
float center[] = { 0, 0, 0 };  //  the place of the viewpoint
float fDistance = 0.2f;  //  distance factor
float fRotate = 0;  //  rotation factor
bool bAnim = false;  //  whether to rotate

bool bDrawList = false;  //  Whether to use the display list
GLint tableList = 0;  //  table list
GLint rabbitList = 0;  //  rabbit list
int rabbitNumber = 1;  // the number of rabbits

void Draw_Leg() // Draw the leg
{
	glScalef(1, 3, 1);  // Stretch the model three times in the y direction
	glutSolidCube(1.0);  // Draw a cube with a side length of one
}

void DrawTable()  //  Draw the table
{
	// Draw the cube
	glPushMatrix();
	glTranslatef(0, 3.5, 0);
	glScalef(5, 1, 4);  //  Stretch the model five times in the x direction and four times in the z direction
	glutSolidCube(1.0);  //  Draw a cube with a side length of one
	glPopMatrix();

	//  Draw the four legs
	glPushMatrix();
	glTranslatef(1.5, 1.5, 1);  //  Move the leg to its position
	Draw_Leg();  // Draw a leg
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 1.5, 1);  //  Move the leg to its position
	Draw_Leg();  // Draw a leg
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, 1.5, -1);  //  Move the leg to its position
	Draw_Leg();  // Draw a leg
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 1.5, -1);  //  Move the leg to its position
	Draw_Leg();  // Draw a leg
	glPopMatrix();
}


GLint GenTableList()
{
	GLint lid = glGenLists(2);  //  Generate two empty display lists
	
	//  Draw the table
	glNewList(lid, GL_COMPILE);  //Used to create and replace a display list function prototype. Specify the name of the display list and the compile mode is compile only. Store first, not execute
	DrawTable();
	glEndList();

	//  Draw the rabbits
	glNewList(lid + 1, GL_COMPILE);
	DrawBunny();
	glEndList();

	return lid;  //  Return the display list number
}

void Draw_Table_List()  //  Draw the scene( new way )
{
	glPushMatrix();

	//  Translate and scale
	glTranslatef(2.0, 4.5, 1.5);
	glScalef(2, 2, 2);

	//  Draw the rabbits
	for (int i = 1; i <= rabbitNumber; i++)
	{
		glCallList(rabbitList);//  If it is a list mode, it will be called by callList.
		if (i % 4 == 0)  //  If it is needed to wrap here
			glTranslatef(2.0f, 0.0f, -0.5f);
		else
			glTranslatef(-0.66f, 0.0f, 0.0f);
	}

	glPopMatrix();

	glCallList(tableList);  //  Call the display list to draw the table
}

void DrawScene()  //  Draw the scene( old way )
{
	glPushMatrix();

	//  Translate and scale
	glTranslatef(2.0, 4.5, 1.5);
	glScalef(2, 2, 2);

	// Draw the rabbits
	for (int i = 1; i <= rabbitNumber; i++)
	{
		DrawBunny();  //  Draw rabbit directly
		if (i % 4 == 0)  //  If it is needed to wrap here
			glTranslatef(2.0f, 0.0f, -0.5f);
		else
			glTranslatef(-0.66f, 0.0f, 0.0f);
	}
	glPopMatrix();

	DrawTable();  //  Draw the table directly
}

void reshape(int width, int height)
{
	if (height == 0)										//  Prevent A divide by zero
	{
		height = 1;										//  Make height equal one
	}

	glViewport(0, 0, width, height);						//  Reset the current viewport

	glMatrixMode(GL_PROJECTION);						//  Select the projection matrix
	glLoadIdentity();									//  Reset the projection matrix

	float whRatio = (GLfloat)width / (GLfloat)height;
	gluPerspective(45, whRatio, 1, 1000);              //  Set projection orientation

	glMatrixMode(GL_MODELVIEW);							//  Select the modelview matrix
}

void idle()
{
	glutPostRedisplay();  //  Call the current drawing function
}

void key(unsigned char k, int x, int y)
{
	switch (k)
	{
	case 27:
	case 'q': {exit(0); break; }

	case 'a':  //  The object moves to the left
	{
		eye[0] += fDistance;
		center[0] += fDistance;
		break;
	}
	case 'd':  //  The object moves to the right
	{
		eye[0] -= fDistance;
		center[0] -= fDistance;
		break;
	}
	case 'w':  //  The object moves up
	{
		eye[1] -= fDistance;
		center[1] -= fDistance;
		break;
	}
	case 's':  //  The object moves down
	{
		eye[1] += fDistance;
		center[1] += fDistance;
		break;
	}
	case 'z':  //  Move forward
	{
		eye[2] *= 0.95;
		break;
	}
	case 'c':  //  Move backwards
	{
		eye[2] *= 1.05;
		break;
	}
	case 'l':  //  Switch display list and non-display list drawing mode
	{
		bDrawList = !bDrawList;
		break;
	}
	case ' ':  //  Rotate
	{
		bAnim = !bAnim;
		break;
	}
	case'i':
	{
		if (rabbitNumber < 16) rabbitNumber++;  //  When the number of rabbits is less than 16, the number of rabbits increases
		break;
	}
	case'k':
	{
		if (rabbitNumber > 1) rabbitNumber--;  //  When the number of rabbits is greater than 1, the number of rabbits is reduced
		break;
	}
	default: break;
	}
}

void getFPS()
{
	static int frame = 0, time, timebase = 0;
	static char buffer[256];  // String buffer

	char mode[64];  //  the mode
	if (bDrawList)  //  Whether to draw with the display list
		strcpy_s(mode, "display list");
	else
		strcpy_s(mode, "naive");

	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);

	//  Returns the time interval in which glutGet(GLUT_ELAPSED_TIME) is called twice in milliseconds
	if (time - timebase > 1000) {  //  When the time interval is greater than 1000ms
		sprintf_s(buffer, "FPS:%4.2f %s",
			frame*1000.0 / (time - timebase), mode);  //  Write into buffer 
		timebase = time;  //  Last time's time interval
		frame = 0;
	}

	char *c;
	glDisable(GL_DEPTH_TEST);  //  Prohibit depth testing
	glMatrixMode(GL_PROJECTION);  //  Select projection matrix
	glPushMatrix();               //  Save the original matrix
	glLoadIdentity();             //  Load unit matrix
	glOrtho(0, 480, 0, 480, -1, 1);    //  Position orthographic projection
	glMatrixMode(GL_MODELVIEW);   //  Select Modelview Matrix
	glPushMatrix();               //  Save the original matrix
	glLoadIdentity();             //  Load unit matrix
	glRasterPos2f(10, 10);
	for (c = buffer; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
	glMatrixMode(GL_PROJECTION);  //  Select projection matrix
	glPopMatrix();                //  Reset to original save matrix
	glMatrixMode(GL_MODELVIEW);   //  Select Modelview Matrix
	glPopMatrix();                //  Reset to original save matrix
	glEnable(GL_DEPTH_TEST);      //  Open depth test
}

void redraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0.5, 0, 1);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	// The place of the camera,
	// the place of the object 
	// and the observation direction
	gluLookAt(eye[0], eye[1], eye[2],
		center[0], center[1], center[2],
		0, 1, 0);

	glEnable(GL_DEPTH_TEST);  //  Open depth test
	glEnable(GL_LIGHTING);  //  Turn on lighting
	GLfloat gray[] = { 0.4, 0.4, 0.4, 1.0 };  //  Set gray
	GLfloat light_pos[] = { 10, 10, 10, 1 };  //  Set the light source position
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, gray);  //  Specify the ambient light intensity of the entire scene
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);  //  Set the illumination position of the 0th light source
	glLightfv(GL_LIGHT0, GL_AMBIENT, gray);  //  Set the illumination color after the multiple reflection of the 0th light source (ambient light color)
	glEnable(GL_LIGHT0);  //  Turn on the 0th light source

	if (bAnim)
		fRotate += 0.5f;  //  Change the rotation factor
	glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis

	glScalef(0.4, 0.4, 0.4);  //  Scale
	if (!bDrawList)
		DrawScene();						// old way ( ordinary drawing )
	else
		Draw_Table_List();                  // new way( display list drawing ) 

	getFPS();  //  Get frames per second 
	glutSwapBuffers();  //  Swap buffer
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);  //  Initialize the glut library 
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);  //  Specify the window display mode that the function glutCreateWindow will create. RGB mode Double buffering
	glutInitWindowSize(480, 480);  //  Set the window position, which is the position of the top left corner of the window relative to the entire screen
	int windowHandle = glutCreateWindow("Exercise 4");  //  Set the window title

	glutDisplayFunc(redraw);  //  Register a draw callback function that specifies the function to call when the window content needs to be redrawn
	glutReshapeFunc(reshape);  //  The callback function when the registration window size changes.
	glutKeyboardFunc(key);  //  Register key callback function
	glutIdleFunc(idle);  //  Register global callback function : call when idle

	tableList = GenTableList();  // initialize the display list
	rabbitList = tableList + 1;

	glutMainLoop();  //  Glut event processing loop
	return 0;
}


