
#pragma once

#if defined(RELEASE)
 #define CHECK_OPENGL_ERROR

#else
 #define CHECK_OPENGL_ERROR \
 { GLenum error; \
  error = glGetError(); \
  if (error != GL_NO_ERROR) { \
   char buf[255]; \
   sprintf(buf, "OpenGL error: %s at line %i of %s", (const char*) gluErrorString(error), __LINE__, __FILE__); \
  } \
 }

#endif


//For GL logging and errors
 #define GL_LOG(msg, ...) fprintf(stderr, "GL Log: " msg "\n", ##__VA_ARGS__)

static const char*
ErrorString( GLenum error )
{
    const char*  msg;
    switch( error ) {
#define Case( Token )  case Token: msg = #Token; break;
    Case( GL_NO_ERROR );
    Case( GL_INVALID_VALUE );
    Case( GL_INVALID_ENUM );
    Case( GL_INVALID_OPERATION );
    Case( GL_STACK_OVERFLOW );
    Case( GL_STACK_UNDERFLOW );
    Case( GL_OUT_OF_MEMORY );
#undef Case
    }

    return msg;
}



static void
_CheckError( const char* file, int line )
{
    GLenum  error = glGetError();

    do {
    fprintf( stderr, "[%s:%d] %s\n", file, line, ErrorString(error) );
    } while ((error = glGetError()) != GL_NO_ERROR );

}

#define CheckError()  _CheckError( __FILE__, __LINE__ )


