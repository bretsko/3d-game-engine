#include "Common.h"

#include <stdio.h>
#include <cstring>
#include <math.h>
#include <fstream>
#include <map>

#include "AssimpLoader.h"


//------------------ Includes and Constants ------------------------

//namespace{

struct MyMesh{

    GLuint vao;
    GLuint texIndex;
    GLuint uniformBlockIndex;
    int numFaces;
};

vector<struct MyMesh> myMeshes;


// This is for a shader uniform block
struct MyMaterial{

    float diffuse[4];
    float ambient[4];
    float specular[4];
    float emissive[4];
    float shininess;
    int texCount;
};

// Model Matrix (part of the OpenGL Model View Matrix)
float modelMatrix[16];

// For push and pop matrix
vector<float *> matrixStack;

// Vertex Attribute Locations
GLuint vertexLoc=0, normalLoc=1, texCoordLoc=2;

// Uniform Bindings Points
GLuint matricesUniLoc = 1, materialUniLoc = 2;

// The sampler uniform for textured models
// we are assuming a single texture so this will
//always be texture unit 0
GLuint texUnit = 0;

// Uniform Buffer for Matrices
// this buffer will contain 3 matrices: projection, view and model
// each matrix is a float array with 16 components
GLuint matricesUniBuffer;

#define MatricesUniBufferSize sizeof(float) * 16 * 3
#define ProjMatrixOffset 0
#define ViewMatrixOffset sizeof(float) * 16
#define ModelMatrixOffset sizeof(float) * 16 * 2
#define MatrixSize sizeof(float) * 16

// Camera Position
float camX = 0, camY = 0, camZ = 5;

// Mouse Tracking Variables
int startX, startY, tracking = 0;

// Camera Spherical Coordinates
float alpha = 0.0f, beta = 0.0f;
float r = 5.0f;

static inline float
DegToRad(float degrees)
{
    return (float)(degrees * (M_PI / 180.0f));
};

// Frame counting and FPS computation
long timecount,timebase = 0,frame = 0;
char s[32];

// Returns 1 if an OpenGL error occurred, 0 otherwise.
#define printOpenGLError() printOglError(__FILE__, __LINE__)

// Program and Shader Identifiers
GLuint program, vertexShader, fragmentShader;

// Shader Names
char *vertexFileName = "shaders/dirlightdiffambpix.vert";
char *fragmentFileName = "shaders/dirlightdiffambpix.frag";

// Create an instance of the Importer class
Assimp::Importer importer;

// the global Assimp scene object
const aiScene* scene = NULL;

// scale factor for the model to fit in the window
float scaleFactor;


// images / texture
// map image filenames to textureIds
// pointer to texture Array
map<string, GLuint> textureIdMap;

// Replace the model name by your model's filename
static const string modelname = "resources/bench.obj";




// Very, VERY simple OBJ loader.
// Here is a short list of features a real function would provide : 
// - Binary files. Reading a model should be just a few memcpy's away, not parsing a file at runtime. In short : OBJ is not very great.
// - Animations & bones (includes bones weights)
// - Multiple UVs
// - All attributes should be optional, not "forced"
// - More stable. Change a line in the OBJ file and it crashes.
// - More secure. Change another line and you can inject code.
// - Loading from memory, stream, etc

// current rotation angle
//static float angle = 0.f;




//------------------ AssimpLoader ------------------------

//------------------ Functions ------------------------




//TODO?: const string&
bool AssimpLoader::load_file ( const char *  pFile)
{

    //check if file exists
    std::ifstream fin(pFile);
    if(!fin.fail()) {
        fin.close();
    }
    else{
        printf("Couldn't open file: %s\n", pFile);
        printf("%s\n", importer.GetErrorString());
        return false;
    }

    scene = importer.ReadFile( pFile, aiProcessPreset_TargetRealtime_Quality);

    // If the import failed, report it
    if( !scene)
    {
        printf("%s\n", importer.GetErrorString());
        return false;
    }

    // Now we can access the file's contents.
    printf("Import of scene %s succeeded.",pFile);

    aiVector3D scene_min, scene_max, scene_center;
    get_bounding_box(&scene_min, &scene_max);
    float tmp;
    tmp = scene_max.x-scene_min.x;
    tmp = scene_max.y - scene_min.y > tmp?scene_max.y - scene_min.y:tmp;
    tmp = scene_max.z - scene_min.z > tmp?scene_max.z - scene_min.z:tmp;
    scaleFactor = 1.f / tmp;

    // We're done. Everything will be cleaned up by the importer destructor
    return true;
}


bool AssimpLoader::load(
        const char * path,
        vector<unsigned short> & indices,
        vector<vec3> & vertices,
        vector<vec2> & uvs,
        vector<vec3> & normals
        ){

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(path, 0/*aiProcess_JoinIdenticalVertices | aiProcess_SortByPType*/);
    if( !scene) {
        fprintf( stderr, importer.GetErrorString());
        getchar();
        return false;
    }
    const aiMesh* mesh = scene->mMeshes[0]; // In this simple example code we always use the 1rst mesh (in OBJ files there is often only one anyway)

    // Fill vertices positions
    vertices.reserve(mesh->mNumVertices);
    for(unsigned int i=0; i<mesh->mNumVertices; i++){
        aiVector3D pos = mesh->mVertices[i];
        vertices.push_back(vec3(pos.x, pos.y, pos.z));
    }

    // Fill vertices texture coordinates
    uvs.reserve(mesh->mNumVertices);
    for(unsigned int i=0; i<mesh->mNumVertices; i++){
        aiVector3D UVW = mesh->mTextureCoords[0][i]; // Assume only 1 set of UV coords; AssImp supports 8 UV sets.
        uvs.push_back(vec2(UVW.x, UVW.y));
    }

    // Fill vertices normals
    normals.reserve(mesh->mNumVertices);
    for(unsigned int i=0; i<mesh->mNumVertices; i++){
        aiVector3D n = mesh->mNormals[i];
        normals.push_back(vec3(n.x, n.y, n.z));
    }


    // Fill face indices
    indices.reserve(3*mesh->mNumFaces);
    for (unsigned int i=0; i<mesh->mNumFaces; i++){
        // Assume the model has only triangles.
        indices.push_back(mesh->mFaces[i].mIndices[0]);
        indices.push_back(mesh->mFaces[i].mIndices[1]);
        indices.push_back(mesh->mFaces[i].mIndices[2]);
    }

    // The "scene" pointer will be deleted automatically by "importer"

}




