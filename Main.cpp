// This program is from the OpenGL Programming Guide. It shows a robot.
// This program is a modified version of the original source codes
// https://cs.lmu.edu/~ray/notes/openglexamples/
// This program runs well under the settings you have done for the Assignment1. Please go
// back to settings if you have compile or link errors for this program.
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <vector>

#define BODY_WIDTH 2
#define BODY_HEIGHT 4
#define BODY_DEPTH 1

// DEFAULT IS 3!!!!!!!!
const int ROBOT_COUNT = 10; // realize that this is actually only a quarter of the number of robots :)

std::vector<std::vector<int>> positions;
std::vector<int> pos = { 0, 0, 0 };

// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 0.0f, z = 5.0f;
// all variables initialized to 1.0, meaning
// the triangle will initially be white
float red = 1.0f, blue = 1.0f, green = 1.0f;
// angle for rotating triangle
float angle = 0.0f;

// The robot arm is specified by (1) the angle that the upper arm makes
// relative to the x-axis, called shoulderAngle, and (2) the angle that the
// lower arm makes relative to the upper arm, called elbowAngle. These angles
// are adjusted in 5 degree increments by a keyboard callback.
static int shoulderAngle = 0, elbowAngle = 0, hipAngle = 0, kneeAngle = 0;
static int shoulderAngle2 = 0, elbowAngle2 = 0, hipAngle2 = 0, kneeAngle2 = 0;
float height = 0;
bool baxis = true, isWire = true, rising = true, floating = false, flail = false;
// Handles the keyboard event: the left and right arrows bend the elbow, the
// up and down keys bend the shoulder.

int randNum(int max) {
	return rand() % max + 1;
}

//  to reshape the display when the window size is changed by the user
void changeSize(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0) h = 1;
	float ratio = w * 1.0 / h;
	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);
	// Reset Matrix
	glLoadIdentity();
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void randomize() {
	positions.clear();
	for (int i = -ROBOT_COUNT; i < ROBOT_COUNT; ++i) {
		for (int j = -ROBOT_COUNT; j < ROBOT_COUNT; ++j) {
			pos[0] = i * (rand() % (ROBOT_COUNT * 10) + 1);
			pos[2] = j * (rand() % (ROBOT_COUNT * 10) + 1);
			positions.push_back(pos);
		}
	}
}

void special(int key, int, int) {
	float fraction = 0.5f;
	switch (key) {
	case GLUT_KEY_F4: (elbowAngle -= 5) %= 360; (elbowAngle2 -= 5) %= 360; break;
	case GLUT_KEY_F5: (shoulderAngle -= 5) %= 360; (shoulderAngle2 -= 5) %= 360; break;
	case GLUT_KEY_F6: (hipAngle -= 5) %= 360; (hipAngle2 -= 5) %= 360; break;
	case GLUT_KEY_F7: (kneeAngle -= 5) %= 360; (kneeAngle2 -= 5) %= 360; break;
	case GLUT_KEY_PAGE_UP: 
		(shoulderAngle += 5) %= 360; (elbowAngle += 5) %= 360; (hipAngle -= 5) %= 360; (kneeAngle -= 5) %= 360; 
		(shoulderAngle2 += 5) %= 360; (elbowAngle2 += 5) %= 360; (hipAngle2 -= 5) %= 360; (kneeAngle2 -= 5) %= 360;
		break;
	case GLUT_KEY_PAGE_DOWN: 
		(shoulderAngle -= 5) %= 360; (elbowAngle -= 5) %= 360; (hipAngle += 5) %= 360; (kneeAngle += 5) %= 360; 
		(shoulderAngle2 -= 5) %= 360; (elbowAngle2 -= 5) %= 360; (hipAngle2 += 5) %= 360; (kneeAngle2 += 5) %= 360;
		break;
	case GLUT_KEY_LEFT:
		angle -= 0.02f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_RIGHT:
		angle += 0.02f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_UP:
		x += lx * fraction;
		z += lz * fraction;
		break;
	case GLUT_KEY_DOWN:
		x -= lx * fraction;
		z -= lz * fraction;
		break;
	case GLUT_KEY_INSERT:
		floating = floating ? false : true;
		break;
	case GLUT_KEY_END:
		flail = flail ? false : true;
		break;
	case GLUT_KEY_F9:
		shoulderAngle = 0;
		shoulderAngle2 = 0;
		elbowAngle = 0;
		elbowAngle2 = 0;
		hipAngle = 0;
		hipAngle2 = 0;
		kneeAngle = 0;
		kneeAngle2 = 0;
		break;
	case GLUT_KEY_F8:
		randomize();
		break;
	default: return;
	}
	glutPostRedisplay();
}
// wireBox(w, h, d) makes a wireframe box with width w, height h and
// depth d centered at the origin. It uses the GLUT wire cube function.
// The calls to glPushMatrix and glPopMatrix are essential here; they enable
// this function to be called from just about anywhere and guarantee that
// the glScalef call does not pollute code that follows a call to myWireBox.
void wireBox(GLdouble width, GLdouble height, GLdouble depth) {
	if (isWire) {
		glPushMatrix();
		glColor3f(1, 1, 1);
		glScalef(width, height, depth);
		glutWireCube(1.0);
		glPopMatrix();
	}
	else {
		glPushMatrix();
		glColor3f(1, 1, 1);
		glScalef(width, height, depth);
		glutSolidCube(1.0);
		glPopMatrix();
	}
}
void drawAxes()
{
	// Draw a red x-axis, a green y-axis, and a blue z-axis.
	glBegin(GL_LINES);
	glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(5, 0, 0);
	glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 5, 0);
	glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 5);
	glEnd();
}

