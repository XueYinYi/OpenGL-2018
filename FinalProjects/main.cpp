//
//  main.cpp
//  FinalProject
//
//  Created by Yin on 11/26/18.
//  Copyright © 2018 Yin. All rights reserved.
//



#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#include "glew.h"
#endif

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include <vector>



//    This is a sample OpenGL / GLUT program
//
//    The objective is to draw a 3d object and change the color of the axes
//        with a glut menu
//
//    The left mouse button does rotation
//    The middle mouse button does scaling
//    The user interface allows:
//        1. The axes to be turned on and off
//        2. The color of the axes to be changed
//        3. Debugging to be turned on and off
//        4. Depth cueing to be turned on and off
//        5. The projection to be changed
//        6. The transformations to be reset
//        7. The program to quit
//
//    Author:            Joe Graphics

// NOTE: There are a lot of good reasons to use const variables instead
// of #define's.  However, Visual C++ does not allow a const variable
// to be used as an array size or as the case in a switch( ) statement.  So in
// the following, all constants are const variables except those which need to
// be array sizes or cases in switch( ) statements.  Those are #defines.


// title of these windows:

const char *WINDOWTITLE = { "OpenGL / GLUT Sample -- Joe Graphics" };
const char *GLUITITLE   = { "User Interface Window" };


// what the glui package defines as true and false:

const int GLUITRUE  = { true  };
const int GLUIFALSE = { false };


// the escape key:
#define NUMCURVES 5
#define NUMPOINTS 20
#define ESCAPE        0x1b
#define MS_PER_CYCLE    35000
float Time;


// initial window size:

const int INIT_WINDOW_SIZE = { 600 };


// size of the box:

const float BOXSIZE = { 2.f };



// multiplication factors for input interaction:
//  (these are known from previous experience)

const float ANGFACT = { 1. };
const float SCLFACT = { 0.005f };


// minimum allowable scale factor:

const float MINSCALE = { 0.05f };


// active mouse buttons (or them together):

const int LEFT   = { 4 };
const int MIDDLE = { 2 };
const int RIGHT  = { 1 };


// which projection:

enum Projections
{
    ORTHO,
    PERSP
};


// which button:

enum ButtonVals
{
    RESET,
    QUIT
};

enum Imgs
{
    ONE,
    TWO
    
};

bool LineOn = true;
bool PointOn = true;

// window background color (rgba):

const GLfloat BACKCOLOR[ ] = { 0., 0., 0., 1. };


// line width for the axes:

const GLfloat AXES_WIDTH   = { 3. };


// the color numbers:
// this order must match the radio button order

enum Tex
{
    ON,
    OFF,
    OPEN
};

enum Colors
{
    RED,
    YELLOW,
    GREEN,
    CYAN,
    BLUE,
    MAGENTA,
    WHITE,
    BLACK
};

char * ColorNames[ ] =
{
    "Red",
    "Yellow",
    "Green",
    "Cyan",
    "Blue",
    "Magenta",
    "White",
    "Black"
};


// the color definitions:
// this order must match the menu order

const GLfloat Colors[ ][3] =
{
    { 1., 0., 0. },        // red
    { 1., 1., 0. },        // yellow
    { 0., 1., 0. },        // green
    { 0., 1., 1. },        // cyan
    { 0., 0., 1. },        // blue
    { 1., 0., 1. },        // magenta
    { 1., 1., 1. },        // white
    { 0., 0., 0. },        // black
};


// fog parameters:

const GLfloat FOGCOLOR[4] = { .0, .0, .0, 1. };
const GLenum  FOGMODE     = { GL_LINEAR };
const GLfloat FOGDENSITY  = { 0.30f };
const GLfloat FOGSTART    = { 1.5 };
const GLfloat FOGEND      = { 4. };


// non-constant global variables:

int        ActiveButton;            // current button that is down
GLuint    AxesList;                // list to hold the axes
int        AxesOn;                    // != 0 means to draw the axes
int        DebugOn;                // != 0 means to print debugging info
int        DepthCueOn;                // != 0 means to use intensity depth cueing
GLuint    BoxList;
GLuint    Arc;
GLuint    XwingList;
GLuint    FalcomList;                // object display list
int        MainWindow;                // window id for main graphics window
float    Scale;                    // scaling factor
int        WhichColor;                // index into Colors[ ]
int        WhichProjection;        // ORTHO or PERSP
int        Xmouse, Ymouse;            // mouse values
float    Xrot, Yrot;
GLuint tex0, tex1,tex2,tex3;
int        TexOn;
int        ImgOn;
// rotation angles in degrees

float tex_anima;
GLuint  Donut;
GLuint Teapot;
GLuint  movingBox;

bool LIGHT0On = true;
bool LIGHT1On = true;
bool LIGHT2On = true;
bool LIGHT3On = true;
bool LIGHT4On = true;
bool LIGHT5On = true;
bool LIGHT6On = true;
bool LIGHT9On = true;





bool LIGHTTHREE = true;
bool FREEZE = false;
bool LOOK = false;
struct Point
{
    float x0, y0, z0;       // initial coordinates
    float x,  y,  z;        // animated coordinates
    void setPt(float x0, float y0, float z0) {
        this->x0 = x0;
        this->y0 = y0;
        this->z0 = z0;
    }
    void reset() {
        x = x0;
        y = y0;
        z = z0;
    }
};

struct Curve
{
    float r, g, b;
    Point p0, p1, p2, p3;
    void color(float r, float g, float b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }
    void reset() {
        p0.reset();
        p1.reset();
        p2.reset();
        p3.reset();
    }
};
Curve Curves[NUMCURVES];
Curve Stem;                // if you are not
Curve line1,line2,line3,line4,line5,line6,line7,line8,line9,line10,line11,line12,line13;




// function prototypes:

void    Animate( );
void    Display( );
void    DoAxesMenu( int );
void    DoColorMenu( int );
void    DoDepthMenu( int );
void    DoTexMenu(int);
void    DoImgMenu(int);
void    DoDebugMenu( int );
void    DoMainMenu( int );
void    DoProjectMenu( int );
void    DoRasterString( float, float, float, char * );
void    DoStrokeString( float, float, float, float, char * );
float    ElapsedSeconds( );
void    InitGraphics( );
void    InitLists( );
void    InitMenus( );
void    Keyboard( unsigned char, int, int );
void    MouseButton( int, int, int, int );
void    MouseMotion( int, int );
void    Reset( );
void    Resize( int, int );
void    Background();
void    Background2();
void    Background3();

void    Visibility( int );

void    Axes( float );
void    HsvRgb( float[3], float [3] );
void    loadTexture();
void    SetMaterial( float, float, float, float);
void    SetPointLight( int, float , float , float , float , float, float );
void    SetSpotLight( int, float , float , float , float , float , float , float , float , float  );
void    CreateBezierCur(Curve curve, GLfloat width);
void    RotateX(Point *p, float deg, float xc, float yc, float zc);
void    RotateY(Point *p, float deg, float xc, float yc, float zc);
void    RotateZ(Point *p, float deg, float xc, float yc, float zc);


// main program:

int
main( int argc, char *argv[ ] )
{
    // turn on the glut package:
    // (do this before checking argc and argv since it might
    // pull some command line arguments out)
    
    glutInit( &argc, argv );
    
    
    // setup all the graphics stuff:
    
    InitGraphics( );
    
    
    // create the display structures that will not change:
    
    InitLists( );
    
    
    // init all the global variables used by Display( ):
    // this will also post a redisplay
    
    Reset( );
    
    
    // setup all the user interface stuff:
    
    InitMenus( );
    loadTexture();
    
    // draw the scene once and wait for some interaction:
    // (this will never return)
    
    glutSetWindow( MainWindow );
    glutMainLoop( );
    loadTexture();
    
    // this is here to make the compiler happy:
    
    return 0;
}

bool    Distort;        // global -- true means to distort the texture

struct point {
    float x, y, z;        // coordinates
    float nx, ny, nz;    // surface normal
    float s, t;        // texture coords
};

int        NumLngs, NumLats;
struct point *    Pts;

struct point *
PtsPointer( int lat, int lng )
{
    if( lat < 0 )    lat += (NumLats-1);
    if( lng < 0 )    lng += (NumLngs-1);
    if( lat > NumLats-1 )    lat -= (NumLats-1);
    if( lng > NumLngs-1 )    lng -= (NumLngs-1);
    return &Pts[ NumLngs*lat + lng ];
}



void
DrawPoint( struct point *p )
{
    glNormal3f( p->nx, p->ny, p->nz );
    glTexCoord2f( p->s, p->t );
    glVertex3f( p->x, p->y, p->z );
}

