#include "mesh.h"

#include <iostream>

const static int nbOfCircles = 100;
const static int nbOfVerticesPCircle = 100;

void Mesh::init()
{

    //Pour faire une sphère
    initSphere();
    //init GPU geometry
      // Create a single handle, vertex array object that contains attributes,
  // vertex buffer objects (e.g., vertex's position, normal, and color)
#ifdef _MY_OPENGL_IS_33_
  glGenVertexArrays(1, &m_vao); // If your system doesn't support OpenGL 4.5, you should use this instead of glCreateVertexArrays.
#else
  glCreateVertexArrays(1, &m_vao);
#endif
  glBindVertexArray(m_vao);

  // Generate a GPU buffer to store the positions of the vertices
  size_t vertexBufferSize = sizeof(float)*m_vertexPositions.size(); // Gather the size of the buffer from the CPU-side vector
#ifdef _MY_OPENGL_IS_33_
  glGenBuffers(1, &m_posVbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_posVbo);
  glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, m_vertexPositions.data(), GL_DYNAMIC_READ);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
  glEnableVertexAttribArray(0);
#else
  glCreateBuffers(1, &m_posVbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_posVbo);
  glNamedBufferStorage(m_posVbo, vertexBufferSize, m_vertexPositions.data(), GL_DYNAMIC_STORAGE_BIT); // Create a data storage on the GPU and fill it from a CPU array
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
  glEnableVertexAttribArray(0);
#endif

  //Same for a color buffer
  size_t colorBufferSize = sizeof(float)*m_vertexNormals.size(); // Gather the size of the buffer from the CPU-side vector
#ifdef _MY_OPENGL_IS_33_
  glGenBuffers(1, &m_normalVbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_normalVbo);
  glBufferData(GL_ARRAY_BUFFER, colorBufferSize, m_vertexNormals.data(), GL_DYNAMIC_READ);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
#else
  glCreateBuffers(1, &m_posVbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_posVbo);
  glNamedBufferStorage(m_posVbo, vertexBufferSize, m_vertexPositions.data(), GL_DYNAMIC_STORAGE_BIT); // Create a data storage on the GPU and fill it from a CPU array
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
  glEnableVertexAttribArray(0);
#endif

  // Same for an index buffer object that stores the list of indices of the
  // triangles forming the mesh
  size_t indexBufferSize = sizeof(unsigned int)*m_triangleIndices.size();
#ifdef _MY_OPENGL_IS_33_
  glGenBuffers(1, &m_ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, m_triangleIndices.data(), GL_DYNAMIC_READ);
#else
  glCreateBuffers(1, &g_ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ibo);
  glNamedBufferStorage(g_ibo, indexBufferSize, m_triangleIndices.data(), GL_DYNAMIC_STORAGE_BIT);
#endif



  glBindVertexArray(0); // deactivate the VAO for now, will be activated again when rendering

}

void Mesh::initSphere()
{
    initVerticesSphere();
    initTrianglesSphere();

    for (int i = 0; i<m_vertexPositions.size(); i++) {
      m_vertexNormals.push_back((m_vertexPositions[i]));
    }

}

/**
 * Initializes the coordinates of the vertices and normals
*/
void Mesh::initVerticesSphere() {
for (int i = 0 ; i<nbOfCircles; i++) {
  float theta = -M_PI/2 + i*M_PI/(nbOfCircles-1);
  initCircleSphere(theta);
}
}         

/**
 * Initializes the mesh triangles
*/
void Mesh::initTrianglesSphere() {

for(int i = 0; i<nbOfCircles-1; i++) {
    for(int n=0 ; n<nbOfVerticesPCircle; n++) {
        //TODO : le pb est de ce cote
            m_triangleIndices.push_back(nbOfVerticesPCircle*i+n);
            m_triangleIndices.push_back(nbOfVerticesPCircle*i + (n+1)%nbOfVerticesPCircle);
            m_triangleIndices.push_back(nbOfVerticesPCircle*(i+1)+n);

            m_triangleIndices.push_back(nbOfVerticesPCircle*(i+1)+n);
            m_triangleIndices.push_back(nbOfVerticesPCircle*i+(n+1)%nbOfVerticesPCircle);
            m_triangleIndices.push_back(nbOfVerticesPCircle*(i+1) +(n+1)%nbOfVerticesPCircle);
    }
}


}

/**
 * Initializes a circle of vertexes at an angle theta of the center of the mesh sphere
*/
void Mesh::initCircleSphere(float theta) {
  float height = sin(theta);
  float radius = abs(cos(theta));

  for (int i = 0 ; i< nbOfVerticesPCircle ; i++) {
    float phi = -M_PI + i*2*M_PI/(nbOfVerticesPCircle-1);
    float x = radius * cos(phi);
    float y = radius * sin(phi);

    m_vertexPositions.push_back(x);
    m_vertexPositions.push_back(y);
    m_vertexPositions.push_back(height);
      
  }

}                                                 // should properly set up the geometry buffer

void Mesh::render(GLuint g_program) {
  
  glUniform1i(glGetUniformLocation(g_program, "isLighted"), int(isLighted));

  glUniform3f(glGetUniformLocation(g_program, "planetColor"), planetColor[0], planetColor[1], planetColor[2]);

  glUniformMatrix4fv(glGetUniformLocation(g_program, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

  glBindVertexArray(m_vao);     // activate the VAO storing geometry data
  glDrawElements(GL_TRIANGLES, m_triangleIndices.size(), GL_UNSIGNED_INT, 0); // Call for rendering: stream the current GPU geometry through the current GPU program
}