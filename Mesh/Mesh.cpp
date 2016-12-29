#include "Mesh.h"
//#include "RenderSystem/VBOindexer.h"



Mesh::Mesh() : parent(NULL) {

}
//Mesh::Mesh():  vao(NULL), numFaces(0), materialIndex(0) {

//    shape = NULL;
//    boneIds = NULL;
//    boneWeights = NULL;
//    rootNode = NULL;
//    recenter = true;

//    load(filename);

//    string err;
//    bool error = tinyobj::LoadObj(shapes, materials,err, filename);
//    //    if (!error) {
//    //        printf("%s\n", err.c_str());
//    //    }
//    vec3 min, max;
//    for (size_t i = 0; i < shapes.size(); ++i) {

//        _triangles.emplace_back();
//        _materials.emplace_back();
//        for (size_t j = 0; j < shapes[i].mesh.indices.size() / 3; ++j) {
//            std::array<vec3, 3> triplet;

//            //            vector<float> positions;
//            //            vector<float> normals;
//            //            vector<float> texcoords;
//            //            vector<unsigned int> indices;


//            for (int k = 0; k < 3; ++k) {
//                size_t idx = shapes[i].mesh.indices[3 * j + k];
//                triplet[k] = { shapes[i].mesh.positions[3 * idx + 0],
//                               shapes[i].mesh.positions[3 * idx + 1],
//                               shapes[i].mesh.positions[3 * idx + 2] };
//            }
//            _triangles[i].emplace_back(triplet);
//            _materials[i].emplace_back();
//            _materials[i].back().amb =
//                    Color{ materials[i].ambient[0], materials[i].ambient[1],
//                    materials[i].ambient[2] };
//        _materials[i].back().diff =
//                Color{ materials[i].diffuse[0], materials[i].diffuse[1],
//                materials[i].diffuse[2] };
//    _materials[i].back().emiss =
//            Color{ materials[i].emission[0], materials[i].emission[1],
//            materials[i].emission[2] };
//_materials[i].back().spec =
//        Color{ materials[i].specular[0], materials[i].specular[1],
//materials[i].specular[2] };

//for (int k = 0; k < 3; ++k) {
//    min = glm::min(min, triplet[k]);
//    max = glm::max(triplet[k], max);
//};
//}
//_boxes.emplace_back(min, max);
//}



//------------------ Functions ------------------------


////TODO: convert vector<float> (shapes[0].mesh.positions) to vector<vec3> indexed_vertices

////indexVBO(shapes[0].mesh.positions, shapes[0].mesh.texcoords, shapes[0].mesh.normals, indices, indexed_vertices, indexed_uvs, indexed_normals);


//}


//geometry::ray_path Mesh::intersect(ray ray) const {
//    ray_path list;

//    for (unsigned i = 0; i < _boxes.size(); ++i) {
//        if (_boxes[i].intersect(ray).empty()) {
//            continue;
//        }
//        for (unsigned k = 0; k < _triangles[i].size(); ++k) {
//            const auto &triplet = _triangles[i][k];
//            for (const auto &point : triplet.intersect(ray)) {
//                if (list.empty()) {
//                    _first_material = _materials[i][k];
//                    _first_normal = triplet.get_normal({
//                                                           0, 0, 0,
//                                                       });
//                }
//                list.push_back(point);
//            }
//        }
//    }

//    list.sort();

//    return list;
//}



//tinyobj
//num_t Mesh::get_color(const vec3 &point) const { return 0; }

//vec3 Mesh::get_normal(const vec3 &point) const { return _first_normal; }

//material Mesh::get_material() const { return _first_material; }



//Mesh::~Mesh()
//{
//   delete vao;
//   delete faces;
//    delete shape;
//    delete rootNode;
//    delete boneIds;
//    delete boneWeights;

//    for (auto mesh : meshes){
//        delete mesh;
//    }

//    for (auto mat : materials){
//        delete mat;
//    }

//    for (auto anim : animations){
//        delete anim;
//    }
//}