//void AssimpLoader::get_bounding_box_for_node (const aiNode* nd,
// aiVector3D* min,
// aiVector3D* max)

//{
// aiMatrix4x4 prev;
// unsigned int n = 0, t;

// for (; n < nd->mNumMeshes; ++n) {
//  const aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
//  for (t = 0; t < mesh->mNumVertices; ++t) {

//   aiVector3D tmp = mesh->mVertices[t];

//   min->x = aisgl_min(min->x,tmp.x);
//   min->y = aisgl_min(min->y,tmp.y);
//   min->z = aisgl_min(min->z,tmp.z);

//   max->x = aisgl_max(max->x,tmp.x);
//   max->y = aisgl_max(max->y,tmp.y);
//   max->z = aisgl_max(max->z,tmp.z);
//  }
// }

// for (n = 0; n < nd->mNumChildren; ++n) {
//  get_bounding_box_for_node(nd->mChildren[n],min,max);
// }
//}

//void AssimpLoader::get_bounding_box (aiVector3D* min, aiVector3D* max)
//{
// min->x = min->y = min->z =  1e10f;
// max->x = max->y = max->z = -1e10f;
// get_bounding_box_for_node(scene->mRootNode,min,max);
//}


void AssimpLoader::get_bounding_box_for_node (const struct aiNode* nd,
                                              aiVector3D* min,
                                              aiVector3D* max,
                                              aiMatrix4x4* trafo
                                              ){
    aiMatrix4x4 prev;
    unsigned int n = 0, t;

    prev = *trafo;
    aiMultiplyMatrix4(trafo,&nd->mTransformation);

    for (; n < nd->mNumMeshes; ++n) {
        const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
        for (t = 0; t < mesh->mNumVertices; ++t) {

            aiVector3D tmp = mesh->mVertices[t];
            aiTransformVecByMatrix4(&tmp,trafo);

            min->x = aisgl_min(min->x,tmp.x);
            min->y = aisgl_min(min->y,tmp.y);
            min->z = aisgl_min(min->z,tmp.z);

            max->x = aisgl_max(max->x,tmp.x);
            max->y = aisgl_max(max->y,tmp.y);
            max->z = aisgl_max(max->z,tmp.z);
        }
    }

    for (n = 0; n < nd->mNumChildren; ++n) {
        get_bounding_box_for_node(nd->mChildren[n],min,max,trafo);
    }
    *trafo = prev;
}


void AssimpLoader::get_bounding_box (aiVector3D* min, aiVector3D* max)
{
    aiMatrix4x4 trafo;
    aiIdentityMatrix4(&trafo);

    min->x = min->y = min->z =  1e10f;
    max->x = max->y = max->z = -1e10f;
    get_bounding_box_for_node(scene->mRootNode,min,max,&trafo);
}



void AssimpLoader::apply_material(const struct aiMaterial *mtl)
{
    float c[4];

    GLenum fill_mode;
    int ret1, ret2;
    aiColor4D diffuse;
    aiColor4D specular;
    aiColor4D ambient;
    aiColor4D emission;
    float shininess, strength;
    int two_sided;
    int wireframe;
    unsigned int max;

    set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
    if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
        color4_to_float4(&diffuse, c);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

    set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
    if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
        color4_to_float4(&specular, c);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

    set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
    if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
        color4_to_float4(&ambient, c);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

    set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
    if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
        color4_to_float4(&emission, c);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

    max = 1;
    ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
    if(ret1 == AI_SUCCESS) {
        max = 1;
        ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
        if(ret2 == AI_SUCCESS)
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
        else
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    }
    else {
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
        set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
    }

    max = 1;
    if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
        fill_mode = wireframe ? GL_LINE : GL_FILL;
    else
        fill_mode = GL_FILL;
    glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

    max = 1;
    if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
        glDisable(GL_CULL_FACE);
    else
        glEnable(GL_CULL_FACE);
}












int AssimpLoader::printOglError(char *file, int line)
{

    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s\n",
               file, line, gluErrorString(glErr));
        retCode = 1;
    }
    return retCode;
}


// ----------------------------------------------------
// VECTOR STUFF
//


// res = a cross b;
void AssimpLoader::crossProduct( float *a, float *b, float *res) {

    res[0] = a[1] * b[2]  -  b[1] * a[2];
    res[1] = a[2] * b[0]  -  b[2] * a[0];
    res[2] = a[0] * b[1]  -  b[0] * a[1];
}


// Normalize a vec3
void AssimpLoader::normalize(float *a) {

    float mag = sqrt(a[0] * a[0]  +  a[1] * a[1]  +  a[2] * a[2]);

    a[0] /= mag;
    a[1] /= mag;
    a[2] /= mag;
}


// ----------------------------------------------------
// MATRIX STUFF
//

// Push and Pop for modelMatrix

void AssimpLoader::pushMatrix() {

    float *aux = (float *)malloc(sizeof(float) * 16);
    memcpy(aux, modelMatrix, sizeof(float) * 16);
    matrixStack.push_back(aux);
}

void AssimpLoader::popMatrix() {

    float *m = matrixStack[matrixStack.size()-1];
    memcpy(modelMatrix, m, sizeof(float) * 16);
    matrixStack.pop_back();
    free(m);
}

// sets the square matrix mat to the identity matrix,
// size refers to the number of rows (or columns)
void  AssimpLoader::setIdentityMatrix( float *mat, int size) {

    // fill matrix with 0s
    for (int i = 0; i < size * size; ++i)
        mat[i] = 0.0f;

    // fill diagonal with 1s
    for (int i = 0; i < size; ++i)
        mat[i + i * size] = 1.0f;
}


//
// a = a * b;
//
void AssimpLoader::multMatrix(float *a, float *b) {

    float res[16];

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            res[j*4 + i] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                res[j*4 + i] += a[k*4 + i] * b[j*4 + k];
            }
        }
    }
    memcpy(a, res, 16 * sizeof(float));

}


// Defines a transformation matrix mat with a translation
void AssimpLoader::setTranslationMatrix(float *mat, float x, float y, float z) {

    setIdentityMatrix(mat,4);
    mat[12] = x;
    mat[13] = y;
    mat[14] = z;
}

