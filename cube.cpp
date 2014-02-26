#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include "Glee.h"
#include <GL/glut.h>
#endif
#include <math.h> 
#include "cube.h"
#include "Matrix4.h"
#include "Vector3.h"
#include "Camera.h"
#include "objreader.h"

#include "Node.h"
#include "Group.h"
#include "MatrixTransform.h"
#include "Bench.h"
#include "Spot.h"
#include "FancyBench.h"
#include "Statue.h"

#define PI 3.14159265359
#define ENABLE_CAMERA false
#define DRAW_CURVE true

using namespace std;

int Window::width  = 712;   // set window width in pixels here
int Window::height = 712;   // set window height in pixels here

bool fullscreen = false;

Curve* Window::curve = nullptr; 
Curve* Window::curve2 = nullptr; 
int Window::alongCurve = 0;
Shader* Window::toon = nullptr;
Shader* Window::perPixel = nullptr;
Shader* Window::active = nullptr;

MatrixTransform *rootNode;

GLuint fboId;
GLuint depthTextureId;

GLfloat LIGHT2_position[] = { 0.0, 20.0, 0.0, 1.0 };
GLfloat LIGHT3_position[] = { -35.0, 20.0, 0.0, 1.0 };
GLfloat LIGHT4_position[] = { 35.0, 20.0, 0.0, 1.0 };
GLfloat LIGHT2_origin[] = { 0.0, 0.0, 0.0, 1.0 };

// OBJ reader
int nVerts;
float *vertices;
float *normals;
float *texcoords;
int nIndices;
int *indices;

bool debug = false;

GLint sample_enabled_loc = -1;

//char *object = (char*)("bench_v01.obj");

GLuint texture[10];

//----------------------------------------------------------------------------
// Callback method called when system is idle.
void Window::idleCallback(void)
{
  displayCallback();  // call display routine to re-draw cube
}

void Window::updateShaders() {
  if(active != nullptr) {
    GLint my_sampler_uniform_location = glGetUniformLocationARB(active->getPid(), "myTexture");
    glUniform1i(my_sampler_uniform_location,0);

    sample_enabled_loc = glGetUniformLocationARB(active->getPid(), "sampleEnabled");
    glUniform1i(sample_enabled_loc,false);
  }
}

//----------------------------------------------------------------------------
// Callback method called when window is resized.
void Window::reshapeCallback(int w, int h)
{
  width = w;
  height = h;
  glViewport(0, 0, w, h);  // set new viewport size
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-10.0, 10.0, -10.0, 10.0, 10, 1000.0); // set perspective projection viewing frustum
  //glTranslatef(0, 5, -10);
  //
  gluLookAt(-10, 30, 40, 0, 0, 0, 0, 1, 0);   // cam position from above to the left
  //gluLookAt(0, 0, 40, 0, 0, 0, 0, 1, 0);   // cam position looking straight
  gluLookAt(-10, 400, 400, 0, 0, 0, 0, 1, 0);   // cam position looking far away
  //gluLookAt(-10, 30, 50, 0, 0, 0, 0, 1, 0);   // cam position from directly above
  //gluLookAt(0, 60, 10, 0, 0, 0, 0, 1, 0);   // cam position from directly above
  //gluLookAt(0, 5, 0, 0, 0, 0, 0, 1, 0);   // cam position from directly above
  glMatrixMode(GL_MODELVIEW);

  Vector3L* one = new Vector3L(0, 15, -15);
  Vector3L* two = new Vector3L(-15, 15, -11);
  Vector3L* three = new Vector3L(-20, 15, 40);
  Vector3L* four = new Vector3L(-50, 15, 0);
  curve = new Curve(one, two, three, four);
  curve->calculateG();

  Vector3L* one2 = new Vector3L(0, 15, 15);
  Vector3L* two2 = new Vector3L(50, 15, 20);
  Vector3L* three2 = new Vector3L(50, 15, -20);
  Vector3L* four2 = new Vector3L(0, 15, -15);
  curve2 = new Curve(one2, two2, three2, four2);
  curve2->calculateG();

  //shadowMapper = new Shader("test.vert", "test.frag", true);
  perPixel = new Shader("diffuse_shading.vert", "diffuse_shading.frag", true);
  toon = new Shader("toon_shading.vert", "toon_shading.frag", true);

  float vAmbientLightBright[4] = {0.1f, 0.1f, 0.1f, 1.0f};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, vAmbientLightBright);

  updateShaders();
}

#define RENDER_WIDTH 1024.0
#define RENDER_HEIGHT 768.0
#define SHADOW_MAP_RATIO 2

