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
float x=5.0f,y = 5.0f, z=0.0f;
float gravity = 0.8f;
float jumpHeight = 3.0f;
float maxJumpHeight = 8.0f;
bool isJumping = false;
float moveSpeed = 1.0f;
float deltaAngle = 0.0f;
float oldMouseX = 0;
float oldMouseY = 0;
unsigned int tex;

int mapHeight = 100;
int mapWidth = 100;
int map_x = 9;
int map_y = 9;
int steps = 0;

bool isFacingLeft = false;
bool isFacingRight = false;
bool isFacingNorth = true;
bool isFacingSouth = false;

int mapData[20][20] = {
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0.0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},

};

int curY = 0;
int curX = 0;
int rMouseFlag = 0, lMouseFlag = 0;

GLuint	texture[2];			// Storage For One Texture ( NEW )

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
        "hammertone.bmp",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
        );
        
        texture[1] = SOIL_load_OGL_texture
        (
        "wood.bmp",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
        );

    if(texture[0] == 0)
        return false;
    if(texture[1] == 0){
    	return false;
    }
 
 
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   	
   	glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 
    return true;                                        // Return Success
}

int isPassable(int x, int y){
	if(x < 0|| y<0 || x > 19 || y > 19){
		printf("Collision detected\n");
		return 0;
	}
	if(mapData[y][x] != 0){
		printf("Collision detected");
		return 0;
	}
	
	return 1;
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
	
//	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);	
	int i;
	float dist = 10.55;
	float increment = 15;
	float startpoint = 5;
	float height = 13;
//	glClearColor3f(0.5,0.2,0.3);
	

/*	if(LoadGLTextures()){
		glBindTexture(GL_TEXTURE_2D, texture[0]);
	}
	*/
/*	
	glBegin(GL_QUADS);
	glTexCoord2f(1.0,0.0);
	glVertex3f(startpoint,0.0,100.0);
	glTexCoord2f(1.0,1.0);
	glVertex3f(startpoint, height, 100.0);
	glTexCoord2f(0.0,1.0);
	glVertex3f(-startpoint,height,100.0);
	glTexCoord2f(0.0,0.0);
	glVertex3f(-startpoint,0.0,100.0);
	glEnd();
	
	
	//Draw to left of Northern Wall
	for(i = 0; i < 9; i++){
		increment = dist + startpoint;
		
		glBegin(GL_QUADS);
		glTexCoord2f(1.0,0.0);
		glVertex3f(startpoint,0.0,100.0);
		glTexCoord2f(1.0,1.0);
		glVertex3f(startpoint, height, 100.0);
		glTexCoord2f(0.0,1.0);
		glVertex3f(increment,height,100.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(increment,0.0,100.0);
		glEnd();
		startpoint = increment;
		
	}
	startpoint = 5;
	increment = 0;
	//Draw LeftSide of northern wall
	for(i = 0; i < 9; i++){
		increment = dist + startpoint;
		glBegin(GL_QUADS);
		glTexCoord2f(1.0,0.0);
		glVertex3f(-startpoint,0.0,100.0);
		glTexCoord2f(1.0,1.0);
		glVertex3f(-startpoint, height, 100.0);
		glTexCoord2f(0.0,1.0);
		glVertex3f(-increment,height,100.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(-increment,0.0,100.0);
		glEnd();
		startpoint = increment;
	}
	
	startpoint = 5;
	increment = 0;
	
	glBegin(GL_QUADS);
	glTexCoord2f(1.0,0.0);
	glVertex3f(startpoint,0.0,-100.0);
	glTexCoord2f(1.0,1.0);
	glVertex3f(startpoint, height, -100.0);
	glTexCoord2f(0.0,1.0);
	glVertex3f(-startpoint,height,-100.0);
	glTexCoord2f(0.0,0.0);
	glVertex3f(-startpoint,0.0,-100.0);
	glEnd();
	//southern wall
	for(i = 0; i < 9; i++){
		increment = dist + startpoint;
		
		glBegin(GL_QUADS);
		glTexCoord2f(1.0,0.0);
		glVertex3f(startpoint,0.0,-100.0);
		glTexCoord2f(1.0,1.0);
		glVertex3f(startpoint, height, -100.0);
		glTexCoord2f(0.0,1.0);
		glVertex3f(increment,height,-100.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(increment,0.0,-100.0);
		glEnd();
		startpoint = increment;
		
	}
	
	startpoint = 5;
	increment = 0;
	
	for(i = 0; i < 9; i++){
		increment = dist + startpoint;
		glBegin(GL_QUADS);
		glTexCoord2f(1.0,0.0);
		glVertex3f(-startpoint,0.0,-100.0);
		glTexCoord2f(1.0,1.0);
		glVertex3f(-startpoint, height, -100.0);
		glTexCoord2f(0.0,1.0);
		glVertex3f(-increment,height,-100.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(-increment,0.0,-100.0);
		glEnd();
		startpoint = increment;
	}
		float xStartpoint = increment;
		startpoint = 5;
		increment = 0;
		
	
	for(i = 0; i < 10; i++){
		increment = dist + startpoint;
		glBegin(GL_QUADS);
		glTexCoord2f(1.0,0.0);
		glVertex3f(-xStartpoint,0.0,-startpoint);
		glTexCoord2f(1.0,1.0);
		glVertex3f(-xStartpoint, height, -startpoint);
		glTexCoord2f(0.0,1.0);
		glVertex3f(-xStartpoint,height,-increment);
		glTexCoord2f(0.0,0.0);
		glVertex3f(-xStartpoint,0.0,-increment);
		glEnd();
		startpoint = increment;
	}
	    
		startpoint = 5;
		increment = 0;
		
		
		
	for(i = 0; i < 10; i++){
		increment = dist + startpoint;
		glBegin(GL_QUADS);
		glTexCoord2f(1.0,0.0);
		glVertex3f(-xStartpoint,0.0,startpoint);
		glTexCoord2f(1.0,1.0);
		glVertex3f(-xStartpoint, height, startpoint);
		glTexCoord2f(0.0,1.0);
		glVertex3f(-xStartpoint,height,increment);
		glTexCoord2f(0.0,0.0);
		glVertex3f(-xStartpoint,0.0,increment);
		glEnd();
		startpoint = increment;
	}
		startpoint = 5;
		increment = 0;
		
		glBegin(GL_QUADS);
		glVertex3f(-xStartpoint,0.0,startpoint);
		glTexCoord2f(1.0,1.0);
		glVertex3f(-xStartpoint,height,-startpoint);
		glTexCoord2f(0.0,1.0);
		glVertex3f(-xStartpoint,height,startpoint);
		glTexCoord2f(1.0,0.0);
		glVertex3f(-xStartpoint,0.0,-startpoint);
		glTexCoord2f(0.0,0.0);
		glEnd();
	
	for(i = 0; i < 10; i++){
		increment = dist + startpoint;
		glBegin(GL_QUADS);
		glTexCoord2f(1.0,0.0);
		glVertex3f(xStartpoint,0.0,-startpoint);
		glTexCoord2f(1.0,1.0);
		glVertex3f(xStartpoint, height, -startpoint);
		glTexCoord2f(0.0,1.0);
		glVertex3f(xStartpoint,height,-increment);
		glTexCoord2f(0.0,0.0);
		glVertex3f(xStartpoint,0.0,-increment);
		glEnd();
		startpoint = increment;
		
	}
	    
		startpoint = 5;
		increment = 0;
		
		glBegin(GL_QUADS);
		glVertex3f(xStartpoint,height,startpoint);
		glTexCoord2f(1.0,1.0);
		glVertex3f(xStartpoint,height,-startpoint);
		glTexCoord2f(0.0,1.0);
		glVertex3f(xStartpoint,0.0,-startpoint);
		glTexCoord2f(1.0,0.0);
		glVertex3f(xStartpoint,0.0,startpoint);
		glTexCoord2f(0.0,0.0);
		glEnd();
		
		
	for(i = 0; i < 10; i++){
		increment = dist + startpoint;
		glBegin(GL_QUADS);
		glTexCoord2f(1.0,0.0);
		glVertex3f(xStartpoint,0.0,startpoint);
		glTexCoord2f(1.0,1.0);
		glVertex3f(xStartpoint, height, startpoint);
		glTexCoord2f(0.0,1.0);
		glVertex3f(xStartpoint,height,increment);
		glTexCoord2f(0.0,0.0);
		glVertex3f(xStartpoint,0.0,increment);
		glEnd();
		startpoint = increment;
		
	}
	
	*/
	glColor3f(0.0,0.0,0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(-100.0,height,100.0);
	glVertex3f(100.0,height, 100.0);
	glVertex3f(100.0,0.0,100.0);
	glVertex3f(-100.0, 0.0, 100.0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(-100.0,0.0,100.0);
	glVertex3f(-100.0,0.0,95.0);
	glVertex3f(-100.0,height,95.0);
	glVertex3f(-100.0, height, 100.0);
	glEnd();
	
	glBegin(GL_LINE_LOOP);
	glVertex3f(-100.0,height, 95.0);
	glVertex3f(100.0,height, 95.0);
	glVertex3f(100.0,0.0, 95.0);
	glVertex3f(-100.0, 0.0, 95.0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(100.0,height,100.0);
	glVertex3f(100.0,height,95.0);
	glVertex3f(100.0,0.0,95.0);
	glVertex3f(100.0, 0.0, 100.0);
	glEnd();
	
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_QUADS);
	glVertex3f(-100.0,height,100.0);
	glVertex3f(100.0,height, 100.0);
	glVertex3f(100.0,0.0,100.0);
	glVertex3f(-100.0, 0.0, 100.0);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(-100.0,0.0,100.0);
	glVertex3f(-100.0,0.0,95.0);
	glVertex3f(-100.0,height,95.0);
	glVertex3f(-100.0, height, 100.0);
	glEnd();
	
	glBegin(GL_QUADS);
	glVertex3f(-100.0,height, 95.0);
	glVertex3f(100.0,height, 95.0);
	glVertex3f(100.0,0.0, 95.0);
	glVertex3f(-100.0, 0.0, 95.0);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(100.0,height,100.0);
	glVertex3f(100.0,height,95.0);
	glVertex3f(100.0,0.0,95.0);
	glVertex3f(100.0, 0.0, 100.0);
	glEnd();
	
	//SouthWall
	glColor3f(0.0,0.0,0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(-100.0,height,-100.0);
	glVertex3f(100.0,height, -100.0);
	glVertex3f(100.0,0.0,-100.0);
	glVertex3f(-100.0, 0.0, -100.0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(-100.0,0.0,-100.0);
	glVertex3f(-100.0,0.0,-95.0);
	glVertex3f(-100.0,height,-95.0);
	glVertex3f(-100.0, height, -100.0);
	glEnd();
	
	glBegin(GL_LINE_LOOP);
	glVertex3f(-100.0,height, -95.0);
	glVertex3f(100.0,height, -95.0);
	glVertex3f(100.0,0.0, -95.0);
	glVertex3f(-100.0, 0.0, -95.0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(100.0,height,-100.0);
	glVertex3f(100.0,height,-95.0);
	glVertex3f(100.0,0.0,-95.0);
	glVertex3f(100.0, 0.0, -100.0);
	glEnd();
	
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_QUADS);
	glVertex3f(-100.0,height,-100.0);
	glVertex3f(100.0,height, -100.0);
	glVertex3f(100.0,0.0,-100.0);
	glVertex3f(-100.0, 0.0, -100.0);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(-100.0,0.0,-100.0);
	glVertex3f(-100.0,0.0,-95.0);
	glVertex3f(-100.0,height,-95.0);
	glVertex3f(-100.0, height, -100.0);
	glEnd();
	
	glBegin(GL_QUADS);
	glVertex3f(-100.0,height, -95.0);
	glVertex3f(100.0,height, -95.0);
	glVertex3f(100.0,0.0, -95.0);
	glVertex3f(-100.0, 0.0, -95.0);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(100.0,height,-100.0);
	glVertex3f(100.0,height,-95.0);
	glVertex3f(100.0,0.0,-95.0);
	glVertex3f(100.0, 0.0, -100.0);
	glEnd();
	
	//Right Wall
	glColor3f(0.0,0.0,0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(100.0,height,100.0);
	glVertex3f(100.0,height, -100.0);
	glVertex3f(100.0,0.0,-100.0);
	glVertex3f(100.0, 0.0, 100.0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(100.0,0.0,-100.0);
	glVertex3f(95.0,0.0,-100.0);
	glVertex3f(95.0,height,-100.0);
	glVertex3f(100.0, height, -100.0);
	glEnd();
	
	glBegin(GL_LINE_LOOP);
	glVertex3f(95.0,height, -100.0);
	glVertex3f(95.0,height, 100.0);
	glVertex3f(95.0,0.0, 100.0);
	glVertex3f(95.0, 0.0, -100.0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(100.0,height,100.0);
	glVertex3f(95.0,height,100.0);
	glVertex3f(95.0,0.0,100.0);
	glVertex3f(100.0, 0.0, 100.0);
	glEnd();
	
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_QUADS);
	glVertex3f(100.0,height,100.0);
	glVertex3f(100.0,height, -100.0);
	glVertex3f(100.0,0.0,-100.0);
	glVertex3f(100.0, 0.0, 100.0);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(100.0,0.0,-100.0);
	glVertex3f(95.0,0.0,-100.0);
	glVertex3f(95.0,height,-100.0);
	glVertex3f(100.0, height, -100.0);
	glEnd();
	
	glBegin(GL_QUADS);
	glVertex3f(95.0,height, -100.0);
	glVertex3f(95.0,height, 100.0);
	glVertex3f(95.0,0.0, 100.0);
	glVertex3f(95.0, 0.0, -100.0);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(100.0,height,100.0);
	glVertex3f(95.0,height,100.0);
	glVertex3f(95.0,0.0,100.0);
	glVertex3f(100.0, 0.0, 100.0);
	glEnd();
	
	//Left Wall
	glColor3f(0.0,0.0,0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(-100.0,height,100.0);
	glVertex3f(-100.0,height, -100.0);
	glVertex3f(-100.0,0.0,-100.0);
	glVertex3f(-100.0, 0.0, 100.0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(-100.0,0.0,-100.0);
	glVertex3f(-95.0,0.0,-100.0);
	glVertex3f(-95.0,height,-100.0);
	glVertex3f(-100.0, height, -100.0);
	glEnd();
	
	glBegin(GL_LINE_LOOP);
	glVertex3f(-95.0,height, -100.0);
	glVertex3f(-95.0,height, 100.0);
	glVertex3f(-95.0,0.0, 100.0);
	glVertex3f(-95.0, 0.0, -100.0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(-100.0,height,100.0);
	glVertex3f(-95.0,height,100.0);
	glVertex3f(-95.0,0.0,100.0);
	glVertex3f(-100.0, 0.0, 100.0);
	glEnd();
	
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_QUADS);
	glVertex3f(-100.0,height,100.0);
	glVertex3f(-100.0,height, -100.0);
	glVertex3f(-100.0,0.0,-100.0);
	glVertex3f(-100.0, 0.0, 100.0);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(-100.0,0.0,-100.0);
	glVertex3f(-95.0,0.0,-100.0);
	glVertex3f(-95.0,height,-100.0);
	glVertex3f(-100.0, height, -100.0);
	glEnd();
	
	glBegin(GL_QUADS);
	glVertex3f(-95.0,height, -100.0);
	glVertex3f(-95.0,height, 100.0);
	glVertex3f(-95.0,0.0, 100.0);
	glVertex3f(-95.0, 0.0, -100.0);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(-100.0,height,100.0);
	glVertex3f(-95.0,height,100.0);
	glVertex3f(-95.0,0.0,100.0);
	glVertex3f(-100.0, 0.0, 100.0);
	glEnd();
	
	
}

void renderScene(void) {

	// Clear Color and Depth Buffers

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
//	glFrustum(-1.0,1.0,-1.0,1.0,1.0,100.0);
	gluLookAt(	x, y, z,
				x+lx, y + ly, z+lz,
				0.0f, 1.0f, 0.0f);
	 //gluPerspective(60.0, 1.0,1.0,100.0);
//	glFrustum(-1.0,1.0,-1.0,1.0,1.0,100.0);			
		if(y > 5.0){
			y -= gravity;
			gluLookAt(	x, y, z,
				x+lx, y + ly, z+lz,
				0.0f, 1.0f, 0.0f);
				
		}
		
		
		if( y == 1.0){
			isJumping == true;
		}
		glColor3f(0.0f, 1.0f, 1.f);
/*	if(LoadGLTextures()){
		glBindTexture(GL_TEXTURE_2D, texture[1]);
	}
	*/
	// Draw ground	
	glBegin(GL_QUADS);
	glVertex3f(-100.0, 0.0f, -100.0f);
	glTexCoord2f(0.0,0.0);
	glVertex3f(-100.0f, 0.0f, 100.0f);
	glTexCoord2f(0.0,10.0);
	glVertex3f( 100.0f, 0.0f, 100.0f);
	glTexCoord2f(10.0,0.0);
	glVertex3f( 100.0f, 0.0f, -100.0f);
	glTexCoord2f(10.0,10.0);
	glEnd();
	
	

	drawWalls();
	/*
	// Draw 36 SnowMen
	for(int i = -3; i < 3; i++)
		for(int j=-3; j < 3; j++) {
			glPushMatrix();
			glTranslatef(i*10.0,0,j * 10.0);
			drawSnowMan();
			glPopMatrix();
		}*/
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

void checkFacing(){
	if(angle <= 0.7 && angle >= -0.7){
		isFacingNorth = true;
		isFacingLeft = false;
		isFacingSouth = false;
		isFacingRight = false;
	}
	if(angle > 0.7 && angle < 2.5){
		isFacingRight = true;
		isFacingNorth = false;
		isFacingLeft = false;
		isFacingSouth = false;
	}
	if(angle >= 2.5 && angle <= -2.5){
		isFacingSouth = true;
		isFacingLeft = false;
		isFacingRight = false;
		isFacingNorth = false;
	}
	if(angle < -0.7 && angle > -2.5){
		isFacingLeft = true;
		isFacingSouth = false;
		isFacingRight = false;
		isFacingNorth = false;
	}
	
}

void processNormalKeys(unsigned char key, int MouseX, int MouseY) {

	if (key == 27)
	exit(0);
	else if(key == 'a'){
		angle -= 0.1;
		if(angle < -3.3){
			angle = 3.1;
		}
		lx = sin(angle);
		lz = cos(angle) * -1;
		printf("angle: %f\n", angle);
		checkFacing();
		
	}
	else if(key == 'd'){
		angle += 0.1;
		if(angle > 3.3){
			angle = -3.1;
		}
		lx = sin(angle);
		lz = cos(angle) * -1;
		printf("angle: %f\n", angle);
		checkFacing();
	}
	else if(key == 'w'){
		if(isPassable(map_x,map_y+1)){
			x+=lx;
			z += lz;
			steps++;
			if(isFacingNorth){
					if(steps >= 10){
					map_y++;
					steps = 0;	
				}
			}
			if(isFacingSouth){
				if(steps >=10){
				map_y--;
				steps = 0;
				}
			}
			if(isFacingRight){
				if(steps >= 10){
				map_x++;
				steps = 0;	
				}
			}
			if(isFacingLeft){
				if(steps >= 10){
				map_x--;
				steps = 0;	
				}
			}
		}
		
	
	}
	else if(key == 's'){
		if(isPassable(map_x,map_y-1)){
			z -= lz;
			x-=lx;
			steps++;
			if(isFacingNorth){
				if(steps >=10){
				map_y--;
				steps = 0;
				}
			}
			if(isFacingSouth){
				if(steps >= 10){
				map_y++;
				steps = 0;	
				}
			}
			if(isFacingRight){
				if(steps >= 10){
				map_x--;
				steps = 0;	
				}
			}
			if(isFacingLeft){
				if(steps >= 10){
				map_x++;
				steps = 0;	
				}
			}
		}
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

void mouseFunc(int btn, int state, int x, int y){

	if(btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
			rMouseFlag = 1;
			curX = x;
			curY = y;
			}
	else{
		rMouseFlag = 0;
	}
		if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
			lMouseFlag = 1;
			curX = x;
			curY = y;
			}
	else{
		lMouseFlag = 0;
	}
}

void motionFunc(int x, int y){
	int dy = curY - y;
	int dx = curX - x;
	if(rMouseFlag == 1){
		if(dx > 0){
			angle -= 0.02;
		}
		if(dx < 0){
			angle += 0.02;
		}
		lx = sin(angle)*(hypot(lx, lz));
		lz = -cos(angle)*(hypot(lx, lz));
		checkFacing();
	}
	if(lMouseFlag ==1){
		if(dy > 0){
			ly +=0.02;
		}
		if(dy < 0){
			ly -=0.02;
		}
	}
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
	glutInitWindowSize(800,800);
	glutCreateWindow("MP");
	// OpenGL init
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	// register callbacks
//	drawWalls();
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
//	glutFullScreen();
	glutIdleFunc(renderScene);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutMotionFunc(motionFunc);
	glutMouseFunc(mouseFunc);
//	glutPassiveMotionFunc(moveMouse);
	
	
	// enter GLUT event processing cycle
	glutMainLoop();

}
