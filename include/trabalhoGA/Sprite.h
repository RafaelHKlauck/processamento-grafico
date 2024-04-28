#ifndef SPRITE_H
#define SPRITE_H

#include "shader/Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Sprite {
  public: 
    Sprite() {} // Constructor
    ~Sprite(); // Destructor
    void initialize(GLuint textId, glm::vec3 pos = glm::vec3(0.0, 0.0, 0.0), glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0), float angle = 0.0); 
    void draw();
    void finalize();
    void setShader(Shader *shader) { this->shader = shader; }

    
    // Variaveis com as infos para aplicar as transformações no objeto
    glm::vec3 pos, scale;
    float angle;


  protected:
    void update();
    GLuint VAO; // identificador do buffer de geometria, indicando os atributos dos vértices
    GLuint texID; // identificador do buffer de textura

    // Uma referencia ao programa de shader que a sprite ira usar para seu desenho
    Shader *shader;
};

#endif