unsigned char* loadPPM(const char* filename, int& width, int& height)
{
	const int BUFSIZE = 128;
	FILE* fp;
	unsigned int read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;

	if ( (fp=fopen(filename, "rb")) == NULL)
	{
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}

	// Read magic number:
	retval_fgets = fgets(buf[0], BUFSIZE, fp);

	// Read width and height:
	do
	{
		retval_fgets=fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	retval_sscanf=sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width  = atoi(buf[1]);
	height = atoi(buf[2]);

	// Read maxval:
	do
	{
	  retval_fgets=fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	// Read image data:
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);
	if (read != 1)
	{
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}

	return rawData;
}

// load image file into texture object
GLuint loadTexture(const std::string &file)
{
  GLuint texture;     // storage for one texture
  int twidth, theight;   // texture width/height [pixels]
  unsigned char* tdata;  // texture pixel data
  
  // Load image file
  tdata = loadPPM(file.c_str(), twidth, theight);
  if (tdata==NULL) return -1;
  
  // Create ID for texture
  glGenTextures(1, &texture);   
  
  // Set this texture to be the one we are working with
  glBindTexture(GL_TEXTURE_2D, texture);
  
  // Generate the texture
  glTexImage2D(GL_TEXTURE_2D, 0, 3, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);
  
  // Set bi-linear filtering for both minification and magnification
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return texture;
}

//----------------------------------------------------------------------------
// Callback method called when window readraw is necessary or
// when glutPostRedisplay() was called.
void Window::displayCallback(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers

  if(ENABLE_CAMERA && curve != nullptr) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-10.0, 10.0, -10.0, 10.0, 10, 1000.0);
    double distance = (alongCurve % 1001) / 1000.0;

    Vector4L p;
    Vector3L l;

    if(alongCurve <= 1000) {
      p = curve->point(distance);
      l = curve->pointNormal(distance);
    } else {
      p = curve2->point(distance);
      l = curve2->pointNormal(distance);
    }

    gluLookAt(p[0], p[1], p[2], l[0], l[1] + 15, l[2], 0, 1, 0);
    alongCurve++;
    alongCurve = alongCurve % 2002;
  }

  if(DRAW_CURVE && debug && curve != nullptr) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glTranslatef(curve->p0->get(0), curve->p0->get(1), curve->p0->get(2));
    glutSolidSphere(1, 10, 10);
    glLoadIdentity();
    glTranslatef(curve->p1->get(0), curve->p1->get(1), curve->p1->get(2));
    glutSolidSphere(1, 10, 10);
    glLoadIdentity();
    glTranslatef(curve->p2->get(0), curve->p2->get(1), curve->p2->get(2));
    glutSolidSphere(1, 10, 10);
    glLoadIdentity();
    glTranslatef(curve->p3->get(0), curve->p3->get(1), curve->p3->get(2));
    glutSolidSphere(1, 10, 10);
    glLoadIdentity();
    glTranslatef(curve2->p0->get(0), curve2->p0->get(1), curve2->p0->get(2));
    glutSolidSphere(1, 10, 10);
    glLoadIdentity();
    glTranslatef(curve2->p1->get(0), curve2->p1->get(1), curve2->p1->get(2));
    glutSolidSphere(1, 10, 10);
    glLoadIdentity();
    glTranslatef(curve2->p2->get(0), curve2->p2->get(1), curve2->p2->get(2));
    glutSolidSphere(1, 10, 10);
    glLoadIdentity();
    glTranslatef(curve2->p3->get(0), curve2->p3->get(1), curve2->p3->get(2));
    glutSolidSphere(1, 10, 10);
    glLoadIdentity();
    glBegin(GL_LINE_STRIP);
    glColor3f(0, 1, 0);
    for(int i = 0; i <= 2001; i++) {
      Vector4L p;

      if(i <= 1000) {
        p = curve->point(i / 1000.0);
      } else {
        if(i == 1001) {
          glEnd();
          glBegin(GL_LINE_STRIP);
        }
        p = curve2->point((i % 1001) / 1000.0);
      }

      glVertex3f(p[0], p[1] - 5, p[2]);
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glColor3f(1, 1, 1);
  }

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glLightfv(GL_LIGHT2, GL_POSITION, LIGHT2_position);
    //glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0);
  glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, .001);

	glLightfv(GL_LIGHT3, GL_POSITION, LIGHT3_position);
    //glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 0);
  glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, .001);

	glLightfv(GL_LIGHT4, GL_POSITION, LIGHT4_position);
    //glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, 0);
  glLightf(GL_LIGHT4, GL_QUADRATIC_ATTENUATION, .001);
	
	/*Matrix4 light2Postion;
	light2Postion = light2Postion.translation(LIGHT2_position[0], LIGHT2_position[1], LIGHT2_position[2]);
	glLoadMatrixd(light2Postion.transpose().getPointer());

	glutSolidSphere(3,10,10);*/

	glLoadIdentity();

	



	/*Matrix4 benchTranslate;
	Matrix4 benchScale;
	Matrix4 benchRotateY;
	Matrix4 benchResult;
	benchScale = benchScale.scaling(.1, .1, .1);
	benchTranslate = benchTranslate.translation(0, 20, 0);
	benchRotateY.rotateY(-1.5708);
	benchResult = benchScale.multiply(benchTranslate);
	benchResult = benchResult.multiply(benchRotateY);
	glLoadMatrixd(benchResult.transpose().getPointer());
	
    glBindTexture(GL_TEXTURE_2D, texture[2]);    // gotwood texture
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < nVerts; i++) {
				
		if ( (i / 3) % 3 == 0) {
			glColor3f(1, 1, 1);
		}
		else if ( (i / 3) % 3 == 1) {
			glColor3f(1, 1, 1);
		}
		else if ( (i / 3) % 3 == 2) {
			glColor3f(1, 1, 1);
		}

		glNormal3f(normals[0 + (indices[i] * 3)], 
			normals[1 + (indices[i] * 3)],
			normals[2 + (indices[i] * 3)] );

		glTexCoord2f(texcoords[0 + (indices[i] * 2)], texcoords[1 + (indices[i] * 2)]);
				
		glVertex3f(vertices[0 + (indices[i] * 3)], 
			vertices[1 + (indices[i] * 3)],
			vertices[2 + (indices[i] * 3)] );
	}

	glEnd();
	*/

	glLoadIdentity();
	
	glEnable(GL_TEXTURE);
    glEnable(GL_TEXTURE_2D);
    glUniform1i(sample_enabled_loc,true);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
	// Begin drawing floor
    
	glBegin(GL_QUADS);
	glColor3f(1.0, 1.0, 1.0);

	Vector3 botLeft(-50,0,20);
	Vector3 botRight(50,0,20);
	Vector3 topRight(50,0,-20);
	Vector3 topLeft(-50,0,-20);
	Vector3 diagDown, diagUp, normalV;
	diagDown = botRight.subtract(topLeft);
	diagUp = topRight.subtract(botLeft);
	normalV = normalV.cross(diagDown, diagUp);
	
	//glNormal3f(normalV.getX(), normalV.getY(), normalV.getZ());
  glNormal3f(0, 1, 0);
	glTexCoord2f(0.0, 1.0);	
	glVertex3f(botLeft.getX(), botLeft.getY(), botLeft.getZ());		// bot left
	glTexCoord2f(1.0, 1.0);	
	glVertex3f(botRight.getX(), botRight.getY(), botRight.getZ());		// bot right
	glTexCoord2f(1.0, 0.0);	
	glVertex3f(topRight.getX(), topRight.getY(), topRight.getZ());		// top right
	glTexCoord2f(0.0, 0.0);	
	glVertex3f(topLeft.getX(), topLeft.getY(), topLeft.getZ());	// top left
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	// Begin drawing alcove
	glBegin(GL_QUADS);

	Vector3 botLeft2(-15,0,-20);
	Vector3 botRight2(15,0,-20);
	Vector3 topRight2(15,0,-40);
	Vector3 topLeft2(-15,0,-40);
	diagDown = botRight2.subtract(topLeft2);
	diagUp = topRight2.subtract(botLeft2);
	normalV = normalV.cross(diagDown, diagUp);
	
	glNormal3f(normalV.getX(), normalV.getY(), normalV.getZ());
	glTexCoord2f(0.0, 1.0);	
	glVertex3f(botLeft2.getX(), botLeft2.getY(), botLeft2.getZ());		// bot left
	glTexCoord2f(1.0, 1.0);	
	glVertex3f(botRight2.getX(), botRight2.getY(), botRight2.getZ());		// bot right
	glTexCoord2f(1.0, 0.0);	
	glVertex3f(topRight2.getX(), topRight2.getY(), topRight2.getZ());		// top right
	glTexCoord2f(0.0, 0.0);	
	glVertex3f(topLeft2.getX(), topLeft2.getY(), topLeft2.getZ());	// top left
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[7]);
	// wall painting
	glBegin(GL_QUADS);

	Vector3 botLeft15(-30,10,19);
	Vector3 botRight15(30,10,19);
	Vector3 topRight15(30,25,19);
	Vector3 topLeft15(-30,25,19);
	//Vector3 diagDown, diagUp, normalV;
	diagDown = botRight15.subtract(topLeft15);
	diagUp = topRight15.subtract(botLeft15);
	normalV = normalV.cross(diagUp, diagDown);

	glNormal3f(normalV.getX(), normalV.getY(), normalV.getZ());
	glTexCoord2f(0.0, 1.0);	
	glVertex3f(botLeft15.getX(), botLeft15.getY(), botLeft15.getZ());		// bot left
	glTexCoord2f(1.0, 1.0);	
	glVertex3f(botRight15.getX(), botRight15.getY(), botRight15.getZ());		// bot right
	glTexCoord2f(1.0, 0.0);	
	glVertex3f(topRight15.getX(), topRight15.getY(), topRight15.getZ());		// top right
	glTexCoord2f(0.0, 0.0);	
	glVertex3f(topLeft15.getX(), topLeft15.getY(), topLeft15.getZ());	// top left
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[3]);
	
	//wall 1
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);

	Vector3 botLeft3(-50,0,20);
	Vector3 botRight3(50,0,20);
	Vector3 topRight3(50,40,20);
	Vector3 topLeft3(-50,40,20);
	diagDown = botRight3.subtract(topLeft3);
	diagUp = topRight3.subtract(botLeft3);
	normalV = normalV.cross(diagUp, diagDown);

	glNormal3f(normalV.getX(), normalV.getY(), normalV.getZ());
	glTexCoord2f(0.0, 1.0);	
	glVertex3f(botLeft3.getX(), botLeft3.getY(), botLeft3.getZ());		// bot left
	glTexCoord2f(1.0, 1.0);	
	glVertex3f(botRight3.getX(), botRight3.getY(), botRight3.getZ());		// bot right
	glTexCoord2f(1.0, 0.0);	
	glVertex3f(topRight3.getX(), topRight3.getY(), topRight3.getZ());		// top right
	glTexCoord2f(0.0, 0.0);	
	glVertex3f(topLeft3.getX(), topLeft3.getY(), topLeft3.getZ());	// top left
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	// right wall painting
	glBegin(GL_QUADS);

	Vector3 botLeft14(49,10,-10);
	Vector3 botRight14(49,10,10);
	Vector3 topRight14(49,30,10);
	Vector3 topLeft14(49,30,-10);
	//Vector3 diagDown, diagUp, normalV;
	diagDown = botRight14.subtract(topLeft14);
	diagUp = topRight14.subtract(botLeft14);
	normalV = normalV.cross(diagDown, diagUp);

	glNormal3f(normalV.getX(), normalV.getY(), normalV.getZ());
	glTexCoord2f(0.0, 1.0);	
	glVertex3f(botLeft14.getX(), botLeft14.getY(), botLeft14.getZ());		// bot left
	glTexCoord2f(1.0, 1.0);	
	glVertex3f(botRight14.getX(), botRight14.getY(), botRight14.getZ());		// bot right
	glTexCoord2f(1.0, 0.0);	
	glVertex3f(topRight14.getX(), topRight14.getY(), topRight14.getZ());		// top right
	glTexCoord2f(0.0, 0.0);	
	glVertex3f(topLeft14.getX(), topLeft14.getY(), topLeft14.getZ());	// top left
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, texture[3]);

	//right wall 
	glBegin(GL_QUADS);

	Vector3 botLeft4(50,0,-20);
	Vector3 botRight4(50,0,20);
	Vector3 topRight4(50,40,20);
	Vector3 topLeft4(50,40,-20);
	//Vector3 diagDown, diagUp, normalV;
	diagDown = botRight4.subtract(topLeft4);
	diagUp = topRight4.subtract(botLeft4);
	normalV = normalV.cross(diagDown, diagUp);

	glNormal3f(normalV.getX(), normalV.getY(), normalV.getZ());
	glTexCoord2f(0.0, 1.0);	
	glVertex3f(botLeft4.getX(), botLeft4.getY(), botLeft4.getZ());		// bot left
	glTexCoord2f(1.0, 1.0);	
	glVertex3f(botRight4.getX(), botRight4.getY(), botRight4.getZ());		// bot right
	glTexCoord2f(1.0, 0.0);	
	glVertex3f(topRight4.getX(), topRight4.getY(), topRight4.getZ());		// top right
	glTexCoord2f(0.0, 0.0);	
	glVertex3f(topLeft4.getX(), topLeft4.getY(), topLeft4.getZ());	// top left
	glEnd();

	//left wall 
	glBegin(GL_QUADS);

	Vector3 botLeft5(-50,0,20);
	Vector3 botRight5(-50,0,-20);
	Vector3 topRight5(-50,40,-20);
	Vector3 topLeft5(-50,40,20);
	//Vector3 diagDown, diagUp, normalV;
	diagDown = botRight5.subtract(topLeft5);
	diagUp = topRight5.subtract(botLeft5);
	normalV = normalV.cross(diagDown, diagUp);

	glNormal3f(normalV.getX(), normalV.getY(), normalV.getZ());
	glTexCoord2f(0.0, 1.0);	
	glVertex3f(botLeft5.getX(), botLeft5.getY(), botLeft5.getZ());		// bot left
	glTexCoord2f(1.0, 1.0);	
	glVertex3f(botRight5.getX(), botRight5.getY(), botRight5.getZ());		// bot right
	glTexCoord2f(1.0, 0.0);	
	glVertex3f(topRight5.getX(), topRight5.getY(), topRight5.getZ());		// top right
	glTexCoord2f(0.0, 0.0);	
	glVertex3f(topLeft5.getX(), topLeft5.getY(), topLeft5.getZ());	// top left
	glEnd();


	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glBegin(GL_QUADS);

	Vector3 botLeft16(-49,10,10);
	Vector3 botRight16(-49,10,-10);
	Vector3 topRight16(-49,30,-10);
	Vector3 topLeft16(-49,30,10);
	//Vector3 diagDown, diagUp, normalV;
	diagDown = botRight16.subtract(topLeft16);
	diagUp = topRight16.subtract(botLeft16);
	normalV = normalV.cross(diagDown, diagUp);

	glNormal3f(normalV.getX(), normalV.getY(), normalV.getZ());
	glTexCoord2f(0.0, 1.0);	
	glVertex3f(botLeft16.getX(), botLeft16.getY(), botLeft16.getZ());		// bot left
	glTexCoord2f(1.0, 1.0);	
	glVertex3f(botRight16.getX(), botRight16.getY(), botRight16.getZ());		// bot right
	glTexCoord2f(1.0, 0.0);	
	glVertex3f(topRight16.getX(), topRight16.getY(), topRight16.getZ());		// top right
	glTexCoord2f(0.0, 0.0);	
	glVertex3f(topLeft16.getX(), topLeft16.getY(), topLeft16.getZ());	// top left
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[3]);

	// back right wall 
	glBegin(GL_QUADS);

	Vector3 botLeft6(15,0,-20);
	Vector3 botRight6(50,0,-20);
	Vector3 topRight6(50,40,-20);
	Vector3 topLeft6(15,40,-20);
	//Vector3 diagDown, diagUp, normalV;
	diagDown = botRight6.subtract(topLeft6);
	diagUp = topRight6.subtract(botLeft6);
	normalV = normalV.cross(diagDown, diagUp);

	glNormal3f(normalV.getX(), normalV.getY(), normalV.getZ());
	glTexCoord2f(0.0, 1.0);	
	glVertex3f(botLeft6.getX(), botLeft6.getY(), botLeft6.getZ());		// bot left
	glTexCoord2f(1.0, 1.0);	
	glVertex3f(botRight6.getX(), botRight6.getY(), botRight6.getZ());		// bot right
	glTexCoord2f(1.0, 0.0);	
	glVertex3f(topRight6.getX(), topRight6.getY(), topRight6.getZ());		// top right
	glTexCoord2f(0.0, 0.0);	
	glVertex3f(topLeft6.getX(), topLeft6.getY(), topLeft6.getZ());	// top left
	glEnd();

	// back left wall 
	glBegin(GL_QUADS);

	Vector3 botLeft7(-50,0,-20);
	Vector3 botRight7(-15,0,-20);
	Vector3 topRight7(-15,40,-20);
	Vector3 topLeft7(-50,40,-20);
	//Vector3 diagDown, diagUp, normalV;
	diagDown = botRight7.subtract(topLeft7);
	diagUp = topRight7.subtract(botLeft7);
	normalV = normalV.cross(diagDown, diagUp);

	glNormal3f(normalV.getX(), normalV.getY(), normalV.getZ());
	glTexCoord2f(0.0, 1.0);	
	glVertex3f(botLeft7.getX(), botLeft7.getY(), botLeft7.getZ());		// bot left
	glTexCoord2f(1.0, 1.0);	
	glVertex3f(botRight7.getX(), botRight7.getY(), botRight7.getZ());		// bot right
	glTexCoord2f(1.0, 0.0);	
	glVertex3f(topRight7.getX(), topRight7.getY(), topRight7.getZ());		// top right
	glTexCoord2f(0.0, 0.0);	
	glVertex3f(topLeft7.getX(), topLeft7.getY(), topLeft7.getZ());	// top left
	glEnd();

	//HERE!!!!!!!!!!!!

	glBindTexture(GL_TEXTURE_2D, texture[4]);
	// alcove right wall 
	glBegin(GL_QUADS);

	Vector3 botLeft8(15,0,-40);
	Vector3 botRight8(15,0,-20);
	Vector3 topRight8(15,40,-20);
	Vector3 topLeft8(15,40,-40);
	//Vector3 diagDown, diagUp, normalV;
	diagDown = botRight8.subtract(topLeft8);
	diagUp = topRight8.subtract(botLeft8);
	normalV = normalV.cross(diagDown, diagUp);

	glNormal3f(normalV.getX(), normalV.getY(), normalV.getZ());
	glTexCoord2f(0.0, 1.0);	
	glVertex3f(botLeft8.getX(), botLeft8.getY(), botLeft8.getZ());		// bot left
	glTexCoord2f(1.0, 1.0);	
	glVertex3f(botRight8.getX(), botRight8.getY(), botRight8.getZ());		// bot right
	glTexCoord2f(1.0, 0.0);	
	glVertex3f(topRight8.getX(), topRight8.getY(), topRight8.getZ());		// top right
	glTexCoord2f(0.0, 0.0);	
	glVertex3f(topLeft8.getX(), topLeft8.getY(), topLeft8.getZ());	// top left
	glEnd();

	// alcove back wall 
	glBegin(GL_QUADS);

	Vector3 botLeft9(-15,0,-40);
	Vector3 botRight9(15,0,-40);
	Vector3 topRight9(15,40,-40);
	Vector3 topLeft9(-15,40,-40);
	//Vector3 diagDown, diagUp, normalV;
	diagDown = botRight9.subtract(topLeft9);
	diagUp = topRight9.subtract(botLeft9);
	normalV = normalV.cross(diagDown, diagUp);

	glNormal3f(normalV.getX(), normalV.getY(), normalV.getZ());
	glTexCoord2f(0.0, 1.0);	
	glVertex3f(botLeft9.getX(), botLeft9.getY(), botLeft9.getZ());		// bot left
	glTexCoord2f(1.0, 1.0);	
	glVertex3f(botRight9.getX(), botRight9.getY(), botRight9.getZ());		// bot right
	glTexCoord2f(1.0, 0.0);	
	glVertex3f(topRight9.getX(), topRight9.getY(), topRight9.getZ());		// top right
	glTexCoord2f(0.0, 0.0);	
	glVertex3f(topLeft9.getX(), topLeft9.getY(), topLeft9.getZ());	// top left
	glEnd();

	// alcove left wall 
	glBegin(GL_QUADS);

	Vector3 botLeft10(-15,0,-20);
	Vector3 botRight10(-15,0,-40);
	Vector3 topRight10(-15,40,-40);
	Vector3 topLeft10(-15,40,-20);
	//Vector3 diagDown, diagUp, normalV;
	diagDown = botRight10.subtract(topLeft10);
	diagUp = topRight10.subtract(botLeft10);
	normalV = normalV.cross(diagDown, diagUp);

	glNormal3f(normalV.getX(), normalV.getY(), normalV.getZ());
	glTexCoord2f(0.0, 1.0);	
	glVertex3f(botLeft10.getX(), botLeft10.getY(), botLeft10.getZ());		// bot left
	glTexCoord2f(1.0, 1.0);	
	glVertex3f(botRight10.getX(), botRight10.getY(), botRight10.getZ());		// bot right
	glTexCoord2f(1.0, 0.0);	
	glVertex3f(topRight10.getX(), topRight10.getY(), topRight10.getZ());		// top right
	glTexCoord2f(0.0, 0.0);	
	glVertex3f(topLeft10.getX(), topLeft10.getY(), topLeft10.getZ());	// top left
	glEnd();

	// alcove ceiling
	glBegin(GL_QUADS);

	Vector3 botLeft11(-15,40,-20);
	Vector3 botRight11(15,40,-20);
	Vector3 topRight11(15,40,-40);
	Vector3 topLeft11(-15,40,-40);
	diagDown = botRight11.subtract(topLeft11);
	diagUp = topRight11.subtract(botLeft11);
	normalV = normalV.cross(diagUp, diagDown);
	
	glNormal3f(normalV.getX(), normalV.getY(), normalV.getZ());
	glTexCoord2f(0.0, 1.0);	
	glVertex3f(botLeft11.getX(), botLeft11.getY(), botLeft11.getZ());		// bot left
	glTexCoord2f(1.0, 1.0);	
	glVertex3f(botRight11.getX(), botRight11.getY(), botRight11.getZ());		// bot right
	glTexCoord2f(1.0, 0.0);	
	glVertex3f(topRight11.getX(), topRight11.getY(), topRight11.getZ());		// top right
	glTexCoord2f(0.0, 0.0);	
	glVertex3f(topLeft11.getX(), topLeft11.getY(), topLeft11.getZ());	// top left
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[5]);
	// ceiling
	glBegin(GL_QUADS);

	Vector3 botLeft12(-50,40,20);
	Vector3 botRight12(50,40,20);
	Vector3 topRight12(50,40,-20);
	Vector3 topLeft12(-50,40,-20);
	diagDown = botRight12.subtract(topLeft12);
	diagUp = topRight12.subtract(botLeft12);
	normalV = normalV.cross(diagUp, diagDown);
	
	glNormal3f(normalV.getX(), normalV.getY(), normalV.getZ());
	glTexCoord2f(0.0, 1.0);	
	glVertex3f(botLeft12.getX(), botLeft12.getY(), botLeft12.getZ());		// bot left
	glTexCoord2f(1.0, 1.0);	
	glVertex3f(botRight12.getX(), botRight12.getY(), botRight12.getZ());		// bot right
	glTexCoord2f(1.0, 0.0);	
	glVertex3f(topRight12.getX(), topRight12.getY(), topRight12.getZ());		// top right
	glTexCoord2f(0.0, 0.0);	
	glVertex3f(topLeft12.getX(), topLeft12.getY(), topLeft12.getZ());	// top left
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[6]);
	// ceiling cat
	glBegin(GL_QUADS);

	Vector3 botLeft17(-50,39,20);
	Vector3 botRight17(-45,39,20);
	Vector3 topRight17(-45,39,15);
	Vector3 topLeft17(-50,39,15);
	diagDown = botRight17.subtract(topLeft17);
	diagUp = topRight17.subtract(botLeft17);
	normalV = normalV.cross(diagUp, diagDown);
	
	glNormal3f(normalV.getX(), normalV.getY(), normalV.getZ());
	glTexCoord2f(0.0, 1.0);	
	glVertex3f(botLeft17.getX(), botLeft17.getY(), botLeft17.getZ());		// bot left
	glTexCoord2f(1.0, 1.0);	
	glVertex3f(botRight17.getX(), botRight17.getY(), botRight17.getZ());		// bot right
	glTexCoord2f(1.0, 0.0);	
	glVertex3f(topRight17.getX(), topRight17.getY(), topRight17.getZ());		// top right
	glTexCoord2f(0.0, 0.0);	
	glVertex3f(topLeft17.getX(), topLeft17.getY(), topLeft17.getZ());	// top left
	glEnd();

	glUniform1i(sample_enabled_loc,false);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE);

	// Draw Scene graph
	Matrix4 IDENTITY;
	IDENTITY.identity();

	rootNode->draw(IDENTITY);

	glEnd();

	glFlush();  
	glutSwapBuffers();
}

