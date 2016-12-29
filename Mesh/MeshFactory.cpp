
#include "Common.h"

#include "MeshFactory.h"
#include <memory>



shared_ptr<vector<Mesh>>  MeshFactory::createMesh(MeshType type) {

    string path;

    switch (type) {
    case MeshCube:{
        path = "resources/cube.obj";
        break;
    }
    case MeshBench:{
        path ="resources/bench.obj";
        break;
    }
    case MilitiaBlend:{
        path ="resources/militia.blend";
        break;
    }
    case FaceFBX:{
        path ="resources/face.fbx";
        break;
    }
    case MeshSuzanne:{
        path ="resources/suzanne.obj";
        break;
    }
    case MeshRoom:{
        path ="resources/room.obj";
        break;
    }
    case MeshRoomThickWalls:{
        path ="resources/room_thickwalls.obj";
        break;
    }
    case MeshKidsRoom:{
        path ="resources/kids_room.obj";
        break;
    }

    case Mesh1:{
        path ="resources/1.obj";
        break;
    }
    case Mesh2:{
        path ="resources/2.obj";
        break;
    }
    case Mesh3:{
        path ="resources/3.obj";
        break;
    }
    case Mesh4:{
        path ="resources/4.obj";
        break;
    }
    case Mesh5:{
        path ="resources/5.obj";
        break;
    }
    case Mesh6:{
        path ="resources/6.obj";
        break;
    }
    case MeshSkeletons:{
        path ="resources/Group.obj";
        break;
    }
    }

    shared_ptr<vector<Mesh>> meshes = loadAssimpScene(path.c_str());

    return meshes;

};


shared_ptr<vector<Mesh>> MeshFactory::loadAssimpScene(const char * path)
{

    Assimp::Importer importer;

    importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);
    importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, aiComponent_COLORS | aiComponent_LIGHTS | aiComponent_CAMERAS);
    importer.SetPropertyInteger(AI_CONFIG_PP_SLM_VERTEX_LIMIT, 65535);
    importer.SetPropertyInteger(AI_CONFIG_PP_SLM_TRIANGLE_LIMIT, 65535);

    unsigned int flags = aiProcess_CalcTangentSpace
            | aiProcess_Triangulate
            // | aiProcess_JoinIdenticalVertices
            | aiProcess_SortByPType
            | aiProcess_FlipUVs
            | aiProcess_FindDegenerates
            | aiProcess_ImproveCacheLocality
            | aiProcess_RemoveComponent;

    const struct aiScene* sc = importer.ReadFile(path, flags);
    if( !sc) {
        fprintf( stderr, importer.GetErrorString());
        getchar();
        return false;
    }

    shared_ptr<vector<Mesh>> meshes = loadMeshes(sc);
    // loadMaterials(sc);

    //    if (meshdata) {
    // loadMeshdata(sc);
    //    }

    //    loadNodes(sc);
    //    loadAnimations(sc);
    //    calcBoundingBox(sc);
    //    setBoneNodes();

    return meshes;
}