void
MjbSphere( float radius, int slices, int stacks )
{
    struct point top, bot;        // top, bottom points
    struct point *p;
    
    // set the globals:
    
    NumLngs = slices;
    NumLats = stacks;
    
    if( NumLngs < 3 )
        NumLngs = 3;
    
    if( NumLats < 3 )
        NumLats = 3;
    
    
    // allocate the point data structure:
    
    Pts = new struct point[ NumLngs * NumLats ];
    
    
    // fill the Pts structure:
    
    for( int ilat = 0; ilat < NumLats; ilat++ )
    {
        float lat = -M_PI/2.  +  M_PI * (float)ilat / (float)(NumLats-1);
        float xz = cos( lat );
        float y = sin( lat );
        for( int ilng = 0; ilng < NumLngs; ilng++ )
        {
            float lng = -M_PI  +  2. * M_PI * (float)ilng / (float)(NumLngs-1);
            float x =  xz * cos( lng );
            float z = -xz * sin( lng );
            p = PtsPointer( ilat, ilng );
            p->x  = radius * x;
            p->y  = radius * y;
            p->z  = radius * z;
            p->nx = x;
            p->ny = y;
            p->nz = z;
            if( Distort )
            {
                //p->s = ((lng + M_PI) / sin(tex_anima*(2.*M_PI)));
                p->s = (lng + M_PI)/sin(2*tex_anima);
                p->t = ((lat + M_PI / 2.) / cos(tex_anima * (M_PI*2.)));
            }
            else
            {
                p->s = ( lng + M_PI    ) / ( 2.*M_PI );
                p->t = ( lat + M_PI/2. ) / M_PI;
            }
        }
    }
    
    top.x =  0.;        top.y  = radius;    top.z = 0.;
    top.nx = 0.;        top.ny = 1.;        top.nz = 0.;
    top.s  = 0.;        top.t  = 1.;
    
    bot.x =  0.;        bot.y  = -radius;    bot.z = 0.;
    bot.nx = 0.;        bot.ny = -1.;        bot.nz = 0.;
    bot.s  = 0.;        bot.t  =  0.;
    
    
    // connect the north pole to the latitude NumLats-2:
    
    glBegin( GL_QUADS );
    for( int ilng = 0; ilng < NumLngs-1; ilng++ )
    {
        p = PtsPointer( NumLats-1, ilng );
        DrawPoint( p );
        
        p = PtsPointer( NumLats-2, ilng );
        DrawPoint( p );
        
        p = PtsPointer( NumLats-2, ilng+1 );
        DrawPoint( p );
        
        p = PtsPointer( NumLats-1, ilng+1 );
        DrawPoint( p );
    }
    glEnd( );
    
    // connect the south pole to the latitude 1:
    
    glBegin( GL_QUADS );
    for( int ilng = 0; ilng < NumLngs-1; ilng++ )
    {
        p = PtsPointer( 0, ilng );
        DrawPoint( p );
        
        p = PtsPointer( 0, ilng+1 );
        DrawPoint( p );
        
        p = PtsPointer( 1, ilng+1 );
        DrawPoint( p );
        
        p = PtsPointer( 1, ilng );
        DrawPoint( p );
    }
    glEnd( );
    
    
    // connect the other 4-sided polygons:
    
    glBegin( GL_QUADS );
    for( int ilat = 2; ilat < NumLats-1; ilat++ )
    {
        for( int ilng = 0; ilng < NumLngs-1; ilng++ )
        {
            p = PtsPointer( ilat-1, ilng );
            DrawPoint( p );
            
            p = PtsPointer( ilat-1, ilng+1 );
            DrawPoint( p );
            
            p = PtsPointer( ilat, ilng+1 );
            DrawPoint( p );
            
            p = PtsPointer( ilat, ilng );
            DrawPoint( p );
        }
    }
    glEnd( );
    
    delete [ ] Pts;
    Pts = NULL;
}




// draw the complete scene:
float Grey[ ] = { 0.3, 0.3, 0.3, 1.};
float White[ ] = { 1.,1.,1.,1. };
float Red[ ] = { 1.,0.,0.,1. };
float Green[ ] = { 0.,1.,0.,1. };
float Blue[ ] = { 0.,0.,1.,1. };
float Yellow[]={ 0.,1.,1.,1. };

// utility to create an array from 3 separate values:
float *
Array3( float a, float b, float c )
{
    static float array[4];
    array[0] = a;
    array[1] = b;
    array[2] = c;
    array[3] = 1.;
    return array;
}
// utility to create an array from a multiplier and an array:
float *
MulArray3( float factor, float array0[3] )
{
    static float array[4];
    array[0] = factor * array0[0];
    array[1] = factor * array0[1];
    array[2] = factor * array0[2];
    array[3] = 1.;
    return array;
}


void
SetSpotLight( int ilight, float x, float y, float z, float xdir, float ydir, float zdir, float r, float g, float b )
{
    glLightfv( ilight, GL_POSITION, Array3( x, y, z ) );
    glLightfv( ilight, GL_SPOT_DIRECTION, Array3(xdir,ydir,zdir) );
    glLightf( ilight, GL_SPOT_EXPONENT, 1. );
    glLightf( ilight, GL_SPOT_CUTOFF, 45. );
    glLightfv( ilight, GL_AMBIENT, Array3( 0., 0., 0. ) );
    glLightfv( ilight, GL_DIFFUSE, Array3( r, g, b ) );
    glLightfv( ilight, GL_SPECULAR, Array3( r, g, b ) );
    glLightf ( ilight, GL_CONSTANT_ATTENUATION, 1. );
    glLightf ( ilight, GL_LINEAR_ATTENUATION, 0. );
    glLightf ( ilight, GL_QUADRATIC_ATTENUATION, 0. );
    glEnable( ilight );
}
void
SetPointLight( int ilight, float x, float y, float z, float r, float g, float b )
{
    glLightfv( ilight, GL_POSITION, Array3( x, y, z ) );
    glLightfv( ilight, GL_AMBIENT, Array3( 0., 0., 0. ) );
    glLightfv( ilight, GL_DIFFUSE, Array3( r, g, b ) );
    glLightfv( ilight, GL_SPECULAR, Array3( r, g, b ) );
    glLightf ( ilight, GL_CONSTANT_ATTENUATION, 1. );
    glLightf ( ilight, GL_LINEAR_ATTENUATION, 0. );
    glLightf ( ilight, GL_QUADRATIC_ATTENUATION, 0. );
    glEnable( ilight );
}


int    ReadInt( FILE * );
short    ReadShort( FILE * );


struct bmfh
{
    short bfType;
    int bfSize;
    short bfReserved1;
    short bfReserved2;
    int bfOffBits;
} FileHeader;

struct bmih
{
    int biSize;
    int biWidth;
    int biHeight;
    short biPlanes;
    short biBitCount;
    int biCompression;
    int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    int biClrUsed;
    int biClrImportant;
} InfoHeader;

const int birgb = { 0 };



/**
 ** read a BMP file into a Texture:
 **/

