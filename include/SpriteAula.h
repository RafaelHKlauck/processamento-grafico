#include "shader/Shader.h"
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class SpriteAula {
  private:
    double lastTime; // Último tempo registrado
    double interval = 0; // Intervalo desejado em segundos
    
  public:
    SpriteAula() {} // Constructor
    ~SpriteAula(); // Destructor
    void initialize(GLuint textId, glm::vec3 pos = glm::vec3(0.0, 0.0, 0.0), glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0), float angle = 0.0); 
    void draw();
    void finalize();
    void setMovementInterval(double interval) { this->interval = interval; }
    void moveRight();
    void moveUp();
    void moveDown();
    void setShader(Shader *shader) { this->shader = shader; }


  protected:
    void update();
    void rotateUp();
    void rotateDown();
    GLuint VAO; // identificador do buffer de geometria, indicando os atributos dos vértices
    GLuint texID; // identificador do buffer de textura

    // Variaveis com as infos para aplicar as transformações no objeto
    glm::vec3 pos, scale;
    float angle;

    // Uma referencia ao programa de shader que a sprite ira usar para seu desenho
    Shader *shader;
};