// Defines a transformation matrix mat with a scale
void AssimpLoader::setScaleMatrix(float *mat, float sx, float sy, float sz) {

    setIdentityMatrix(mat,4);
    mat[0] = sx;
    mat[5] = sy;
    mat[10] = sz;
}

// Defines a transformation matrix mat with a rotation
// angle alpha and a rotation axis (x,y,z)
void AssimpLoader::setRotationMatrix(float *mat, float angle, float x, float y, float z) {

    float radAngle = DegToRad(angle);
    float co = cos(radAngle);
    float si = sin(radAngle);
    float x2 = x*x;
    float y2 = y*y;
    float z2 = z*z;

    mat[0] = x2 + (y2 + z2) * co;
    mat[4] = x * y * (1 - co) - z * si;
    mat[8] = x * z * (1 - co) + y * si;
    mat[12]= 0.0f;

    mat[1] = x * y * (1 - co) + z * si;
    mat[5] = y2 + (x2 + z2) * co;
    mat[9] = y * z * (1 - co) - x * si;
    mat[13]= 0.0f;

    mat[2] = x * z * (1 - co) - y * si;
    mat[6] = y * z * (1 - co) + x * si;
    mat[10]= z2 + (x2 + y2) * co;
    mat[14]= 0.0f;

    mat[3] = 0.0f;
    mat[7] = 0.0f;
    mat[11]= 0.0f;
    mat[15]= 1.0f;

}

// ----------------------------------------------------
// Model Matrix
//
// Copies the modelMatrix to the uniform buffer


void AssimpLoader::setModelMatrix() {

    glBindBuffer(GL_UNIFORM_BUFFER,matricesUniBuffer);
    glBufferSubData(GL_UNIFORM_BUFFER,
                    ModelMatrixOffset, MatrixSize, modelMatrix);
    glBindBuffer(GL_UNIFORM_BUFFER,0);

}

// The equivalent to glTranslate applied to the model matrix
void AssimpLoader::translate(float x, float y, float z) {

    float aux[16];

    setTranslationMatrix(aux,x,y,z);
    multMatrix(modelMatrix,aux);
    setModelMatrix();
}

// The equivalent to glRotate applied to the model matrix
void AssimpLoader::rotate(float angle, float x, float y, float z) {

    float aux[16];

    setRotationMatrix(aux,angle,x,y,z);
    multMatrix(modelMatrix,aux);
    setModelMatrix();
}

// The equivalent to glScale applied to the model matrix
void AssimpLoader::scale(float x, float y, float z) {

    float aux[16];

    setScaleMatrix(aux,x,y,z);
    multMatrix(modelMatrix,aux);
    setModelMatrix();
}

// ----------------------------------------------------
// Projection Matrix
//
// Computes the projection Matrix and stores it in the uniform buffer

void AssimpLoader::buildProjectionMatrix(float fov, float ratio, float nearp, float farp) {

    float projMatrix[16];

    float f = 1.0f / tan (fov * (M_PI / 360.0f));

    setIdentityMatrix(projMatrix,4);

    projMatrix[0] = f / ratio;
    projMatrix[1 * 4 + 1] = f;
    projMatrix[2 * 4 + 2] = (farp + nearp) / (nearp - farp);
    projMatrix[3 * 4 + 2] = (2.0f * farp * nearp) / (nearp - farp);
    projMatrix[2 * 4 + 3] = -1.0f;
    projMatrix[3 * 4 + 3] = 0.0f;

    glBindBuffer(GL_UNIFORM_BUFFER,matricesUniBuffer);
    glBufferSubData(GL_UNIFORM_BUFFER, ProjMatrixOffset, MatrixSize, projMatrix);
    glBindBuffer(GL_UNIFORM_BUFFER,0);

}


// ----------------------------------------------------
// View Matrix
//
// Computes the viewMatrix and stores it in the uniform buffer
//
// note: it assumes the camera is not tilted,
// i.e. a vertical up vector along the Y axis (remember gluLookAt?)
//

void AssimpLoader::setCamera(float posX, float posY, float posZ,
               float lookAtX, float lookAtY, float lookAtZ) {

    float dir[3], right[3], up[3];

    up[0] = 0.0f; up[1] = 1.0f; up[2] = 0.0f;

    dir[0] =  (lookAtX - posX);
    dir[1] =  (lookAtY - posY);
    dir[2] =  (lookAtZ - posZ);
    normalize(dir);

    crossProduct(dir,up,right);
    normalize(right);

    crossProduct(right,dir,up);
    normalize(up);

    float viewMatrix[16],aux[16];

    viewMatrix[0]  = right[0];
    viewMatrix[4]  = right[1];
    viewMatrix[8]  = right[2];
    viewMatrix[12] = 0.0f;

    viewMatrix[1]  = up[0];
    viewMatrix[5]  = up[1];
    viewMatrix[9]  = up[2];
    viewMatrix[13] = 0.0f;

    viewMatrix[2]  = -dir[0];
    viewMatrix[6]  = -dir[1];
    viewMatrix[10] = -dir[2];
    viewMatrix[14] =  0.0f;

    viewMatrix[3]  = 0.0f;
    viewMatrix[7]  = 0.0f;
    viewMatrix[11] = 0.0f;
    viewMatrix[15] = 1.0f;

    setTranslationMatrix(aux, -posX, -posY, -posZ);

    multMatrix(viewMatrix, aux);

    glBindBuffer(GL_UNIFORM_BUFFER, matricesUniBuffer);
    glBufferSubData(GL_UNIFORM_BUFFER, ViewMatrixOffset, MatrixSize, viewMatrix);
    glBindBuffer(GL_UNIFORM_BUFFER,0);
}








