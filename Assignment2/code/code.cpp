#include<gl/glut.h>

float fTranslate = 0.0f;  //Translation factor
float fRotate = 0.0f;  //Rotation factor
float fScale = 1.0f;  //Scaling factor

void Draw_Cuboid(GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2, GLfloat z1, GLfloat z2)  //Draw the cuboid
{
	int i, j;
	GLfloat vertex[8][3] = {//Eight vertices of the cuboid
		{x1,y1,z1},
		{x1,y2,z1},
		{x2,y2,z1},
		{x2,y1,z1},
		{x1,y1,z2},
		{x1,y2,z2},
		{x2,y2,z2},
		{x2,y1,z2}
	};

	GLint flat[6][4] = {//Six planes of the cuboid
		{0,1,2,3},
		{1,2,6,5},
		{0,4,7,3},
		{2,3,7,6},
		{0,1,5,4},
		{4,5,6,7}
	};

	glBegin(GL_QUADS);  //Draw the cuboid
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 4; j++) {
			glVertex3fv(vertex[flat[i][j]]);
		}
	}
	glEnd();
}

void Draw_Table()  //Draw the table
{
	Draw_Cuboid(0.0, 1.0, 0.0, 0.8, 0.6, 0.8);  //draw the desktop
	//Draw four table legs
	Draw_Cuboid(0.1, 0.3, 0.1, 0.3, 0.0, 0.6);
	Draw_Cuboid(0.7, 0.9, 0.1, 0.3, 0.0, 0.6);
	Draw_Cuboid(0.1, 0.3, 0.5, 0.7, 0.0, 0.6);
	Draw_Cuboid(0.7, 0.9, 0.5, 0.7, 0.0, 0.6);
}

void reshape(int width, int height)
{
	if (height == 0)										// Prevent A divide by zero 
	{
		height = 1;										// Making height equal one
	}

	glViewport(0, 0, width, height);						// Reset the current viewport

	glMatrixMode(GL_PROJECTION);						// Select the projection matrix
	glLoadIdentity();									// Reset the projection matrix

	// Calculate the aspect ratio of the window
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select the modelview matrix
	glLoadIdentity();									// Reset the modelview matrix
}

void idle()  //Perform continuous animation
{
	glutPostRedisplay();
}

void redraw()
{
	//display in wireframe mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glClear(GL_COLOR_BUFFER_BIT);  //Clear window
	glLoadIdentity();  //Reset the current modelview matrix

	//Translation
	glPushMatrix();
	glTranslatef(-2.7f, 0.0f, -6.0f);  //Place the table left
	glTranslatef(0.0f, fTranslate, 0.0f);  //Translate in Y direction
	Draw_Table();  //Draw table
	glPopMatrix();

	//Rotation
	glPushMatrix();
	glTranslatef(0.0f, 0.4f, -6.0f);  //Place the table at center
	glRotatef(fRotate, 0, 1.0f, 0);  //Rotate around Y direction
	glTranslatef(-0.5f, -0.4f, 0.0f);  //Rotate the table around the center
	Draw_Table();  //Draw table
	glPopMatrix();

	//Scaling
	glPushMatrix();
	glTranslatef(1.7f, 0.0f, -6.0f);  //Place the table right
	glScalef(fScale, fScale, fScale);  //Scale
	Draw_Table();  //Draw table
	glPopMatrix();

	//Update the factors
	fTranslate += 0.005f;
	fRotate += 0.5f;
	fScale -= 0.005f;

	if (fTranslate > 0.5f) fTranslate = 0.0f;  //Return to the original position when moving to a certain position.
	if (fScale < 0.5f) fScale = 1.0f; //Change back to its original size when it is reduced to a certain extend.
	glutSwapBuffers();

}
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);  //Initialize the glut library 
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);  //Specify the window display mode that the function glutCreateWindow will create. RGB mode Double buffering 
	glutInitWindowPosition(100, 100);  //Set the window position, which is the position of the top left corner of the window relative to the entire screen
	glutInitWindowSize(640, 480);  // Set the window size (may be covered by other windows)
	glutCreateWindow("Exercise2");  //Set the window title
	glutDisplayFunc(redraw);  //Register a draw callback function that specifies the function to call when the window content needs to be redrawn
	glutReshapeFunc(reshape);  //the callback function when the registration window size changes.
	glutIdleFunc(idle);  //Perform continuous animation
	glutMainLoop();  //Glut event processing loop
	return 0;
}
