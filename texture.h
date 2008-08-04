#include "Bitmap.h"

GLuint TextureLoad(const char * filename, GLboolean alpha,
					GLenum minfilter, GLenum magfilter,
					GLenum swrap, GLenum twrap);

GLuint TextureLoad(const char * filename, GLboolean alpha,
					GLenum minfilter, GLenum magfilter,
					GLenum wrap);
