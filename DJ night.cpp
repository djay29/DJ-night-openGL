#include <GL/glut.h>
#include <math.h>
#include<string>

#ifdef WIN32
//to correct ASCI deviations in Microsoft VC++ 6.0
#define M_PI (3.1415926535897932384626433832795)
#define TEXTID      3
#define ESCAPE 27
double drand48()
{   return (rand()%10000)/10000.0; }
//end of corrections
#endif

#define MAX_POINTS 5000
GLfloat curx, cury;
GLfloat x[MAX_POINTS], y[MAX_POINTS];
GLfloat xacc[MAX_POINTS], yacc[MAX_POINTS];
GLfloat red, green, blue;
int step; int length;
float x_pos_txt[]={-1.60,-1.43,-1.31,-1.195,-1.055,-0.91,-0.73};
float y_pos_stick=0.2,hc=0.01;
const int stacks=100;
const int slices=100;
int cd=0,view=0,perf=1, l=1, numPoints, state=1, sstate=1, flag=0,f=2,vflag=0;

struct particle {
	GLfloat x,y,z;
	GLfloat r,g,b;
	GLfloat xd,yd,zd;
	GLfloat cs;
} p[10000];//Define particles for confetti

float RandomBetween(float smallNumber, float bigNumber) {//Generates random number between smallNumber and bigNumber
    float diff = bigNumber - smallNumber;
    return (((float) rand() / RAND_MAX) * diff) + smallNumber;
}

void initialize()   {//Initializes fire particles
    int j; double temp, temp2;
    numPoints = 400;
    curx = (GLfloat)RandomBetween(-1.65,1.65);
    cury = -1.5;
    red = 1.0;
    green = 0.5;
    blue = 0.0;
    glPointSize(1.5);
    step = 0;
    length = 700 + 300*drand48();
    //initialize the blast
    for (j=0 ; j<numPoints ; j++ )  {
        x[j] = curx;
        y[j] = cury;
        temp = drand48();
        temp2 = drand48()*2.0*M_PI;
        xacc[j] = (cos(temp2) * temp)/length;
        yacc[j] = (sin(temp2) * temp)/length;
    }
}

void enablefunc()   {
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);//Light for the disco ball
	glEnable(GL_LIGHT1);//Spotlight for the singer
	glEnable(GL_NORMALIZE);//To specify normals to polygons
}

void InitGL(void)   {   //To initialize confetti particles
	for (int i=0;i<10000;i++){
		p[i].xd=-(rand()/32767.0f-0.5f)/200.0f;
		p[i].zd=-(rand()/32767.0f-0.5f)/200.0f;
		p[i].yd=-rand()/32767.0f/100.0f;
		p[i].x=(rand()/32767.0f-0.5f);
		p[i].y=1.0f;
		p[i].z=(rand()/32767.0f-0.5f);
		p[i].b=RandomBetween(0.0,1.0);
		p[i].g=RandomBetween(0.0,1.0);
		p[i].r=RandomBetween(0.0,1.0);
	}
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

}





void drawText(const char *text, int length, float x, float y,float z)   {   //Draws text using bitmap fonts
    glRasterPos3f(x,y,z);
    for(int i=0;i<length;i++)   {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)text[i]);
    }
}

void drawFilledCircle(GLfloat x, GLfloat y, GLfloat z)  {   //Draws a filled circle in x-z plane
    int i;
    double radius = 0.2;
    double twicePi = 2.0 * 3.142;
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(x, y, z);
    for (i = 0; i <= 20; i++)   {
        glVertex3f ( (x + (radius * cos(i * twicePi / 20)) ),y, (z + (radius * sin(i * twicePi / 20) ) ) );
    }
    glEnd();
}

void DrawSticks(float cx, float cz, float r, int num_segments) {    //Draws sticks around the drum

    glLineWidth(3.0);
    glBegin(GL_LINES);
    for (int ii = 0; ii < num_segments; ii++)   {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
        float x = r * cosf(theta);//calculate the x component
        float z = r * sinf(theta);//calculate the z component
        if(ii%10==0)    {
            glVertex3f(x + cx,0.0, z + cz);//Top of the Stick
            glVertex3f(x + cx,0.25, z + cz);//Bottom of the Stick
        }

    }
    glEnd();
}

void DrawCirclexy(float cx, float cy, float r, int num_segments) {  //draws circle in the x-y plane
    glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < num_segments; ii++)   {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);//calculate the y component
        glVertex2f(x + cx, y + cy);//output vertex
    }
    glEnd();
}
void timer(int) {    //To repeatedly change the co ordinates of an object and redraw it
    glutPostRedisplay();
    glutTimerFunc(1000/60,timer,0);//Recursively calls itself after 1/60 seconds i.e, to maintain 60 FPS

	    if(x_pos_txt[0]<1.60)
		x_pos_txt[0]+=0.00005;
	    else
		x_pos_txt[0]=-1.60;

	    if(x_pos_txt[1]<1.60)
		x_pos_txt[1]+=0.00005;
	    else
		x_pos_txt[1]=-1.60;

	    if(x_pos_txt[2]<1.60)
		x_pos_txt[2]+=0.00005;
	    else
		x_pos_txt[2]=-1.60;

	    if(x_pos_txt[3]<1.60)
		x_pos_txt[3]+=0.00005;
	    else
		x_pos_txt[3]=-1.60;

	    if(x_pos_txt[4]<1.60)
		x_pos_txt[4]+=0.00005;
	    else
		x_pos_txt[4]=-1.60;

	    if(x_pos_txt[5]<1.60)
		x_pos_txt[5]+=0.00005;
	    else
		x_pos_txt[5]=-1.60;

	    if(x_pos_txt[6]<1.60)
		x_pos_txt[6]+=0.00005;
	    else
		x_pos_txt[6]=-1.60;
		
		

    switch(state)//To change the y co ordinates of the drum sticks
    {
        case 1:
            if(y_pos_stick<0.4)
            {
                y_pos_stick+=0.00008;
            }
            else
                state=-1;
            break;
        case -1:
            if(y_pos_stick>0.2)
            {
                y_pos_stick-=0.00008;
            }
            else
                state=1;
            break;
    }
    switch(sstate)//To change the height of the cylinder in the speaker
    {
        case 1:
            if(hc<0.05)
            {
                hc+=0.001;
            }
            else
                sstate=-1;
            break;
        case -1:
            if(hc>0.01)
            {
                hc-=0.001;
            }
            else
                sstate=1;
            break;
    }


}

void ctimer(int)//To automatically stop confetti after singer is changed
{
    cd=0;
    glutPostRedisplay();
}

void draw_confetti(void)//Draws confetti particles
{
    glPushMatrix();
        glTranslatef(0.0f,0.4f,-5.7f);
		glBegin(GL_POINTS);
		for (int i=0;i<2000;i++)
		{
			p[i].y+=p[i].yd;
			p[i].yd-=rand()/32767.0f/10000.0f;
			if (p[i].y<=-0.8f)
			{
				p[i].xd=-1.0;
				p[i].zd=-(rand()/32767.0f-0.5f)/200.0f;
				p[i].yd=-rand()/32767.0f/100.0f;
				p[i].x=(rand()/32767.0f-0.5f);
				p[i].y=1.0f;
				p[i].z=(rand()/32767.0f-0.5f);
				p[i].b=RandomBetween(0.0,1.0);
				p[i].g=RandomBetween(0.0,1.0);
				p[i].r=RandomBetween(0.0,1.0);
			}
			glColor3f(p[i].r,p[i].g,p[i].b);
			glVertex3f(p[i].x,p[i].y,p[i].z);
		}
		glEnd();
    glPopMatrix();
}