unsigned char *
BmpToTexture( char *filename, int *width, int *height )
{
    
    int s, t, e;        // counters
    int numextra;        // # extra bytes each line in the file is padded with
    FILE *fp;
    unsigned char *texture;
    int nums, numt;
    unsigned char *tp;
    
    
    fp = fopen( filename, "rb" );
    if( fp == NULL )
    {
        fprintf( stderr, "Cannot open Bmp file '%s'\n", filename );
        return NULL;
    }
    
    FileHeader.bfType = ReadShort( fp );
    
    
    // if bfType is not 0x4d42, the file is not a bmp:
    
    if( FileHeader.bfType != 0x4d42 )
    {
        fprintf( stderr, "Wrong type of file: 0x%0x\n", FileHeader.bfType );
        fclose( fp );
        return NULL;
    }
    
    
    FileHeader.bfSize = ReadInt( fp );
    FileHeader.bfReserved1 = ReadShort( fp );
    FileHeader.bfReserved2 = ReadShort( fp );
    FileHeader.bfOffBits = ReadInt( fp );
    
    
    InfoHeader.biSize = ReadInt( fp );
    InfoHeader.biWidth = ReadInt( fp );
    InfoHeader.biHeight = ReadInt( fp );
    
    nums = InfoHeader.biWidth;
    numt = InfoHeader.biHeight;
    
    InfoHeader.biPlanes = ReadShort( fp );
    InfoHeader.biBitCount = ReadShort( fp );
    InfoHeader.biCompression = ReadInt( fp );
    InfoHeader.biSizeImage = ReadInt( fp );
    InfoHeader.biXPelsPerMeter = ReadInt( fp );
    InfoHeader.biYPelsPerMeter = ReadInt( fp );
    InfoHeader.biClrUsed = ReadInt( fp );
    InfoHeader.biClrImportant = ReadInt( fp );
    
    
    // fprintf( stderr, "Image size found: %d x %d\n", ImageWidth, ImageHeight );
    
    
    texture = new unsigned char[ 3 * nums * numt ];
    if( texture == NULL )
    {
        fprintf( stderr, "Cannot allocate the texture array!\b" );
        return NULL;
    }
    
    
    // extra padding bytes:
    
    numextra =  4*(( (3*InfoHeader.biWidth)+3)/4) - 3*InfoHeader.biWidth;
    
    
    // we do not support compression:
    
    if( InfoHeader.biCompression != birgb )
    {
        fprintf( stderr, "Wrong type of image compression: %d\n", InfoHeader.biCompression );
        fclose( fp );
        return NULL;
    }
    
    
    
    rewind( fp );
    fseek( fp, 14+40, SEEK_SET );
    
    if( InfoHeader.biBitCount == 24 )
    {
        for( t = 0, tp = texture; t < numt; t++ )
        {
            for( s = 0; s < nums; s++, tp += 3 )
            {
                *(tp+2) = fgetc( fp );        // b
                *(tp+1) = fgetc( fp );        // g
                *(tp+0) = fgetc( fp );        // r
            }
            
            for( e = 0; e < numextra; e++ )
            {
                fgetc( fp );
            }
        }
    }
    
    fclose( fp );
    
    *width = nums;
    *height = numt;
    return texture;
}



int
ReadInt( FILE *fp )
{
    unsigned char b3, b2, b1, b0;
    b0 = fgetc( fp );
    b1 = fgetc( fp );
    b2 = fgetc( fp );
    b3 = fgetc( fp );
    return ( b3 << 24 )  |  ( b2 << 16 )  |  ( b1 << 8 )  |  b0;
}


short
ReadShort( FILE *fp )
{
    unsigned char b1, b0;
    b0 = fgetc( fp );
    b1 = fgetc( fp );
    return ( b1 << 8 )  |  b0;
}


// this is where one would put code that is to be called
// everytime the glut main loop has nothing to do
//
// this is typically where animation parameters are set
//
// do not call Display( ) from here -- let glutMainLoop( ) do it



// delimiters for parsing the obj file:

#define OBJDELIMS        " \t"


struct Vertex
{
    float x, y, z;
};


struct Normal
{
    float nx, ny, nz;
};


struct TextureCoord
{
    float s, t, p;
};


struct face
{
    int v, n, t;
};



void    Cross( float [3], float [3], float [3] );
char *    ReadRestOfLine( FILE * );
void    ReadObjVTN( char *, int *, int *, int * );
float    Unit( float [3] );
float    Unit( float [3], float [3] );


int
LoadObjFile( char *name )
{
    char *cmd;        // the command string
    char *str;        // argument string
    
    std::vector <struct Vertex> Vertices(10000);
    std::vector <struct Normal> Normals(10000);
    std::vector <struct TextureCoord> TextureCoords(10000);
    
    Vertices.clear();
    Normals.clear();
    TextureCoords.clear();
    
    struct Vertex sv;
    struct Normal sn;
    struct TextureCoord st;
    
    
    // open the input file:
    
    FILE *fp = fopen( name, "r" );
    if( fp == NULL )
    {
        fprintf( stderr, "Cannot open .obj file '%s'\n", name );
        return 1;
    }
    
    
    float xmin = 1.e+37f;
    float ymin = 1.e+37f;
    float zmin = 1.e+37f;
    float xmax = -xmin;
    float ymax = -ymin;
    float zmax = -zmin;
    
    glBegin( GL_TRIANGLES );
    
    for( ; ; )
    {
        char *line = ReadRestOfLine( fp );
        if( line == NULL )
            break;
        
        
        // skip this line if it is a comment:
        
        if( line[0] == '#' )
            continue;
        
        
        // skip this line if it is something we don't feel like handling today:
        
        if( line[0] == 'g' )
            continue;
        
        if( line[0] == 'm' )
            continue;
        
        if( line[0] == 's' )
            continue;
        
        if( line[0] == 'u' )
            continue;
        
        
        // get the command string:
        
        cmd = strtok( line, OBJDELIMS );
        
        
        // skip this line if it is empty:
        
        if( cmd == NULL )
            continue;
        
        
        if( strcmp( cmd, "v" )  ==  0 )
        {
            str = strtok( NULL, OBJDELIMS );
            sv.x = atof(str);
            
            str = strtok( NULL, OBJDELIMS );
            sv.y = atof(str);
            
            str = strtok( NULL, OBJDELIMS );
            sv.z = atof(str);
            
            Vertices.push_back( sv );
            
            if( sv.x < xmin )    xmin = sv.x;
            if( sv.x > xmax )    xmax = sv.x;
            if( sv.y < ymin )    ymin = sv.y;
            if( sv.y > ymax )    ymax = sv.y;
            if( sv.z < zmin )    zmin = sv.z;
            if( sv.z > zmax )    zmax = sv.z;
            
            continue;
        }
        
        
        if( strcmp( cmd, "vn" )  ==  0 )
        {
            str = strtok( NULL, OBJDELIMS );
            sn.nx = atof( str );
            
            str = strtok( NULL, OBJDELIMS );
            sn.ny = atof( str );
            
            str = strtok( NULL, OBJDELIMS );
            sn.nz = atof( str );
            
            Normals.push_back( sn );
            
            continue;
        }
        
        
        if( strcmp( cmd, "vt" )  ==  0 )
        {
            st.s = st.t = st.p = 0.;
            
            str = strtok( NULL, OBJDELIMS );
            st.s = atof( str );
            
            str = strtok( NULL, OBJDELIMS );
            if( str != NULL )
                st.t = atof( str );
            
            str = strtok( NULL, OBJDELIMS );
            if( str != NULL )
                st.p = atof( str );
            
            TextureCoords.push_back( st );
            
            continue;
        }
        
        
        if( strcmp( cmd, "f" )  ==  0 )
        {
            struct face vertices[10];
            for( int i = 0; i < 10; i++ )
            {
                vertices[i].v = 0;
                vertices[i].n = 0;
                vertices[i].t = 0;
            }
            
            int sizev = (int)Vertices.size();
            int sizen = (int)Normals.size();
            int sizet = (int)TextureCoords.size();
            
            int numVertices = 0;
            bool valid = true;
            int vtx = 0;
            char *str;
            while( ( str = strtok( NULL, OBJDELIMS ) )  !=  NULL )
            {
                int v, n, t;
                ReadObjVTN( str, &v, &t, &n );
                
                // if v, n, or t are negative, they are wrt the end of their respective list:
                
                if( v < 0 )
                    v += ( sizev + 1 );
                
                if( n < 0 )
                    n += ( sizen + 1 );
                
                if( t < 0 )
                    t += ( sizet + 1 );
                
                
                // be sure we are not out-of-bounds (<vector> will abort):
                
                if( t > sizet )
                {
                    if( t != 0 )
                        fprintf( stderr, "Read texture coord %d, but only have %d so far\n", t, sizet );
                    t = 0;
                }
                
                if( n > sizen )
                {
                    if( n != 0 )
                        fprintf( stderr, "Read normal %d, but only have %d so far\n", n, sizen );
                    n = 0;
                }
                
                if( v > sizev )
                {
                    if( v != 0 )
                        fprintf( stderr, "Read vertex coord %d, but only have %d so far\n", v, sizev );
                    v = 0;
                    valid = false;
                }
                
                vertices[vtx].v = v;
                vertices[vtx].n = n;
                vertices[vtx].t = t;
                vtx++;
                
                if( vtx >= 10 )
                    break;
                
                numVertices++;
            }
            
            
            // if vertices are invalid, don't draw anything this time:
            
            if( ! valid )
                continue;
            
            if( numVertices < 3 )
                continue;
            
            
            // list the vertices:
            
            int numTriangles = numVertices - 2;
            
            for( int it = 0; it < numTriangles; it++ )
            {
                int vv[3];
                vv[0] = 0;
                vv[1] = it + 1;
                vv[2] = it + 2;
                
                // get the planar normal, in case vertex normals are not defined:
                
                struct Vertex *v0 = &Vertices[ vertices[ vv[0] ].v - 1 ];
                struct Vertex *v1 = &Vertices[ vertices[ vv[1] ].v - 1 ];
                struct Vertex *v2 = &Vertices[ vertices[ vv[2] ].v - 1 ];
                
                float v01[3], v02[3], norm[3];
                v01[0] = v1->x - v0->x;
                v01[1] = v1->y - v0->y;
                v01[2] = v1->z - v0->z;
                v02[0] = v2->x - v0->x;
                v02[1] = v2->y - v0->y;
                v02[2] = v2->z - v0->z;
                Cross( v01, v02, norm );
                Unit( norm, norm );
                glNormal3fv( norm );
                
                for( int vtx = 0; vtx < 3 ; vtx++ )
                {
                    if( vertices[ vv[vtx] ].t != 0 )
                    {
                        struct TextureCoord *tp = &TextureCoords[ vertices[ vv[vtx] ].t - 1 ];
                        glTexCoord2f( tp->s, tp->t );
                    }
                    
                    if( vertices[ vv[vtx] ].n != 0 )
                    {
                        struct Normal *np = &Normals[ vertices[ vv[vtx] ].n - 1 ];
                        glNormal3f( np->nx, np->ny, np->nz );
                    }
                    
                    struct Vertex *vp = &Vertices[ vertices[ vv[vtx] ].v - 1 ];
                    glVertex3f( vp->x, vp->y, vp->z );
                }
            }
            continue;
        }
        
        
        if( strcmp( cmd, "s" )  ==  0 )
        {
            continue;
        }
        
    }
    
    glEnd();
    fclose( fp );
    
    fprintf( stderr, "Obj file range: [%8.3f,%8.3f,%8.3f] -> [%8.3f,%8.3f,%8.3f]\n",
            xmin, ymin, zmin,  xmax, ymax, zmax );
    fprintf( stderr, "Obj file center = (%8.3f,%8.3f,%8.3f)\n",
            (xmin+xmax)/2., (ymin+ymax)/2., (zmin+zmax)/2. );
    fprintf( stderr, "Obj file  span = (%8.3f,%8.3f,%8.3f)\n",
            xmax-xmin, ymax-ymin, zmax-zmin );
    
    return 0;
}