// Load a model into a VAO, VBOs etc.
shared_ptr<vector<Mesh>>  MeshFactory::loadMeshes(const struct aiScene* sc)
{

    vector<Mesh> meshes;

    unsigned int n = 0;
    GLuint buffer;

    cerr  <<  "\n\n AI: Number of meshes: " << sc->mNumMeshes << endl;

    for (n = 0; n < sc->mNumMeshes; ++n) {
        const struct aiMesh* mesh = sc->mMeshes[n];

        // auto myMesh =make_shared<Mesh>();
        // auto myMesh = new Mesh(); //TODO: on the stack?
        Mesh myMesh;
        myMesh.numFaces = mesh->mNumFaces;
        myMesh.materialIndex = mesh->mMaterialIndex;


        assert(mesh->mNumFaces * 3 < 65535);

        myMesh.vao =make_shared<GLVAO>();

        // Prep face array for VBO
        int *faceArray;
        faceArray = (int *)malloc(sizeof(int) * mesh->mNumFaces * 3);
        assert(faceArray);

        // Copy face data
        unsigned int faceIndex = 0;
        for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
            const struct aiFace* face = &mesh->mFaces[t];
            faceArray[faceIndex++] = face->mIndices[0];
            faceArray[faceIndex++] = face->mIndices[1];
            faceArray[faceIndex++] = face->mIndices[2];
        }

        // Faces
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * mesh->mNumFaces * 3, faceArray, GL_STATIC_DRAW);
        myMesh.vao->setIndex(buffer);
        free(faceArray);

        //CORRECT
        //        glGenBuffers(1, &vertexbuffer);
        //        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        //        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

        // Positions
        if (mesh->HasPositions()) {
            glGenBuffers(1, &buffer);
            glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mVertices, GL_STATIC_DRAW);
            myMesh.vao->setPosition(buffer);
        }

        // Normals
        if (mesh->HasNormals()) {
            glGenBuffers(1, &buffer);
            glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mNormals, GL_STATIC_DRAW);
            myMesh.vao->setNormal(buffer);
        }

        // UVs
        for (unsigned int i = 0; i < UV_CHANNELS; ++i) {
            if (mesh->HasTextureCoords(i)) {
                glGenBuffers(1, &buffer);
                glBindBuffer(GL_ARRAY_BUFFER, buffer);
                glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mTextureCoords[i], GL_STATIC_DRAW);
                myMesh.vao->setTexUV(buffer, i);
            }
        }

        //  Bone IDs and Weights
        //        if (mesh->HasBones()) {
        //            loadBones(mesh, myMesh);
        //            recenter = false;

        //            glGenBuffers(1, &buffer);
        //            glBindBuffer(GL_ARRAY_BUFFER, buffer);
        //            glBufferData(GL_ARRAY_BUFFER, sizeof(int)*4*mesh->mNumVertices, boneIds, GL_STATIC_DRAW);
        //            myMesh->vao->setBoneId(buffer);

        //            glGenBuffers(1, &buffer);
        //            glBindBuffer(GL_ARRAY_BUFFER, buffer);
        //            glBufferData(GL_ARRAY_BUFFER, sizeof(float)*4*mesh->mNumVertices, boneWeights, GL_STATIC_DRAW);
        //            myMesh->vao->setBoneWeight(buffer);

        //            freeBones();
        //        }

        //Tangents and Bittangents, for normal mapping
        //TODO: Determine if there actually is a normal map in use on this mesh!
        //        if (mesh->HasTangentsAndBitangents()) {
        //            glGenBuffers(1, &buffer);
        //            glBindBuffer(GL_ARRAY_BUFFER, buffer);
        //            glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mTangents, GL_STATIC_DRAW);
        //            myMesh->vao->setTangent(buffer);

        //            glGenBuffers(1, &buffer);
        //            glBindBuffer(GL_ARRAY_BUFFER, buffer);
        //            glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mBitangents, GL_STATIC_DRAW);
        //            myMesh->vao->setBitangent(buffer);
        //        }


        //        myMesh->vertices;
        //        myMesh->vertices.reserve(mesh->mNumVertices);

        //        for (unsigned int t = 0; t < mesh->mNumVertices; ++t) {
        //            myMesh->vertices.push_back(glm::vec4(mesh->mVertices[t].x, mesh->mVertices[t].y, mesh->mVertices[t].z, 1.0f));
        //        }

        myMesh.vao->unbind();
        meshes.push_back(std::move(myMesh));
        // CHECK_OPENGL_ERROR;
    }

    return make_shared<vector<Mesh>>(std::move(meshes));
}


///**
//* Load materials.
//* Only supports simple materials with a single texture at the moment.
//**/
//void Object::loadMaterials(const struct aiScene* sc)
//{
//    unsigned int n;
//    aiString path;

//    for (n = 0; n < sc->mNumMaterials; n++) {
//        const aiMaterial* pMaterial = sc->mMaterials[n];
//        AssimpMaterial *myMat = new AssimpMaterial();

//        // Diffuse texture
//        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
//            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
//                //  myMat->diffuse = loadTexture(&path);
//            }
//        }

//        // Normal map
//        if (pMaterial->GetTextureCount(aiTextureType_NORMALS) > 0) {
//            if (pMaterial->GetTexture(aiTextureType_NORMALS, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
//                // myMat->normal = loadTexture(&path);
//            }
//        }

//        // Lightmap (e.g. Ambient Occlusion)
//        if (pMaterial->GetTextureCount(aiTextureType_LIGHTMAP) > 0) {
//            if (pMaterial->GetTexture(aiTextureType_LIGHTMAP, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
//                // myMat->lightmap = loadTexture(&path);
//            }
//        }

//        materials.push_back(myMat);
//    }
//}


/**
* The physics code needs the actual mesh data.
* If the mesh is used for physics, we load the faces and verts
*
* param bool update - Update the existing array of `Mesh`, v.s. creating a new.
**/
//void Object::loadMeshdata(const struct aiScene* sc)
//{
//    unsigned int n = 0;