int AssimpLoader::loadGLTextures(const aiScene* scene)
{
//    ILboolean success;

//    /* initialization of DevIL */
//    ilInit();

//    /* scan scene's materials for textures */
//    for (unsigned int m=0; m<scene->mNumMaterials; ++m)
//    {
//        int texIndex = 0;
//        aiString path; // filename

//        aiReturn texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
//        while (texFound == AI_SUCCESS) {
//            //fill map with textures, OpenGL image ids set to 0
//            textureIdMap[path.data] = 0;
//            // more textures?
//            texIndex++;
//            texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
//        }
//    }

//    int numTextures = textureIdMap.size();

//    /* create and fill array with DevIL texture ids */
//    ILuint* imageIds = new ILuint[numTextures];
//    ilGenImages(numTextures, imageIds);

//    /* create and fill array with GL texture ids */
//    GLuint* textureIds = new GLuint[numTextures];
//    glGenTextures(numTextures, textureIds); /* Texture name generation */

//    /* get iterator */
//    map<string, GLuint>::iterator itr = textureIdMap.begin();
//    int i=0;
//    for (; itr != textureIdMap.end(); ++i, ++itr)
//    {
//        //save IL image ID
//        string filename = (*itr).first;  // get filename
//        (*itr).second = textureIds[i];   // save texture id for filename in map

//        ilBindImage(imageIds[i]); /* Binding of DevIL image name */
//        ilEnable(IL_ORIGIN_SET);
//        ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
//        success = ilLoadImage((ILstring)filename.c_str());

//        if (success) {
//            /* Convert image to RGBA */
//            ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

//            /* Create and load textures to OpenGL */
//            glBindTexture(GL_TEXTURE_2D, textureIds[i]);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH),
//                         ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE,
//                         ilGetData());
//        }
//        else
//            printf("Couldn't load Image: %s\n", filename.c_str());
//    }
//    /* Because we have already copied image data into texture data
//    we can release memory used by image. */
//    ilDeleteImages(numTextures, imageIds);

//    //Cleanup
//    delete [] imageIds;
//    delete [] textureIds;

    //return success;
    return true;
}



//// Can't send color down as a pointer to aiColor4D because AI colors are ABGR.
//void Color4f(const aiColor4D *color)
//{
// glColor4f(color->r, color->g, color->b, color->a);
//}




void AssimpLoader::set_float4(float f[4], float a, float b, float c, float d)
{
    f[0] = a;
    f[1] = b;
    f[2] = c;
    f[3] = d;
}

void AssimpLoader::color4_to_float4(const aiColor4D *c, float f[4])
{
    f[0] = c->r;
    f[1] = c->g;
    f[2] = c->b;
    f[3] = c->a;
}



//void genVAOsAndUniformBuffer(const aiScene *sc) {

//    struct MyMesh aMesh;
//    struct MyMaterial aMat;
//    GLuint buffer;

//    // For each mesh
//    for (unsigned int n = 0; n < sc->mNumMeshes; ++n)
//    {
//        const aiMesh* mesh = sc->mMeshes[n];

//        // create array with faces
//        // have to convert from Assimp format to array
//        unsigned int *faceArray;
//        faceArray = (unsigned int *)malloc(sizeof(unsigned int) * mesh->mNumFaces * 3);
//        unsigned int faceIndex = 0;

//        for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
//            const aiFace* face = &mesh->mFaces[t];

//            memcpy(&faceArray[faceIndex], face->mIndices,3 * sizeof(unsigned int));
//            faceIndex += 3;
//        }
//        aMesh.numFaces = sc->mMeshes[n]->mNumFaces;

//        // generate Vertex Array for mesh
//        glGenVertexArrays(1,&(aMesh.vao));
//        glBindVertexArray(aMesh.vao);

//        // buffer for faces
//        glGenBuffers(1, &buffer);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->mNumFaces * 3, faceArray, GL_STATIC_DRAW);

//        // buffer for vertex positions
//        if (mesh->HasPositions()) {
//            glGenBuffers(1, &buffer);
//            glBindBuffer(GL_ARRAY_BUFFER, buffer);
//            glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mVertices, GL_STATIC_DRAW);
//            glEnableVertexAttribArray(vertexLoc);
//            glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0);
//        }

//        // buffer for vertex normals
//        if (mesh->HasNormals()) {
//            glGenBuffers(1, &buffer);
//            glBindBuffer(GL_ARRAY_BUFFER, buffer);
//            glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mNormals, GL_STATIC_DRAW);
//            glEnableVertexAttribArray(normalLoc);
//            glVertexAttribPointer(normalLoc, 3, GL_FLOAT, 0, 0, 0);
//        }

//        // buffer for vertex texture coordinates
//        if (mesh->HasTextureCoords(0)) {
//            float *texCoords = (float *)malloc(sizeof(float)*2*mesh->mNumVertices);
//            for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {

//                texCoords[k*2]   = mesh->mTextureCoords[0][k].x;
//                texCoords[k*2+1] = mesh->mTextureCoords[0][k].y;

//            }
//            glGenBuffers(1, &buffer);
//            glBindBuffer(GL_ARRAY_BUFFER, buffer);
//            glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*mesh->mNumVertices, texCoords, GL_STATIC_DRAW);
//            glEnableVertexAttribArray(texCoordLoc);
//            glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, 0, 0, 0);
//        }

//        // unbind buffers
//        glBindVertexArray(0);
//        glBindBuffer(GL_ARRAY_BUFFER,0);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

//        // create material uniform buffer
//        aiMaterial *mtl = sc->mMaterials[mesh->mMaterialIndex];

//        aiString texPath;   //contains filename of texture
//        if(AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, 0, &texPath)){
//            //bind texture
//            unsigned int texId = textureIdMap[texPath.data];
//            aMesh.texIndex = texId;
//            aMat.texCount = 1;
//        }
//        else
//            aMat.texCount = 0;

//        float c[4];
//        set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
//        aiColor4D diffuse;
//        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
//            color4_to_float4(&diffuse, c);
//        memcpy(aMat.diffuse, c, sizeof(c));

//        set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
//        aiColor4D ambient;
//        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
//            color4_to_float4(&ambient, c);
//        memcpy(aMat.ambient, c, sizeof(c));

//        set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
//        aiColor4D specular;
//        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
//            color4_to_float4(&specular, c);
//        memcpy(aMat.specular, c, sizeof(c));

//        set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
//        aiColor4D emission;
//        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
//            color4_to_float4(&emission, c);
//        memcpy(aMat.emissive, c, sizeof(c));

//        float shininess = 0.0;
//        unsigned int max;
//        aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
//        aMat.shininess = shininess;