//Represents a vertex.
//class Mesh::Vertex{
//public:
//    //The list of polygons that this vertex lies in.
//    vector<vec3*> adjacentNormals;
//    //The actual vertex location.
//    vec3 *point;
//    //The vertex normal
//    vec3 normal;
//    //vec3 *surfaceNormal; cant hold surface normal because these points are shared
//    //static int id;
//    Vertex(vec3* point):point(point){
//        //Vertex::id++;
//    }

//    void addAdjacentNormal(vec3* normal){
//        adjacentNormals.push_back(normal);
//    }

//    //Averages normals of adjacent polygons.
//    void calculateNormal(){
//        for(auto i = adjacentNormals.begin(); i < adjacentNormals.end(); i++){
//            normal = normal + **i;
//        }
//        normal = normal/(float)adjacentNormals.size();
//        normal = glm::normalize(normal);
//        //cout << normal.x << endl;
//    }
//};

////Represents a triangle face.
//class Mesh::Polygon{
//public:
//    //friend class Mesh::Vertex;
//    //The face normal;
//    vec3 normal;
//    //raw vertex coordinates that this polygon is made of.
//    vector<vec3*> vertices;
//    int size;
//    //When creating the polygon, calculate normal.
//    Polygon():size(0){}

//    void addVertex(vec3* vertex){
//        vertices.push_back(vertex);
//        size++;
//    }

//    void calculateNormal(){
//        if (size < 3)
//            cout << "Warning: can't calculate normal with less than three vertices." << endl;
//        else
//        {
//            normal = glm::normalize(glm::cross(  *vertices.at(2) - *vertices.at(0), *vertices.at(1) - *vertices.at(0) ));
//        }
//    }

//    //Checks if this polygon contains the specified point.
//    bool contains(vec3* point){
//        for(int i = 0; i < vertices.size(); i++){
//            if(comparevec3(*vertices.at(i), *point))
//                return true;
//        }
//        return false;
//    }
//};

//class Mesh::BBox{
//public:
//    GLuint vboVertices;
//    GLuint iboElements;
//    mat4 transform;
//    //call this when creating buffers, use same vao
//    BBox(vec3 min, vec3 max, vec3 centerPoint, vec3 size){
//        //Create a unit cube.
//        GLfloat vertices[] = {
//            -0.5, -0.5, -0.5,
//             0.5, -0.5, -0.5,
//             0.5,  0.5, -0.5,
//            -0.5,  0.5, -0.5,
//            -0.5, -0.5,  0.5,
//             0.5, -0.5,  0.5,
//             0.5,  0.5,  0.5,
//            -0.5,  0.5,  0.5,
//        };

//        GLushort elements[] = {
//            0, 1, 2, 3,
//            4, 5, 6, 7,
//            0, 4, 1, 5, 2, 6, 3, 7
//        };

//        /*
//        cout << min.x << " " << min.y << " " << min.z << " " << endl;
//        cout << max.x << " " << max.y << " " << max.z << " " << endl;
//        */
//        //Generate the buffers
//        glGenBuffers(1, &vboVertices);
//        glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//        glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind buffer

//        glGenBuffers(1, &iboElements);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboElements);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



//        transform =  glm::translate(mat4(1.0f), centerPoint) * glm::scale(mat4(1.0f), size);
//    }
//    //pass in transformation. wanna calls this before drawing object
//    //transformation matrix will need to be restored in object's draw
//    void draw(GLuint vPosition_loc, GLuint mTransformation_loc, mat4& parentTransformation){
//        //Apply same matrix that is transforming object, and in addition apply our own
//        //transform matrix.
//        mat4 M = parentTransformation * transform;
//        glUniformMatrix4fv(mTransformation_loc, 1, GL_FALSE, glm::value_ptr(M));

//        glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
//        //need vposition pointer in shader (vPosition)
//        glVertexAttribPointer(vPosition_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
//        glEnableVertexAttribArray(vPosition_loc);

//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboElements);
//        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
//        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4*sizeof(GLushort)));
//        glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8*sizeof(GLushort)));
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//        //could restore original tranformation here

