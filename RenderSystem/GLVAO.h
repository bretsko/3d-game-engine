#pragma once

#include "Common.h"

#define UV_CHANNELS 2


/**
* VAO wrapper with software fallback
**/
class GLVAO
{
    public:
  // VAO
  GLuint vao;

  // VBOs - separate
  GLuint index;
  GLuint position;
  GLuint color;
  GLuint normal;
  GLuint texuv[UV_CHANNELS];
  GLuint boneid;
  GLuint boneweight;
  GLuint tangent;
  GLuint bitangent;

  // VBOs - interleaved
  GLuint interleaved_pnt;
  GLuint interleaved_pc;
  GLuint interleaved_pc34;


  GLVAO();
  ~GLVAO();


  // Set buffer ids - separate
  void setIndex(GLuint vbo);
  void setPosition(GLuint vbo);
  void setColor(GLuint vbo);
  void setNormal(GLuint vbo);
  void setTexUV(GLuint vbo, unsigned int channel);
  void setBoneId(GLuint vbo);
  void setBoneWeight(GLuint vbo);
  void setTangent(GLuint vbo);
  void setBitangent(GLuint vbo);

  // Set buffer ids - interleaved
  void setInterleavedPNT(GLuint vbo);
  void setInterleavedPC(GLuint vbo);
  void setInterleavedPC34(GLuint vbo);

  // Get buffer id, for updating the data
  GLuint getInterleavedPNT() { return this->interleaved_pnt; }
  GLuint getInterleavedPC() { return this->interleaved_pc; }
  GLuint getInterleavedPC34() { return this->interleaved_pc34; }

  // Bind the VAO
  void bind();
  void unbind();
};