void
Cross( float v1[3], float v2[3], float vout[3] )
{
    float tmp[3];
    
    tmp[0] = v1[1]*v2[2] - v2[1]*v1[2];
    tmp[1] = v2[0]*v1[2] - v1[0]*v2[2];
    tmp[2] = v1[0]*v2[1] - v2[0]*v1[1];
    
    vout[0] = tmp[0];
    vout[1] = tmp[1];
    vout[2] = tmp[2];
}



float
Unit( float v[3] )
{
    float dist;
    
    dist = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
    
    if( dist > 0.0 )
    {
        dist = sqrt( dist );
        v[0] /= dist;
        v[1] /= dist;
        v[2] /= dist;
    }
    
    return dist;
}



float
Unit( float vin[3], float vout[3] )
{
    float dist;
    
    dist = vin[0]*vin[0] + vin[1]*vin[1] + vin[2]*vin[2];
    
    if( dist > 0.0 )
    {
        dist = sqrt( dist );
        vout[0] = vin[0] / dist;
        vout[1] = vin[1] / dist;
        vout[2] = vin[2] / dist;
    }
    else
    {
        vout[0] = vin[0];
        vout[1] = vin[1];
        vout[2] = vin[2];
    }
    
    return dist;
}


char *
ReadRestOfLine( FILE *fp )
{
    static char *line;
    std::vector<char> tmp(1000);
    tmp.clear();
    
    for( ; ; )
    {
        int c = getc( fp );
        
        if( c == EOF  &&  tmp.size() == 0 )
        {
            return NULL;
        }
        
        if( c == EOF  ||  c == '\n' )
        {
            delete [] line;
            line = new char [ tmp.size()+1 ];
            for( int i = 0; i < (int)tmp.size(); i++ )
            {
                line[i] = tmp[i];
            }
            line[ tmp.size() ] = '\0';    // terminating null
            return line;
        }
        else
        {
            tmp.push_back( c );
        }
    }
    
    return "";
}


void
ReadObjVTN( char *str, int *v, int *t, int *n )
{
    // can be one of v, v//n, v/t, v/t/n:
    
    if( strstr( str, "//") )                // v//n
    {
        *t = 0;
        sscanf( str, "%d//%d", v, n );
        return;
    }
    else if( sscanf( str, "%d/%d/%d", v, t, n ) == 3 )    // v/t/n
    {
        return;
    }
    else
    {
        *n = 0;
        if( sscanf( str, "%d/%d", v, t ) == 2 )        // v/t
        {
            return;
        }
        else                        // v
        {
            *n = *t = 0;
            sscanf( str, "%d", v );
        }
    }
}