//        glDisableVertexAttribArray(vPosition_loc);
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//        //Don't want to delete buffers.
//    }
//};
////Assigns a unique color ID to this mesh.
//void Mesh::setColorID(){
//    colorID[0] = globalColorID[0];
//    colorID[1] = globalColorID[1];
//    colorID[2] = globalColorID[2];
//    globalColorID[0] += 1;
//    if(globalColorID[0] > 255)
//    {
//         globalColorID[0] = 0;
//         globalColorID[1]++;
//         if(globalColorID[1] > 255)
//         {
//              globalColorID[1] = 0;
//              globalColorID[2]++;
//         }
//    }
//}

//bool Mesh::colorMatch(unsigned char *color){
//    return (colorID[0] == color[0] && colorID[1] == color[1] && colorID[2] == color[2]);
//}

////Note, polygons have variable number of vertices.
//Mesh::Mesh(const char *coords, const char *polys, bool isTextured){
//    //TODO Create bounding box.
//    //Calculate middle point
//    //Stick vertices into same buffer.
//    //Draw part of object (up to a certain index) using GL_TRIANGLES, and then draw the box
//    this->isTextured = isTextured;
//    setColorID();
//    //Indicates that we know which vetices are associated with each polygon.
//    smartPolys = true;
//    //load the vertices. they are in order
//    std::ifstream file;
//    std::string unused;

//    file.open(coords);
//    int totalVerts;
//    int indexNum;
//    float x, y, z;
//    char junk;
//    file >> totalVerts;
//    rawCoords.reserve(totalVerts);
//    rawVerts.reserve(totalVerts);
//    vertexList.reserve(totalVerts);
//    for(int i = 0; i < totalVerts; i++){
//        file >> indexNum;
//        file >> junk;
//        file >> x;
//        file >> junk;
//        file >> y;
//        file >> junk;
//        file >> z;
//        rawCoords.push_back(vec3(x, y, z));
//        //these will be out of order
//        vertexList.push_back(new Vertex(&rawCoords.back()));

//        //initialize max and min
//        if(i == 0){
//            maxCoords = rawCoords.back();
//            minCoords = rawCoords.back();
//        }
//        //Look for max and min points.
//        if(x > maxCoords.x)
//            maxCoords.x = x;
//        else if(x < minCoords.x)
//            minCoords.x = x;
//        if(y > maxCoords.y)
//            maxCoords.y = y;
//        else if(y < minCoords.y)
//            minCoords.y = y;
//        if(z > maxCoords.z)
//            maxCoords.z = z;
//        else if(z < minCoords.z)
//            minCoords.z = z;
//    }
//    size = glm::vec4(maxCoords.x - minCoords.x, maxCoords.y - minCoords.y, maxCoords.z - minCoords.z, 1.0f);
//    center = vec3((minCoords.x + maxCoords.x)/2.0f, (minCoords.y + maxCoords.y)/2.0f, (minCoords.z + maxCoords.z)/2.0f);
//    //cout << "Center of figure "<< center.x << " " << center.y << " " << center.z << endl;
//    currentCenter = center;
//    file.close();
//    //Load the polygons
//    file.open(polys);
//    int totalPolys;
//    std::string section;
//    std::string sectionPrev;
//    std::string line;
//    vector<std::string> splitLine;

//    file >> totalPolys;
//    std::getline(file, unused);

//    //These are the texture coordinates of a face, taking into account that it
//    //it is going to be triangulated using my algorihtmn. Basically it only works for cubes.
//    vector<vec2> originals;
//    originals.push_back(vec2(0.0f, 0.0f));
//    originals.push_back(vec2(1.0f, 0.0f));
//    originals.push_back(vec2(1.0f, 1.0f));
//    originals.push_back(vec2(0.0f, 0.0f));
//    originals.push_back(vec2(1.0f, 1.0f));
//    originals.push_back(vec2(0.0f, 1.0f));
//    originals.push_back(vec2(0.0f, 0.0f));

//    /*
//    Vertex Normals are handled correctly.
//    Creates correct amount of surface normals
//    */

