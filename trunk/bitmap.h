#pragma once
#include <GL/glut.h>

typedef struct                       /**** BMP file header structure ****/
{
	unsigned short bfType;           /* Magic number for file */
	unsigned int   bfSize;           /* Size of file */
	unsigned short bfReserved1;      /* Reserved */
	unsigned short bfReserved2;      /* ... */
	unsigned int   bfOffBits;        /* Offset to bitmap data */
} BitmapFileHeader;

typedef struct                       /**** BMP file info structure ****/
{
	unsigned int   biSize;           /* Size of info header */
	int             biWidth;          /* Width of image */
	int             biHeight;         /* Height of image */
	unsigned short biPlanes;         /* Number of color planes */
	unsigned short biBitCount;       /* Number of bits per pixel */
	unsigned int   biCompression;    /* Type of compression to use */
	unsigned int   biSizeImage;      /* Size of image data */
	int             biXPelsPerMeter;  /* X pixels per meter */
	int             biYPelsPerMeter;  /* Y pixels per meter */
	unsigned int   biClrUsed;        /* Number of colors used */
	unsigned int   biClrImportant;   /* Number of important colors */
} BitmapInfoHeader;

typedef struct                       /**** Colormap entry structure ****/
{
	unsigned char  rgbBlue;          /* Blue value */
	unsigned char  rgbGreen;         /* Green value */
	unsigned char  rgbRed;           /* Red value */
	unsigned char  rgbReserved;      /* Reserved */
} RGBQuad;

typedef struct                       /**** Bitmap information structure ****/
{
	BitmapInfoHeader bmiHeader;      /* Image header */
	RGBQuad          bmiColors[256]; /* Image colormap */
} BitmapInfo;

GLubyte * LoadDIBitmap(const char * filename, BitmapInfo **info);

#define _BFType 0x4D42             /* "MB" */
#define _BIRGB       0             /* No compression - straight BGR data */
#define _BIRLE8      1             /* 8-bit run-length compression */
#define _BIRLE4      2             /* 4-bit run-length compression */
#define _BIBitFields 3             /* RGB bitmap with RGB masks */