void
Animate( )
{
    
    
    // put animation stuff in here -- change some global variables
    // for Display( ) to find:
    
    // force a call to Display( ) next time it is convenient:
    
    int ms = glutGet( GLUT_ELAPSED_TIME );
    ms %= MS_PER_CYCLE;
    Time = (float)ms / (float)MS_PER_CYCLE;        // [0.,1.)
    tex_anima = cos(Time*3.1415926);
    
    
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


// draw the complete scene:

void
Display( )
{
    
    if( DebugOn != 0 )
    {
        fprintf( stderr, "Display\n" );
    }
    
    
    // set which window we want to do the graphics into:
    
    glutSetWindow( MainWindow );
    
    
    // erase the background:
    
    glDrawBuffer( GL_BACK );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable( GL_DEPTH_TEST );
    
    
    // specify shading to be flat:
    
    glShadeModel( GL_FLAT );
    
    
    // set the viewport to a square centered in the window:
    
    GLsizei vx = glutGet( GLUT_WINDOW_WIDTH );
    GLsizei vy = glutGet( GLUT_WINDOW_HEIGHT );
    GLsizei v = vx < vy ? vx : vy;            // minimum dimension
    GLint xl = ( vx - v ) / 2;
    GLint yb = ( vy - v ) / 2;
    glViewport( xl, yb,  v, v );
    
    
    // set the viewing volume:
    // remember that the Z clipping  values are actually
    // given as DISTANCES IN FRONT OF THE EYE
    // USE gluOrtho2D( ) IF YOU ARE DOING 2D !
    
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    if( WhichProjection == ORTHO )
        glOrtho( -3., 3.,     -3., 3.,     0.1, 1000. );
    else
        gluPerspective( 90., 1.,    0.1, 1000. );
    
    
    // place the objects into the scene:
    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
    
    
    // set the eye position, look-at position, and up-vector:
    
    gluLookAt( 4., 8.,8.,     1., 1., 1.,     0., 1., 0. );
    glLightfv( GL_LIGHT0, GL_POSITION, Array3(5, 6,5) );
    
    
    // rotate the scene:
    
    glRotatef( (GLfloat)Yrot, 0., 1., 0. );
    glRotatef( (GLfloat)Xrot, 1., 0., 0. );
    
    
    // uniformly scale the scene:
    
    if( Scale < MINSCALE )
        Scale = MINSCALE;
    glScalef( (GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale );
    
    
    // set the fog parameters:
    
    if( DepthCueOn != 0 )
    {
        glFogi( GL_FOG_MODE, FOGMODE );
        glFogfv( GL_FOG_COLOR, FOGCOLOR );
        glFogf( GL_FOG_DENSITY, FOGDENSITY );
        glFogf( GL_FOG_START, FOGSTART );
        glFogf( GL_FOG_END, FOGEND );
        glEnable( GL_FOG );
    }
    else
    {
        glDisable( GL_FOG );
    }
    
    
    // possibly draw the axes:
    
    if( AxesOn != 0 )
    {
        glColor3fv( &Colors[WhichColor][0] );
        glCallList( AxesList );
    }
    
    
    // since we are using glScalef( ), be sure normals get unitized:
    
    
    
    
    glEnable( GL_NORMALIZE );
   
    glLightfv( GL_LIGHT7, GL_POSITION, Array3(0, 0,-5) );
    //SetSpotLight(GL_LIGHT1, 4.55, 5.65, 5.6, 5.5, 6.6, 6.55, 0.356, 0.709, 0.925);
    SetSpotLight(GL_LIGHT1, 4.5, 4.59, 5.6,   0., 0., 0.,   0.356, 0.709, 0.925);
    SetSpotLight(GL_LIGHT2, 8.0, 4.63, 10.39,   0., 0., 0.,   0.356, 0.709, 0.925);
    SetSpotLight(GL_LIGHT3, 8.7, 6.43, 10.,   4.7, 4.43, 6.,   1., 0., 0.);
    glLightfv(GL_LIGHT3, GL_POSITION, Array3(0., 6.43 * cos(Time*360 ), 8. * cos(Time*360)));
    SetSpotLight(GL_LIGHT4, 7.2, 6.35, 7.65,   5.2, 4.35, 5.65,   1., 0., 0.);
    glLightfv(GL_LIGHT4, GL_POSITION, Array3(0., 6.35* cos(Time*360 ), 7.65 * cos(Time*360)));
    SetSpotLight(GL_LIGHT5, 2.8,3.89,3.85,   0., 0., 0.,   1., 0., 0.);
    SetSpotLight(GL_LIGHT6, 2.8, 2.83, 2.69,   0., 0., 0.,   1., 0., 0.);
    //SetPointLight(GL_LIGHT1,  4.55, 5.65, 5.6, 0.356, 0.709, 0.925);
   // SetPointLight(GL_LIGHT1, 0, 0, 40*cos(Time)-10, 1, 1, 0);
//     SetPointLight(GL_LIGHT2,  5.0, 5.63, 5.39, 0.356, 0.709, 0.925);
//    SetPointLight(GL_LIGHT3,  0.,2.8,4.6, 0.356, 0.709, 0.925);
//    SetPointLight(GL_LIGHT4,  0, 2.6, 4.6, 0.356, 0.709, 0.925);
//    SetPointLight(GL_LIGHT5,  2.8,3.89,3.85, 0.356, 0.709, 0.925);
//    SetPointLight(GL_LIGHT6,  2.8, 2.83, 2.69, 0.356, 0.709, 0.925);
//
    
    
    glEnable(GL_LIGHTING);
    if (LIGHT0On) {

        glEnable( GL_LIGHT0 );
    }else{
        glDisable(GL_LIGHT0);

    }
    if( LIGHT1On )
        glEnable( GL_LIGHT1 );
    else
        glDisable( GL_LIGHT1 );
    if( LIGHT2On )
        glEnable( GL_LIGHT2 );
    else
        glDisable( GL_LIGHT2 );
    if( LIGHT3On )
        glEnable( GL_LIGHT3 );
    else
        glDisable( GL_LIGHT3 );
    if( LIGHT4On )
        glEnable( GL_LIGHT4 );
    else
        glDisable( GL_LIGHT4 );
    if( LIGHT5On )
        glEnable( GL_LIGHT5 );
    else
        glDisable( GL_LIGHT5 );
    if( LIGHT6On )
        glEnable( GL_LIGHT6 );
    else
        glDisable( GL_LIGHT6 );
    if( LIGHT9On )
        glEnable( GL_LIGHT7 );
    else
        glDisable( GL_LIGHT7 );

    unsigned char *
    BmpToTexture( char *filename, int *width, int *height );
    int ear_width ;
    int ear_height;
    unsigned char *Textures;
    ear_width = 1953;
    ear_height = 1263;

    Textures =
    BmpToTexture("/Users/yin/Desktop/GPU Open GL/FinalProject/FinalProject/obj/deathstar/texture.bmp", &ear_width, &ear_height);
    
//    ear_width = 1536;
//    ear_height = 960;
//    Textures =
//    BmpToTexture("/Users/yin/Desktop/GPU Open GL/FinalProject/FinalProject/stars.bmp", &ear_width, &ear_height);

    
    glPushMatrix();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &tex1);
    glBindTexture(GL_TEXTURE_2D, tex1);


    glTexImage2D(GL_TEXTURE_2D, 0, 3, ear_width, ear_height, 0, GL_RGB, GL_UNSIGNED_BYTE, Textures);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex1);
    glRotated(-30*cos(Time*360), 0, 1, 0);
    glScalef(0.009, 0.009, 0.009);
    glRotated(60,0,1,0);
    glCallList( BoxList );
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    
  
    ear_width = 2844;
    ear_height = 2844;
    Textures =
    BmpToTexture("/Users/yin/Desktop/GPU Open GL/FinalProject/FinalProject/obj/star-wars-arc-170-pbr-obj/cbd9c-khl0z.bmp", &ear_width, &ear_height);
    glPushMatrix();
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glGenTextures(1, &tex2);
    glBindTexture(GL_TEXTURE_2D, tex2);

    
    glTexImage2D(GL_TEXTURE_2D, 0, 3, ear_width, ear_height, 0, GL_RGB, GL_UNSIGNED_BYTE, Textures);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex2);
    glTranslated(0, 3, 2*cos(Time*360));
    glTranslated(3., 3., 3.);
    glScalef(0.001, 0.001, 0.001);
    glRotated(135,0,1,0);
    glRotated(60,1,0,0);
    glRotated(60,0,0,1);
    glCallList( Arc );
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    
    
    ear_width = 2048;
    ear_height = 2048;
    Textures =
    BmpToTexture("/Users/yin/Desktop/GPU Open GL/FinalProject/FinalProject/obj/starwars-millennium-falcon-obj/cb0rw-3xz8e.bmp", &ear_width, &ear_height);
    glPushMatrix();
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &tex3);
    glBindTexture(GL_TEXTURE_2D, tex3);


    glTexImage2D(GL_TEXTURE_2D, 0, 3, ear_width, ear_height, 0, GL_RGB, GL_UNSIGNED_BYTE, Textures);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex3);
    glTranslated(0.5, 0.5*cos(Time*360), 1*cos(Time*360));
    glTranslated(5., 4., 5.);
    glScalef(0.0001, 0.0001, 0.0001);
    glRotated(-160,0,1,0);
    glRotated(30,1,0,0);
    glCallList( FalcomList );
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
//
    //glCallList( FalcomList );

    

    //falcom engeing
    glPushMatrix();
    SetMaterial(0.356, 0.709, 0.925, 1);
    glTranslated(0.5, 0.5*cos(Time*360), 1*cos(Time*360));
    glTranslated(4.7, 4.43, 5.85);
    MjbSphere(0.04, 100, 100);
    glPopMatrix();
    
    
    glPushMatrix();
    SetMaterial(0.356, 0.709, 0.925, 1);
    glTranslated(0.5, 0.5*cos(Time*360), 1*cos(Time*360));
    glTranslated(5.2, 4.35, 5.65);
    MjbSphere(0.04, 100, 100);
    glPopMatrix();
    
    
    
    ear_width = 1536;
    ear_height = 960;
    Textures =
    BmpToTexture("/Users/yin/Desktop/GPU Open GL/FinalProject/FinalProject/123.bmp", &ear_width, &ear_height);    //-z

    
    

    //ball
    glPushMatrix();
    glShadeModel( GL_SMOOTH );
    SetMaterial(1, 1, 1, 1);
    //glScalef(100, 100, 100);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glGenTextures(1, &tex0);
    glBindTexture(GL_TEXTURE_2D, tex0);
    
    glTexImage2D(GL_TEXTURE_2D, 0, 3, ear_width, ear_height, 0, GL_RGB, GL_UNSIGNED_BYTE, Textures);
    

    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    //glTexImage2D(GL_TEXTURE_2D, 0, 3, ear_width, ear_height, 0, GL_RGB, GL_UNSIGNED_BYTE, Textures);
    //glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex0);
    
    MjbSphere(13, 100, 100);
    glPopMatrix();
    
    // draw some gratuitous text that is fixed on the screen:
    //
    // the projection matrix is reset to define a scene whose
    // world coordinate system goes from 0-100 in each axis
    //
    // this is called "percent units", and is just a convenience
    //
    // the modelview matrix is reset to identity as we don't
    // want to transform these coordinates
    
    
    //glDisable(GL_LIGHTING);
    //weapon
    glPushMatrix();
    SetMaterial(1, 0, 0, 0.0001);
    
    RotateZ(&line1.p0, 3 * cosf(2*tex_anima), 0, 0, 0);
    RotateZ(&line1.p3, 3 * cosf(2*tex_anima), 0, 0, 0);
    RotateZ(&line1.p1, 3 * cosf(2*tex_anima), 0, 0, 0);
    RotateZ(&line1.p2, 3 * cosf(2*tex_anima), 0, 0, 0);
    glTranslated(.1*cos(Time*360), cos(Time*360)*.1, cos(Time*360)*.1);
   
    CreateBezierCur(line1, 12);
    glPopMatrix();
    
    glPushMatrix();
    SetMaterial(1, 0, 0, 0.0001);
    RotateZ(&line2.p0, 3 * cosf(2*tex_anima*360), 0, 0, 0);
    RotateZ(&line2.p3, 3 * cosf(2*tex_anima*360), 0, 0, 0);
    RotateZ(&line2.p1, 3 * cosf(2*tex_anima*360), 0, 0, 0);
    RotateZ(&line2.p2, 3 * cosf(2*tex_anima*360), 0, 0, 0);
    glTranslated(.1*cos(Time*360), cos(Time*360)*.1, cos(Time*360)*.1);
    CreateBezierCur(line2, 12);
    
    glPopMatrix();
    
    
    glPushMatrix();
    
    SetMaterial(1, 0, 0, 0.0001);
    glTranslated(1*cos(Time*360), cos(Time*360)*1, cos(Time*360)*1);
    glTranslated(4, 3.5, 2.5);
    CreateBezierCur(line3, 12);
    CreateBezierCur(line4, 12);
    glPopMatrix();
    
    
    
    
    
    
    
    
    
    glDisable( GL_DEPTH_TEST );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluOrtho2D( 0., 100.,     0., 100. );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
    glColor3f( 1., 1., 1. );
    DoRasterString( 5., 5., 0., "Yin Xue 933014789" );
    
    
    
    
    // swap the double-buffered framebuffers:
    
    glutSwapBuffers( );
    
    
    // be sure the graphics buffer has been sent:
    // note: be sure to use glFlush( ) here, not glFinish( ) !
    
    glFlush( );
}

