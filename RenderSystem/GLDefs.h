
#pragma once

class vec3;


//TODO:
// Constants for VBOs <-> Shaders
// (glBindAttribLocation)
#define ATTRIB_POSITION 0         // vPosition
#define ATTRIB_TEXUV  1           // vTexUV0, vTexUV1
#define ATTRIB_NORMAL 2           // vNormal
#define ATTRIB_BONEID 4           // vBoneIDs
#define ATTRIB_BONEWEIGHT 5       // vBoneWeights
#define ATTRIB_TEXTCOORD 6        // vCoord
#define ATTRIB_COLOR 7            // vColor
#define ATTRIB_TANGENT 8          // vTangent
#define ATTRIB_BITANGENT 9        // vBitangent

// Shader IDs
// TODO: Should this be an enum instead?
#define SHADER_BASIC 0
#define SHADER_SKYBOX 1
#define SHADER_ENTITY_STATIC 2
#define SHADER_ENTITY_BONES 3
#define SHADER_TERRAIN_PLAIN 4
#define SHADER_TERRAIN_NORMALMAP 5
#define SHADER_WATER 6
#define SHADER_TEXT 7
#define SHADER_ENTITY_STATIC_BUMP 8



enum PrimitiveType
{
    TRIANGLE_LIST = 0,
    LINE_LIST,
    POINT_LIST,
    TRIANGLE_STRIP,
    LINE_STRIP,
    TRIANGLE_FAN
};

/// %Geometry type.
enum GeometryType
{
    GEOM_STATIC = 0,
    GEOM_SKINNED = 1,
    GEOM_INSTANCED = 2,
    GEOM_BILLBOARD = 3,
    GEOM_DIRBILLBOARD = 4,
    GEOM_STATIC_NOINSTANCING = 5,
    MAX_GEOMETRYTYPES = 5,
};

/// Blending mode.
enum BlendMode
{
    BLEND_REPLACE = 0,
    BLEND_ADD,
    BLEND_MULTIPLY,
    BLEND_ALPHA,
    BLEND_ADDALPHA,
    BLEND_PREMULALPHA,
    BLEND_INVDESTALPHA,
    BLEND_SUBTRACT,
    BLEND_SUBTRACTALPHA,
    MAX_BLENDMODES
};

/// Depth or stencil compare mode.
enum CompareMode
{
    CMP_ALWAYS = 0,
    CMP_EQUAL,
    CMP_NOTEQUAL,
    CMP_LESS,
    CMP_LESSEQUAL,
    CMP_GREATER,
    CMP_GREATEREQUAL,
    MAX_COMPAREMODES
};

/// Culling mode.
enum CullMode
{
    CULL_NONE = 0,
    CULL_CCW,
    CULL_CW,
    MAX_CULLMODES
};

/// Fill mode.
enum FillMode
{
    FILL_SOLID = 0,
    FILL_WIREFRAME,
    FILL_POINT
};

/// Stencil operation.
enum StencilOp
{
    OP_KEEP = 0,
    OP_ZERO,
    OP_REF,
    OP_INCR,
    OP_DECR
};

/// Vertex/index buffer lock state.
enum LockState
{
    LOCK_NONE = 0,
    LOCK_HARDWARE,
    LOCK_SHADOW,
    LOCK_SCRATCH
};

/// Vertex elements.
enum VertexElement
{
    ELEMENT_POSITION = 0,
    ELEMENT_NORMAL,
    ELEMENT_COLOR,
    ELEMENT_TEXCOORD1,
    ELEMENT_TEXCOORD2,
    ELEMENT_CUBETEXCOORD1,
    ELEMENT_CUBETEXCOORD2,
    ELEMENT_TANGENT,
    ELEMENT_BLENDWEIGHTS,
    ELEMENT_BLENDINDICES,
    ELEMENT_INSTANCEMATRIX1,
    ELEMENT_INSTANCEMATRIX2,
    ELEMENT_INSTANCEmat3,
    // Custom 32-bit integer object index. Due to API limitations, not supported on D3D9
    ELEMENT_OBJECTINDEX,
    MAX_VERTEX_ELEMENTS
};

/// Texture filtering mode.
enum TextureFilterMode
{
    FILTER_NEAREST = 0,
    FILTER_BILINEAR,
    FILTER_TRILINEAR,
    FILTER_ANISOTROPIC,
    FILTER_DEFAULT,
    MAX_FILTERMODES
};

/// Texture addressing mode.
enum TextureAddressMode
{
    ADDRESS_WRAP = 0,
    ADDRESS_MIRROR,
    ADDRESS_CLAMP,
    ADDRESS_BORDER,
    MAX_ADDRESSMODES
};

/// Texture coordinates.
enum TextureCoordinate
{
    COORD_U = 0,
    COORD_V,
    COORD_W,
    MAX_COORDS
};

