#pragma once
#include "Mesh.h"


class MeshFactory
{
public:

    static shared_ptr<vector<Mesh>> createMesh(MeshType type);
    //private:
   static shared_ptr<vector<Mesh>> loadMeshes(const struct aiScene* sc);
    static shared_ptr<vector<Mesh>> loadAssimpScene(const char * path);

private:
    MeshFactory() = delete;
};
