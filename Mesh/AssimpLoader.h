#pragma once

#include <assimp/cimport.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

class AssimpLoader {
public:

    //AssimpLoader (const char* path);


    //------------------ Functions ------------------------


    void load_file_simple (const char* path);

    bool load(
            const char * path,
            vector<unsigned short> & indices,
            vector<vec3> & vertices,
            vector<vec2> & uvs,
            vector<vec3> & normals
            );

    bool load_file ( const char *  pFile);

    void get_bounding_box (aiVector3D* min, aiVector3D* max);

    void get_bounding_box_for_node (const struct aiNode* nd,
                                    aiVector3D* min,
                                    aiVector3D* max,
                                    aiMatrix4x4* trafo
                                    );

    //recursively renders the scene
    void render_scene (const aiScene *sc, const aiNode* nd);
    void render_scene ();

    void set_float4(float f[4], float a, float b, float c, float d);

    void color4_to_float4(const aiColor4D *c, float f[4]);

    int printOglError(char *file, int line);

    // res = a cross b;
    void crossProduct( float *a, float *b, float *res);

    // Normalize a vec3
    void normalize(float *a);
    void pushMatrix();
    void popMatrix();
    void genVAOsAndUniformBuffer(const aiScene *sc);

    // Reshape Callback Function
    void changeSize(int w, int h);

    // Events from the Keyboard
    void processKeys(unsigned char key, int xx, int yy);

    // Mouse Events
    void processMouseButtons(int button, int state, int xx, int yy);

    // Track mouse motion while buttons are pressed
    void processMouseMotion(int xx, int yy);
    void mouseWheel(int wheel, int direction, int x, int y);

    // Shader Stuff
    void printShaderInfoLog(GLuint obj);
    void printProgramInfoLog(GLuint obj);
    GLuint setupShaders();

    // sets the square matrix mat to the identity matrix,
    // size refers to the number of rows (or columns)
    void setIdentityMatrix( float *mat, int size);

    // a = a * b;
    void multMatrix(float *a, float *b);

    // Defines a transformation matrix mat with a translation
    void setTranslationMatrix(float *mat, float x, float y, float z);

    // Defines a transformation matrix mat with a scale
    void setScaleMatrix(float *mat, float sx, float sy, float sz);

    // Defines a transformation matrix mat with a rotation
    // angle alpha and a rotation axis (x,y,z)
    void setRotationMatrix(float *mat, float angle, float x, float y, float z);

    // Defines a transformation matrix mat with a scale

    void setModelMatrix();

    // The equivalent to glTranslate applied to the model matrix
    void translate(float x, float y, float z);

    // The equivalent to glRotate applied to the model matrix
    void rotate(float angle, float x, float y, float z);

    // The equivalent to glScale applied to the model matrix
    void scale(float x, float y, float z);

    void buildProjectionMatrix(float fov, float ratio, float nearp, float farp);
    void setCamera(float posX, float posY, float posZ,
                   float lookAtX, float lookAtY, float lookAtZ);
    int loadGLTextures(const aiScene* scene);

    void reshape(int width, int height);
    void apply_material(const struct aiMaterial *mtl);


    //------------------ Properties ------------------------


    const struct aiScene* scene = NULL;
    GLuint scene_list = 0;

    //static unique_ptr<camera> create(string scene_desc, Scene *scene);
    //vector<const node *> nodes() const;
    //vector<const light *> lights() const;
private:
    //vector<unique_ptr<node> > _nodes;
    //vector<unique_ptr<light> > _lights;
};


//bool loadOBJ(
// const char * path,
// vector<vec3> & out_vertices,
// vector<vec2> & out_uvs,
// vector<vec3> & out_normals
//);
