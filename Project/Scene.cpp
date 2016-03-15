#include <windows.h>
#include <GL\glut.h>
#include <math.h>
#include <stdio.h>

// angle of rotation for the camera direction
float angle=0.0;
// actual vector representing the camera’s direction
float lx=0.0f,lz=-1.0f, ly = 0.0f;
// XZ position of the camera
float x=0.0f,y = 1.0f, z=5.0f;
float gravity = 0.03f;
float jumpHeight = 3.0f;
float maxJumpHeight = 4.0f;
bool isJumping = false;
float moveSpeed = 1.0f;
float deltaAngle = 0.0f;
float oldMouseX = 0;
float oldMouseY = 0;

void drawSnowMan() {

	glColor3f(1.0f, 1.0f, 1.0f);
	
	// Draw Body
	glTranslatef(0.0f ,0.75f, 0.0f);
	glutSolidSphere(0.75f,20,20);
	
	// Draw Head
	glTranslatef(0.0f, 0.85f, 0.0f);
	glutSolidSphere(0.29f,20,20);
	
	// Draw Eyes
	glPushMatrix();
	glColor3f(0.0f,0.0f,0.0f);
	glTranslatef(0.05f, 0.10f, 0.25f);
	glutSolidSphere(0.05f,10,10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f,10,10);
	glPopMatrix();
	
	// Draw Nose
	glColor3f(1.0f, 0.5f , 0.5f);
	glRotatef(0.0f,1.0f, 0.0f, 0.0f);
	glutSolidCone(0.1f,0.5f,10,2);
}

void drawWalls(){
	glColor3f(1.0,1.0,1.0);
	
	//Draw Northern Wall
	
	glBegin(GL_QUADS);
	glVertex3f(1000.0,0.0,100.0);
	glVertex3f(1000.0, 0.0, 100.0);
	glVertex3f(-1000.0,50.0,100.0);
	glVertex3f(-1000.0,0.0,100.0);
	glEnd();
}

void renderScene(void) {

	// Clear Color and Depth Buffers

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(	x, y, z,
				x+lx, y+ly, z+lz,
				0.0f, 1.0f, 0.0f);
				
		if(y > 1.0){
			y -= gravity;
			gluLookAt(	x, y, z,
				x+lx, y, z+lz,
				0.0f, 1.0f, 0.0f);
				
		}
		
		
		if( y == 1.0){
			isJumping == true;
		}

	// Draw ground
	glColor3f(0.2f, 0.2f, 0.8f);
	glBegin(GL_QUADS);
	glVertex3f(-100.0f, 0.0f, -100.0f);
	glVertex3f(-100.0f, 0.0f, 100.0f);
	glVertex3f( 100.0f, 0.0f, 100.0f);
	glVertex3f( 100.0f, 0.0f, -100.0f);
	glEnd();

	drawWalls();
	// Draw 36 SnowMen
	for(int i = -3; i < 3; i++)
		for(int j=-3; j < 3; j++) {
			glPushMatrix();
			glTranslatef(i*10.0,0,j * 10.0);
			drawSnowMan();
			glPopMatrix();
		}
		glutSwapBuffers();
	}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
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

void processNormalKeys(unsigned char key, int MouseX, int MouseY) {

	if (key == 27)
	exit(0);
	else if(key == 'a'){
		angle -= 0.1;
		lx = sin(angle);
		lz = cos(angle) * -1;
	}
	else if(key == 'd'){
		angle += 0.1;
		lx = sin(angle);
		lz = cos(angle) * -1;
	}
	else if(key == 'w'){
		x+=lx;
		z += lz;
	}
	else if(key == 's'){
		z -= lz;
		x-=lx;
	}
}

void moveMouse(int MouseX, int MouseY)
{
 /* int diffX = MouseX - oldMouseX;
  int diffY = MouseY - oldMouseY;
  oldMouseX = MouseX;
  oldMouseY = MouseY;
  lx += (float) diffY;
  lz += (float) diffX;
  gluLookAt(	diffX, diffY, z,
				x+lx, y, z+lz,
				0.0f, 1.0f, 0.0f);*/
  int diffY = MouseY - oldMouseY;
  deltaAngle = (x - oldMouseX) * 0.001f;
  lx = sin(angle + deltaAngle);
  lz = cos(angle + deltaAngle);
  //ly += 1;
 // int diffY = MouseY - oldMouseY;
  oldMouseX = MouseX;
  oldMouseY = MouseY;
  
}

void processSpecialKeys(int key, int xx, int yy) {

	float fraction = 1.0f;
	float hypotenusex = lx / sin(angle);
	float hypotenusez = lz / cos(angle);
	switch (key) {
		case GLUT_KEY_LEFT :
					/*
						angle -= 0.1;
						lx = sin(angle) ;
						lz = cos(angle) * -1 ;
						*/
						break;
		case GLUT_KEY_RIGHT :
					/*
						angle += 0.1;
						lx = sin(angle);
						lz = cos(angle) * -1;*/
						break;
		case GLUT_KEY_UP :
						x+=lx;
						z += lz;
						
						break;
		case GLUT_KEY_DOWN :
						z -= lz;
						x-=lx;
						break;
						
		case GLUT_KEY_F1:
						printf("jump");
					
						if(y < maxJumpHeight){
							y+=jumpHeight;
							isJumping = true;
							
							
						}
						break;
						
						
	}	
}

int main(int argc, char **argv) {

	// init GLUT and create window
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(600,600);
	glutCreateWindow("Lighthouse3D – GLUT Tutorial");
	
	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutFullScreen();
	glutIdleFunc(renderScene);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutPassiveMotionFunc(moveMouse);
	// OpenGL init
	glEnable(GL_DEPTH_TEST);
	
	// enter GLUT event processing cycle
	glutMainLoop();

}
