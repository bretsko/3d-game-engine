#pragma once

#include <string>
using std::string;

typedef enum {FLAT, SMOOTH} ShadingType;

typedef enum {NORMAL_MODE = 1, COLOR_ID} LightingType;

typedef enum MeshType{
    MilitiaBlend,
    MeshKidsRoom,
    FaceFBX,
    Mesh1,
    Mesh2,
    Mesh3,
    Mesh4,
    Mesh5,
    Mesh6,
    MeshSkeletons,
    MeshCube,
    MeshSuzanne,
    MeshBench,
    MeshRoom,
    MeshRoomThickWalls
}MeshType;

//-------------Object names in the Scene --------------------

const string FLOOR = "FLOOR";
const string WALL = "WALL";
const string WALL1 = "WALL1";
const string FACE = "FACE";
const string FACE1 = "FACE1";



