//    //modify to make triangles
//    for(int i  = 0; i < totalPolys; i++){
//        Polygon* poly = new Polygon();
//        std::getline(file, line);
//        //cout << line << endl;
//        split(line, ' ', splitLine);
//        section = splitLine.at(0); //that label at the beginning of the part.
//        int first = atoi((splitLine.at(1).c_str())) -1; //first point. Use it as first point for all triangles.
//        poly->addVertex(&rawCoords.at(first));

//        int current = -1;
//        int previous;
//        int count = 1;
//        int textureIndex = 0;
//        for(auto j = splitLine.begin() + 2; j < splitLine.end(); j++){

//            previous = current;
//            current = atoi((*j).c_str()) - 1;
//            poly->addVertex(&rawCoords.at(current));

//            if(count > 1){
//                if(count == 2){
//                    poly->calculateNormal();
//                }
//                rawVerts.push_back(rawCoords.at(first));
//                rawVerts.push_back(rawCoords.at(previous));
//                rawVerts.push_back(rawCoords.at(current));

//                //Very hard-coded
//                textureCoords.push_back(originals.at(textureIndex++));
//                if(textureIndex == originals.size())
//                    textureIndex = 0;
//                textureCoords.push_back(originals.at(textureIndex++));
//                textureCoords.push_back(originals.at(textureIndex++));

//                //Since the verts are in order, it would be good to push the appropriate coordinates directly.
//                surfaceNormals.push_back(poly->normal);
//                surfaceNormals.push_back(poly->normal);
//                surfaceNormals.push_back(poly->normal);

//                vertexList.at(first)->addAdjacentNormal(&(poly->normal)); //associate poly normal with vertex
//                vertexNRefs.push_back(&vertexList.at(first)->normal); //we only keep references until its time to calculate
//                vertexList.at(previous)->addAdjacentNormal(&(poly->normal));
//                vertexNRefs.push_back(&vertexList.at(previous)->normal);
//                vertexList.at(current)->addAdjacentNormal(&(poly->normal));
//                vertexNRefs.push_back(&vertexList.at(current)->normal);
//            }
//            count++;
//        }
//        polygons.push_back(poly);
//        splitLine.clear();
//    }
//    /*
//    cout << "Polygon Count " << polygons.size() << endl;
//    cout << "Surface Normal Count " << surfaceNormals.size() << endl; //17832

//    cout << "Point Count " << rawVerts.size() << endl; //17832
//    cout << "Texture Points " << textureCoords.size() << endl;
//    */
//    file.close();
//}

////This constructor takes a vector of vec3 and generates vertex and polygon objects.
//Mesh::Mesh(vector<vec3> &vertices){
//    setColorID();
//    smartPolys = false;

//    rawVerts = vertices;
//    for(int i = 0; i < vertices.size(); i++){

//        if(i == 0){
//            maxCoords = vertices.front();
//            minCoords = vertices.front();
//        }

//        if(vertices.at(i).x > maxCoords.x)
//            maxCoords.x = vertices.at(i).x;
//        if(vertices.at(i).x < minCoords.x)
//            minCoords.x = vertices.at(i).x;
//        if(vertices.at(i).y > maxCoords.y)
//            maxCoords.y = vertices.at(i).y;
//        if(vertices.at(i).y < minCoords.y)
//            minCoords.y = vertices.at(i).y;
//        if(vertices.at(i).z > maxCoords.z)
//            maxCoords.z = vertices.at(i).z;
//        if(vertices.at(i).z < minCoords.z)
//            minCoords.z = vertices.at(i).z;

//        Vertex* vertex  = new Vertex(&rawVerts.at(i));
//        vertexList.push_back(vertex);
//        //Every three points, create a new polygon object.
//        if((i + 1) % 3 == 0){
//            Polygon* poly = new Polygon();
//            for(int j = 0; j < 3; j++){
//                poly->addVertex(&(rawVerts.at(i - 2 +  j)));
//            }
//            polygons.push_back(poly); //something bad happens here.
//        }
//    }
//    center = vec3((minCoords.x + maxCoords.x)/2.0f, (minCoords.y + maxCoords.y)/2.0f, (minCoords.z + maxCoords.z)/2.0f);
//    //center = size/2.0f;
//    cout << "Center of terrain "<< center.x << " " << center.y << " " << center.z << endl;
//    currentCenter = center;