void drawRobot(GLdouble x, GLdouble y, GLdouble z) {
	glPushMatrix();
	glTranslatef(x, y, z);
	// Draw the upper body at the orgin
	wireBox(BODY_WIDTH, BODY_HEIGHT, BODY_DEPTH);
	// Draw the upper arm, rotated shoulder degrees about the z-axis. Note that
	// the thing about glutWireBox is that normally its origin is in the middle
	// of the box, but we want the "origin" of our box to be at the left end of
	// the box, so it needs to first be shifted 1 unit in the x direction, then
	// rotated.
	glTranslatef(1.0, 1.5, 0.0); // (4) move to the right end of the upper body (attachment)
	glRotatef((GLfloat)shoulderAngle, 0.0, 0.0, 1.0); //(3) then rotate shoulder
	glTranslatef(1.0, 0.0, 0.0); // (2) shift to the right on the x axis to have the left end at the origin
	wireBox(2.0, 0.4, 1.0); // (1) draw the upper arm box
	// Now we are ready to draw the lower arm. Since the lower arm is attached
	// to the upper arm we put the code here so that all rotations we do are
	// relative to the rotation that we already made above to orient the upper
	// arm. So, we want to rotate elbow degrees about the z-axis. But, like
	// before, the anchor point for the rotation is at the end of the box, so
	// we translate <1,0,0> before rotating. But after rotating we have to
	// position the lower arm at the end of the upper arm, so we have to
	// translate it <1,0,0> again.
	glTranslatef(1.0, 0.0, 0.0); // (4) move to the right end of the upper arm
	glRotatef((GLfloat)elbowAngle, 0.0, 0.0, 1.0); // (3) rotate
	glTranslatef(1.0, 0.0, 0.0); // (2) shift to the right on the x axis to have the left end at the origin
	wireBox(2.0, 0.4, 1.0); // (1) draw the lower arm
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, z);
	//Left arm
	glTranslatef(-1.0, 1.5, 0.0); // (4) move to the left end of the upper body (attachment)
	glRotatef(-(GLfloat)shoulderAngle2, 0.0, 0.0, 1.0); //(3) then rotate shoulder
	glTranslatef(-1.0, 0.0, 0.0); // (2) shift to the right on the x axis to have the left end at the origin
	wireBox(2.0, 0.4, 1.0); // (1) draw the upper arm box
	// Now we are ready to draw the lower arm. Since the lower arm is attached
	// to the upper arm we put the code here so that all rotations we do are
	// relative to the rotation that we already made above to orient the upper
	// arm. So, we want to rotate elbow degrees about the z-axis. But, like
	// before, the anchor point for the rotation is at the end of the box, so
	// we translate <1,0,0> before rotating. But after rotating we have to
	// position the lower arm at the end of the upper arm, so we have to
	// translate it <1,0,0> again.
	glTranslatef(-1.0, 0.0, 0.0); // (4) move to the right end of the upper arm
	glRotatef(-(GLfloat)elbowAngle2, 0.0, 0.0, 1.0); // (3) rotate
	glTranslatef(-1.0, 0.0, 0.0); // (2) shift to the right on the x axis to have the left end at the origin
	wireBox(2.0, 0.4, 1.0); // (1) draw the lower arm

	glPopMatrix();

	// left leg
	glPushMatrix();
	glTranslatef(x, y, z);
	// hip
	glTranslatef(-0.7, -2, 0.0);
	glRotatef(-(GLfloat)hipAngle2, 0.0, 0.0, 1.0); 
	glTranslatef(0.0, -1.0, 0.0); 
	wireBox(0.6, 2.0, 1.0);
	// shin
	glTranslatef(0.0, -1.0, 0.0); 
	glRotatef(-(GLfloat)kneeAngle2, 0.0, 0.0, 1.0); 
	glTranslatef(0.0, -1.0, 0.0); 
	wireBox(0.4, 2.0, 1.0); 

	glPopMatrix();

	// left leg
	glPushMatrix();
	glTranslatef(x, y, z);
	// hip
	glTranslatef(0.7, -2, 0.0);
	glRotatef((GLfloat)hipAngle, 0.0, 0.0, 1.0);
	glTranslatef(0.0, -1.0, 0.0);
	wireBox(0.6, 2.0, 1.0);
	// shin
	glTranslatef(0.0, -1.0, 0.0);
	glRotatef((GLfloat)kneeAngle, 0.0, 0.0, 1.0);
	glTranslatef(0.0, -1.0, 0.0);
	wireBox(0.4, 2.0, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, z);
	glTranslatef(0.0, 3, 0.0);
	if (isWire) {
		glutWireSphere(1, 15, 15);
	}
	else {
		glutSolidSphere(1, 15, 15);
	}
	glPopMatrix();
}