/// Texture usage types.
enum TextureUsage
{
    TEXTURE_STATIC = 0,
    TEXTURE_DYNAMIC,
    TEXTURE_RENDERTARGET,
    TEXTURE_DEPTHSTENCIL
};

/// Cube map faces.
enum CubeMapFace
{
    FACE_POSITIVE_X = 0,
    FACE_NEGATIVE_X,
    FACE_POSITIVE_Y,
    FACE_NEGATIVE_Y,
    FACE_POSITIVE_Z,
    FACE_NEGATIVE_Z,
    MAX_CUBEMAP_FACES
};

/// Cubemap single image layout modes.
enum CubeMapLayout
{
    CML_HORIZONTAL = 0,
    CML_HORIZONTALNVIDIA,
    CML_HORIZONTALCROSS,
    CML_VERTICALCROSS,
    CML_BLENDER
};

/// Update mode for render surface viewports.
enum RenderSurfaceUpdateMode
{
    SURFACE_MANUALUPDATE = 0,
    SURFACE_UPDATEVISIBLE,
    SURFACE_UPDATEALWAYS
};

/// Shader types.
enum ShaderType
{
    VS = 0,
    PS,
};

/// Shader parameter groups for determining need to update. On APIs that support constant buffers, these correspond to different constant buffers.
enum ShaderParameterGroup
{
    SP_FRAME = 0,
    SP_CAMERA,
    SP_ZONE,
    SP_LIGHT,
    SP_MATERIAL,
    SP_OBJECT,
    SP_CUSTOM,
    MAX_SHADER_PARAMETER_GROUPS
};

/// Texture units.
enum TextureUnit
{
    TU_DIFFUSE = 0,
    TU_ALBEDOBUFFER = 0,
    TU_NORMAL = 1,
    TU_NORMALBUFFER = 1,
    TU_SPECULAR = 2,
    TU_EMISSIVE = 3,
    TU_ENVIRONMENT = 4,
    TU_VOLUMEMAP = 5,
    TU_CUSTOM1 = 6,
    TU_CUSTOM2 = 7,
    TU_LIGHTRAMP = 8,
    TU_LIGHTSHAPE = 9,
    TU_SHADOWMAP = 10,
    TU_FACESELECT = 11,
    TU_INDIRECTION = 12,
    TU_DEPTHBUFFER = 13,
    TU_LIGHTBUFFER = 14,
    TU_ZONE = 15,
    MAX_MATERIAL_TEXTURE_UNITS = 8,
    MAX_TEXTURE_UNITS = 16
};

/// Billboard camera facing modes.
enum FaceCameraMode
{
    FC_NONE = 0,
    FC_ROTATE_XYZ,
    FC_ROTATE_Y,
    FC_LOOKAT_XYZ,
    FC_LOOKAT_Y,
    FC_DIRECTION
};

/// Shadow type
enum ShadowQuality
{
    SHADOWQUALITY_SIMPLE_16BIT = 0,
    SHADOWQUALITY_SIMPLE_24BIT,
    SHADOWQUALITY_PCF_16BIT,
    SHADOWQUALITY_PCF_24BIT,
    SHADOWQUALITY_VSM,
    SHADOWQUALITY_BLUR_VSM
};

// Scale calculation from bounding box diagonal.
extern  const vec3 DOT_SCALE;

static const int QUALITY_LOW = 0;
static const int QUALITY_MEDIUM = 1;
static const int QUALITY_HIGH = 2;
static const int QUALITY_MAX = 15;

static const unsigned CLEAR_COLOR = 0x1;
static const unsigned CLEAR_DEPTH = 0x2;
static const unsigned CLEAR_STENCIL = 0x4;

static const unsigned MASK_NONE = 0x0;
static const unsigned MASK_POSITION = 0x1;
static const unsigned MASK_NORMAL = 0x2;
static const unsigned MASK_COLOR = 0x4;
static const unsigned MASK_TEXCOORD1 = 0x8;
static const unsigned MASK_TEXCOORD2 = 0x10;
static const unsigned MASK_CUBETEXCOORD1 = 0x20;
static const unsigned MASK_CUBETEXCOORD2 = 0x40;
static const unsigned MASK_TANGENT = 0x80;
static const unsigned MASK_BLENDWEIGHTS = 0x100;
static const unsigned MASK_BLENDINDICES = 0x200;
static const unsigned MASK_INSTANCEMATRIX1 = 0x400;
static const unsigned MASK_INSTANCEMATRIX2 = 0x800;
static const unsigned MASK_INSTANCEmat3 = 0x1000;
static const unsigned MASK_OBJECTINDEX = 0x2000;
static const unsigned MASK_DEFAULT = 0xffffffff;
static const unsigned NO_ELEMENT = 0xffffffff;

static const int MAX_RENDERTARGETS = 4;
static const int MAX_VERTEX_STREAMS = 4;
static const int MAX_CONSTANT_REGISTERS = 256;

static const int BITS_PER_COMPONENT = 8;