void specialKeyPressed (int key, int x, int y) {

}

void Window::keyPressed (unsigned char key, int x, int y) {
  if(key == 'f') {
    if(!fullscreen){
      glutFullScreen();
      fullscreen = true;
    } else if(fullscreen){
      glutReshapeWindow(600, 600);
      glutPositionWindow(100,100);
      fullscreen = false;
    }
  } else if(key == 'p') {
    if(active != nullptr) {
      active->unbind();
    }
  } else if(key == 'P') {
    if(active != nullptr) {
      active->unbind();
    }
    active = perPixel;
    active->bind();
    updateShaders();
  } else if(key == 't') {
    if(active != nullptr) {
      active->unbind();
    }
  } else if(key == 'T') {
    if(active != nullptr) {
      active->unbind();
    }
    active = toon;
    active->bind();
    updateShaders();
  } else if(key == 'd') {
    debug = !debug;
  }
}

void initScene(float x, float y, float z) {
	Matrix4 IDENTITY;
	IDENTITY.identity();
	rootNode = new MatrixTransform(IDENTITY);   

	// left and right benches
	Matrix4 rightBenchPosition, leftBenchPosition, middleBenchPosition;
	Matrix4 middleBenchRotate, leftBenchRotate;
	Matrix4 benchScale;
	benchScale = benchScale.scaling(.07,.07,.07);
	rightBenchPosition = rightBenchPosition.translation(30, 0, 0);
	rightBenchPosition = rightBenchPosition.multiply(benchScale);

	leftBenchPosition = leftBenchPosition.translation(-30, 0, 0);
	leftBenchRotate.rotateY( (180*PI) / 180 );
	leftBenchPosition = leftBenchPosition.multiply(benchScale);
	leftBenchPosition = leftBenchPosition.multiply(leftBenchRotate);

	middleBenchPosition.identity();
	middleBenchRotate.rotateY( (90*PI) / 180 );
	middleBenchPosition = middleBenchPosition.multiply(benchScale);
	middleBenchPosition = middleBenchPosition.multiply(middleBenchRotate);
	MatrixTransform *rightTranslate = new MatrixTransform(rightBenchPosition);      
	MatrixTransform *leftTranslate = new MatrixTransform(leftBenchPosition);   
	MatrixTransform *middleTranslate = new MatrixTransform(middleBenchPosition);
	rootNode->addChild(rightTranslate);
	rightTranslate->addChild(new FancyBench());
	rootNode->addChild(leftTranslate);
	leftTranslate->addChild(new FancyBench());
	rootNode->addChild(middleTranslate);  
	middleTranslate->addChild(new FancyBench());

	Matrix4 statueScale;
	Matrix4 statueTransform;
	statueScale = statueScale.scaling(.45,.45,.45);
	statueTransform = statueTransform.translation(-3, 0, -25);
  statueTransform = statueTransform.multiply(statueScale);
  MatrixTransform* statueScaling = new MatrixTransform(statueTransform);
  statueScaling->addChild(new Statue(1,0,0));

  rootNode->addChild(statueScaling);

}