//}

///*
//    Uses two different ways of calculating normals, depending on if a file was read.
//*/
//void Mesh::calculateNormals()
//{
//    if(smartPolys == false){

//        for(auto p = polygons.begin(); p < polygons.end(); p++){
//            (**p).calculateNormal();
//            for(int i = 0; i < 3;  i++){
//                surfaceNormals.push_back((**p).normal);
//            }
//        }

//        for(auto v = vertexList.begin(); v < vertexList.end(); v++){
//            for(auto p = polygons.begin(); p < polygons.end(); p++){
//                if((**p).contains((**v).point)){
//                    (**v).addAdjacentNormal(&(**p).normal);
//                }
//            }
//        (**v).calculateNormal();
//        vertexNormals.push_back((**v).normal);
//        }
//    }
//    else{
//        //surface normals were already provided. Now just calculate normal per vertex
//        for(auto v = vertexList.begin(); v < vertexList.end(); v++){
//            (**v).calculateNormal();
//        }
//        for(auto n = vertexNRefs.begin(); n < vertexNRefs.end(); n++){
//            vertexNormals.push_back(**n);
//        }
//    }
//}

////There is one vertex array object per mesh.
//void Mesh::createGLBuffer(bool smooth, GLuint* vao, int index){
//    //Create a vertex array
//    vaoIndex = index;
//    this->vao = vao;
//    glBindVertexArray(vao[index]);

//    boundingBox = new BBox(minCoords, maxCoords, center, vec3(size));

//    sizeofVertices = rawVerts.size()*sizeof(vec3);
//    sizeofNormals = surfaceNormals.size()*sizeof(vec3);
//    sizeofTCoords = 0;
//    if(isTextured)
//        sizeofTCoords = textureCoords.size()*sizeof(vec2);

//    glGenBuffers(1, &vertexBuffer);
//    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
//    glBufferData(GL_ARRAY_BUFFER, sizeofVertices + sizeofNormals + sizeofTCoords, NULL, GL_DYNAMIC_DRAW);

//    //Hold the points to draw here
//    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeofVertices, rawVerts.data());
//    //Hold the normals here
//    if(smooth){
//        glBufferSubData(GL_ARRAY_BUFFER, sizeofVertices, sizeofNormals, vertexNormals.data());
//    }
//    else{
//        glBufferSubData(GL_ARRAY_BUFFER, sizeofVertices, sizeofNormals, surfaceNormals.data());
//    }

//    if(isTextured){
//        glBufferSubData(GL_ARRAY_BUFFER, sizeofNormals + sizeofVertices, sizeofTCoords, textureCoords.data());
//    }
//    //Create the bounding box buffers. Current vao is still bound.

//    //Unbind buffers
//    glBindVertexArray(0);
//}

////Should pass in names as parameters.
//void Mesh::setupShader(GLuint _program, Camera* camera_){
//    currentShading = FLAT;
//    this->camera = camera_;
//    wireframe = false;
//    glBindVertexArray(vao[vaoIndex]);
//    glUseProgram(_program);
//    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
//    //Initialize the vertex position attribute from the vertex shader

//    vPosition = glGetAttribLocation(_program, "vPosition");
//    vNormal = glGetAttribLocation(_program, "vNormal");
//    if(isTextured)
//        vertexUV = glGetAttribLocation(_program, "vertexUV");

//    const float maxValue = 255.0;
//    //This stuff is hard coded.-------------
//    glm::vec4 materialAmbient(.4f, .4f, .4f, 1.0f);
//    materialSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

//    glm::vec4 lightAmbient(0.5f, 0.5f, 0.5f, 1.0f);
//    lightSpecular = glm::vec4 (1.0f, 1.0f, 1.0f, 1.0f); //A bright red specular color
//    lightDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); //white