//    cerr  <<  "\n\n AI: Number of meshes: " << sc->mNumMeshes << endl;

//    for (; n < sc->mNumMeshes; ++n) {
//        const struct aiMesh* mesh = sc->mMeshes[n];

//        // Grab existing object or create new one

//        auto myMesh = new Mesh();

//        // auto myMesh =make_shared<Mesh>();
//        myMesh->numFaces = mesh->mNumFaces;


//        // Copy face data
//        myMesh->faces = new vector<AssimpFace>();
//        myMesh->faces->reserve(mesh->mNumFaces);
//        for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
//            AssimpFace face;
//            face.a = mesh->mFaces[t].mIndices[0];
//            face.b = mesh->mFaces[t].mIndices[1];
//            face.c = mesh->mFaces[t].mIndices[2];
//            myMesh->faces->push_back(face);
//        }

//        // Copy vertices
//        myMesh->vertices;

//        myMesh->vertices.reserve(mesh->mNumVertices);

//        cerr  <<  "\n\n AI: Number of vertices: " << mesh->mNumVertices << endl;

//        for (unsigned int t = 0; t < mesh->mNumVertices; ++t) {

//            myMesh->vertices.push_back(glm::vec4(mesh->mVertices[t].x, mesh->mVertices[t].y, mesh->mVertices[t].z, 1.0f));

//        }

//        meshes.push_back(myMesh);

//    }
//}





/**
* Load a texture specified at a specified path.
*
* TODO: We should save these in a map so we don't load the same stuff multiple times.
**/
//Texture Object::loadTexture(aiString* path)
//{
//    class Sprite {
//    public:
//        int w;
//        int h;
//        GLuint pixels;
//        //SDL_Surface *orig;
//    };

//    //    string p(path->data);

//    //    if (p.substr(0, 2) == ".\\") p = p.substr(2, p.size() - 2);
//    //    if (p.substr(0, 2) == "./") p = p.substr(2, p.size() - 2);
//    //    if (p.substr(0, 2) == "//") p = p.substr(2, p.size() - 2);

//    return Texture("afs");

//    // return render->loadSprite("models/" + p, mod);
//}


// Load the node tree

//void Object::loadNodes(const struct aiScene* sc)
//{
//    assert(sc != NULL);
//    rootNode = loadNode(sc->mRootNode, 0);
//    assert(rootNode != NULL);
//}


/**
* Load a node (and it's children) from the node tree
**/
//AssimpNode* Object::loadNode(aiNode* nd, unsigned int depth)
//{
//    unsigned int i;
//    AssimpNode* myNode = new AssimpNode();
//    myNode->name = string(nd->mName.C_Str());

//    aiMatrix4x4 m = nd->mTransformation;
//    m.Transpose();
//    myNode->transform = glm::make_mat4((float *) &m);

//    for (i = 0; i < nd->mNumMeshes; i++) {
//        myNode->meshes.push_back(nd->mMeshes[i]);
//        meshes[nd->mMeshes[i]]->nd = myNode;
//    }

//    for (i = 0; i < nd->mNumChildren; i++) {
//        AssimpNode* child = loadNode(nd->mChildren[i], depth + 1);
//        if (child != NULL) myNode->addChild(child);
//    }

//    return myNode;
//}



//Load the animations for this assimp model
//void Object::loadAnimations(const struct aiScene* sc)
//{
//    unsigned int n;

//    for (n = 0; n < sc->mNumAnimations; n++) {
//        const aiAnimation* pAnimation = sc->mAnimations[n];

//        AssimpAnimation* loaded = loadAnimation(pAnimation);
//        if (loaded) {
//            animations.push_back(loaded);
//        }
//    }
//}


//Load a single animation
//AssimpAnimation* Object::loadAnimation(const aiAnimation* anim)
//{
//    unsigned int n, m;
//    AssimpAnimKey key;
//    AssimpAnimation *out;

//    out = new AssimpAnimation();
//    out->name = string(anim->mName.C_Str());
//    out->ticsPerSec = (float)anim->mTicksPerSecond;
//    out->duration = (float)anim->mDuration;

//    // Load animation channels
//    out->anims.reserve(anim->mNumChannels);
//    for (n = 0; n < anim->mNumChannels; n++) {
//        const aiNodeAnim* pNodeAnim = anim->mChannels[n];

//        AssimpNodeAnim* na = new AssimpNodeAnim();
//        na->name = string(pNodeAnim->mNodeName.C_Str());