//        glGenBuffers(1,&(aMesh.uniformBlockIndex));
//        glBindBuffer(GL_UNIFORM_BUFFER,aMesh.uniformBlockIndex);
//        glBufferData(GL_UNIFORM_BUFFER, sizeof(aMat), (void *)(&aMat), GL_STATIC_DRAW);

//        myMeshes.push_back(aMesh);
//    }
//}


void AssimpLoader::genVAOsAndUniformBuffer(const aiScene *sc) {

    struct MyMesh aMesh;
    struct MyMaterial aMat;
    GLuint buffer;

    // For each mesh
    for (unsigned int n = 0; n < sc->mNumMeshes; ++n)
    {
        const aiMesh* mesh = sc->mMeshes[n];

        // create array with faces
        // have to convert from Assimp format to array
        unsigned int *faceArray;
        faceArray = (unsigned int *)malloc(sizeof(unsigned int) * mesh->mNumFaces * 3);
        unsigned int faceIndex = 0;

        for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
            const aiFace* face = &mesh->mFaces[t];

            memcpy(&faceArray[faceIndex], face->mIndices,3 * sizeof(unsigned int));
            faceIndex += 3;
        }
        aMesh.numFaces = sc->mMeshes[n]->mNumFaces;

        // generate Vertex Array for mesh
        glGenVertexArrays(1,&(aMesh.vao));
        glBindVertexArray(aMesh.vao);

        // buffer for faces
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->mNumFaces * 3, faceArray, GL_STATIC_DRAW);

        // buffer for vertex positions
        if (mesh->HasPositions()) {
            glGenBuffers(1, &buffer);
            glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(vertexLoc);
            glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0);
        }

        // buffer for vertex normals
        if (mesh->HasNormals()) {
            glGenBuffers(1, &buffer);
            glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mNormals, GL_STATIC_DRAW);
            glEnableVertexAttribArray(normalLoc);
            glVertexAttribPointer(normalLoc, 3, GL_FLOAT, 0, 0, 0);
        }

        // buffer for vertex texture coordinates
        if (mesh->HasTextureCoords(0)) {
            float *texCoords = (float *)malloc(sizeof(float)*2*mesh->mNumVertices);
            for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {

                texCoords[k*2]   = mesh->mTextureCoords[0][k].x;
                texCoords[k*2+1] = mesh->mTextureCoords[0][k].y;

            }
            glGenBuffers(1, &buffer);
            glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*mesh->mNumVertices, texCoords, GL_STATIC_DRAW);
            glEnableVertexAttribArray(texCoordLoc);
            glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, 0, 0, 0);
        }

        // unbind buffers
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

        // create material uniform buffer
        aiMaterial *mtl = sc->mMaterials[mesh->mMaterialIndex];

        aiString texPath; //contains filename of texture
        if(AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, 0, &texPath)){
            //bind texture
            unsigned int texId = textureIdMap[texPath.data];
            aMesh.texIndex = texId;
            aMat.texCount = 1;
        }
        else
            aMat.texCount = 0;

        float c[4];
        set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
        aiColor4D diffuse;
        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
            color4_to_float4(&diffuse, c);
        memcpy(aMat.diffuse, c, sizeof(c));

        set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
        aiColor4D ambient;
        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
            color4_to_float4(&ambient, c);
        memcpy(aMat.ambient, c, sizeof(c));

        set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
        aiColor4D specular;
        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
            color4_to_float4(&specular, c);
        memcpy(aMat.specular, c, sizeof(c));

        set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
        aiColor4D emission;
        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
            color4_to_float4(&emission, c);
        memcpy(aMat.emissive, c, sizeof(c));

        float shininess = 0.0;
        unsigned int max;
        aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
        aMat.shininess = shininess;

        glGenBuffers(1,&(aMesh.uniformBlockIndex));
        glBindBuffer(GL_UNIFORM_BUFFER,aMesh.uniformBlockIndex);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(aMat), (void *)(&aMat), GL_STATIC_DRAW);

        myMeshes.push_back(aMesh);
    }
}


// Reshape Callback Function
void AssimpLoader::changeSize(int w, int h) {

    float ratio;
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0)
        h = 1;

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    ratio = (1.0f * w) / h;
    buildProjectionMatrix(53.13f, ratio, 0.1f, 100.0f);
}




// Events from the Keyboard
void AssimpLoader::processKeys(unsigned char key, int xx, int yy)
{
    switch(key) {

    case 27:

        //glutLeaveMainLoop();
        break;

    case 'z': r -= 0.1f; break;
    case 'x': r += 0.1f; break;
    case 'm': glEnable(GL_MULTISAMPLE); break;
    case 'n': glDisable(GL_MULTISAMPLE); break;
    }
    camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
    camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
    camY = r *              sin(beta * 3.14f / 180.0f);

    //  uncomment this if not using an idle func
    // glutPostRedisplay();
}



// Mouse Events
void AssimpLoader::processMouseButtons(int button, int state, int xx, int yy)
{
//    // start tracking the mouse
//    if (state == GLUT_DOWN)  {
//        startX = xx;
//        startY = yy;
//        if (button == GLUT_LEFT_BUTTON)
//            tracking = 1;
//        else if (button == GLUT_RIGHT_BUTTON)
//            tracking = 2;
//    }

//    //stop tracking the mouse
//    else if (state == GLUT_UP) {
//        if (tracking == 1) {
//            alpha += (startX - xx);
//            beta += (yy - startY);
//        }
//        else if (tracking == 2) {
//            r += (yy - startY) * 0.01f;
//        }
//        tracking = 0;
//    }
}

// Track mouse motion while buttons are pressed
void AssimpLoader::processMouseMotion(int xx, int yy)
{

    int deltaX, deltaY;
    float alphaAux, betaAux;
    float rAux;

    deltaX =  startX - xx;
    deltaY =  yy - startY;

    // left mouse button: move camera
    if (tracking == 1) {


        alphaAux = alpha + deltaX;
        betaAux = beta + deltaY;

        if (betaAux > 85.0f)
            betaAux = 85.0f;
        else if (betaAux < -85.0f)
            betaAux = -85.0f;

        rAux = r;

        camX = rAux * cos(betaAux * 3.14f / 180.0f) * sin(alphaAux * 3.14f / 180.0f);
        camZ = rAux * cos(betaAux * 3.14f / 180.0f) * cos(alphaAux * 3.14f / 180.0f);
        camY = rAux * sin(betaAux * 3.14f / 180.0f);
    }
    // right mouse button: zoom
    else if (tracking == 2) {

        alphaAux = alpha;
        betaAux = beta;
        rAux = r + (deltaY * 0.01f);

        camX = rAux * cos(betaAux * 3.14f / 180.0f) * sin(alphaAux * 3.14f / 180.0f);
        camZ = rAux * cos(betaAux * 3.14f / 180.0f) * cos(alphaAux * 3.14f / 180.0f);
        camY = rAux * sin(betaAux * 3.14f / 180.0f);
    }


    //  uncomment this if not using an idle func
    // glutPostRedisplay();
}