void CreateBezierCur(Curve curve, GLfloat width) {
    if (LineOn) {
        glBegin(GL_LINE_STRIP);
        glColor3f(0.8, 0.8, 0.8);
        glVertex3f(curve.p0.x, curve.p0.y, curve.p0.z);
        glVertex3f(curve.p1.x, curve.p1.y, curve.p1.z);
        glVertex3f(curve.p2.x, curve.p2.y, curve.p2.z);
        glVertex3f(curve.p3.x, curve.p3.y, curve.p3.z);
        glEnd();
    }
    
    if (PointOn) {
        glPushMatrix();
        glTranslatef(curve.p0.x, curve.p0.y, curve.p0.z);
        glColor3f(1, 1, 1);
        glutSolidSphere(0.04, 50, 50);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(curve.p1.x, curve.p1.y, curve.p1.z);
        glColor3f(0.8, 0.8, 0.8);
        glutSolidSphere(0.03, 50, 50);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(curve.p2.x, curve.p2.y, curve.p2.z);
        glColor3f(0.8, 0.8, 0.8);
        glutSolidSphere(0.03, 50, 50);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(curve.p3.x, curve.p3.y, curve.p3.z);
        glColor3f(1, 1, 1);
        glutSolidSphere(0.04, 50, 50);
        glPopMatrix();
    }
    
    glLineWidth(width);
    glColor3f(curve.r, curve.g, curve.b);
    glBegin(GL_LINE_STRIP);
    for (int it = 0; it <= NUMPOINTS; it++) {
        float t = (float)it / (float)NUMPOINTS;
        float omt = 1.f - t;
        float x = omt*omt*omt*curve.p0.x + 3.f*t*omt*omt*curve.p1.x + 3.f*t*t*omt*curve.p2.x + t*t*t*curve.p3.x;
        float y = omt*omt*omt*curve.p0.y + 3.f*t*omt*omt*curve.p1.y + 3.f*t*t*omt*curve.p2.y + t*t*t*curve.p3.y;
        float z = omt*omt*omt*curve.p0.z + 3.f*t*omt*omt*curve.p1.z + 3.f*t*t*omt*curve.p2.z + t*t*t*curve.p3.z;
        glVertex3f(x, y, z);
    }
    glEnd();
    glLineWidth(1.);
}













void RotateX( Point *p, float deg, float xc, float yc, float zc )
{
    float rad = deg * (M_PI/180.f);         // radians
    float x = p->x0 - xc;
    float y = p->y0 - yc;
    float z = p->z0 - zc;
    
    float xp = x;
    float yp = y*cos(rad) - z*sin(rad);
    float zp = y*sin(rad) + z*cos(rad);
    
    p->x = xp + xc;
    p->y = yp + yc;
    p->z = zp + zc;
}

void RotateY( Point *p, float deg, float xc, float yc, float zc )
{
    float rad = deg * (M_PI/180.f);         // radians
    float x = p->x0 - xc;
    float y = p->y0 - yc;
    float z = p->z0 - zc;
    
    float xp =  x*cos(rad) + z*sin(rad);
    float yp =  y;
    float zp = -x*sin(rad) + z*cos(rad);
    
    p->x = xp + xc;
    p->y = yp + yc;
    p->z = zp + zc;
}


void RotateZ( Point *p, float deg, float xc, float yc, float zc )
{
    float rad = deg * (M_PI/180.f);         // radians
    float x = p->x0 - xc;
    float y = p->y0 - yc;
    float z = p->z0 - zc;
    
    float xp = x*cos(rad) - y*sin(rad);
    float yp = x*sin(rad) + y*cos(rad);
    float zp = z;
    
    p->x = xp + xc;
    p->y = yp + yc;
    p->z = zp + zc;
}