//        // Positions
//        na->position.reserve(pNodeAnim->mNumPositionKeys);
//        for (m = 0; m < pNodeAnim->mNumPositionKeys; m++) {
//            key.time = (float)pNodeAnim->mPositionKeys[m].mTime;
//            key.vec = convVector(pNodeAnim->mPositionKeys[m].mValue);
//            na->position.push_back(key);
//        }

//        // Rotations
//        na->rotation.reserve(pNodeAnim->mNumRotationKeys);
//        for (m = 0; m < pNodeAnim->mNumRotationKeys; m++) {
//            key.time = (float)pNodeAnim->mRotationKeys[m].mTime;
//            key.quat = convQuaternion(pNodeAnim->mRotationKeys[m].mValue);
//            na->rotation.push_back(key);
//        }

//        // Scales
//        na->scale.reserve(pNodeAnim->mNumScalingKeys);
//        for (m = 0; m < pNodeAnim->mNumScalingKeys; m++) {
//            key.time = (float)pNodeAnim->mScalingKeys[m].mTime;
//            key.vec = convVector(pNodeAnim->mScalingKeys[m].mValue);
//            na->scale.push_back(key);
//        }

//        out->anims.push_back(na);
//    }

//    return out;
//}



//Calculate the bone ids and bone weights
//void Object::loadBones(const aiMesh* mesh, Mesh* myMesh)
//{
//    unsigned int m;
//    int n;
//    unsigned int *idx;

//    // Allocate space for the IDs and weights
//    boneIds = (int*) malloc(sizeof(int) * 4 * mesh->mNumVertices);
//    boneWeights = (float*) malloc(sizeof(float) * 4 * mesh->mNumVertices);

//    // Set to nothing
//    for (m = 0; m < (4 * mesh->mNumVertices); m++) {
//        boneIds[m] = 0;
//        boneWeights[m] = 0.0f;
//    }

//    // Keeps track of which index a given vertex is up to
//    idx = (unsigned int*) malloc(sizeof(unsigned int) * mesh->mNumVertices);
//    memset(idx, 0, sizeof(unsigned int) * mesh->mNumVertices);

//    // Loop through the weights of all the bones
//    // Save the id and the weight in the arrays as required
//    for (n = 0; n < mesh->mNumBones; n++) {
//        aiBone* bone = mesh->mBones[n];

//        for (m = 0; m < bone->mNumWeights; m++) {
//            aiVertexWeight w = bone->mWeights[m];

//            if (w.mWeight < 0.01f) continue;
//            if (idx[w.mVertexId] == MAX_WEIGHTS) continue;

//            boneIds[w.mVertexId * 4 + idx[w.mVertexId]] = n;
//            boneWeights[w.mVertexId * 4 + idx[w.mVertexId]] = w.mWeight;

//            idx[w.mVertexId]++;
//        }

//        // We also have to save the bone data in our structure
//        AssimpBone* bn = new AssimpBone();
//        bn->name = string(bone->mName.C_Str());
//        bn->offset = glm::make_mat4((float *) &(bone->mOffsetMatrix));

//        aiMatrix4x4 m = bone->mOffsetMatrix;
//        m.Transpose();
//        bn->offset = glm::make_mat4((float *) &m);

//        myMesh->bones.push_back(bn);
//    }

//    free(idx);
//}


// Find a node by name

//AssimpNode* Object::findNode(string name)
//{
//    return findNode(rootNode, name);
//}


/**
* Find a node by name
**/
//AssimpNode* Object::findNode(AssimpNode* nd, string name)
//{
//    if (nd->name == name) return nd;

//    for (vector<AssimpNode*>::iterator it = nd->children.begin(); it != nd->children.end(); ++it) {
//        AssimpNode* maybe = findNode((*it), name);
//        if (maybe) return maybe;
//    }

//    return NULL;
//}


/**
* Returns an array where each index refers to four
* ints (which get mapped to a glm::ivec4) of bone ids
* The index is the vertex index
**/
//int* Object::getBoneIds()
//{
//    return boneIds;
//}


/**
* Returns an array where each index refers to four
* floats (which get mapped to a glm::vec4) of bone weights
* The index is the vertex index
**/
//float* Object::getBoneWeights()
//{
//    return boneWeights;
//}

//void Object::freeBones()
//{
//    free(boneIds);
//    free(boneWeights);
//}


///**
//* The AssimpBones have a pointer to their associated AssimpNode
//* We need to hook these up
//**/
//void Object::setBoneNodes()
//{
//    for (auto it = meshes.begin(); it != meshes.end(); ++it) {
//        auto mesh = *it;