void AssimpLoader::mouseWheel(int wheel, int direction, int x, int y) {

    r += direction * 0.1f;
    camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
    camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
    camY = r *              sin(beta * 3.14f / 180.0f);
}



// Shader Stuff
void AssimpLoader::printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}


void AssimpLoader::printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}

#include <stdio.h>
#include <stdlib.h>


char *textFileRead(char *fn) {


    FILE *fp;
    char *content = NULL;

    int count=0;

    if (fn != NULL) {
        fp = fopen(fn,"rt");

        if (fp != NULL) {

      fseek(fp, 0, SEEK_END);
      count = ftell(fp);
      rewind(fp);

            if (count > 0) {
                content = (char *)malloc(sizeof(char) * (count+1));
                count = fread(content,sizeof(char),count,fp);
                content[count] = '\0';
            }
            fclose(fp);
        }
    }
    return content;
}

int textFileWrite(char *fn, char *s) {

    FILE *fp;
    int status = 0;

    if (fn != NULL) {
        fp = fopen(fn,"w");

        if (fp != NULL) {

            if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s))
                status = 1;
            fclose(fp);
        }
    }
    return(status);
}


GLuint AssimpLoader::setupShaders() {

    char *vs = NULL,*fs = NULL,*fs2 = NULL;

    GLuint p,v,f;

    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);

    vs = textFileRead(vertexFileName);
    fs = textFileRead(fragmentFileName);

    const char * vv = vs;
    const char * ff = fs;

    glShaderSource(v, 1, &vv,NULL);
    glShaderSource(f, 1, &ff,NULL);

    free(vs);free(fs);

    glCompileShader(v);
    glCompileShader(f);

    printShaderInfoLog(v);
    printShaderInfoLog(f);

    p = glCreateProgram();
    glAttachShader(p,v);
    glAttachShader(p,f);

    glBindFragDataLocation(p, 0, "output");

    glBindAttribLocation(p,vertexLoc,"position");
    glBindAttribLocation(p,normalLoc,"normal");
    glBindAttribLocation(p,texCoordLoc,"texCoord");

    glLinkProgram(p);
    glValidateProgram(p);
    printProgramInfoLog(p);

    program = p;
    vertexShader = v;
    fragmentShader = f;

    GLuint k = glGetUniformBlockIndex(p,"Matrices");
    glUniformBlockBinding(p, k, matricesUniLoc);
    glUniformBlockBinding(p, glGetUniformBlockIndex(p,"Material"), materialUniLoc);

    texUnit = glGetUniformLocation(p,"texUnit");

    return(p);
}



void AssimpLoader::render_scene (const aiScene *sc, const aiNode* nd)
{

    // Get node transformation matrix
    aiMatrix4x4 m = nd->mTransformation;
    // OpenGL matrices are column major
    m.Transpose();

    // save model matrix and apply node transformation
    pushMatrix();

    float aux[16];
    memcpy(aux,&m,sizeof(float) * 16);
    multMatrix(modelMatrix, aux);
    setModelMatrix();


    // draw all meshes assigned to this node
    for (unsigned int n=0; n < nd->mNumMeshes; ++n){
        // bind material uniform
        glBindBufferRange(GL_UNIFORM_BUFFER, materialUniLoc, myMeshes[nd->mMeshes[n]].uniformBlockIndex, 0, sizeof(struct MyMaterial));
        // bind texture
        glBindTexture(GL_TEXTURE_2D, myMeshes[nd->mMeshes[n]].texIndex);
        // bind VAO
        glBindVertexArray(myMeshes[nd->mMeshes[n]].vao);
        // draw
        glDrawElements(GL_TRIANGLES,myMeshes[nd->mMeshes[n]].numFaces*3,GL_UNSIGNED_INT,0);

    }

    // draw all children
    for (unsigned int n=0; n < nd->mNumChildren; ++n){
        render_scene(sc, nd->mChildren[n]);
    }
    popMatrix();
}




void AssimpLoader::reshape(int width, int height)
{
    const double aspectRatio = (float) width / height, fieldOfView = 45.0;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // gluPerspective(fieldOfView, aspectRatio, 1.0, 1000.0);  /* Znear and Zfar */

    glViewport(0, 0, width, height);
}


//void AssimpLoader::render_scene (const struct aiScene *sc, const struct aiNode* nd)
//{
//    unsigned int i;
//    unsigned int n = 0, t;
//    aiMatrix4x4 m = nd->mTransformation;

//    // update transform
//    aiTransposeMatrix4(&m);
//    glPushMatrix();
//    glMultMatrixf((float*)&m);

//    // draw all meshes assigned to this node
//    for (; n < nd->mNumMeshes; ++n) {
//        const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];

//        apply_material(sc->mMaterials[mesh->mMaterialIndex]);

//        if(mesh->mNormals == NULL) {
//            glDisable(GL_LIGHTING);
//        } else {
//            glEnable(GL_LIGHTING);
//        }

//        for (t = 0; t < mesh->mNumFaces; ++t) {
//            const struct aiFace* face = &mesh->mFaces[t];
//            GLenum face_mode;

//            switch(face->mNumIndices) {
//            case 1: face_mode = GL_POINTS; break;
//            case 2: face_mode = GL_LINES; break;
//            case 3: face_mode = GL_TRIANGLES; break;
//            default: face_mode = GL_POLYGON; break;
//            }

//            glBegin(face_mode);

//            for(i = 0; i < face->mNumIndices; i++) {
//                int index = face->mIndices[i];
//                if(mesh->mColors[0] != NULL)
//                    glColor4fv((float*)&mesh->mColors[0][index]);
//                if(mesh->mNormals != NULL)
//                    glNormal3fv(&mesh->mNormals[index].x);
//                glVertex3fv(&mesh->mVertices[index].x);
//            }