//    shininess = .25f* 128.0f;
//    //To get diffuse lighting, calculate angle of each triangle.
//    //So get the surface normal of every three points.
//    glm::vec4 ambientProduct = lightAmbient * materialAmbient;

//    //Get uniform variable locations.
//    lightDiffuse_loc = glGetUniformLocation(_program, "LightDiffuse");
//    materialDiffuse_loc = glGetUniformLocation(_program, "DiffuseReflect");
//    materialDiffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
//    GLuint ambientProduct_loc = glGetUniformLocation(_program, "AmbientProduct");
//    lightSpecular_loc = glGetUniformLocation(_program, "LightSpecular");
//    materialSpecular_loc = glGetUniformLocation(_program, "MaterialSpecular");
//    lightPosition_loc = glGetUniformLocation(_program, "LightPosition");
//    shininess_loc = glGetUniformLocation(_program, "Shininess");
//    alpha_loc = glGetUniformLocation(_program, "Alpha");
//    GLuint colorID_loc = glGetUniformLocation(_program, "colorID");

//    //Set uniform variables
//    glUniform4fv(ambientProduct_loc, 1, glm::value_ptr(ambientProduct));
//    glUniform4fv(lightDiffuse_loc, 1, glm::value_ptr(lightDiffuse));
//    glUniform4fv(materialDiffuse_loc, 1, glm::value_ptr(materialDiffuse));
//    glUniform4fv(lightSpecular_loc, 1, glm::value_ptr(lightSpecular));
//    glUniform4fv(materialSpecular_loc, 1, glm::value_ptr(materialSpecular));
//    //Dont know why this only works when negative. 4th element is zero to signify directional
//    lightPosition = glm::vec4(0.0, 0.8, 1.0, 0.0);

//    glUniform4fv(lightPosition_loc, 1, glm::value_ptr(lightPosition));
//    glUniform4fv(colorID_loc, 1, glm::value_ptr(glm::vec4(colorID[0]/255.0f, colorID[1]/255.0f, colorID[2]/255.0f, 1.0f)));
//    glUniform1f(shininess_loc, shininess);
//    alpha = 1.0f;
//    glUniform1f(alpha_loc, alpha);
//    //Set up pointers to subroutines in shader.
//    normalModeIndex = glGetSubroutineIndex(_program, GL_VERTEX_SHADER, "normalMode");
//    colorKeyIndex = glGetSubroutineIndex(_program, GL_VERTEX_SHADER, "colorKeyMode");

//    subroutines[0] = normalModeIndex;

//    mTransformation_loc = glGetUniformLocation(_program, "mTransformation");
//    modelView_loc = glGetUniformLocation(_program, "ModelView");
//    mProjection_loc = glGetUniformLocation(_program, "Projection");

//    glUniformMatrix4fv(mTransformation_loc, 1, GL_FALSE, glm::value_ptr(mTransformation));
//    this->program = _program;
//    glBindVertexArray(0);
//}


//void Mesh::setDiffuse(glm::vec4 materialDiffuse){
//    this->materialDiffuse = materialDiffuse;
//    //Pass to shader.
//    glBindVertexArray(vao[vaoIndex]);
//    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
//    glUseProgram(program);
//    glUniform4fv(materialDiffuse_loc, 1, glm::value_ptr(materialDiffuse));
//}

//void Mesh::setSpecular(glm::vec4 materialDiffuse, float shininess){
//    this->materialSpecular = materialSpecular;
//    this->shininess = shininess;
//    glBindVertexArray(vao[vaoIndex]);
//    glUseProgram(program);
//    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
//    glUniform4fv(materialSpecular_loc, 1, glm::value_ptr(materialSpecular));
//    glUniform1f(shininess_loc, shininess);
//}

//void Mesh::setAlpha(float alpha){
//    this->alpha = alpha;
//    glBindVertexArray(vao[vaoIndex]);
//    glUseProgram(program);
//    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
//    glUniform1f(alpha_loc, alpha);
//}
///*
//  http://www.opengl.org/discussion_boards/showthread.php/163929-image-loading?p=1158293#post1158293
//*/
//void Mesh::loadTexture(const char *filename){
//    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);
//    FIBITMAP* image = FreeImage_Load(format, filename);