//        for (unsigned int i = 0; i < mesh->bones.size(); i++) {
//            AssimpBone *bn = mesh->bones[i];
//            bn->nd = findNode(rootNode, bn->name);
//        }
//    }
//}

//// Returns the bounding size of the mesh of the first frame

//void Object::calcBoundingBox(const struct aiScene* sc)
//{
//    aiMatrix4x4 trafo;
//    aiIdentityMatrix4(&trafo);
//    aiVector3D min, max;

//    // Calculate bounds and size
//    min.x = min.y = min.z = 1e10f;
//    max.x = max.y = max.z = -1e10f;
//    calcBoundingBoxNode(sc->mRootNode, &min, &max, &trafo, sc);
//    boundingSize = btVector3(max.x - min.x, max.y - min.y, max.z - min.z);

//    // Recenter the model to the middle of the bounding box
//    if (recenter) {
//        glm::vec4 translate(
//                    (min.x + max.x) / -2.0f,
//                    (min.y + max.y) / -2.0f,
//                    (min.z + max.z) / -2.0f,
//                    1.0f
//                    );
//        rootNode->transform[3] = translate;
//        //  if (debug_enabled("loadbones")) {
//        //   cout << "Recenter '" << rootNode->name << "' to " << rootNode->transform[3][0] << "x" << rootNode->transform[3][1] << "x" << rootNode->transform[3][2] << endl;
//        //  }
//    }
//}


//// Returns the bounding size of the mesh of the first frame

//void Object::calcBoundingBoxNode(const aiNode* nd, aiVector3D* min, aiVector3D* max, aiMatrix4x4* trafo, const struct aiScene* sc)
//{
//    aiMatrix4x4 prev;
//    unsigned int n = 0, t;


//    prev = *trafo;
//    aiMultiplyMatrix4(trafo, &nd->mTransformation);

//    for (; n < nd->mNumMeshes; ++n) {
//        const struct aiMesh* mesh = sc->mMeshes[nd->mMeshes[n]];

//        // If the material name contains NOPHYSICS, we don't count the mesh towards the bounding size
//        aiMaterial* material = sc->mMaterials[mesh->mMaterialIndex];
//        if (material) {
//            aiString name;
//            material->Get(AI_MATKEY_NAME, name);
//            if (strstr(name.C_Str(), "NOPHYSICS") != NULL) continue;
//        }

//        // Get the min and max vertexes for the mesh
//        for (t = 0; t < mesh->mNumVertices; ++t) {
//            aiVector3D tmp = mesh->mVertices[t];
//            aiTransformVecByMatrix4(&tmp, trafo);

//            min->x = std::min(min->x, tmp.x);
//            min->y = std::min(min->y, tmp.y);
//            min->z = std::min(min->z, tmp.z);

//            max->x = std::max(max->x, tmp.x);
//            max->y = std::max(max->y, tmp.y);
//            max->z = std::max(max->z, tmp.z);
//        }
//    }

//    // Calculate for children nodes too
//    for (n = 0; n < nd->mNumChildren; ++n) {
//        calcBoundingBoxNode(nd->mChildren[n], min, max, trafo, sc);
//    }

//    *trafo = prev;
//}


//// Returns the bounding size of the mesh of the first frame
//btVector3 Object::getBoundingSize()
//{
//    return boundingSize;
//}


///**
//* Returns the bounding size of the mesh of the first frame
//* HE = half extents
//**/
//btVector3 Object::getBoundingSizeHE()
//{
//    return btVector3(boundingSize.x() / 2.0f, boundingSize.y() / 2.0f, boundingSize.z() / 2.0f);
//}


//// Convert a vector to our internal format
//vec3 Object::convVector(aiVector3D in)
//{
//    return vec3(in.x, in.y, in.z);
//}


//// Convert a vector to our internal format

//glm::quat Object::convQuaternion(aiQuaternion in)
//{
//    return glm::quat(in.w, in.x, in.y, in.z);
//}


//void Object::createCollisionShape()
//{
//    btVector3 sizeHE = getBoundingSizeHE();
//    shape = new btBoxShape(sizeHE);
//}


//btCollisionShape* Object::getCollisionShape()
//{
//    if (shape == NULL) createCollisionShape();
//    return shape;
//}


//Object::~Object()
//{
//delete shape;
//delete rootNode;
// delete boneIds;
// delete boneWeights;

//    for (auto mesh : *meshes){
//        delete mesh;
//    }

//    for (auto mat : materials){
//        delete mat;
//    }

//    for (auto anim : animations){
//        delete anim;
//    }
//}