//            glEnd();
//        }

//    }

//    // draw all children
//    for (n = 0; n < nd->mNumChildren; ++n) {
//        render_scene(sc, nd->mChildren[n]);
//    }

//    glPopMatrix();
//}

void AssimpLoader::render_scene (){
    render_scene(scene,scene->mRootNode);
}



//void do_motion (void)
//{
//    static GLint prev_time = 0;
//    static GLint prev_fps_time = 0;
//    static int frames = 0;

//    int time =0;
//    //glutGet(GLUT_ELAPSED_TIME);
//    angle += (time-prev_time)*0.005;
//    prev_time = time;

//    frames += 1;
//    if ((time - prev_fps_time) > 1000) // update every seconds
//    {
//        int current_fps = frames * 1000 / (time - prev_fps_time);
//        //printf("%d fps\n", current_fps);
//        frames = 0;
//        prev_fps_time = time;
//    }


//   // glutPostRedisplay ();
//}



//void display(void)
//{
//    float tmp;

//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    //reshape(800,600);

//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//   // gluLookAt(0.f,0.f,3.f,0.f,0.f,-5.f,0.f,1.f,0.f);

//    // rotate it around the y axis
//    glRotatef(angle,0.f,1.f,0.f);

//    // scale the whole asset to fit into our view frustum
//    tmp = scene_max.x-scene_min.x;
//    tmp = aisgl_max(scene_max.y - scene_min.y,tmp);
//    tmp = aisgl_max(scene_max.z - scene_min.z,tmp);
//    tmp = 2.f / tmp;

//    //tmp = 1;
//    glScalef(tmp, tmp, tmp);

//    // center the model
//    glTranslatef( -scene_center.x, -scene_center.y, -scene_center.z );

//    // if the display list has not been made yet, create a new one and
//    // fill it with scene contents
//    if(scene_list == 0) {
//        scene_list = glGenLists(1);
//        glNewList(scene_list, GL_COMPILE);
//        // now begin at the root node of the imported data and traverse
//        // the scenegraph by multiplying subsequent local transforms
//        // together on GL's matrix stack.
//        assimp_render_scene(scene, scene->mRootNode);
//        glEndList();
//    }

//    glCallList(scene_list);

//    //glutSwapBuffers();

//    //do_motion();
//}




//void assimp_init(){

//    struct aiLogStream stream;

//    // get a handle to the predefined STDOUT log stream and attach
//    // it to the logging system. It remains active for all further
//    // calls to aiImportFile(Ex) and aiApplyPostProcessing.
//    stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT,NULL);
//    aiAttachLogStream(&stream);

//    // ... same procedure, but this stream now writes the
//    // log messages to assimp_log.txt
//    stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE,"assimp_log.txt");
//    aiAttachLogStream(&stream);

//    //-----------------------ASSIMP LOAD MODEL-------------------------------
//    // the model name can be specified on the command line. If none
//    // is specified, we try to locate one of the more expressive test
//    // models from the repository (/models-nonbsd may be missing in
//    // some distributions so we need a fallback from /models!).
//     AssimpLoader("bench.obj");

//    glClearColor(0.1f,0.1f,0.1f,1.f);

//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);    // Uses default lighting parameters

//    glEnable(GL_DEPTH_TEST);

//    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
//    glEnable(GL_NORMALIZE);

//    // XXX docs say all polygons are emitted CCW, but tests show that some aren't.
//    if(getenv("MODEL_IS_BROKEN"))
//        glFrontFace(GL_CW);

//    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

//   // glutGet(GLUT_ELAPSED_TIME);
//   // glutMainLoop();

//    // cleanup - calling 'aiReleaseImport' is important, as the library
//    // keeps internal resources until the scene is freed again. Not
//    // doing so can cause severe resource leaking.
//    aiReleaseImport(scene);

//    // We added a log stream to the library, it's our job to disable it
//    // again. This will definitely release the last resources allocated
//    // by Assimp.
//    aiDetachAllLogStreams();
//}






//bool loadOBJ(
//        const char * path,
//        vector<vec3> & out_vertices,
//        vector<vec2> & out_uvs,
//        vector<vec3> & out_normals
//        ){
//    printf("Loading OBJ file %s...\n", path);

//    vector<unsigned int> vertexIndices, uvIndices, normalIndices;
//    vector<vec3> temp_vertices;
//    vector<vec2> temp_uvs;
//    vector<vec3> temp_normals;


//    FILE * file = fopen(path, "r");
//    if( file == NULL ){
//        printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
//        getchar();
//        return false;
//    }

//    while( 1 ){

//        char lineHeader[128];
//        // read the first word of the line
//        int res = fscanf(file, "%s", lineHeader);
//        if (res == EOF)
//            break; // EOF = End Of File. Quit the loop.

//        // else : parse lineHeader

//        if ( strcmp( lineHeader, "v" ) == 0 ){
//            vec3 vertex;
//            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
//            temp_vertices.push_back(vertex);
//        }else if ( strcmp( lineHeader, "vt" ) == 0 ){
//            vec2 uv;
//            fscanf(file, "%f %f\n", &uv.x, &uv.y );
//            uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
//            temp_uvs.push_back(uv);
//        }else if ( strcmp( lineHeader, "vn" ) == 0 ){
//            vec3 normal;
//            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
//            temp_normals.push_back(normal);
//        }else if ( strcmp( lineHeader, "f" ) == 0 ){
//            string vertex1, vertex2, vertex3;
//            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
//            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
//            if (matches != 9){
//                printf("File can't be read by our simple parser :-( Try exporting with other options\n");
//                return false;
//            }
//            vertexIndices.push_back(vertexIndex[0]);
//            vertexIndices.push_back(vertexIndex[1]);
//            vertexIndices.push_back(vertexIndex[2]);
//            uvIndices    .push_back(uvIndex[0]);
//            uvIndices    .push_back(uvIndex[1]);
//            uvIndices    .push_back(uvIndex[2]);
//            normalIndices.push_back(normalIndex[0]);
//            normalIndices.push_back(normalIndex[1]);
//            normalIndices.push_back(normalIndex[2]);
//        }else{
//            // Probably a comment, eat up the rest of the line
//            char stupidBuffer[1000];
//            fgets(stupidBuffer, 1000, file);
//        }

