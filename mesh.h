#ifndef MESH_H
#define MESH_H

#include <memory>
#include <vector>
#include <iostream>

#include "gl_includes.h"

class Mesh
{
public:
    void init();
    void initSphere();     
    void initVerticesSphere();
    void initTrianglesSphere();  
    void initCircleSphere(float theta);                                                   // should properly set up the geometry buffer
    void render(GLuint g_program);                                                        // should be called in the main rendering loop
    static std::shared_ptr<Mesh> genSphere(const size_t resolution = 16); // should generate a unit sphere
    

    //setter and getter for model matrix
    void setModelMatrix(glm::mat4 modelMatrix) {this->modelMatrix = modelMatrix;}
    glm::mat4 getModelMatrix() {return modelMatrix;}

    //Setter for isLigthed variable
    void setIsLighted(bool isLighted) {this->isLighted = isLighted;}

    //Setter for planet color
    void setPlanetColor(glm::vec3 planetColor) {this->planetColor = planetColor;}
    
    //Getter of texture coordinates
    std::vector<float> getMVertexCoords() {return m_vertexTexCoords;}

private:
    glm::mat4 modelMatrix = glm::mat4(1.0);
    std::vector<float> m_vertexPositions;
    std::vector<float> m_vertexNormals;
    std::vector<unsigned int> m_triangleIndices;
    GLuint m_vao = 0;
    GLuint m_posVbo = 0;
    GLuint m_normalVbo = 0;
    GLuint m_texCoordVbo = 0;
    GLuint m_ibo = 0;
    bool isLighted = true;
    glm::vec3 planetColor = glm::vec3(1.0);
    std::vector<float> m_vertexTexCoords;

};

#endif //MESH_H