void draw_human(GLfloat sc[], GLfloat pc[], GLfloat hc[], GLfloat s[], int c)//int c specifies if the audience member has a cap
{//Draws humans in the audience
    GLUquadricObj *p=gluNewQuadric();
    glColor3fv(s);
    //Audience face
    //Audience front of the face
    glPushMatrix();
    glScalef(0.3f,0.3f,1.0f);
    glNormal3f(0.0,0.0,1.0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.3,0.2,-5.0);
    glVertex3f(-0.3,-0.2,-5.0);
    glVertex3f(-0.1,-0.4,-5.0);
    glVertex3f(0.1,-0.4,-5.0);
    glVertex3f(0.3,-0.2,-5.0);
    glVertex3f(0.3,0.2,-5.0);
    glVertex3f(0.225,0.35,-5.0);
    glVertex3f(-0.225,0.35,-5.0);
    glEnd();
    //Audience eyebrows
    glColor3f(hc[0],hc[1],hc[2]);
    glBegin(GL_LINES);
    glVertex3f(-0.2,0.15,-5.251);
    glVertex3f(-0.1,0.15,-5.251);
    glVertex3f(0.2,0.15,-5.251);
    glVertex3f(0.1,0.15,-5.251);
    glEnd();
    glPointSize(3.0);
    //Audience eyes
    glColor3f(0.0,0.0,0.0);
    glBegin(GL_POINTS);
    glVertex3f(-0.15,0.08,-5.251);
    glVertex3f(0.15,0.08,-5.251);
    glEnd();
    //Audience lips
    glColor3f(1.0,0.8,0.9);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex3f(-0.1,-0.2,-5.251);
    glVertex3f(0.1,-0.2,-5.251);
    glEnd();
    //Audience cap
    if(c==1)//To check if the audience member has a cap
    {
        glColor3f(1.0,0.0,0.0);
        //Audience cap back
        glBegin(GL_POLYGON);
        glVertex3f(-0.3,0.15,-4.99);
        glVertex3f(-0.3,0.4,-4.99);
        glVertex3f(-0.25,0.45,-4.99);
        glVertex3f(0.25,0.45,-4.99);
        glVertex3f(0.3,0.4,-4.99);
        glVertex3f(0.3,0.15,-4.99);
        glEnd();
        //Audience cap sides
        glBegin(GL_QUAD_STRIP);
        glVertex3f(-0.3,0.15,-5.251);
        glVertex3f(-0.3,0.15,-4.99);
        glVertex3f(-0.3,0.4,-5.251);
        glVertex3f(-0.3,0.4,-4.99);
        glVertex3f(-0.25,0.45,-5.251);
        glVertex3f(-0.25,0.45,-4.99);
        glVertex3f(0.25,0.45,-5.251);
        glVertex3f(0.25,0.45,-4.99);
        glVertex3f(0.3,0.4,-5.251);
        glVertex3f(0.3,0.4,-4.99);
        glVertex3f(0.3,0.15,-5.251);
        glVertex3f(0.3,0.15,-4.99);
        glEnd();
        //Audience cap hood
        glBegin(GL_POLYGON);
        glVertex3f(-0.3,0.15,-5.25);
        glVertex3f(-0.3,0.15,-5.4);
        glVertex3f(-0.275,0.15,-5.45);
        glVertex3f(0.275,0.15,-5.45);
        glVertex3f(0.3,0.15,-5.4);
        glVertex3f(0.3,0.15,-5.25);
        glEnd();
        //Audience cap front
        glBegin(GL_POLYGON);
        glVertex3f(-0.3,0.15,-5.251);
        glVertex3f(-0.3,0.4,-5.251);
        glVertex3f(-0.25,0.45,-5.251);
        glVertex3f(0.25,0.45,-5.251);
        glVertex3f(0.3,0.4,-5.251);
        glVertex3f(0.3,0.15,-5.251);
        glEnd();
    }
    glColor3f(s[0],s[1],s[2]);
    //Audience right ear
    glNormal3f(0.0,-1.0,0.0);
    glBegin(GL_QUADS);
    glVertex3f(0.3,-0.075,-5.2);
    glVertex3f(0.3,0.05,-5.2);
    glVertex3f(0.375,0.1,-5.2);
    glVertex3f(0.375,-0.1125,-5.2);
    glEnd();
    //Audience left ear
    glNormal3f(0.0,-1.0,0.0);
    glBegin(GL_QUADS);
    glVertex3f(-0.3,-0.075,-5.2);
    glVertex3f(-0.3,0.05,-5.2);
    glVertex3f(-0.375,0.1,-5.2);
    glVertex3f(-0.375,-0.1125,-5.2);
    glEnd();
    //Audience neck
    glPushMatrix();
    glTranslated(0.0,-0.55,-5.15);
    glPushMatrix();
    glRotated(90.0,-1.0,0.0,0.0);
    gluCylinder(p,0.1,0.1,0.15,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    glPopMatrix();
    //Audience back of the face
    glNormal3f(0.0,0.0,1.0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.3,0.2,-5.25);
    glVertex3f(-0.3,-0.2,-5.25);
    glVertex3f(-0.1,-0.4,-5.25);
    glVertex3f(0.1,-0.4,-5.25);
    glVertex3f(0.3,-0.2,-5.25);
    glVertex3f(0.3,0.2,-5.25);
    glVertex3f(0.225,0.35,-5.25);
    glVertex3f(-0.225,0.35,-5.25);
    glEnd();
    //Audience hair
    glColor3f(hc[0],hc[1],hc[2]);
    glNormal3f(0.0,0.0,-1.0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.3,0.2,-5.0);
    glVertex3f(-0.3,-0.1,-5.0);
    glVertex3f(0.3,-0.1,-5.0);
    glVertex3f(0.3,0.2,-5.0);
    glVertex3f(0.225,0.35,-5.0);
    glVertex3f(-0.225,0.35,-5.0);
    glEnd();
    glBegin(GL_QUAD_STRIP);
    glVertex3f(-0.3,-0.1,-5.0);
    glVertex3f(-0.3,-0.1,-5.25);
    glVertex3f(-0.3,0.2,-5.0);
    glVertex3f(-0.3,0.2,-5.25);
    glVertex3f(-0.225,0.35,-5.0);
    glVertex3f(-0.225,0.35,-5.25);
    glVertex3f(0.225,0.35,-5.0);
    glVertex3f(0.225,0.35,-5.25);
    glVertex3f(0.3,0.2,-5.0);
    glVertex3f(0.3,0.2,-5.25);
    glVertex3f(0.3,-0.1,-5.0);
    glVertex3f(0.3,-0.1,-5.25);
    glEnd();
    glColor3f(s[0],s[1],s[2]);
    //Audience side of the head
    glNormal3f(0.0,-1.0,0.0);
    glBegin(GL_QUAD_STRIP);
    glVertex3f(-0.3,-0.1,-5.0);
    glVertex3f(-0.3,-0.1,-5.25);
    glVertex3f(-0.3,-0.2,-5.0);
    glVertex3f(-0.3,-0.2,-5.25);
    glVertex3f(-0.125,-0.4,-5.0);
    glVertex3f(-0.125,-0.4,-5.25);
    glVertex3f(0.125,-0.4,-5.0);
    glVertex3f(0.125,-0.4,-5.25);
    glVertex3f(0.3,-0.2,-5.0);
    glVertex3f(0.3,-0.2,-5.25);
    glVertex3f(0.3,-0.1,-5.0);
    glVertex3f(0.3,-0.1,-5.25);
    glEnd();
    //Audience shirt
    glColor3f(sc[0],sc[1],sc[2]);
    glNormal3f(0.0,-1.0,0.0);
    glBegin(GL_QUAD_STRIP);
    glVertex3f(-0.50,-0.55,-5.0);
    glVertex3f(-0.50,-1.95,-5.0);
    glVertex3f(0.50,-0.55,-5.0);
    glVertex3f(0.50,-1.95,-5.0);
    glVertex3f(0.65,-0.55,-5.125);
    glVertex3f(0.65,-1.95,-5.125);
    glVertex3f(0.50,-0.55,-5.25);
    glVertex3f(0.50,-1.95,-5.25);
    glVertex3f(-0.50,-0.55,-5.25);
    glVertex3f(-0.50,-1.95,-5.25);
    glVertex3f(-0.65,-0.55,-5.125);
    glVertex3f(-0.65,-1.95,-5.125);
    glVertex3f(-0.50,-0.55,-5.0);
    glVertex3f(-0.50,-1.95,-5.0);
    glEnd();
    //Audience arms
    //Audience right arm
    glColor3f(s[0],s[1],s[2]);
    glNormal3f(-1.0,0.0,0.0);
    glBegin(GL_QUAD_STRIP);
    glVertex3f(0.6,-0.55,-5.1);
    glVertex3f(0.6,-2.0,-5.1);
    glVertex3f(0.7,-0.55,-5.1);
    glVertex3f(0.7,-2.0,-5.1);
    glVertex3f(0.7,-0.55,-5.2);
    glVertex3f(0.7,-2.0,-5.2);
    glVertex3f(0.6,-0.55,-5.2);
    glVertex3f(0.6,-2.0,-5.2);
    glVertex3f(0.6,-0.55,-5.1);
    glVertex3f(0.6,-2.0,-5.1);
    glEnd();
    //Audience left arm
    glNormal3f(0.0,-1.0,0.0);
    glBegin(GL_QUAD_STRIP);
    glVertex3f(-0.6,-0.55,-5.1);
    glVertex3f(-0.6,-2.0,-5.1);
    glVertex3f(-0.7,-0.55,-5.1);
    glVertex3f(-0.7,-2.0,-5.1);
    glVertex3f(-0.7,-0.55,-5.2);
    glVertex3f(-0.7,-2.0,-5.2);
    glVertex3f(-0.6,-0.55,-5.2);
    glVertex3f(-0.6,-2.0,-5.2);
    glVertex3f(-0.6,-0.55,-5.1);
    glVertex3f(-0.6,-2.0,-5.1);
    glEnd();
    //Audience pant
    //Audience pant front side
    glColor3f(pc[0],pc[1],pc[2]);
    glNormal3f(0.0,0.0,-1.0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.5,-1.95,-5.0);
    glVertex3f(-0.4,-3.95,-5.0);
    glVertex3f(-0.25,-3.95,-5.0);
    glVertex3f(-0.1,-2.5,-5.0);
    glVertex3f(0.0,-2.5,-5.0);
    glVertex3f(0.0,-1.95,-5.0);
    glEnd();
    glNormal3f(0.0,0.0,-1.0);
    glBegin(GL_POLYGON);
    glVertex3f(0.0,-1.95,-5.0);
    glVertex3f(0.0,-2.5,-5.0);
    glVertex3f(0.1,-2.5,-5.0);
    glVertex3f(0.25,-3.95,-5.0);
    glVertex3f(0.4,-3.95,-5.0);
    glVertex3f(0.5,-1.95,-5.0);
    glEnd();
    //Audience pant inner side
    glNormal3f(1.0,1.0,0.0);
    glBegin(GL_QUADS);
    glVertex3f(-0.25,-3.95,-5.0);
    glVertex3f(0.0,-2.5,-5.0);
    glVertex3f(0.0,-2.5,-5.25);
    glVertex3f(-0.25,-3.95,-5.25);

    glVertex3f(0.25,-3.95,-5.25);
    glVertex3f(0.0,-2.5,-5.25);
    glVertex3f(0.0,-2.5,-5.0);
    glVertex3f(0.25,-3.95,-5.0);
    glEnd();
    //Audience pant back side
    glNormal3f(0.0,0.0,1.0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.5,-1.95,-5.25);
    glVertex3f(-0.4,-3.95,-5.25);
    glVertex3f(-0.3,-3.95,-5.25);
    glVertex3f(-0.1,-2.5,-5.25);
    glVertex3f(0.0,-2.5,-5.25);
    glVertex3f(0.0,-1.95,-5.25);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(0.0,-1.95,-5.25);
    glVertex3f(0.0,-2.5,-5.25);
    glVertex3f(0.1,-2.5,-5.25);
    glVertex3f(0.3,-3.95,-5.25);
    glVertex3f(0.4,-3.95,-5.25);
    glVertex3f(0.5,-1.95,-5.25);
    glEnd();
    //pant left side
    glNormal3f(1.0,0.0,0.0);
    glBegin(GL_QUADS);
    glVertex3f(-0.5,-1.85,-5.25);
    glVertex3f(-0.4,-3.95,-5.25);
    glVertex3f(-0.4,-3.95,-5.0);
    glVertex3f(-0.5,-1.85,-5.0);
    glEnd();
    //Audience pant right side
    glNormal3f(-1.0,0.0,0.0);
    glBegin(GL_QUADS);
    glVertex3f(0.5,-1.85,-5.25);
    glVertex3f(0.4,-3.95,-5.25);
    glVertex3f(0.4,-3.95,-5.0);
    glVertex3f(0.5,-1.85,-5.0);
    glEnd();
    glPopMatrix();
}


void drum(void)//Draws a drum
{
    GLUquadricObj *p=gluNewQuadric();
    glColor3f(1.0,0.9,0.6);
    glPushMatrix();
    glTranslated(1.4,0.0,-6.0);
    glPushMatrix();
    glRotated(90.0,-1.0,0.0,0.0);
    gluCylinder(p,0.2,0.2,0.25,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    glPopMatrix();
    //Drum Front Left Stand
    glColor3f(0.5,0.5,0.5);
    glPushMatrix();
    glTranslated(1.25,0.0,-5.85);
    glPushMatrix();
    glRotated(90.0,1.0,0.0,0.0);
    gluCylinder(p,0.02,0.02,0.5,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    glPopMatrix();
    //Drum Front Right Stand
    glPushMatrix();
    glTranslated(1.55,0.0,-5.85);
    glPushMatrix();
    glRotated(90.0,1.0,0.0,0.0);
    gluCylinder(p,0.02,0.02,0.5,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    glPopMatrix();
    //Drum Back Left Stand
    glPushMatrix();
    glTranslated(1.25,0.0,-6.15);
    glPushMatrix();
    glRotated(90.0,1.0,0.0,0.0);
    gluCylinder(p,0.02,0.02,0.5,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    glPopMatrix();
    //Drum Back Right Stand
    glPushMatrix();
    glTranslated(1.55,0.0,-6.15);
    glPushMatrix();
    glRotated(90.0,1.0,0.0,0.0);
    gluCylinder(p,0.02,0.02,0.5,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    glPopMatrix();
    //Lines around the drum
    glColor3f(0.0,0.0,0.0);
    glPushMatrix();
    glTranslated(1.4,0.0,-6.0);
    DrawSticks(0.0,0.0,0.2,100);
    glPopMatrix();
    //Top of the drum
    glColor3f(0.8,0.8,0.8);
    glPushMatrix();
    glTranslated(1.4,0.25,-6.0);
    drawFilledCircle(0.0,0.0,0.0);
    glPopMatrix();
    //Bottom of the drum
    glColor3f(0.8,0.8,0.8);
    glPushMatrix();
    glTranslated(1.4,0.0,-6.0);
    drawFilledCircle(0.0,0.0,0.0);
    glPopMatrix();
}

void draw_speaker(void)//Draws a speaker
{
    GLUquadricObj *p=gluNewQuadric();
    //Speaker
    //Front of the Speaker
    glColor3f(0.5,0.5,0.5);
    glBegin(GL_QUADS);
    glNormal3f(0.0,0.0,1.0);
    glVertex3f(1.8,-0.8,-5.0);
    glVertex3f(1.8,0.2,-5.0);
    glVertex3f(2.5,0.2,-5.0);
    glVertex3f(2.5,-0.8,-5.0);
    glEnd();
    //Back of the Speaker
    glBegin(GL_QUADS);
    glNormal3f(0.0,0.0,1.0);
    glVertex3f(1.8,-0.8,-5.5);
    glVertex3f(1.8,0.2,-5.5);
    glVertex3f(2.5,0.2,-5.5);
    glVertex3f(2.5,-0.8,-5.5);
    glEnd();
    //Circles of the Speaker
    glColor3f(0.0,0.0,0.0);
    glPushMatrix();
        glTranslated(2.15,-0.1,-4.99);
        DrawCirclexy(0.0,0.0,0.15,100);
        gluCylinder(p,0.15,0.15,hc,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    glPushMatrix();
        glTranslated(2.15,-0.5,-4.99);
        DrawCirclexy(0.0,0.0,0.15,100);
        gluCylinder(p,0.15,0.15,hc,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    glColor3f(0.5,0.5,0.5);
    //Sides of the Speaker
    glBegin(GL_QUAD_STRIP);
    glNormal3f(0.0,-1.0,0.0);
    glVertex3f(1.8,-0.8,-5.0);
    glVertex3f(1.8,-0.8,-5.5);
    glVertex3f(1.8,0.2,-5.0);
    glVertex3f(1.8,0.2,-5.5);
    glVertex3f(2.5,0.2,-5.0);
    glVertex3f(2.5,0.2,-5.5);
    glVertex3f(2.5,-0.8,-5.0);
    glVertex3f(2.5,-0.8,-5.5);
    glEnd();
    glColor3f(0.1,0.1,0.1);
    glBegin(GL_LINE_LOOP);
    glVertex3f(1.9,0.1,-5.0);
    glVertex3f(1.9,-0.7,-5.0);
    glVertex3f(2.4,-0.7,-5.0);
    glVertex3f(2.4,0.1,-5.0);
    glEnd();
    //Stands of the Speaker
    glColor3f(0.5,0.5,0.5);
    //Left Front Stand of the Speakers
    glPushMatrix();
        glTranslatef(1.85,-1.6,-5.1);
    glPushMatrix();
        glRotatef(90.0,-1.0,0.0,0.0);
        gluCylinder(p,0.02,0.02,0.8,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    glPopMatrix();
    //Left Back Stand of the Speakers
    glPushMatrix();
        glTranslatef(1.85,-1.6,-5.4);
    glPushMatrix();
        glRotatef(90.0,-1.0,0.0,0.0);
        gluCylinder(p,0.02,0.02,0.8,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    glPopMatrix();
    //Right front Stand of the Speakers
    glPushMatrix();
        glTranslatef(2.45,-1.6,-5.1);
    glPushMatrix();
        glRotatef(90.0,-1.0,0.0,0.0);
        gluCylinder(p,0.02,0.02,0.8,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    glPopMatrix();
    //Right Back Stand of the Speakers
    glPushMatrix();
        glTranslatef(2.45,-1.6,-5.4);
    glPushMatrix();
        glRotatef(90.0,-1.0,0.0,0.0);
        gluCylinder(p,0.02,0.02,0.8,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    glPopMatrix();
}

void draw_blast(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;
    //RGB color value for the disco ball
    const float rc = 0.0;//Red value for the disco ball
    const float gc = RandomBetween(0.4,0.8);//Green value for the disco ball
    const float bc = RandomBetween(0.8,1.0);//Blue value for the disco ball
    if(cd==1)//checks whether to draw the confetti particles or not
    {
        draw_confetti();
    }

    //Disco Ball
    glColor3f(rc,gc,bc);
    glPushMatrix();
        glTranslated(0,1.6,-6);
        glRotated(a,0,1,0);
        glutSolidSphere(0.2,15,15);
    glPopMatrix();
    int i;
    glColor3f(1.0,0.0,0.0);
    //Stage
    //Front of the Stage
    glBegin(GL_QUADS);
        glVertex3f(-1.7,-1.5,-5.0);
        glVertex3f(-1.7,-0.5,-5.0);
        glVertex3f(1.7,-0.5,-5.0);
        glVertex3f(1.7,-1.5,-5.0);
    glEnd();
    //Back of the Stage
    glBegin(GL_QUADS);
        glVertex3f(-1.7,-1.5,-7.5);
        glVertex3f(-1.7,-0.5,-7.5);
        glVertex3f(1.7,-0.5,-7.5);
        glVertex3f(1.7,-1.5,-7.5);
    glEnd();
    //Top of the Stage
    glBegin(GL_QUADS);
        glVertex3f(-1.7,-0.5,-7.5);
        glVertex3f(-1.7,-0.5,-5.0);
        glVertex3f(1.7,-0.5,-5.0);
        glVertex3f(1.7,-0.5,-7.5);
    glEnd();
    //Bottom of the Stage
    glBegin(GL_QUADS);
        glVertex3f(-1.7,-1.5,-7.5);
        glVertex3f(-1.7,-1.5,-5.0);
        glVertex3f(1.7,-1.5,-5.0);
        glVertex3f(1.7,-1.5,-7.5);
    glEnd();
    //Left of the Stage
    glBegin(GL_QUADS);
        glVertex3f(-1.7,-0.5,-7.5);
        glVertex3f(-1.7,-1.5,-7.5);
        glVertex3f(-1.7,-1.5,-5.0);
        glVertex3f(-1.7,-0.5,-5.0);
    glEnd();
    //Right of the Stage
    glBegin(GL_QUADS);
        glVertex3f(1.7,-0.5,-7.5);
        glVertex3f(1.7,-1.5,-7.5);
        glVertex3f(1.7,-1.5,-5.0);
        glVertex3f(1.7,-0.5,-5.0);
    glEnd();
    //End of Stage
    glColor3f(0.5,0.5,0.5);
    //Right Back Stand on the Stage
    glPushMatrix();
        glTranslatef(1.65,-0.5,-7.4);
    glPushMatrix();
        glRotatef(90.0,-1.0,0.0,0.0);
        GLUquadricObj *p=gluNewQuadric();
        gluCylinder(p,0.04,0.04,2.0,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    glPopMatrix();
    //Left Back Stand on the Stage
    glPushMatrix();
        glTranslatef(-1.65,-0.5,-7.4);
    glPushMatrix();
        glRotatef(90.0,-1.0,0.0,0.0);
        gluCylinder(p,0.04,0.04,2.0,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    glPopMatrix();
    //Left Front Stand on the Stage
    glPushMatrix();
        glTranslatef(-1.65,-0.5,-5.0);
    glPushMatrix();
        glRotatef(90.0,-1.0,0.0,0.0);
        gluCylinder(p,0.04,0.04,2.0,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    glPopMatrix();
    //Right Front Stand on the Stage
    glPushMatrix();
        glTranslatef(1.65,-0.5,-5.0);
    glPushMatrix();
        glRotatef(90.0,-1.0,0.0,0.0);
        gluCylinder(p,0.04,0.04,2.0,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    glPopMatrix();
    //Right Top Stand on the Stage
    glPushMatrix();
        glTranslatef(1.65,1.5,-7.4);
        gluCylinder(p,0.04,0.04,2.4,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    //Left Top Stand on the Stage
    glPushMatrix();
        glTranslatef(-1.65,1.5,-7.4);
        gluCylinder(p,0.04,0.04,2.4,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    //Front Top Stand on the Stage
    glPushMatrix();
        glTranslatef(-1.65,1.5,-5.0);
    glPushMatrix();
        glRotatef(90.0,0.0,1.0,0.0);
        gluCylinder(p,0.04,0.04,3.3,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    glPopMatrix();

    //Steps
    glPushMatrix();
    glTranslated(0.0,0.0,-0.5);
    glColor3f(0.9,0.2,0.2);
    glBegin(GL_QUAD_STRIP);
    glVertex3f(-1.65,-0.83,-5.5);
    glVertex3f(-1.65,-0.83,-6.5);
    glVertex3f(-2.0,-0.83,-5.5);
    glVertex3f(-2.0,-0.83,-6.5);
    glVertex3f(-2.0,-1.16,-5.5);
    glVertex3f(-2.0,-1.16,-6.5);
    glVertex3f(-2.35,-1.16,-5.5);
    glVertex3f(-2.35,-1.16,-6.5);
    glVertex3f(-2.35,-1.5,-5.5);
    glVertex3f(-2.35,-1.5,-6.5);
    glVertex3f(-1.65,-1.5,-5.5);
    glVertex3f(-1.65,-1.5,-6.5);
    glEnd();
    //StepsFrontSide
    glColor3f(0.9,0.0,0.0);
    glBegin(GL_POLYGON);
    glVertex3f(-1.65,-0.83,-5.5);
    glVertex3f(-2.0,-0.83,-5.5);
    glVertex3f(-2.0,-1.16,-5.5);
    glVertex3f(-2.35,-1.16,-5.5);
    glVertex3f(-2.35,-1.5,-5.5);
    glVertex3f(-1.65,-1.5,-5.5);
    glEnd();
    //StepsBackSide
    glBegin(GL_POLYGON);
    glVertex3f(-1.65,-0.83,-6.5);
    glVertex3f(-2.0,-0.83,-6.5);
    glVertex3f(-2.0,-1.16,-6.5);
    glVertex3f(-2.35,-1.16,-6.5);
    glVertex3f(-2.35,-1.5,-6.5);
    glVertex3f(-1.65,-1.5,-6.5);
    glEnd();
    glPopMatrix();

    //RedCarpet
    glNormal3f(0.0,-1.0,0.0);
    glBegin(GL_QUADS);
    glVertex3f(-0.75,-1.5,-5.0);
    glVertex3f(-0.75,-1.5,1.0);
    glVertex3f(0.75,-1.5,1.0);
    glVertex3f(0.75,-1.5,-5.0);
    glEnd();

    //Keyboard
    //Keyboard Body
    glColor3f(0.6,0.6,0.6);
    glBegin(GL_QUAD_STRIP);
    glNormal3f(0.0,-1.0,0.0);
    glVertex3f(-1.5,0.45,-6.5);
    glVertex3f(-0.7,0.45,-6.5);
    glVertex3f(-1.5,0.45,-6.0);
    glVertex3f(-0.7,0.45,-6.0);
    glVertex3f(-1.5,0.5,-5.7);
    glVertex3f(-0.7,0.5,-5.7);
    glVertex3f(-1.5,0.5,-5.5);
    glVertex3f(-0.7,0.5,-5.5);
    glVertex3f(-1.5,0.3,-5.5);
    glVertex3f(-0.7,0.3,-5.5);
    glVertex3f(-1.5,0.3,-6.5);
    glVertex3f(-0.7,0.3,-6.5);
    glVertex3f(-1.5,0.45,-6.5);
    glVertex3f(-0.7,0.45,-6.5);
    glEnd();
    //Keyboard Base
    glColor3f(0.4,0.4,0.4);
    glNormal3f(0.0,-1.0,0.0);
    glBegin(GL_QUADS);
    glVertex3f(-1.5,0.3,-6.5);
    glVertex3f(-1.5,0.3,-5.5);
    glVertex3f(-0.7,0.3,-5.5);
    glVertex3f(-0.7,0.3,-6.5);
    glEnd();
    //Keyboard Left Side
    glColor3f(0.4,0.4,0.4);
    glBegin(GL_POLYGON);
    glVertex3f(-0.7,0.3,-6.5);
    glVertex3f(-0.7,0.3,-5.5);
    glVertex3f(-0.7,0.5,-5.5);
    glVertex3f(-0.7,0.5,-5.7);
    glVertex3f(-0.7,0.45,-6.0);
    glVertex3f(-0.7,0.45,-6.5);
    glEnd();
    //Keyboard Right Side
    glBegin(GL_POLYGON);
    glVertex3f(-1.5,0.3,-6.5);
    glVertex3f(-1.5,0.3,-5.5);
    glVertex3f(-1.5,0.5,-5.5);
    glVertex3f(-1.5,0.5,-5.7);
    glVertex3f(-1.5,0.45,-6.0);
    glVertex3f(-1.5,0.45,-6.5);
    glEnd();
    //Keyboard Stands
    glColor3f(0.5,0.5,0.5);
    //Keyboard Left Front Stand
    glPushMatrix();
        glTranslatef(-1.45,-0.5,-5.55);
    glPushMatrix();
        glRotatef(90.0,-1.0,0.0,0.0);
        gluCylinder(p,0.02,0.02,0.8,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    glPopMatrix();
    //Keyboard Right Front Stand
    glPushMatrix();
        glTranslatef(-0.75,-0.5,-5.55);
    glPushMatrix();
        glRotatef(90.0,-1.0,0.0,0.0);
        gluCylinder(p,0.02,0.02,0.8,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    glPopMatrix();
    //Keyboard Left Back Stand
    glPushMatrix();
        glTranslatef(-1.45,-0.5,-6.45);
    glPushMatrix();
        glRotatef(90.0,-1.0,0.0,0.0);
        gluCylinder(p,0.02,0.02,0.8,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    glPopMatrix();
    //Keyboard Right Back Stand
    glPushMatrix();
        glTranslatef(-0.75,-0.5,-6.45);
    glPushMatrix();
        glRotatef(90.0,-1.0,0.0,0.0);
        gluCylinder(p,0.02,0.02,0.8,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    glPopMatrix();


    //Line
    glColor3f(0.3,0.0,0.0);
    glBegin(GL_LINES);
    glVertex3f(-1.65,-0.5,-5.0);
    glVertex3f(1.65,-0.5,-5.0);
    glEnd();

    //Mic
    //Mic Bottom
    glColor3f(0.3,0.3,0.3);
    glPushMatrix();
        glTranslatef(0.0,-0.5,-5.2);
    glPushMatrix();
        glRotatef(90.0,-1.0,0.0,0.0);
        gluCylinder(p,0.2,0.03,0.1,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    glPopMatrix();
    //Mic Stand
    glPushMatrix();
        glTranslatef(0.0,-0.4,-5.2);
    glPushMatrix();
        glRotatef(90.0,-1.0,0.0,0.0);
        gluCylinder(p,0.03,0.03,0.9,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    glPopMatrix();
    //Microphone
    glPushMatrix();
    glTranslated(0.0,0.56,-5.2);
    glutSolidTorus(0.03,0.03,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    glPushMatrix();
    glTranslated(0.0,0.56,-5.25);
    glutSolidTorus(0.03,0.03,(GLint)slices,(GLint)stacks);
    glPopMatrix();

    //Right Drum
    drum();
    //Left Drum
    glPushMatrix();
    glTranslated(-0.5,0.0,0.0);
    drum();
    glPopMatrix();

    //DrumSticks
    glColor3f(0.3,0.0,0.2);
    glLineWidth(6.0);
    glBegin(GL_LINES);
    glVertex3f(1.4,y_pos_stick,-6.0);
    glVertex3f(1.4,0.3,-6.3);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(0.9,y_pos_stick,-6.0);
    glVertex3f(0.9,0.3,-6.3);
    glEnd();

    //Drummer
    glColor3f(0.9,0.5,0.0);
    //Drummer face
    //Drummer front of the face
    glPushMatrix();
    glScalef(0.4f,0.4f,1.0f);
    glPushMatrix();
    glTranslated(2.9,1.9,-1.5);
    glBegin(GL_POLYGON);
    glVertex3f(-0.3,0.2,-5.0);
    glVertex3f(-0.3,-0.2,-5.0);
    glVertex3f(-0.1,-0.4,-5.0);
    glVertex3f(0.1,-0.4,-5.0);
    glVertex3f(0.3,-0.2,-5.0);
    glVertex3f(0.3,0.2,-5.0);
    glVertex3f(0.225,0.35,-5.0);
    glVertex3f(-0.225,0.35,-5.0);
    glEnd();
    //Drummer eyebrows
    glLineWidth(2.0);
    glColor3f(0.0,0.0,0.0);
    glBegin(GL_LINES);
    glVertex3f(-0.2,0.15,-4.99);
    glVertex3f(-0.1,0.15,-4.99);
    glVertex3f(0.2,0.15,-4.99);
    glVertex3f(0.1,0.15,-4.99);
    glEnd();
    glPointSize(3.0);
    //Drummer eyes
    glColor3f(0.0,0.0,0.0);
    glBegin(GL_POINTS);
    glVertex3f(-0.15,0.1,-4.99);
    glVertex3f(0.15,0.1,-4.99);
    glEnd();
    //Drummer beard
    glBegin(GL_POLYGON);
    glVertex3f(-0.2,-0.05,-4.99);
    glVertex3f(-0.2,-0.35,-4.99);
    glVertex3f(0.2,-0.35,-4.99);
    glVertex3f(0.2,-0.05,-4.99);
    glEnd();
    //Drummer hair
    glBegin(GL_QUAD_STRIP);
    glVertex3f(-0.3,-0.1,-5.0);
    glVertex3f(-0.3,-0.1,-5.25);
    glVertex3f(-0.3,0.2,-5.0);
    glVertex3f(-0.3,0.2,-5.25);
    glVertex3f(-0.225,0.35,-5.0);
    glVertex3f(-0.225,0.35,-5.25);
    glVertex3f(0.225,0.35,-5.0);
    glVertex3f(0.225,0.35,-5.25);
    glVertex3f(0.3,0.2,-5.0);
    glVertex3f(0.3,0.2,-5.25);
    glVertex3f(0.3,-0.1,-5.0);
    glVertex3f(0.3,-0.1,-5.25);
    glEnd();
    //Drummer back of hair
    glBegin(GL_POLYGON);
    glVertex3f(-0.3,0.2,-5.26);
    glVertex3f(-0.3,-0.2,-5.26);
    glVertex3f(0.3,-0.2,-5.26);
    glVertex3f(0.3,0.2,-5.26);
    glVertex3f(0.225,0.35,-5.26);
    glVertex3f(-0.225,0.35,-5.26);
    glEnd();
    //Drummer lips
    glColor3f(1.0,0.8,0.9);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex3f(-0.1,-0.2,-4.98);
    glVertex3f(0.1,-0.2,-4.98);
    glEnd();
    glColor3f(0.9,0.5,0.0);
    glLineWidth(1.0);
    glBegin(GL_LINES);
    glVertex3f(-0.1,-0.25,-4.98);
    glVertex3f(0.1,-0.25,-4.98);
    glEnd();
    //Drummer right ear
    glBegin(GL_QUADS);
    glVertex3f(0.3,-0.075+0.1,-5.1);
    glVertex3f(0.3,0.05+0.1,-5.1);
    glVertex3f(0.375,0.1+0.1,-5.1);
    glVertex3f(0.375,-0.1125+0.1,-5.1);
    glEnd();
    //Drummer left ear
    glBegin(GL_QUADS);
    glVertex3f(-0.3,0.025,-5.1);
    glVertex3f(-0.3,0.05+0.1,-5.1);
    glVertex3f(-0.375,0.1+0.1,-5.1);
    glVertex3f(-0.375,-0.1125+0.1,-5.1);
    glEnd();
    //Drummer neck
    glPushMatrix();
    glTranslated(0.0,-0.55,-5.125);
    glPushMatrix();
    glRotated(90.0,-1.0,0.0,0.0);
    glNormal3f(-1.0,1.0,1.0);
    gluCylinder(p,0.1,0.1,0.15,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    glPopMatrix();
    //Drummer back of the face
    glBegin(GL_POLYGON);
    glVertex3f(-0.3,0.2,-5.25);
    glVertex3f(-0.3,-0.2,-5.25);
    glVertex3f(-0.1,-0.4,-5.25);
    glVertex3f(0.1,-0.4,-5.25);
    glVertex3f(0.3,-0.2,-5.25);
    glVertex3f(0.3,0.2,-5.25);
    glVertex3f(0.225,0.35,-5.25);
    glVertex3f(-0.225,0.35,-5.25);
    glEnd();
    //Drummer side of the head
    glBegin(GL_QUAD_STRIP);
    glVertex3f(-0.3,-0.1,-5.0);
    glVertex3f(-0.3,-0.1,-5.25);
    glVertex3f(-0.3,-0.2,-5.0);
    glVertex3f(-0.3,-0.2,-5.25);
    glVertex3f(-0.125,-0.4,-5.0);
    glVertex3f(-0.125,-0.4,-5.25);
    glVertex3f(0.125,-0.4,-5.0);
    glVertex3f(0.125,-0.4,-5.25);
    glVertex3f(0.3,-0.2,-5.0);
    glVertex3f(0.3,-0.2,-5.25);
    glVertex3f(0.3,-0.1,-5.0);
    glVertex3f(0.3,-0.1,-5.25);
    glEnd();
    //Drummer shirt
    glColor3f(0.0,1.0,0.0);
    glBegin(GL_QUAD_STRIP);
    glVertex3f(-0.50,-0.55,-5.0);
    glVertex3f(-0.50,-1.95,-5.0);
    glVertex3f(0.50,-0.55,-5.0);
    glVertex3f(0.50,-1.95,-5.0);
    glVertex3f(0.65,-0.55,-5.125);
    glVertex3f(0.65,-1.95,-5.125);
    glVertex3f(0.50,-0.55,-5.25);
    glVertex3f(0.50,-1.95,-5.25);
    glVertex3f(-0.50,-0.55,-5.25);
    glVertex3f(-0.50,-1.95,-5.25);
    glVertex3f(-0.65,-0.55,-5.125);
    glVertex3f(-0.65,-1.95,-5.125);
    glVertex3f(-0.50,-0.55,-5.0);
    glVertex3f(-0.50,-1.95,-5.0);
    glEnd();
    //Drummer arms
    //Drummer right arm
    glColor3f(0.9,0.5,0.0);
    glBegin(GL_QUAD_STRIP);
    glVertex3f(0.6,-0.55,-5.0);
    glVertex3f(0.6,-1.25,-5.0);
    glVertex3f(0.7,-0.55,-5.0);
    glVertex3f(0.7,-1.25,-5.0);
    glVertex3f(0.7,-0.55,-5.2);
    glVertex3f(0.7,-1.25,-5.2);
    glVertex3f(0.6,-0.55,-5.2);
    glVertex3f(0.6,-1.25,-5.2);
    glVertex3f(0.6,-0.55,-5.0);
    glVertex3f(0.6,-1.25,-5.0);
    glEnd();
    glBegin(GL_QUAD_STRIP);
    glVertex3f(0.6,-1.0,-5.2);
    glVertex3f(0.6,-1.0,-4.75);
    glVertex3f(0.7,-1.0,-5.2);
    glVertex3f(0.7,-1.0,-4.75);
    glVertex3f(0.7,-1.25,-5.2);
    glVertex3f(0.7,-1.25,-4.75);
    glVertex3f(0.6,-1.25,-5.2);
    glVertex3f(0.6,-1.25,-4.75);
    glVertex3f(0.6,-1.0,-5.2);
    glVertex3f(0.6,-1.0,-4.75);
    glEnd();
    //Drummer left arm
    glBegin(GL_QUAD_STRIP);
    glVertex3f(-0.6,-0.55,-5.0);
    glVertex3f(-0.6,-1.25,-5.0);
    glVertex3f(-0.7,-0.55,-5.0);
    glVertex3f(-0.7,-1.25,-5.0);
    glVertex3f(-0.7,-0.55,-5.2);
    glVertex3f(-0.7,-1.25,-5.2);
    glVertex3f(-0.6,-0.55,-5.2);
    glVertex3f(-0.6,-1.25,-5.2);
    glVertex3f(-0.6,-0.55,-5.0);
    glVertex3f(-0.6,-1.25,-5.0);
    glEnd();
    glBegin(GL_QUAD_STRIP);
    glVertex3f(-0.6,-1.0,-5.2);
    glVertex3f(-0.6,-1.0,-4.75);
    glVertex3f(-0.7,-1.0,-5.2);
    glVertex3f(-0.7,-1.0,-4.75);
    glVertex3f(-0.7,-1.25,-5.2);
    glVertex3f(-0.7,-1.25,-4.75);
    glVertex3f(-0.6,-1.25,-5.2);
    glVertex3f(-0.6,-1.25,-4.75);
    glVertex3f(-0.6,-1.0,-5.2);
    glVertex3f(-0.6,-1.0,-4.75);
    glEnd();
    //Drummer pant
    //Drummer pant front side
    glColor3f(0.0,0.0,1.0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.5,-1.95,-5.0);
    glVertex3f(-0.4,-3.95,-5.0);
    glVertex3f(-0.25,-3.95,-5.0);
    glVertex3f(-0.1,-2.5,-5.0);
    glVertex3f(0.0,-2.5,-5.0);
    glVertex3f(0.0,-1.95,-5.0);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(0.0,-1.95,-5.0);
    glVertex3f(0.0,-2.5,-5.0);
    glVertex3f(0.1,-2.5,-5.0);
    glVertex3f(0.25,-3.95,-5.0);
    glVertex3f(0.4,-3.95,-5.0);
    glVertex3f(0.5,-1.95,-5.0);
    glEnd();
    //Drummer pant inner side
    glBegin(GL_QUADS);
    glVertex3f(-0.25,-3.95,-5.0);
    glVertex3f(0.0,-2.5,-5.0);
    glVertex3f(0.0,-2.5,-5.25);
    glVertex3f(-0.25,-3.95,-5.25);

    glVertex3f(0.25,-3.95,-5.25);
    glVertex3f(0.0,-2.5,-5.25);
    glVertex3f(0.0,-2.5,-5.0);
    glVertex3f(0.25,-3.95,-5.0);
    glEnd();
    //Drummer pant back side
    glBegin(GL_POLYGON);
    glVertex3f(-0.5,-1.95,-5.25);
    glVertex3f(-0.4,-3.95,-5.25);
    glVertex3f(-0.3,-3.95,-5.25);
    glVertex3f(-0.1,-2.5,-5.25);
    glVertex3f(0.0,-2.5,-5.25);
    glVertex3f(0.0,-1.95,-5.25);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(0.0,-1.95,-5.25);
    glVertex3f(0.0,-2.5,-5.25);
    glVertex3f(0.1,-2.5,-5.25);
    glVertex3f(0.3,-3.95,-5.25);
    glVertex3f(0.4,-3.95,-5.25);
    glVertex3f(0.5,-1.95,-5.25);
    glEnd();
    //Drummer pant left side
    glBegin(GL_QUADS);
    glVertex3f(-0.5,-1.85,-5.25);
    glVertex3f(-0.4,-3.95,-5.25);
    glVertex3f(-0.4,-3.95,-5.0);
    glVertex3f(-0.5,-1.85,-5.0);
    glEnd();
    //Drummer pant right side
    glBegin(GL_QUADS);
    glVertex3f(0.5,-1.85,-5.25);
    glVertex3f(0.4,-3.95,-5.25);
    glVertex3f(0.4,-3.95,-5.0);
    glVertex3f(0.5,-1.85,-5.0);
    glEnd();
    glPopMatrix();
    glPopMatrix();


    //Singer
    if(perf==0)
    {
        //Male Singer
        glColor3f(0.9,0.5,0.0);
        //Male Singer face
        //Male Singer front of the face
        glPushMatrix();
        glScalef(0.4f,0.4f,1.0f);
        glPushMatrix();
        glTranslated(0.0,1.9,-0.5);
        glBegin(GL_POLYGON);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(-0.3,0.2,-5.0);
        glVertex3f(-0.3,-0.2,-5.0);
        glVertex3f(-0.1,-0.4,-5.0);
        glVertex3f(0.1,-0.4,-5.0);
        glVertex3f(0.3,-0.2,-5.0);
        glVertex3f(0.3,0.2,-5.0);
        glVertex3f(0.225,0.35,-5.0);
        glVertex3f(-0.225,0.35,-5.0);
        glEnd();
        //Male Singer eyebrows
        glColor3f(0.0,0.0,0.0);
        glBegin(GL_LINES);
        glVertex3f(-0.2,0.15,-4.99);
        glVertex3f(-0.1,0.15,-4.99);
        glVertex3f(0.2,0.15,-4.99);
        glVertex3f(0.1,0.15,-4.99);
        glEnd();
        glPointSize(3.0);
        //Male Singer eyes
        glColor3f(0.0,0.0,0.0);
        glBegin(GL_POINTS);
        glVertex3f(-0.15,0.08,-4.99);
        glVertex3f(0.15,0.08,-4.99);
        glEnd();
        //Male Singer lips
        glColor3f(1.0,0.8,0.9);
        glLineWidth(2.0);
        glBegin(GL_LINES);
        glVertex3f(-0.1,-0.2,-4.98);
        glVertex3f(0.1,-0.2,-4.98);
        glEnd();
        //Male Singer cap
        glColor3f(1.0,0.0,0.0);
        //Male Singer cap front
        glBegin(GL_POLYGON);
        glVertex3f(-0.3,0.25,-5.0);
        glVertex3f(-0.3,0.5,-5.0);
        glVertex3f(-0.25,0.55,-5.0);
        glVertex3f(0.25,0.55,-5.0);
        glVertex3f(0.3,0.5,-5.0);
        glVertex3f(0.3,0.25,-5.0);
        glEnd();
        //Male Singer cap sides
        glBegin(GL_QUAD_STRIP);
        glVertex3f(-0.3,0.25,-5.25);
        glVertex3f(-0.3,0.25,-5.0);
        glVertex3f(-0.3,0.5,-5.25);
        glVertex3f(-0.3,0.5,-5.0);
        glVertex3f(-0.25,0.55,-5.25);
        glVertex3f(-0.25,0.55,-5.0);
        glVertex3f(0.25,0.55,-5.25);
        glVertex3f(0.25,0.55,-5.0);
        glVertex3f(0.3,0.5,-5.25);
        glVertex3f(0.3,0.5,-5.0);
        glVertex3f(0.3,0.25,-5.25);
        glVertex3f(0.3,0.25,-5.0);
        glEnd();
        //Male Singer cap hood
        glBegin(GL_POLYGON);
        glVertex3f(-0.3,0.25,-5.0);
        glVertex3f(-0.3,0.25,-4.85);
        glVertex3f(-0.275,0.25,-4.8);
        glVertex3f(0.275,0.25,-4.8);
        glVertex3f(0.3,0.25,-4.85);
        glVertex3f(0.3,0.25,-5.0);
        glEnd();
        //Male Singer cap back
        glBegin(GL_POLYGON);
        glVertex3f(-0.3,0.25,-5.25);
        glVertex3f(-0.3,0.5,-5.25);
        glVertex3f(-0.25,0.55,-5.25);
        glVertex3f(0.25,0.55,-5.25);
        glVertex3f(0.3,0.5,-5.25);
        glVertex3f(0.3,0.25,-5.25);
        glEnd();
        glColor3f(0.9,0.5,0.0);
        //Male Singer right ear
        glBegin(GL_QUADS);
        glVertex3f(0.3,-0.075,-5.1);
        glVertex3f(0.3,0.05,-5.1);
        glVertex3f(0.375,0.1,-5.1);
        glVertex3f(0.375,-0.1125,-5.1);
        glEnd();
        //Male Singer left ear
        glBegin(GL_QUADS);
        glVertex3f(-0.3,-0.075,-5.1);
        glVertex3f(-0.3,0.05,-5.1);
        glVertex3f(-0.375,0.1,-5.1);
        glVertex3f(-0.375,-0.1125,-5.1);
        glEnd();
        //Male Singer neck
        glPushMatrix();
        glTranslated(0.0,-0.55,-5.125);
        glPushMatrix();
        glRotated(90.0,-1.0,0.0,0.0);
        glNormal3f(-1.0,1.0,1.0);
        gluCylinder(p,0.1,0.1,0.15,(GLint)slices,(GLint)stacks);
        glPopMatrix();
        glPopMatrix();
        //Male Singer back of the face
        glBegin(GL_POLYGON);
        glVertex3f(-0.3,0.2,-5.25);
        glVertex3f(-0.3,-0.2,-5.25);
        glVertex3f(-0.1,-0.4,-5.25);
        glVertex3f(0.1,-0.4,-5.25);
        glVertex3f(0.3,-0.2,-5.25);
        glVertex3f(0.3,0.2,-5.25);
        glVertex3f(0.225,0.35,-5.25);
        glVertex3f(-0.225,0.35,-5.25);
        glEnd();
        //Male Singer side of the head
        glBegin(GL_QUAD_STRIP);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(-0.3,0.2,-5.0);
        glVertex3f(-0.3,0.2,-5.25);
        glVertex3f(-0.3,-0.2,-5.0);
        glVertex3f(-0.3,-0.2,-5.25);
        glVertex3f(-0.125,-0.4,-5.0);
        glVertex3f(-0.125,-0.4,-5.25);
        glVertex3f(0.125,-0.4,-5.0);
        glVertex3f(0.125,-0.4,-5.25);
        glVertex3f(0.3,-0.2,-5.0);
        glVertex3f(0.3,-0.2,-5.25);
        glVertex3f(0.3,0.2,-5.0);
        glVertex3f(0.3,0.2,-5.25);
        glVertex3f(0.225,0.35,-5.0);
        glVertex3f(0.225,0.35,-5.25);
        glVertex3f(-0.225,0.35,-5.0);
        glVertex3f(-0.225,0.35,-5.25);
        glEnd();
        //Male Singer shirt
        glColor3f(1.0,1.0,0.0);
        glBegin(GL_QUAD_STRIP);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(-0.50,-0.55,-5.0);
        glVertex3f(-0.50,-1.95,-5.0);
        glVertex3f(0.50,-0.55,-5.0);
        glVertex3f(0.50,-1.95,-5.0);
        glVertex3f(0.65,-0.55,-5.125);
        glVertex3f(0.65,-1.95,-5.125);
        glVertex3f(0.50,-0.55,-5.25);
        glVertex3f(0.50,-1.95,-5.25);
        glVertex3f(-0.50,-0.55,-5.25);
        glVertex3f(-0.50,-1.95,-5.25);
        glVertex3f(-0.65,-0.55,-5.125);
        glVertex3f(-0.65,-1.95,-5.125);
        glVertex3f(-0.50,-0.55,-5.0);
        glVertex3f(-0.50,-1.95,-5.0);
        glEnd();
        //Male Singer arms
        //Male Singer right arm
        glColor3f(0.9,0.5,0.0);
        glBegin(GL_QUAD_STRIP);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(0.6,-0.55,-5.0);
        glVertex3f(0.6,-1.25,-5.0);
        glVertex3f(0.7,-0.55,-5.0);
        glVertex3f(0.7,-1.25,-5.0);
        glVertex3f(0.7,-0.55,-5.2);
        glVertex3f(0.7,-1.25,-5.2);
        glVertex3f(0.6,-0.55,-5.2);
        glVertex3f(0.6,-1.25,-5.2);
        glVertex3f(0.6,-0.55,-5.0);
        glVertex3f(0.6,-1.25,-5.0);
        glEnd();
        glBegin(GL_QUAD_STRIP);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(0.6,-1.0,-5.2);
        glVertex3f(0.1,-1.0,-4.75);
        glVertex3f(0.741,-1.0,-5.2);
        glVertex3f(0.2,-1.0,-4.75);
        glVertex3f(0.741,-1.25,-5.2);
        glVertex3f(0.2,-1.25,-4.75);
        glVertex3f(0.6,-1.25,-5.2);
        glVertex3f(0.1,-1.25,-4.75);
        glVertex3f(0.6,-1.0,-5.2);
        glVertex3f(0.1,-1.0,-4.75);
        glEnd();
        //Male Singer left arm
        glBegin(GL_QUAD_STRIP);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(-0.6,-0.55,-5.0);
        glVertex3f(-0.6,-1.25,-5.0);
        glVertex3f(-0.7,-0.55,-5.0);
        glVertex3f(-0.7,-1.25,-5.0);
        glVertex3f(-0.7,-0.55,-5.2);
        glVertex3f(-0.7,-1.25,-5.2);
        glVertex3f(-0.6,-0.55,-5.2);
        glVertex3f(-0.6,-1.25,-5.2);
        glVertex3f(-0.6,-0.55,-5.0);
        glVertex3f(-0.6,-1.25,-5.0);
        glEnd();
        glBegin(GL_QUAD_STRIP);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(-0.6,-1.0,-5.2);
        glVertex3f(-0.1,-1.0,-4.75);
        glVertex3f(-0.741,-1.0,-5.2);
        glVertex3f(-0.2,-1.0,-4.75);
        glVertex3f(-0.741,-1.25,-5.2);
        glVertex3f(-0.2,-1.25,-4.75);
        glVertex3f(-0.6,-1.25,-5.2);
        glVertex3f(-0.1,-1.25,-4.75);
        glVertex3f(-0.6,-1.0,-5.2);
        glVertex3f(-0.1,-1.0,-4.75);
        glEnd();
        //Male Singer pant
        //Male Singer pant front side
        glColor3f(0.0,0.0,1.0);
        glBegin(GL_POLYGON);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(-0.5,-1.95,-5.0);
        glVertex3f(-0.4,-3.95,-5.0);
        glVertex3f(-0.25,-3.95,-5.0);
        glVertex3f(-0.1,-2.5,-5.0);
        glVertex3f(0.0,-2.5,-5.0);
        glVertex3f(0.0,-1.95,-5.0);
        glEnd();
        glBegin(GL_POLYGON);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(0.0,-1.95,-5.0);
        glVertex3f(0.0,-2.5,-5.0);
        glVertex3f(0.1,-2.5,-5.0);
        glVertex3f(0.25,-3.95,-5.0);
        glVertex3f(0.4,-3.95,-5.0);
        glVertex3f(0.5,-1.95,-5.0);
        glEnd();
        //Male Singer pant inner side
        glBegin(GL_QUADS);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(-0.25,-3.95,-5.0);
        glVertex3f(0.0,-2.5,-5.0);
        glVertex3f(0.0,-2.5,-5.25);
        glVertex3f(-0.25,-3.95,-5.25);

        glVertex3f(0.25,-3.95,-5.25);
        glVertex3f(0.0,-2.5,-5.25);
        glVertex3f(0.0,-2.5,-5.0);
        glVertex3f(0.25,-3.95,-5.0);
        glEnd();
        //Male Singer pant back side
        glBegin(GL_POLYGON);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(-0.5,-1.95,-5.25);
        glVertex3f(-0.4,-3.95,-5.25);
        glVertex3f(-0.3,-3.95,-5.25);
        glVertex3f(-0.1,-2.5,-5.25);
        glVertex3f(0.0,-2.5,-5.25);
        glVertex3f(0.0,-1.95,-5.25);
        glEnd();
        glBegin(GL_POLYGON);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(0.0,-1.95,-5.25);
        glVertex3f(0.0,-2.5,-5.25);
        glVertex3f(0.1,-2.5,-5.25);
        glVertex3f(0.3,-3.95,-5.25);
        glVertex3f(0.4,-3.95,-5.25);
        glVertex3f(0.5,-1.95,-5.25);
        glEnd();
        //Male Singer pant left side
        glBegin(GL_QUADS);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(-0.5,-1.85,-5.25);
        glVertex3f(-0.4,-3.95,-5.25);
        glVertex3f(-0.4,-3.95,-5.0);
        glVertex3f(-0.5,-1.85,-5.0);
        glEnd();
        //Male Singer pant right side
        glBegin(GL_QUADS);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(0.5,-1.85,-5.25);
        glVertex3f(0.4,-3.95,-5.25);
        glVertex3f(0.4,-3.95,-5.0);
        glVertex3f(0.5,-1.85,-5.0);
        glEnd();
        glPopMatrix();
        glPopMatrix();
    }
    else if(perf==1)
    {
        //Female Singer
        glColor3f(0.9,0.5,0.0);
        //Female Singer face
        //Female Singer front of the face
        glPushMatrix();
        glScalef(0.4f,0.4f,1.0f);
        glPushMatrix();
        glTranslated(0.0,1.9,-0.5);
        glBegin(GL_POLYGON);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(-0.3,0.2,-5.0);
        glVertex3f(-0.3,-0.2,-5.0);
        glVertex3f(-0.1,-0.4,-5.0);
        glVertex3f(0.1,-0.4,-5.0);
        glVertex3f(0.3,-0.2,-5.0);
        glVertex3f(0.3,0.2,-5.0);
        glVertex3f(0.225,0.35,-5.0);
        glVertex3f(-0.225,0.35,-5.0);
        glEnd();
        //Female Singer eyebrows
        glColor3f(0.0,0.0,0.0);
        glBegin(GL_LINES);
        glVertex3f(-0.2,0.15,-4.99);
        glVertex3f(-0.1,0.15,-4.99);
        glVertex3f(0.2,0.15,-4.99);
        glVertex3f(0.1,0.15,-4.99);
        glEnd();
        glPointSize(3.5);
        //Female Singer eyes
        glColor3f(0.0,0.0,0.0);
        glBegin(GL_POINTS);
        glVertex3f(-0.15,0.08,-4.99);
        glVertex3f(0.15,0.08,-4.99);
        glEnd();
        //Female Singer lips
        glColor3f(1.0,0.8,0.9);
        glLineWidth(2.0);
        glBegin(GL_LINES);
        glVertex3f(-0.1,-0.2,-4.98);
        glVertex3f(0.1,-0.2,-4.98);
        glEnd();
        //Female Singer hair
        glColor3f(0.9,0.8,0.3);
        glBegin(GL_POLYGON);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(-0.25,0.36,-5.0);
        glVertex3f(0.1,0.225,-5.0);
        glVertex3f(0.3,0.2,-5.0);
        glVertex3f(0.225,0.35,-5.0);
        glEnd();
        //Female Singer hair braid
        glBegin(GL_POLYGON);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(-0.25,0.36,-4.99);
        glVertex3f(-0.4,0.2,-4.99);
        glVertex3f(-0.52,-1.2,-4.99);
        glVertex3f(-0.2,-1.2,-4.99);
        glVertex3f(-0.3,0.2,-4.99);
        glVertex3f(-0.225,0.36,-4.99);
        glEnd();
        //Female Singer back of hair braid
        glBegin(GL_POLYGON);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(-0.25,0.36,-5.1);
        glVertex3f(-0.4,0.2,-5.1);
        glVertex3f(-0.52,-1.2,-5.1);
        glVertex3f(-0.2,-1.2,-5.1);
        glVertex3f(-0.3,0.2,-5.1);
        glVertex3f(-0.225,0.36,-5.1);
        glEnd();
        //Female Singer side of hair braid
        glBegin(GL_QUAD_STRIP);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(-0.25,0.36,-4.99);
        glVertex3f(-0.25,0.36,-5.1);
        glVertex3f(-0.4,0.2,-4.99);
        glVertex3f(-0.4,0.2,-5.1);
        glVertex3f(-0.52,-1.2,-4.99);
        glVertex3f(-0.52,-1.2,-5.1);
        glEnd();
        //Female Singer side of hair
        glBegin(GL_QUAD_STRIP);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(-0.32,-0.1,-5.0);
        glVertex3f(-0.32,-0.1,-5.26);
        glVertex3f(-0.32,0.2,-5.0);
        glVertex3f(-0.32,0.2,-5.26);
        glVertex3f(-0.225,0.4,-5.0);
        glVertex3f(-0.225,0.4,-5.26);
        glVertex3f(0.225,0.4,-5.0);
        glVertex3f(0.225,0.4,-5.26);
        glVertex3f(0.32,0.2,-5.0);
        glVertex3f(0.32,0.2,-5.26);
        glVertex3f(0.32,-0.1,-5.0);
        glVertex3f(0.32,-0.1,-5.26);
        glEnd();
        //Female Singer back of hair
        glBegin(GL_POLYGON);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(-0.3,0.2,-5.26);
        glVertex3f(-0.3,-0.2,-5.26);
        glVertex3f(0.3,-0.2,-5.26);
        glVertex3f(0.3,0.2,-5.26);
        glVertex3f(0.225,0.35,-5.26);
        glVertex3f(-0.225,0.35,-5.26);
        glEnd();
        glColor3f(0.9,0.5,0.0);
        //Female Singer right ear
        glBegin(GL_QUADS);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(0.3,-0.075,-5.1);
        glVertex3f(0.3,0.05,-5.1);
        glVertex3f(0.375,0.1,-5.1);
        glVertex3f(0.375,-0.1125,-5.1);
        glEnd();
        //Female Singer left ear
        glBegin(GL_QUADS);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(-0.3,-0.075,-5.1);
        glVertex3f(-0.3,0.05,-5.1);
        glVertex3f(-0.375,0.1,-5.1);
        glVertex3f(-0.375,-0.1125,-5.1);
        glEnd();
        //Female Singer earings
        glColor3f(0.0,0.0,1.0);
        glPointSize(3.0);
        glBegin(GL_POINTS);
        glVertex3f(0.35,-0.08,-5.1);
        glVertex3f(-0.35,-0.08,-5.1);
        glEnd();
        glColor3f(0.9,0.5,0.0);
        //Female Singer neck
        glPushMatrix();
        glTranslated(0.0,-0.55,-5.125);
        glPushMatrix();
        glRotated(90.0,-1.0,0.0,0.0);
        glNormal3f(-1.0,1.0,1.0);
        gluCylinder(p,0.1,0.1,0.15,(GLint)slices,(GLint)stacks);
        glPopMatrix();
        glPopMatrix();
        //Female Singer back of the face
        glBegin(GL_POLYGON);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(-0.3,0.2,-5.25);
        glVertex3f(-0.3,-0.2,-5.25);
        glVertex3f(-0.1,-0.4,-5.25);
        glVertex3f(0.1,-0.4,-5.25);
        glVertex3f(0.3,-0.2,-5.25);
        glVertex3f(0.3,0.2,-5.25);
        glVertex3f(0.225,0.35,-5.25);
        glVertex3f(-0.225,0.35,-5.25);
        glEnd();
        //Female Singer side of the head
        glBegin(GL_QUAD_STRIP);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(-0.3,0.2,-5.0);
        glVertex3f(-0.3,0.2,-5.25);
        glVertex3f(-0.3,-0.2,-5.0);
        glVertex3f(-0.3,-0.2,-5.25);
        glVertex3f(-0.125,-0.4,-5.0);
        glVertex3f(-0.125,-0.4,-5.25);
        glVertex3f(0.125,-0.4,-5.0);
        glVertex3f(0.125,-0.4,-5.25);
        glVertex3f(0.3,-0.2,-5.0);
        glVertex3f(0.3,-0.2,-5.25);
        glVertex3f(0.3,0.2,-5.0);
        glVertex3f(0.3,0.2,-5.25);
        glVertex3f(0.225,0.35,-5.0);
        glVertex3f(0.225,0.35,-5.25);
        glVertex3f(-0.225,0.35,-5.0);
        glVertex3f(-0.225,0.35,-5.25);
        glEnd();
        //Female Singer shirt
        glColor3f(0.6,0.6,0.6);
        glBegin(GL_QUAD_STRIP);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(-0.50,-0.55,-5.0);
        glVertex3f(-0.50,-1.95,-5.0);
        glVertex3f(0.50,-0.55,-5.0);
        glVertex3f(0.50,-1.95,-5.0);
        glVertex3f(0.65,-0.55,-5.125);
        glVertex3f(0.65,-1.95,-5.125);
        glVertex3f(0.50,-0.55,-5.25);
        glVertex3f(0.50,-1.95,-5.25);
        glVertex3f(-0.50,-0.55,-5.25);
        glVertex3f(-0.50,-1.95,-5.25);
        glVertex3f(-0.65,-0.55,-5.125);
        glVertex3f(-0.65,-1.95,-5.125);
        glVertex3f(-0.50,-0.55,-5.0);
        glVertex3f(-0.50,-1.95,-5.0);
        glEnd();
        //Female Singer arms
        //Female Singer right arm
        glColor3f(0.9,0.5,0.0);
        glBegin(GL_QUAD_STRIP);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(0.6,-0.55,-5.0);
        glVertex3f(0.6,-1.25,-5.0);
        glVertex3f(0.7,-0.55,-5.0);
        glVertex3f(0.7,-1.25,-5.0);
        glVertex3f(0.7,-0.55,-5.2);
        glVertex3f(0.7,-1.25,-5.2);
        glVertex3f(0.6,-0.55,-5.2);
        glVertex3f(0.6,-1.25,-5.2);
        glVertex3f(0.6,-0.55,-5.0);
        glVertex3f(0.6,-1.25,-5.0);
        glEnd();
        glBegin(GL_QUAD_STRIP);
        glNormal3f(0.0,0.0,-1.0);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(0.6,-1.0,-5.2);
        glVertex3f(0.1,-1.0,-4.75);
        glVertex3f(0.741,-1.0,-5.2);
        glVertex3f(0.2,-1.0,-4.75);
        glVertex3f(0.741,-1.25,-5.2);
        glVertex3f(0.2,-1.25,-4.75);
        glVertex3f(0.6,-1.25,-5.2);
        glVertex3f(0.1,-1.25,-4.75);
        glVertex3f(0.6,-1.0,-5.2);
        glVertex3f(0.1,-1.0,-4.75);
        glEnd();
        //Female Singer left arm
        glBegin(GL_QUAD_STRIP);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(-0.6,-0.55,-5.0);
        glVertex3f(-0.6,-1.25,-5.0);
        glVertex3f(-0.7,-0.55,-5.0);
        glVertex3f(-0.7,-1.25,-5.0);
        glVertex3f(-0.7,-0.55,-5.2);
        glVertex3f(-0.7,-1.25,-5.2);
        glVertex3f(-0.6,-0.55,-5.2);
        glVertex3f(-0.6,-1.25,-5.2);
        glVertex3f(-0.6,-0.55,-5.0);
        glVertex3f(-0.6,-1.25,-5.0);
        glEnd();
        glBegin(GL_QUAD_STRIP);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(-0.6,-1.0,-5.2);
        glVertex3f(-0.1,-1.0,-4.75);
        glVertex3f(-0.741,-1.0,-5.2);
        glVertex3f(-0.2,-1.0,-4.75);
        glVertex3f(-0.741,-1.25,-5.2);
        glVertex3f(-0.2,-1.25,-4.75);
        glVertex3f(-0.6,-1.25,-5.2);
        glVertex3f(-0.1,-1.25,-4.75);
        glVertex3f(-0.6,-1.0,-5.2);
        glVertex3f(-0.1,-1.0,-4.75);
        glEnd();
        //Female Singer skirt
        glColor3f(0.9,0.3,0.6);
        glBegin(GL_QUAD_STRIP);
        glVertex3f(-0.6,-1.95,-5.0);
        glVertex3f(-0.7,-3.95,-5.0);
        glVertex3f(0.6,-1.95,-5.0);
        glVertex3f(0.7,-3.95,-5.0);
        glVertex3f(0.6,-1.95,-5.25);
        glVertex3f(0.7,-3.95,-5.25);
        glVertex3f(-0.6,-1.95,-5.25);
        glVertex3f(-0.7,-3.95,-5.25);
        glVertex3f(-0.6,-1.95,-5.0);
        glVertex3f(-0.7,-3.95,-5.0);
        glEnd();
        glPopMatrix();
        glPopMatrix();
    }
    //Keyboardist
    glColor3f(1.0,0.9,0.6);
    //Keyboardist face
    //Keyboardist front of the face
    glPushMatrix();
    glScalef(0.4f,0.4f,1.0f);
    glPushMatrix();
    glTranslated(-2.9,2.4,-1.5);
    glBegin(GL_POLYGON);
    glVertex3f(-0.3,0.2,-5.0);
    glVertex3f(-0.3,-0.2,-5.0);
    glVertex3f(-0.1,-0.4,-5.0);
    glVertex3f(0.1,-0.4,-5.0);
    glVertex3f(0.3,-0.2,-5.0);
    glVertex3f(0.3,0.2,-5.0);
    glVertex3f(0.225,0.35,-5.0);
    glVertex3f(-0.225,0.35,-5.0);
    glEnd();
    //Keyboardist eyebrows
    glColor3f(0.3,0.1,0.0);
    glBegin(GL_LINES);
    glVertex3f(-0.2,0.15,-4.99);
    glVertex3f(-0.1,0.15,-4.99);
    glVertex3f(0.2,0.15,-4.99);
    glVertex3f(0.1,0.15,-4.99);
    glEnd();
    glPointSize(3.0);
    //Keyboardist eyes
    glColor3f(0.0,0.0,0.0);
    glBegin(GL_POINTS);
    glVertex3f(-0.15,0.1,-4.99);
    glVertex3f(0.15,0.1,-4.99);
    glEnd();
    //Keyboardist beard
    glBegin(GL_POLYGON);
    glVertex3f(-0.3,-0.05,-4.99);
    glVertex3f(-0.3,-0.2,-4.99);
    glVertex3f(-0.125,-0.4,-4.99);
    glVertex3f(0.125,-0.4,-4.99);
    glVertex3f(0.3,-0.2,-4.99);
    glVertex3f(0.3,-0.05,-4.99);
    glEnd();
    glBegin(GL_QUAD_STRIP);
    glVertex3f(-0.3,-0.05,-4.99);
    glVertex3f(-0.3,-0.05,-5.125);
    glVertex3f(-0.3,-0.2,-4.99);
    glVertex3f(-0.3,-0.2,-5.125);
    glVertex3f(-0.125,-0.4,-4.99);
    glVertex3f(-0.125,-0.4,-5.125);
    glVertex3f(0.125,-0.4,-4.99);
    glVertex3f(0.125,-0.4,-5.125);
    glVertex3f(0.3,-0.2,-4.99);
    glVertex3f(0.3,-0.2,-5.125);
    glVertex3f(0.3,-0.05,-4.99);
    glVertex3f(0.3,-0.05,-5.125);
    glEnd();
    //Keyboardist lips
    glColor3f(1.0,0.8,0.9);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex3f(-0.1,-0.2,-4.98);
    glVertex3f(0.1,-0.2,-4.98);
    glEnd();
    glColor3f(1.0,0.9,0.6);
    //Keyboardist right ear
    glBegin(GL_QUADS);
    glVertex3f(0.3,-0.075+0.1,-5.1);
    glVertex3f(0.3,0.05+0.1,-5.1);
    glVertex3f(0.375,0.1+0.1,-5.1);
    glVertex3f(0.375,-0.1125+0.1,-5.1);
    glEnd();
    //Keyboardist left ear
    glBegin(GL_QUADS);
    glVertex3f(-0.3,0.025,-5.1);
    glVertex3f(-0.3,0.05+0.1,-5.1);
    glVertex3f(-0.375,0.1+0.1,-5.1);
    glVertex3f(-0.375,-0.1125+0.1,-5.1);
    glEnd();
    //Keyboardist neck
    glPushMatrix();
    glTranslated(0.0,-0.55,-5.125);
    glPushMatrix();
    glRotated(90.0,-1.0,0.0,0.0);
    glNormal3f(-1.0,1.0,1.0);
    gluCylinder(p,0.1,0.1,0.15,(GLint)slices,(GLint)stacks);
    glPopMatrix();
    glPopMatrix();
    //Keyboardist back of the face
    glBegin(GL_POLYGON);
    glVertex3f(-0.3,0.2,-5.25);
    glVertex3f(-0.3,-0.2,-5.25);
    glVertex3f(-0.1,-0.4,-5.25);
    glVertex3f(0.1,-0.4,-5.25);
    glVertex3f(0.3,-0.2,-5.25);
    glVertex3f(0.3,0.2,-5.25);
    glVertex3f(0.225,0.35,-5.25);
    glVertex3f(-0.225,0.35,-5.25);
    glEnd();
    //Keyboardist side of the head
    glBegin(GL_QUAD_STRIP);
    glVertex3f(-0.3,-0.05,-5.25);
    glVertex3f(-0.3,-0.05,-5.0);
    glVertex3f(-0.3,0.2,-5.25);
    glVertex3f(-0.3,0.2,-5.0);
    glVertex3f(-0.225,0.35,-5.25);
    glVertex3f(-0.225,0.35,-5.0);
    glVertex3f(0.225,0.35,-5.25);
    glVertex3f(0.225,0.35,-5.0);
    glVertex3f(0.3,0.2,-5.25);
    glVertex3f(0.3,0.2,-5.0);
    glVertex3f(0.3,-0.05,-5.25);
    glVertex3f(0.3,-0.05,-5.0);
    glEnd();
    glBegin(GL_QUAD_STRIP);
    glVertex3f(-0.3,-0.05,-5.25);
    glVertex3f(-0.3,-0.05,-5.125);
    glVertex3f(-0.3,-0.2,-5.25);
    glVertex3f(-0.3,-0.2,-5.125);
    glVertex3f(-0.125,-0.4,-5.25);
    glVertex3f(-0.125,-0.4,-5.125);
    glVertex3f(0.125,-0.4,-5.25);
    glVertex3f(0.125,-0.4,-5.125);
    glVertex3f(0.3,-0.2,-5.25);
    glVertex3f(0.3,-0.2,-5.125);
    glVertex3f(0.3,-0.05,-5.25);
    glVertex3f(0.3,-0.05,-5.125);
    glEnd();
    //Keyboardist shirt
    glColor3f(1.0,0.3,1.0);
    glBegin(GL_QUAD_STRIP);
    glVertex3f(-0.50,-0.55,-5.0);
    glVertex3f(-0.50,-1.95,-5.0);
    glVertex3f(0.50,-0.55,-5.0);
    glVertex3f(0.50,-1.95,-5.0);
    glVertex3f(0.65,-0.55,-5.125);
    glVertex3f(0.65,-1.95,-5.125);
    glVertex3f(0.50,-0.55,-5.25);
    glVertex3f(0.50,-1.95,-5.25);
    glVertex3f(-0.50,-0.55,-5.25);
    glVertex3f(-0.50,-1.95,-5.25);
    glVertex3f(-0.65,-0.55,-5.125);
    glVertex3f(-0.65,-1.95,-5.125);
    glVertex3f(-0.50,-0.55,-5.0);
    glVertex3f(-0.50,-1.95,-5.0);
    glEnd();
    //Keyboardist arms
    //Keyboardist right arm
    glColor3f(1.0,0.9,0.6);
    glBegin(GL_QUAD_STRIP);
    glVertex3f(0.6,-0.55,-5.0);
    glVertex3f(0.6,-1.25,-5.0);
    glVertex3f(0.7,-0.55,-5.0);
    glVertex3f(0.7,-1.25,-5.0);
    glVertex3f(0.7,-0.55,-5.2);
    glVertex3f(0.7,-1.25,-5.2);
    glVertex3f(0.6,-0.55,-5.2);
    glVertex3f(0.6,-1.25,-5.2);
    glVertex3f(0.6,-0.55,-5.0);
    glVertex3f(0.6,-1.25,-5.0);
    glEnd();
    glBegin(GL_QUAD_STRIP);
    glVertex3f(0.6,-1.0,-5.2);
    glVertex3f(0.6,-1.0,-4.75);
    glVertex3f(0.7,-1.0,-5.2);
    glVertex3f(0.7,-1.0,-4.75);
    glVertex3f(0.7,-1.25,-5.2);
    glVertex3f(0.7,-1.25,-4.75);
    glVertex3f(0.6,-1.25,-5.2);
    glVertex3f(0.6,-1.25,-4.75);
    glVertex3f(0.6,-1.0,-5.2);
    glVertex3f(0.6,-1.0,-4.75);
    glEnd();
    //Keyboardist left arm
    glBegin(GL_QUAD_STRIP);
    glVertex3f(-0.6,-0.55,-5.0);
    glVertex3f(-0.6,-1.25,-5.0);
    glVertex3f(-0.7,-0.55,-5.0);
    glVertex3f(-0.7,-1.25,-5.0);
    glVertex3f(-0.7,-0.55,-5.2);
    glVertex3f(-0.7,-1.25,-5.2);
    glVertex3f(-0.6,-0.55,-5.2);
    glVertex3f(-0.6,-1.25,-5.2);
    glVertex3f(-0.6,-0.55,-5.0);
    glVertex3f(-0.6,-1.25,-5.0);
    glEnd();
    glBegin(GL_QUAD_STRIP);
    glVertex3f(-0.6,-1.0,-5.2);
    glVertex3f(-0.6,-1.0,-4.75);
    glVertex3f(-0.7,-1.0,-5.2);
    glVertex3f(-0.7,-1.0,-4.75);
    glVertex3f(-0.7,-1.25,-5.2);
    glVertex3f(-0.7,-1.25,-4.75);
    glVertex3f(-0.6,-1.25,-5.2);
    glVertex3f(-0.6,-1.25,-4.75);
    glVertex3f(-0.6,-1.0,-5.2);
    glVertex3f(-0.6,-1.0,-4.75);
    glEnd();
    //Keyboardist pant
    //Keyboardist pant front side
    glColor3f(0.7,0.7,0.7);
    glBegin(GL_POLYGON);
    glVertex3f(-0.5,-1.95,-5.0);
    glVertex3f(-0.4,-3.95,-5.0);
    glVertex3f(-0.25,-3.95,-5.0);
    glVertex3f(-0.1,-2.5,-5.0);
    glVertex3f(0.0,-2.5,-5.0);
    glVertex3f(0.0,-1.95,-5.0);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(0.0,-1.95,-5.0);
    glVertex3f(0.0,-2.5,-5.0);
    glVertex3f(0.1,-2.5,-5.0);
    glVertex3f(0.25,-3.95,-5.0);
    glVertex3f(0.4,-3.95,-5.0);
    glVertex3f(0.5,-1.95,-5.0);
    glEnd();
    //Keyboardist pant inner side
    glBegin(GL_QUADS);
    glVertex3f(-0.25,-3.95,-5.0);
    glVertex3f(0.0,-2.5,-5.0);
    glVertex3f(0.0,-2.5,-5.25);
    glVertex3f(-0.25,-3.95,-5.25);

    glVertex3f(0.25,-3.95,-5.25);
    glVertex3f(0.0,-2.5,-5.25);
    glVertex3f(0.0,-2.5,-5.0);
    glVertex3f(0.25,-3.95,-5.0);
    glEnd();
    //Keyboardist pant back side
    glBegin(GL_POLYGON);
    glVertex3f(-0.5,-1.95,-5.25);
    glVertex3f(-0.4,-3.95,-5.25);
    glVertex3f(-0.3,-3.95,-5.25);
    glVertex3f(-0.1,-2.5,-5.25);
    glVertex3f(0.0,-2.5,-5.25);
    glVertex3f(0.0,-1.95,-5.25);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(0.0,-1.95,-5.25);
    glVertex3f(0.0,-2.5,-5.25);
    glVertex3f(0.1,-2.5,-5.25);
    glVertex3f(0.3,-3.95,-5.25);
    glVertex3f(0.4,-3.95,-5.25);
    glVertex3f(0.5,-1.95,-5.25);
    glEnd();
    //Keyboardist pant left side
    glBegin(GL_QUADS);
    glVertex3f(-0.5,-1.85,-5.25);
    glVertex3f(-0.4,-3.95,-5.25);
    glVertex3f(-0.4,-3.95,-5.0);
    glVertex3f(-0.5,-1.85,-5.0);
    glEnd();
    //Keyboardist pant right side
    glBegin(GL_QUADS);
    glVertex3f(0.5,-1.85,-5.25);
    glVertex3f(0.4,-3.95,-5.25);
    glVertex3f(0.4,-3.95,-5.0);
    glVertex3f(0.5,-1.85,-5.0);
    glEnd();
    glPopMatrix();
    glPopMatrix();

    //Audience 1
    GLfloat shirt[]={1.0,1.0,0.0};
    GLfloat pant[]={0.0,0.0,1.0};
    GLfloat hair[]={0.0,0.0,0.0};
    GLfloat skin[]={0.9,0.5,0.0};

    glPushMatrix();
    glTranslated(-1.0,-0.5,1.0);
    draw_human(shirt,pant,hair,skin,0);
    glPopMatrix();

    //Audience 2
    shirt[0]=1.0; shirt[1]=1.0; shirt[2]=1.0;
    pant[0]=0.3; pant[1]=0.3; pant[2]=0.3;
    hair[0]=0.5; hair[1]=0.5; hair[2]=0.5;
    skin[0]=0.9; skin[1]=0.5; skin[2]=0.0;

    glPushMatrix();
    glTranslated(-1.5,-0.5,1.0);
    draw_human(shirt,pant,hair,skin,0);
    glPopMatrix();


    //Audience 5
    shirt[0]=0.3; shirt[1]=1.0; shirt[2]=0.6;
    pant[0]=0.4; pant[1]=0.0; pant[2]=0.0;
    hair[0]=0.9; hair[1]=0.8; hair[2]=0.3;
    skin[0]=1.0; skin[1]=0.9; skin[2]=0.6;

    glPushMatrix();
    glTranslated(1.0,-0.5,1.0);
    draw_human(shirt,pant,hair,skin,0);
    glPopMatrix();

    //Audience 6
    shirt[0]=0.9; shirt[1]=0.5; shirt[2]=0.0;
    pant[0]=0.3; pant[1]=0.3; pant[2]=0.8;
    hair[0]=0.0; hair[1]=0.0; hair[2]=0.0;
    skin[0]=1.0; skin[1]=0.9; skin[2]=0.6;

    glPushMatrix();
    glTranslated(1.5,-0.5,1.0);
    draw_human(shirt,pant,hair,skin,1);
    glPopMatrix();

    //Right Speaker//Audience
    draw_speaker();
    //Left Speaker
    glPushMatrix();
    glTranslated(-4.3,0.0,0.0);
    draw_speaker();
    glPopMatrix();

    if(view==0)//Not display the Welcome board in side view
    {
        //Drawing text
        glColor3f(1.0,0.0,0.0);
        std::string text;
        text="WELCOME TO ACHARYA HABBA 22";
        drawText(text.data(),text.size(),-1.6,1.25,-7.5);
        
        /*drawText(text.data(),text.size(),x_pos_txt[4],1.25,-7.5);
        text="M";
        drawText(text.data(),text.size(),x_pos_txt[5],1.25,-7.5);
        text="E";
        drawText(text.data(),text.size(),x_pos_txt[6],1.25,-7.5);
        text=" ";
        drawText(text.data(),text.size(),x_pos_txt[7],1.25,-7.5);
        text="T";
        drawText(text.data(),text.size(),x_pos_txt[8],1.25,-7.5);
        text="O";
        drawText(text.data(),text.size(),x_pos_txt[9],1.25,-7.5);
        text=" ";
        drawText(text.data(),text.size(),x_pos_txt[10],1.25,-7.5);
        text="A";
        drawText(text.data(),text.size(),x_pos_txt[11],1.25,-7.5);
        text="C";
        drawText(text.data(),text.size(),x_pos_txt[12],1.25,-7.5);
        text="H";
        drawText(text.data(),text.size(),x_pos_txt[13],1.25,-7.5);
        text="A";
        drawText(text.data(),text.size(),x_pos_txt[14],1.25,-7.5);
        text="R";
        drawText(text.data(),text.size(),x_pos_txt[15],1.25,-7.5);
        text="Y";
        drawText(text.data(),text.size(),x_pos_txt[16],1.25,-7.5);
        text="A";
        drawText(text.data(),text.size(),x_pos_txt[17],1.25,-7.5);
         text=" ";
        drawText(text.data(),text.size(),x_pos_txt[18],1.25,-7.5);
        text="H";
        drawText(text.data(),text.size(),x_pos_txt[19],1.25,-7.5);
        text="A";
        drawText(text.data(),text.size(),x_pos_txt[20],1.25,-7.5);
        text="B";
        drawText(text.data(),text.size(),x_pos_txt[21],1.25,-7.5);
        text="B";
        drawText(text.data(),text.size(),x_pos_txt[22],1.25,-7.5);
        text="A";
        drawText(text.data(),text.size(),x_pos_txt[23],1.25,-7.5);
        text=" ";
        drawText(text.data(),text.size(),x_pos_txt[24],1.25,-7.5);
        text="2";
        drawText(text.data(),text.size(),x_pos_txt[25],1.25,-7.5);
        text="0";
        drawText(text.data(),text.size(),x_pos_txt[26],1.25,-7.5);
        text="2";
        drawText(text.data(),text.size(),x_pos_txt[27],1.25,-7.5);
        text="2";
        drawText(text.data(),text.size(),x_pos_txt[28],1.25,-7.5);
*/
     
    }
    //Display Board
    glColor3f(0.0,0.0,0.0);
    glBegin(GL_QUADS);
    glVertex3f(-1.64,1.5,-7.6);
    glVertex3f(-1.64,1.15,-7.6);
    glVertex3f(1.64,1.15,-7.6);
    glVertex3f(1.64,1.5,-7.6);
    glEnd();
    //Display Board border
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_LINE_LOOP);
    glNormal3f(0.0,0.0,1.0);
    glVertex3f(-1.64,1.5,-7.6);
    glVertex3f(-1.64,1.15,-7.6);
    glVertex3f(1.64,1.15,-7.6);
    glVertex3f(1.64,1.5,-7.6);
    glEnd();

    glutTimerFunc(0,timer,0); //To recursively call the timer function and redraw the Text and Drum Sticks and the circles in the Speaker after changing their co ordinates


    double glow = (length - step) / (double)length;
    glColor3f(red*glow, green*glow, blue*glow);
    glBegin(GL_POINTS);
    for (i=0;i<numPoints;i++)
    {
        x[i] -= xacc[i];
        y[i] -= yacc[i];
        glVertex3f(x[i], y[i],-8.5); //Draw firework particles
    }
    glEnd();
    glutSwapBuffers();
}

void display(void)
{
    int i;
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if(view==1)//Left Side View
        gluLookAt(-5.9,0.5,-6.0,0.0,0.0,-6.0,0.0,1.0,0.0);
    else if(view==2)//Right Side View
        gluLookAt(5.9,0.5,-6.0,0.0,0.0,-6.0,0.0,1.0,0.0);
    if (step < 0.9*length)
    {
        for (i=0; i<numPoints; i++)
            yacc[i] -= 0.09 / length; // gravity
        draw_blast();
    }
    step ++;
    if (step > length)
        initialize();//To reinitialize the firework particles

    //Add ambient light
    GLfloat ambientColor[] = {0.8f, 0.8f, 0.8f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    //Add positioned light
	GLfloat lightColor0[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat lightPos0[] = {2.0f, 1.0f, -5.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	GLfloat lightColor1[]= {3.0, 3.0f, 3.0f, 1.0f};
	GLfloat lightPos1[]= {0.0f, 1.5f, -5.6f ,1.0f};
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
    GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.5);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 1.0);
}

void idle(void)
{
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
            exit(0);
            break;
        case 'f':
        case 'F':
            initialize();
            glutPostRedisplay();
            break;
        case 'c':
            cd=1;
            glutPostRedisplay();
            break;
        case 'C':
            cd=0;
            InitGL();
            glutPostRedisplay();
            break;
        case 'v':
        case 'V':
            if(view<2)
                view++;
            else
                view=0;
            break;
        case 'p':
        case 'P':
            if(perf==0)//sets male performer
            {
                perf=1;
                cd=1;
                glutTimerFunc(5000,ctimer,0);
                glutPostRedisplay();
            }
            else
            {
                perf=0;//Sets female performer
                cd=1;
                glutTimerFunc(5000,ctimer,0);
                glutPostRedisplay();
            }
            break;
        case 'l':
        case 'L':
            if(l==0)//Switches ON the spotlight
            {
                glEnable(GL_LIGHT1);
                l=1;
            }
            else
            {
                glDisable(GL_LIGHT1);//Switches OFF the spotlight
                l=0;
            }
    }
}

void reshape (int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,(double)w/(double)h,1.0,100.0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize (1000, 800);
    glutInitWindowPosition(0, 0);
    glutCreateWindow ("Music Concert");
    glClearColor (0.1, 0.1, 0.1, 0.0);
    initialize();
    enablefunc();
    InitGL();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}    
