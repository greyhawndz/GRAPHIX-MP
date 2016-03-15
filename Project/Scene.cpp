#include <windows.h>
#include <GL\glut.h>
#include <math.h>
#include <stdio.h>
#include <SOIL.h>

#define GL_BGR 0x80E0 
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
unsigned int tex;

GLuint	texture[1];			// Storage For One Texture ( NEW )

/*
// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	unsigned char * data;
*/


/*
unsigned int loadTexture(const char* filename){
	SDL_Surface* img= SDL_LoadBMP(filename);
	unsigned int id;
	printf("I'm here");
	glGenTextures(1,&id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->w, img->h, 
				0,GL_RGB,GL_UNSIGNED_INT, img->pixels);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(img);
	return id;
}
*/

/*
GLuint loadBMP_custom(const char * imagepath){
	
	
	
	FILE * file = fopen(imagepath,"rb");
	if (!file)
	{
		printf("Image could not be opened\n"); 
		return 0;
	}
	if ( fread(header, 1, 54, file)!=54 )
	{ // If not 54 bytes read : problem
     	printf("Not a correct BMP file\n");
     	return false;
 	}
 	
 	if ( header[0]!='B' || header[1]!='M' ){
	    printf("Not a correct BMP file\n");
	    return 0;
	 }
	
	//read ints from bytearray 
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);
	
	if (imageSize==0)    
	imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)
	dataPos=54;
	
	//create buffer
	data = new unsigned char [imageSize];
	//read actual data from file into the buffer
	fread(data,1,imageSize,file);
	
	fclose(file);
	
	GLuint textureID;
	glGenTextures(1, &textureID);
	
	
	return textureID;
}
*/



int LoadGLTextures()                                    // Load Bitmaps And Convert To Textures
{
    /* load an image file directly as a new OpenGL texture */
    texture[0] = SOIL_load_OGL_texture
        (
        "wall.bmp",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
        );

    if(texture[0] == 0)
        return false;
 
 
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 
    return true;                                        // Return Success
}

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
	if(LoadGLTextures()){
		glBindTexture(GL_TEXTURE_2D, texture[0]);
	}
	
	
	
	
	
	//Draw Northern Wall
	
	glBegin(GL_QUADS);
	glTexCoord2f(1.0,0.0);
	glVertex3f(1,0.0,100.0);
	glTexCoord2f(1.0,1.0);
	glVertex3f(1, 1.0, 100.0);
	glTexCoord2f(0.0,1.0);
	glVertex3f(0.0,1.0,100.0);
	glTexCoord2f(0.0,0.0);
	glVertex3f(0.0,0.0,100.0);
	
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
	//Start SDL
   
    
    
    
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(600,600);
	glutCreateWindow("Lighthouse3D – GLUT Tutorial");
	// OpenGL init
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
//	glutFullScreen();
	glutIdleFunc(renderScene);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
//	glutPassiveMotionFunc(moveMouse);
	
	
	// enter GLUT event processing cycle
	glutMainLoop();

}