//    FIBITMAP* temp = image;
//    image = FreeImage_ConvertTo32Bits(image);
//    FreeImage_Unload(temp);

//    int w = FreeImage_GetWidth(image);
//    int h = FreeImage_GetHeight(image);

//    GLubyte* texture = new GLubyte[4*w*h];
//    char* pixels = (char*)FreeImage_GetBits(image);

//    //FreeImage loads in BGR format, so you need to swap some bytes(Or use GL_BGR).
//    for(int j= 0; j< w * h; j++){
//        texture[j * 4 + 0] = pixels[j * 4 + 2];
//        texture[j * 4 + 1] = pixels[j * 4 + 1];
//        texture[j * 4 + 2] = pixels[j * 4 + 0];
//        texture[j * 4 + 3] = pixels[j * 4 + 3];
//    }

//    //Bind raw image to texture.
//    glGenTextures(1, &textureLocation);
//    glBindTexture(GL_TEXTURE_2D, textureLocation);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)texture );

//    GLenum huboError = glGetError();
//    if(huboError){
//        cout<<"There was an error loading the texture"<< endl;
//    }
//}

///*
//    Handles animations.
//*/
//void Mesh::draw(){
//    if(wireframe)
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    else
//        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

//    glBindVertexArray(vao[vaoIndex]);
//    glUseProgram(program);

//    if(isTextured)
//        glBindTexture(GL_TEXTURE_2D, textureLocation);

//    glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, subroutines);

//    //Bind to this array, using these buffers.
//    if(drawBox)
//        boundingBox->draw(vPosition, mTransformation_loc, mTransformation);
//    //http://www.idevgames.com/forums/thread-551.html This keeps camera fixed in world coordinates.
//    glUniform4fv(lightPosition_loc, 1, glm::value_ptr( camera->getInverse()* lightPosition ));
//    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
//    glUniformMatrix4fv(modelView_loc, 1, GL_FALSE, glm::value_ptr(camera->getModelView()));
//    glUniformMatrix4fv(mProjection_loc, 1, GL_FALSE, glm::value_ptr(camera->getProjection()));
//    glUniformMatrix4fv(mTransformation_loc, 1, GL_FALSE, glm::value_ptr(mTransformation));

//    //Rebind location of these things
//    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
//    glEnableVertexAttribArray(vPosition);
//    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeofVertices));
//    glEnableVertexAttribArray(vNormal);

//    if(isTextured){
//        glVertexAttribPointer(vertexUV, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeofVertices + sizeofNormals));
//        glEnableVertexAttribArray(vertexUV);
//    }

//    glDrawArrays(GL_TRIANGLES, 0 , rawVerts.size());

//    glDisableVertexAttribArray(vPosition);
//    glDisableVertexAttribArray(vNormal);
//    glBindVertexArray(0);
//}


////Refills subbuffer data. Ideally we'd store it beforehand and just change index.
//void Mesh::changeShading(ShadingType shading){
//    glBindVertexArray(vao[vaoIndex]);
//    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
//    switch(shading){
//        case FLAT:
//            glBufferSubData(GL_ARRAY_BUFFER, sizeofVertices, sizeofNormals, surfaceNormals.data());
//            break;
//        case SMOOTH:
//            glBufferSubData(GL_ARRAY_BUFFER, sizeofVertices, sizeofNormals, vertexNormals.data());
//            break;
//    }
//    currentShading = shading;
//    glBindVertexArray(0);
//}


////Method 1: Undo previous translation? This will require absolute coordinates.
////Method 2: Just multiply with new translation matrix. Values will accumulate.
//void Mesh::translate(vec3& offset){
//    //apply translation directly to current transfomation matrix
//    mat4 translation(1.0f);
//    translation = glm::translate(translation, offset);
//    mTranslation = translation * mTranslation;
//    mTransformation = translation * mTransformation;
//    currentCenter = vec3(translation * glm::vec4(currentCenter, 1.0f));
//}