//    }

//    // For each vertex of each triangle
//    for( unsigned int i=0; i<vertexIndices.size(); i++ ){

//        // Get the indices of its attributes
//        unsigned int vertexIndex = vertexIndices[i];
//        unsigned int uvIndex = uvIndices[i];
//        unsigned int normalIndex = normalIndices[i];

//        // Get the attributes thanks to the index
//        vec3 vertex = temp_vertices[ vertexIndex-1 ];
//        vec2 uv = temp_uvs[ uvIndex-1 ];
//        vec3 normal = temp_normals[ normalIndex-1 ];

//        // Put the attributes in buffers
//        out_vertices.push_back(vertex);
//        out_uvs     .push_back(uv);
//        out_normals .push_back(normal);

//    }

//    return true;
//}




// Rendering Callback Function
//void renderScene(void) {

//    static float step = 0.0f;

//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    // set camera matrix
//    setCamera(camX,camY,camZ,0,0,0);

//    // set the model matrix to the identity Matrix
//    setIdentityMatrix(modelMatrix,4);

//    // sets the model matrix to a scale matrix so that the model fits in the window
//    scale(scaleFactor, scaleFactor, scaleFactor);

//    // keep rotating the model
//    rotate(step, 0.0f, 1.0f, 0.0f);

//    // use our shader
//    glUseProgram(program);

//    // we are only going to use texture unit 0
//    // unfortunately samplers can't reside in uniform blocks
//    // so we have set this uniform separately
//    glUniform1i(texUnit,0);

//    recursive_render(scene, scene->mRootNode);

//    // FPS computation and display
//    frame++;
//    timecount=glutGet(GLUT_ELAPSED_TIME);
//    if (timecount - timebase > 1000) {
//        sprintf(s,"FPS:%4.2f",
//                frame*1000.0/(time-timebase));
//        timebase = timecount;
//        frame = 0;
//        glutSetWindowTitle(s);
//    }

//    // swap buffers
//    glutSwapBuffers();

//    // increase the rotation angle
//    //step++;
//}



//int init()
//{
//    if (!Import3DFromFile(modelname))
//        return(0);

//    LoadGLTextures(scene);

////    glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC) glutGetProcAddress("glGetUniformBlockIndex");
////    glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC) glutGetProcAddress("glUniformBlockBinding");
////    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) glutGetProcAddress("glGenVertexArrays");
////    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glutGetProcAddress("glBindVertexArray");
////    glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC) glutGetProcAddress("glBindBufferRange");
////    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC) glutGetProcAddress("glDeleteVertexArrays");

//    program = setupShaders();
//    genVAOsAndUniformBuffer(scene);

//    glEnable(GL_DEPTH_TEST);
//    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);


//    // Uniform Block

//    glGenBuffers(1,&matricesUniBuffer);
//    glBindBuffer(GL_UNIFORM_BUFFER, matricesUniBuffer);
//    glBufferData(GL_UNIFORM_BUFFER, MatricesUniBufferSize,NULL,GL_DYNAMIC_DRAW);
//    glBindBufferRange(GL_UNIFORM_BUFFER, matricesUniLoc, matricesUniBuffer, 0, MatricesUniBufferSize); //setUniforms();
//    glBindBuffer(GL_UNIFORM_BUFFER,0);

//    glEnable(GL_MULTISAMPLE);


//    return true;
//}



// Main function
//int main(int argc, char **argv) {

//    //  GLUT initialization
//    glutInit(&argc, argv);

//    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA|GLUT_MULTISAMPLE);

//    glutInitContextVersion (3, 3);
//    glutInitContextFlags (GLUT_COMPATIBILITY_PROFILE );

//    glutInitWindowPosition(100,100);
//    glutInitWindowSize(320,320);
//    glutCreateWindow("Lighthouse3D - Assimp Demo");


//    //  Callback Registration
//    glutDisplayFunc(renderScene);
//    glutReshapeFunc(changeSize);
//    glutIdleFunc(renderScene);

//    // Mouse and Keyboard Callbacks
//    glutKeyboardFunc(processKeys);
//    glutMouseFunc(processMouseButtons);
//    glutMotionFunc(processMouseMotion);

//    glutMouseWheelFunc ( mouseWheel ) ;

//    // Init GLEW
//    //glewExperimental = GL_TRUE;
//    glewInit();
//    if (glewIsSupported("GL_VERSION_3_3"))
//        printf("Ready for OpenGL 3.3\n");
//    else {
//        printf("OpenGL 3.3 not supported\n");
//        return(1);
//    }

//    //  Init the app (load model and textures) and OpenGL
//    if (!init())
//        printf("Could not Load the Model\n");

//    printf ("Vendor: %s\n", glGetString (GL_VENDOR));
//    printf ("Renderer: %s\n", glGetString (GL_RENDERER));
//    printf ("Version: %s\n", glGetString (GL_VERSION));
//    printf ("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));


//    // return from main loop
//    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

//    //  GLUT main loop
//    glutMainLoop();

//    // cleaning up
//    textureIdMap.clear();

//    // clear myMeshes stuff
//    for (unsigned int i = 0; i < myMeshes.size(); ++i) {

//        glDeleteVertexArrays(1,&(myMeshes[i].vao));
//        glDeleteTextures(1,&(myMeshes[i].texIndex));
//        glDeleteBuffers(1,&(myMeshes[i].uniformBlockIndex));
//    }
//    // delete buffers
//    glDeleteBuffers(1,&matricesUniBuffer);

//    return(0);
//}



//void AssimpLoader::load_file_simple (const char* path)
//{
//    // we are taking one of the postprocessing presets to avoid
//    // spelling out 20+ single postprocessing flags here.

//    aiVector3D scene_min, scene_max, scene_center;

//    scene = aiImportFile(path,aiProcessPreset_TargetRealtime_MaxQuality);

//    if (scene) {
//        get_bounding_box(&scene_min,&scene_max);
//        scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
//        scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
//        scene_center.z = (scene_min.z + scene_max.z) / 2.0f;
//    }
//}

//~ AssimpLoader::AssimpLoader  (){
//}

//------------------ Utilities ------------------------