void drawScene()
{
	drawRobot(0, 0, 0);
}
// Displays the arm in its current position and orientation. The whole
// function is bracketed by glPushMatrix and glPopMatrix calls because every
// time we call it we are in an "environment" in which a gluLookAt is in
// effect. (Note that in particular, replacing glPushMatrix with
// glLoadIdentity makes you lose the camera setting from gluLookAt).
void display() {
	//glClear(GL_COLOR_BUFFER_BIT);
	//glMatrixMode(GL_MODELVIEW);
	//drawScene(); // draw a robot
	if (floating) {
		if (height <= 0) {
			rising = true;
		}
		else if (height >= 2) {
			rising = false;
		}

		if (rising) {
			height += 0.01f;
		}
		else {
			height -= 0.01f;
		}
	}
	if (flail) {
		(shoulderAngle -= randNum(5)) %= 360;
		(elbowAngle -= randNum(5)) %= 360;
		(hipAngle += randNum(5)) %= 360;
		(kneeAngle += randNum(5)) %= 360;
		(shoulderAngle2 -= randNum(5)) %= 360;
		(elbowAngle2 -= randNum(5)) %= 360;
		(hipAngle2 += randNum(5)) %= 360;
		(kneeAngle2 += randNum(5)) %= 360;
	}
	

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(x, 1.0f, z, x + lx, 1.0f, z + lz, 0.0f, 1.0f, 0.0f);

	if (baxis) {
		drawAxes(); // draw axes
	}

	// Draw the ground (a plane)
	glColor3f(0.52f, 0.13f, 0.13f);
	glBegin(GL_QUADS);
	glVertex3f(-1000.0f, 0.0f, -1000.0f);
	glVertex3f(-1000.0f, 0.0f, 1000.0f);
	glVertex3f(1000.0f, 0.0f, 1000.0f);
	glVertex3f(1000.0f, 0.0f, -1000.0f);
	glEnd();
	// Draw 36 SnowMen
	for (auto e : positions) {
		glPushMatrix();
		glTranslatef(e[0], e[1] + height, e[2]);
		drawRobot(0, 0, 0);
		glPopMatrix();
	}

	glutSwapBuffers(); // for double buffer display
	glFlush();
}
// Handles the reshape event by setting the viewport so that it takes up the
// whole visible region, then sets the projection matrix to something reason-
// able that maintains proper aspect ratio.
void reshape(GLint w, GLint h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, GLfloat(w) / GLfloat(h), 1.0, 20.0);
}
void procKeys(unsigned char key, int x, int y)
{
	switch (key) {
	case '1': isWire = true; break;
	case '2': isWire = false; break;
	case '3': baxis = !baxis; // toggle on off drawing axes
		printf("%d\n", baxis);
		break;
	case '4': (elbowAngle += 5) %= 360; (elbowAngle2 += 5) %= 360; break;
	case '5': (shoulderAngle += 5) %= 360; (shoulderAngle2 += 5) %= 360; break;
	case '6': (hipAngle += 5) %= 360; (hipAngle2 += 5) %= 360; break;
	case '7': (kneeAngle += 5) %= 360; (kneeAngle2 += 5) %= 360; break;
	case 27: exit(0); break;
	}
	glutPostRedisplay();
}
// Perfroms application specific initialization: turn off smooth shading,
// sets the viewing transformation once and for all. In this application we
// won't be moving the camera at all, so it makes sense to do this.
void init() {
	srand(time(nullptr));

	randomize();

	glShadeModel(GL_FLAT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1, 2, 8, 0, 0, 0, 0, 1, 0);
}

void idle() {
	display();
}
// Initializes GLUT, the display mode, and main window; registers callbacks;
// does application initialization; enters the main event loop.
int main(int argc, char** argv) {
	printf("\n\
-----------------------------------------------------------------------\n\
 OpenGL Sample Program for a robot:\n\
 - '1': Wireframe \n\
 - '2': Solid \n\
 - '3': toggle on off to draw axes \n\
 - '4': increment the elbowAngle \n\
 - F4: decrement the elbowAngle \n\
 - '5': increment shoulderAngle \n\
 - F5: decrement shoulderAngle \n\
 - '6': increment hip angle \n\
 - F6: decrement hip angle \n\
 - '7': increment knee angle \n\
 - F7: decrement knee angle \n\
 - Page Up: Flail Direction 1 \n\
 - Page Down: Flail Direction 2 \n\
 - Arrow Keys: Move Camera \n\
 - End: Initiate flail randomly \n\
 - Insert: Initiate Floating \n\
 - F9: reset limb positions \n\
 - F8: Randomize Robot locations \n\
 - ESC to quit \n\
-----------------------------------------------------------------------\n");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Robot Arm");
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(procKeys);
	glutSpecialFunc(special);
	init();
	glutMainLoop();
}