////Move to an absolute coordinate on the map.
//void Mesh::moveTo(vec3& point){
//    //translate to center, then translate to point.
//    mTranslation = glm::translate(mat4(1.0f), point);
//    mTransformation = mTranslation * mRotation * mScale *  glm::translate(mat4(1.0f), -center);
//    currentCenter = point;
//}

//void Mesh::rotate(vec3& rotations, bool positive){
//    changedUniform = true;
//    //May have to translate to center.
//    mat4 allAxes =  mat4(1.0f);
//    float one = 1.0f;
//    if(!positive)
//        one = -1.0f;
//    if(rotations.x != 0.0f)
//        allAxes = glm::rotate(allAxes, rotations.x, vec3(one, 0.0f, 0.0f));
//    if(rotations.y != 0.0f)
//        allAxes = glm::rotate(allAxes, rotations.y, vec3(0.0f, one, 0.0f));
//    if(rotations.z != 0.0f)
//        allAxes = glm::rotate(allAxes, rotations.z, vec3(0.0f, 0.0f, one));
//        mRotation = allAxes * mRotation;
//        mTransformation = mRotation * mTransformation;
//}

//void Mesh::translateOrigin(){
//    mTransformation = mRotation * mScale;
//}

// void Mesh::translateBack(){
//    mTransformation = mTranslation * mRotation * mScale;
//}

////Accumulates rotation
////Scale, translate to center, rotate new, rotate old, translate back to object origin, translate to original location
//void Mesh::rotateSelf(vec3& rotations, bool positive){
//    mat4 tempRotations = mRotation;
//    mRotation = mat4(1.0f); //clear rotations
//    rotate(rotations, positive); //apply new rotations
//    mTransformation =  mTranslation * glm::translate(mat4(1.0f), center) * tempRotations
//     * mRotation *  mScale * glm::translate(mat4(1.0f), -center);
//    mRotation = tempRotations * mRotation; //restore rotation. this accumulates.
//}

////Rotates absolutely.
//void Mesh::rotateCenteredTo(vec3& rotations, bool positive){
//    mat4 tempRotations = mRotation;
//    mRotation = mat4(1.0f); //clear rotations
//    rotate(rotations, positive); //apply new rotations
//    mTransformation =  mTranslation * glm::translate(mat4(1.0f), center) * tempRotations
//     * mRotation *  mScale * glm::translate(mat4(1.0f), -center);
//    mRotation = tempRotations;
//}

////only allow uniform scaling for now
//void Mesh::scale(float scaling){
//    mat4 scale(1.0f);
//    scale = glm::scale(scale, vec3(scaling, scaling, scaling));
//    mTransformation = scale * mTransformation;
//    mScale = scale * mScale;
//    size = mScale * size;
//}


//void Mesh::scaleCenterUniform(float scaling){
//    mat4 scale(1.0f);
//    scale = glm::scale(scale, vec3(scaling, scaling, scaling));
//    mTransformation = mTranslation * glm::translate(mat4(1.0f), center) * mRotation * scale * mScale * glm::translate(mat4(1.0f), -center);
//    mScale = scale * mScale;
//}

//void Mesh::anchorBottom(){
//    center = vec3(minCoords.x, minCoords.y, center.z);
//}
//void Mesh::scaleCenter(vec3& scaleFactor){
//    mat4 scale(1.0f);
//    scale = glm::scale(scale, scaleFactor);
//    mTransformation = mTranslation * glm::translate(mat4(1.0f), center) * mRotation * scale * mScale * glm::translate(mat4(1.0f), -center);
//    mScale = scale * mScale;
//    size = mScale * size;
//}

////Change indices of subroutines.
//void Mesh::setLighting(LightingType lighting){
//    switch(lighting){
//        case NORMAL_MODE:
//            subroutines[0] = normalModeIndex;
//            break;
//        case COLOR_ID:
//            subroutines[0] = colorKeyIndex;
//            break;
//    }
//}



//Mesh::~Mesh(){
//    //TODO, delete pointers
//    glDeleteVertexArrays(1, vao);
//    glDeleteBuffers(1, &vertexBuffer);
//}

//------------------ Utilities ------------------------