/** Load a ppm file from disk.
 @input filename The location of the PPM file.  If the file is not found, an error message
		will be printed and this function will return 0
 @input width This will be modified to contain the width of the loaded image, or 0 if file not found
 @input height This will be modified to contain the height of the loaded image, or 0 if file not found
 @return Returns the RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured
**/



int main(int argc, char *argv[])
{

	
  //ObjReader::readObj(object, nVerts, &vertices, &normals, &texcoords, nIndices, &indices);
  
  glutInit(&argc, argv);      	      	      // initialize GLUT
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   // open an OpenGL context with double buffering, RGB colors, and depth buffering
  glutInitWindowSize(Window::width, Window::height);      // set initial window size
  glutCreateWindow("OpenGL Cube for CSE167");    	      // open window and set window title
  glEnable(GL_NORMALIZE);

  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glEnable(GL_DEPTH_TEST);            	      // enable depth buffering
  glClear(GL_DEPTH_BUFFER_BIT);       	      // clear depth buffer
  glClearColor(0.0, 0.0, 0.0, 0.0);   	      // set clear color to black
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // set polygon drawing mode to fill front and back of each polygon
  //glDisable(GL_CULL_FACE);     // disable backface culling to render both sides of polygons
  glShadeModel(GL_SMOOTH);             	      // set shading to smooth
  glMatrixMode(GL_PROJECTION); 
  glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);     // added because u said tooooooo

  //glutFullScreen();

  glEnable(GL_LIGHTING);

  glColor3f(1.0, 1.0, 1.0);

	GLfloat LIGHT2_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat LIGHT2_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat LIGHT2_specular[] = { 0.0, 1.0, 0.0, 1.0 };

	glLightfv(GL_LIGHT2, GL_AMBIENT, LIGHT2_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, LIGHT2_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, LIGHT2_specular);

	glLightfv(GL_LIGHT3, GL_AMBIENT, LIGHT2_ambient);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, LIGHT2_diffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, LIGHT2_specular);

	glLightfv(GL_LIGHT4, GL_AMBIENT, LIGHT2_ambient);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, LIGHT2_diffuse);
	glLightfv(GL_LIGHT4, GL_SPECULAR, LIGHT2_specular);

	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);
  
  // Install callback functions:
  glutDisplayFunc(Window::displayCallback);
  glutReshapeFunc(Window::reshapeCallback);
  glutIdleFunc(Window::idleCallback);    

  /// key input//////////////////////////////saswa
  glutKeyboardFunc(Window::keyPressed); // use keypressed method
  glutSpecialFunc(specialKeyPressed);

  initScene(0, 0, 0);
  
 
  glActiveTexture(GL_TEXTURE0);
  texture[0] = loadTexture("floor.ppm");
  texture[1] = loadTexture("monalisa.ppm");
  texture[2] = loadTexture("gotwood.ppm");
  texture[3] = loadTexture("wall.ppm");
  texture[4] = loadTexture("whitewall.ppm");
  texture[5] = loadTexture("top.ppm");
  texture[6] = loadTexture("cat.ppm");
  texture[7] = loadTexture("Horse.ppm");
  texture[8] = loadTexture("HEMAN.ppm");

  glutMainLoop();
  return 0;
}