void
DoAxesMenu( int id )
{
    AxesOn = id;
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


void
DoColorMenu( int id )
{
    WhichColor = id - RED;
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}

void DoImgMenu(int id)
{
    ImgOn = id;
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}



void
DoDebugMenu( int id )
{
    DebugOn = id;
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}
void DoTexMenu(int id)
{
    TexOn = id;
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}

void
DoDepthMenu( int id )
{
    DepthCueOn = id;
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


// main menu callback:

void
DoMainMenu( int id )
{
    switch( id )
    {
        case RESET:
            Reset( );
            break;
            
        case QUIT:
            // gracefully close out the graphics:
            // gracefully close the graphics window:
            // gracefully exit the program:
            glutSetWindow( MainWindow );
            glFinish( );
            glutDestroyWindow( MainWindow );
            exit( 0 );
            break;
            
        default:
            fprintf( stderr, "Don't know what to do with Main Menu ID %d\n", id );
    }
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


void
DoProjectMenu( int id )
{
    WhichProjection = id;
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


// use glut to display a string of characters using a raster font:

void
DoRasterString( float x, float y, float z, char *s )
{
    glRasterPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );
    
    char c;            // one character to print
    for( ; ( c = *s ) != '\0'; s++ )
    {
        glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, c );
    }
}


// use glut to display a string of characters using a stroke font:

void
DoStrokeString( float x, float y, float z, float ht, char *s )
{
    glPushMatrix( );
    glTranslatef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
    float sf = ht / ( 119.05f + 33.33f );
    glScalef( (GLfloat)sf, (GLfloat)sf, (GLfloat)sf );
    char c;            // one character to print
    for( ; ( c = *s ) != '\0'; s++ )
    {
        glutStrokeCharacter( GLUT_STROKE_ROMAN, c );
    }
    glPopMatrix( );
}


// return the number of seconds since the start of the program:

float
ElapsedSeconds( )
{
    // get # of milliseconds since the start of the program:
    
    int ms = glutGet( GLUT_ELAPSED_TIME );
    
    // convert it to seconds:
    
    return (float)ms / 1000.f;
}


// initialize the glui window:

void
InitMenus( )
{
    glutSetWindow( MainWindow );
    
    int numColors = sizeof( Colors ) / ( 3*sizeof(int) );
    int colormenu = glutCreateMenu( DoColorMenu );
    for( int i = 0; i < numColors; i++ )
    {
        glutAddMenuEntry( ColorNames[i], i );
    }
    
    int axesmenu = glutCreateMenu( DoAxesMenu );
    glutAddMenuEntry( "Off",  0 );
    glutAddMenuEntry( "On",   1 );
    
    
    int imgmenu = glutCreateMenu( DoImgMenu );
    glutAddMenuEntry( "One",  ONE );
    glutAddMenuEntry( "Two",   TWO );
    
    int depthcuemenu = glutCreateMenu( DoDepthMenu );
    glutAddMenuEntry( "Off",  0 );
    glutAddMenuEntry( "On",   1 );
    
    int Texmenu = glutCreateMenu( DoTexMenu );
    glutAddMenuEntry( "ON",  ON );
    glutAddMenuEntry( "OFF",   OFF );
    glutAddMenuEntry("Distorted", OPEN);
    
    int debugmenu = glutCreateMenu( DoDebugMenu );
    glutAddMenuEntry( "Off",  0 );
    glutAddMenuEntry( "On",   1 );
    
    int projmenu = glutCreateMenu( DoProjectMenu );
    glutAddMenuEntry( "Orthographic",  ORTHO );
    glutAddMenuEntry( "Perspective",   PERSP );
    
    int mainmenu = glutCreateMenu( DoMainMenu );
    glutAddSubMenu(   "Axes",          axesmenu);
    glutAddSubMenu(   "Colors",        colormenu);
    glutAddSubMenu(   "Tex" ,          Texmenu);
    glutAddSubMenu(   "Img" ,          imgmenu);
    glutAddSubMenu(   "Depth Cue",     depthcuemenu);
    glutAddSubMenu(   "Projection",    projmenu );
    glutAddMenuEntry( "Reset",         RESET );
    glutAddSubMenu(   "Debug",         debugmenu);
    glutAddMenuEntry( "Quit",          QUIT );
    
    // attach the pop-up menu to the right mouse button:
    
    glutAttachMenu( GLUT_RIGHT_BUTTON );
}



// initialize the glut and OpenGL libraries:
//    also setup display lists and callback functions

void
InitGraphics( )
{
    // request the display modes:
    // ask for red-green-blue-alpha color, double-buffering, and z-buffering:
    
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    
    // set the initial window configuration:
    
    glutInitWindowPosition( 0, 0 );
    glutInitWindowSize( INIT_WINDOW_SIZE, INIT_WINDOW_SIZE );
    
    // open the window and set its title:
    
    MainWindow = glutCreateWindow( WINDOWTITLE );
    glutSetWindowTitle( WINDOWTITLE );
    
    // set the framebuffer clear values:
    
    glClearColor( BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3] );
    
    // setup the callback functions:
    // DisplayFunc -- redraw the window
    // ReshapeFunc -- handle the user resizing the window
    // KeyboardFunc -- handle a keyboard input
    // MouseFunc -- handle the mouse button going down or up
    // MotionFunc -- handle the mouse moving with a button down
    // PassiveMotionFunc -- handle the mouse moving with a button up
    // VisibilityFunc -- handle a change in window visibility
    // EntryFunc    -- handle the cursor entering or leaving the window
    // SpecialFunc -- handle special keys on the keyboard
    // SpaceballMotionFunc -- handle spaceball translation
    // SpaceballRotateFunc -- handle spaceball rotation
    // SpaceballButtonFunc -- handle spaceball button hits
    // ButtonBoxFunc -- handle button box hits
    // DialsFunc -- handle dial rotations
    // TabletMotionFunc -- handle digitizing tablet motion
    // TabletButtonFunc -- handle digitizing tablet button hits
    // MenuStateFunc -- declare when a pop-up menu is in use
    // TimerFunc -- trigger something to happen a certain time from now
    // IdleFunc -- what to do when nothing else is going on
    
    glutSetWindow( MainWindow );
    glutDisplayFunc( Display );
    glutReshapeFunc( Resize );
    glutKeyboardFunc( Keyboard );
    glutMouseFunc( MouseButton );
    glutMotionFunc( MouseMotion );
    glutPassiveMotionFunc( NULL );
    glutVisibilityFunc( Visibility );
    glutEntryFunc( NULL );
    glutSpecialFunc( NULL );
    glutSpaceballMotionFunc( NULL );
    glutSpaceballRotateFunc( NULL );
    glutSpaceballButtonFunc( NULL );
    glutButtonBoxFunc( NULL );
    glutDialsFunc( NULL );
    glutTabletMotionFunc( NULL );
    glutTabletButtonFunc( NULL );
    glutMenuStateFunc( NULL );
    glutTimerFunc( -1, NULL, 0 );
    glutIdleFunc(Animate) ;
    
    // init glew (a window must be open to do this):
    
    
    
#ifdef WIN32
    GLenum err = glewInit( );
    if( err != GLEW_OK )
    {
        fprintf( stderr, "glewInit Error\n" );
    }
    else
        fprintf( stderr, "GLEW initialized OK\n" );
    fprintf( stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif
    
}

void
SetMaterial( float r, float g, float b, float shininess )
{
    glMaterialfv( GL_BACK, GL_EMISSION, Array3( 0., 0., 0. ) );
    glMaterialfv( GL_BACK, GL_AMBIENT, MulArray3( .4f, White ) );
    glMaterialfv( GL_BACK, GL_DIFFUSE, MulArray3( 1., White ) );
    glMaterialfv( GL_BACK, GL_SPECULAR, Array3( 0., 0., 0. ) );
    glMaterialf ( GL_BACK, GL_SHININESS, 2.f );
    glMaterialfv( GL_FRONT, GL_EMISSION, Array3( 0., 0., 0. ) );
    glMaterialfv( GL_FRONT, GL_AMBIENT, Array3( r, g, b ) );
    glMaterialfv( GL_FRONT, GL_DIFFUSE, Array3( r, g, b ) );
    glMaterialfv( GL_FRONT, GL_SPECULAR, MulArray3( .8f, White ) );
    glMaterialf ( GL_FRONT, GL_SHININESS, shininess );
}




// initialize the display lists that will not change:
// (a display list is a way to store opengl commands in
//  memory so that they can be played back efficiently at a later time
//  with a call to glCallList( )

void
InitLists( )
{
    float dx = BOXSIZE / 2.f;
    float dy = BOXSIZE / 2.f;
    float dz = BOXSIZE / 2.f;
    glutSetWindow( MainWindow );
    
    // create the object:
    
    BoxList = glGenLists( 1 );
    glNewList( BoxList, GL_COMPILE );
    SetMaterial(1, 1, 1, 66);
   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glShadeModel( GL_SMOOTH );
    //load ojb files
    
   
    LoadObjFile( "/Users/yin/Desktop/GPU Open GL/FinalProject/FinalProject/obj/deathstar/death-star-II.obj"
                );

    glEndList( );
    
    Arc = glGenLists( 1 );
    glNewList( Arc, GL_COMPILE );
    SetMaterial(1, 1, 1, 66);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glShadeModel( GL_SMOOTH );
    //load ojb files
   
    LoadObjFile( "/Users/yin/Desktop/GPU Open GL/FinalProject/FinalProject/obj/star-wars-arc-170-pbr-obj/star-wars-arc-170-pbr.obj"
                );
    
    glEndList( );
//
    
    FalcomList = glGenLists( 1 );
    glNewList( FalcomList, GL_COMPILE );
    SetMaterial(1, 1, 1, 66);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glShadeModel( GL_SMOOTH );
    //load ojb files
    
    LoadObjFile( "/Users/yin/Desktop/GPU Open GL/FinalProject/FinalProject/obj/starwars-millennium-falcon-obj/starwars-millennium-falcon.obj"
                );
    glEndList( );

    
    
    line1.color(1.,0.,0.);
    line1.p0.setPt(3.5, 4.59, 4.55);
    line1.p1.setPt(3.2, 4.29, 4.25);
    line1.p2.setPt(3., 4.09, 4.05);
    line1.p3.setPt(2.8,3.89,3.85);
    line1.reset();
    
    line2.color(1.,0.,0.);
    line2.p0.setPt(3.5, 3.63, 3.39);
    line2.p1.setPt(3.3, 3.43,3.19);
    line2.p2.setPt(3, 3.03, 2.89 );
    line2.p3.setPt(2.8, 2.83, 2.69);
    line2.reset();
    
    
    
    line3.color(1.,0.,0.);
    line3.p0.setPt(0., 2.1, 3.9);
    line3.p1.setPt(0., 2.0, 3.8);
    line3.p2.setPt(0., 1.9, 3.7);
    line3.p3.setPt(0.,1.8,3.6);
    line3.reset();
    
    line4.color(1.,0.,0.);
   
    line4.p0.setPt(0., 1.9, 3.9);
    line4.p1.setPt(0, 1.8,3.8);
    line4.p2.setPt(0, 1.7, 3.7 );
    line4.p3.setPt(0, 1.6, 3.6);
    line4.reset();
    
    // create the axes:
    
    AxesList = glGenLists( 1 );
    glNewList( AxesList, GL_COMPILE );
    glLineWidth( AXES_WIDTH );
    Axes( 1.5 );
    glLineWidth( 1. );
    glEndList( );
}




// the keyboard callback:

void
Keyboard( unsigned char c, int x, int y )
{
    if( DebugOn != 0 )
        fprintf( stderr, "Keyboard: '%c' (0x%0x)\n", c, c );
    
    switch( c )
    {
        case 'o':
        case 'O':
            WhichProjection = ORTHO;
            break;
            
        case 'p':
        case 'P':
            WhichProjection = PERSP;
            break;
            
        case 'q':
        case 'Q':
        case ESCAPE:
            DoMainMenu( QUIT );    // will not return here
            break;                // happy compiler
        case 'f':
        case 'F':
            FREEZE = ! FREEZE;
            if( FREEZE )
                glutIdleFunc( NULL );
            else
                glutIdleFunc( Animate );
            break;
            
            //sun
        case '0':
            LIGHT0On = ! LIGHT0On;
            break;
            //light
        case '1':
            LIGHT1On = ! LIGHT1On;
            break;
            //light
        case '2':
            LIGHT2On = ! LIGHT2On;
            break;
        case '3':
            LIGHT3On = ! LIGHT3On;
            break;
            
        case '4':
            LIGHT4On = ! LIGHT4On;
            break;
        case '5':
             LIGHT5On = ! LIGHT5On;
            break;
            
        case '6':
             LIGHT6On = ! LIGHT6On;
            break;
        case '7':
            LineOn = ! LineOn;
            break;
            
        case '8':
            PointOn = ! PointOn;
            break;
        case '9':
            LIGHT9On = ! LIGHT9On;
            break;
            
            
        default:
            fprintf( stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c );
    }
    
    // force a call to Display( ):
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


// called when the mouse button transitions down or up:

void
MouseButton( int button, int state, int x, int y )
{
    int b = 0;            // LEFT, MIDDLE, or RIGHT
    
    if( DebugOn != 0 )
        fprintf( stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y );
    
    
    // get the proper button bit mask:
    
    switch( button )
    {
        case GLUT_LEFT_BUTTON:
            b = LEFT;        break;
            
        case GLUT_MIDDLE_BUTTON:
            b = MIDDLE;        break;
            
        case GLUT_RIGHT_BUTTON:
            b = RIGHT;        break;
            
        default:
            b = 0;
            fprintf( stderr, "Unknown mouse button: %d\n", button );
    }
    
    
    // button down sets the bit, up clears the bit:
    
    if( state == GLUT_DOWN )
    {
        Xmouse = x;
        Ymouse = y;
        ActiveButton |= b;        // set the proper bit
    }
    else
    {
        ActiveButton &= ~b;        // clear the proper bit
    }
}


// called when the mouse moves while a button is down:

void
MouseMotion( int x, int y )
{
    if( DebugOn != 0 )
        fprintf( stderr, "MouseMotion: %d, %d\n", x, y );
    
    
    int dx = x - Xmouse;        // change in mouse coords
    int dy = y - Ymouse;
    
    if( ( ActiveButton & LEFT ) != 0 )
    {
        Xrot += ( ANGFACT*dy );
        Yrot += ( ANGFACT*dx );
    }
    
    
    if( ( ActiveButton & MIDDLE ) != 0 )
    {
        Scale += SCLFACT * (float) ( dx - dy );
        
        // keep object from turning inside-out or disappearing:
        
        if( Scale < MINSCALE )
            Scale = MINSCALE;
    }
    
    Xmouse = x;            // new current position
    Ymouse = y;
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


// reset the transformations and the colors:
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene

void
Reset( )
{
    ActiveButton = 0;
    AxesOn = 1;
    DebugOn = 0;
    TexOn = OFF;
    DepthCueOn = 0;
    Scale  = 1.0;
    WhichColor = WHITE;
    WhichProjection = PERSP;
    Xrot = Yrot = 0.;
    Distort = false;
}


// called when user resizes the window:

void
Resize( int width, int height )
{
    if( DebugOn != 0 )
        fprintf( stderr, "ReSize: %d, %d\n", width, height );
    
    // don't really need to do anything since window size is
    // checked each time in Display( ):
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


// handle a change to the window's visibility:

void
Visibility ( int state )
{
    if( DebugOn != 0 )
        fprintf( stderr, "Visibility: %d\n", state );
    
    if( state == GLUT_VISIBLE )
    {
        glutSetWindow( MainWindow );
        glutPostRedisplay( );
    }
    else
    {
        // could optimize by keeping track of the fact
        // that the window is not visible and avoid
        // animating or redrawing it ...
    }
}



///////////////////////////////////////   HANDY UTILITIES:  //////////////////////////


// the stroke characters 'X' 'Y' 'Z' :

static float xx[ ] = {
    0.f, 1.f, 0.f, 1.f
};

static float xy[ ] = {
    -.5f, .5f, .5f, -.5f
};

static int xorder[ ] = {
    1, 2, -3, 4
};

static float yx[ ] = {
    0.f, 0.f, -.5f, .5f
};

static float yy[ ] = {
    0.f, .6f, 1.f, 1.f
};

static int yorder[ ] = {
    1, 2, 3, -2, 4
};

static float zx[ ] = {
    1.f, 0.f, 1.f, 0.f, .25f, .75f
};

static float zy[ ] = {
    .5f, .5f, -.5f, -.5f, 0.f, 0.f
};

static int zorder[ ] = {
    1, 2, 3, 4, -5, 6
};

// fraction of the length to use as height of the characters:
const float LENFRAC = 0.10f;

// fraction of length to use as start location of the characters:
const float BASEFRAC = 1.10f;

//    Draw a set of 3D axes:
//    (length is the axis length in world coordinates)

void
Axes( float length )
{
    glBegin( GL_LINE_STRIP );
    glVertex3f( length, 0., 0. );
    glVertex3f( 0., 0., 0. );
    glVertex3f( 0., length, 0. );
    glEnd( );
    glBegin( GL_LINE_STRIP );
    glVertex3f( 0., 0., 0. );
    glVertex3f( 0., 0., length );
    glEnd( );
    
    float fact = LENFRAC * length;
    float base = BASEFRAC * length;
    
    glBegin( GL_LINE_STRIP );
    for( int i = 0; i < 4; i++ )
    {
        int j = xorder[i];
        if( j < 0 )
        {
            
            glEnd( );
            glBegin( GL_LINE_STRIP );
            j = -j;
        }
        j--;
        glVertex3f( base + fact*xx[j], fact*xy[j], 0.0 );
    }
    glEnd( );
    
    glBegin( GL_LINE_STRIP );
    for( int i = 0; i < 5; i++ )
    {
        int j = yorder[i];
        if( j < 0 )
        {
            
            glEnd( );
            glBegin( GL_LINE_STRIP );
            j = -j;
        }
        j--;
        glVertex3f( fact*yx[j], base + fact*yy[j], 0.0 );
    }
    glEnd( );
    
    glBegin( GL_LINE_STRIP );
    for( int i = 0; i < 6; i++ )
    {
        int j = zorder[i];
        if( j < 0 )
        {
            
            glEnd( );
            glBegin( GL_LINE_STRIP );
            j = -j;
        }
        j--;
        glVertex3f( 0.0, fact*zy[j], base + fact*zx[j] );
    }
    glEnd( );
    
}


// function to convert HSV to RGB
// 0.  <=  s, v, r, g, b  <=  1.
// 0.  <= h  <=  360.
// when this returns, call:
//        glColor3fv( rgb );

void
HsvRgb( float hsv[3], float rgb[3] )
{
    // guarantee valid input:
    
    float h = hsv[0] / 60.f;
    while( h >= 6. )    h -= 6.;
    while( h <  0. )     h += 6.;
    
    float s = hsv[1];
    if( s < 0. )
        s = 0.;
    if( s > 1. )
        s = 1.;
    
    float v = hsv[2];
    if( v < 0. )
        v = 0.;
    if( v > 1. )
        v = 1.;
    
    // if sat==0, then is a gray:
    
    if( s == 0.0 )
    {
        rgb[0] = rgb[1] = rgb[2] = v;
        return;
    }
    
    // get an rgb from the hue itself:
    
    float i = floor( h );
    float f = h - i;
    float p = v * ( 1.f - s );
    float q = v * ( 1.f - s*f );
    float t = v * ( 1.f - ( s * (1.f-f) ) );
    
    float r, g, b;            // red, green, blue
    switch( (int) i )
    {
        case 0:
            r = v;    g = t;    b = p;
            break;
            
        case 1:
            r = q;    g = v;    b = p;
            break;
            
        case 2:
            r = p;    g = v;    b = t;
            break;
            
        case 3:
            r = p;    g = q;    b = v;
            break;
            
        case 4:
            r = t;    g = p;    b = v;
            break;
            
        case 5:
            r = v;    g = p;    b = q;
            break;
    }
    
    
    rgb[0] = r;
    rgb[1] = g;
    rgb[2] = b;
}
void loadTexture(){
    unsigned char *
    BmpToTexture( char *filename, int *width, int *height );
    int ear_width ;
    int ear_height;
    ear_width = 1024;
    ear_height = 512;
    unsigned char *Textures;
    Textures =
    BmpToTexture("/Users/yin/Desktop/GPU Open GL/pj4/PJ4/PJ4/worldtex.bmp", &ear_width, &ear_height);
    
}

