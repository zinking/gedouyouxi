#include <stdlib.h>
#include "Texture.h"

GLuint							/* O - Texture object or 0 on error */
TextureLoad(const char * filename,	/* I - Bitmap file to load */
			GLboolean alpha,	/* I - Generate alpha for bitmap */
			GLenum minfilter,	/* I - Minification filter */
			GLenum magfilter,	/* I - Magnification filter */
			GLenum swrap,
			GLenum twrap)		/* I - Repeat or clamp */
{
	int         i;             /* Looping var */
	BitmapInfo	*info;          /* Bitmap information */
	GLubyte		*bits;          /* Bitmap RGB pixels */
	GLubyte     *ptr;           /* Pointer into bit buffer */
	GLubyte		*rgba;          /* RGBA pixel buffer */
    GLubyte		*rgbaptr;       /* Pointer into RGBA buffer */
	GLubyte     temp;           /* Swapping variable */
	GLenum      type;           /* Texture type */
	GLuint      texture;        /* Texture object */

	/* Try loading the bitmap file... */
	bits = LoadDIBitmap(filename, &info);
	if (bits == (GLubyte *)0)
		return (0);

	/* Figure out the type of texture... */
	if (info->bmiHeader.biHeight == 1)
		type = GL_TEXTURE_1D;
	else
		type = GL_TEXTURE_2D;

	/* Create and bind a texture object */
	glGenTextures(1, &texture);
	glBindTexture(type, texture);

	/* Set texture parameters */
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, magfilter);
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, minfilter);
	glTexParameteri(type, GL_TEXTURE_WRAP_S, swrap);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, twrap);
	glTexEnvi(type, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	if (alpha) {
		/* Create and use an RGBA image... */
		rgba = (GLubyte *)malloc(info->bmiHeader.biWidth * info->bmiHeader.biHeight * 4);

		for (i = info->bmiHeader.biWidth * info->bmiHeader.biHeight,
			 rgbaptr = rgba, ptr = bits;
			 i > 0;
			 i --, rgbaptr += 4, ptr += 3)
		{
			rgbaptr[0] = ptr[0];
			rgbaptr[1] = ptr[1];
			rgbaptr[2] = ptr[2];
			rgbaptr[3] = (ptr[0] + ptr[1] + ptr[2]) / 3;
		}

		/*
		 * Set texture image; if the minification filter uses mip-mapping
		 * then use gluBuild2D/1DMipmaps() to load the texture...
		 */
		if (minfilter == GL_LINEAR || minfilter == GL_NEAREST)
			glTexImage2D(type, 0, 4, info->bmiHeader.biWidth,
						 info->bmiHeader.biHeight, 0, GL_RGBA,
						 GL_UNSIGNED_BYTE, rgba);
		else if (type == GL_TEXTURE_1D)
			gluBuild1DMipmaps(type, 4, info->bmiHeader.biWidth,
							  GL_RGBA, GL_UNSIGNED_BYTE, rgba);
		else  
			gluBuild2DMipmaps(type, 3, info->bmiHeader.biWidth,
							  info->bmiHeader.biHeight, GL_RGBA,
							  GL_UNSIGNED_BYTE, rgba);
		/* Free the RGBA buffer */
		free(rgba);
	} else {
		/*
		 * Set texture image; if the minification filter uses mip-mapping
		 * then use gluBuild2D/1DMipmaps() to load the texture...
		 */
		if (minfilter == GL_LINEAR || minfilter == GL_NEAREST)
			glTexImage2D(type, 0, 3, info->bmiHeader.biWidth,
						 info->bmiHeader.biHeight, 0, GL_RGB,
						 GL_UNSIGNED_BYTE, bits);
		else if (type == GL_TEXTURE_1D)
			gluBuild1DMipmaps(type, 3, info->bmiHeader.biWidth,
							  GL_RGB, GL_UNSIGNED_BYTE, bits);
		else  
			gluBuild2DMipmaps(type, 3, info->bmiHeader.biWidth,
							  info->bmiHeader.biHeight, GL_RGB,
							  GL_UNSIGNED_BYTE, bits);
	}

	/* Free the bitmap and return... */
	free(info);
	free(bits);

	return (texture);
}

GLuint TextureLoad(const char * filename, GLboolean alpha,
				   GLenum minfilter, GLenum magfilter,
				   GLenum wrap)
{
	return TextureLoad(filename, alpha, minfilter, magfilter, wrap, wrap);
}