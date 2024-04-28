#include "shader/Shader.h"
#include "Sprite.h"
#include "trabalhoGA/Bird.h"
#include "trabalhoGA/Pipe.h"

class Game {
  public:
    Game();
    ~Game();
    void initialize();
    void start();
    void end();
    void reset();

    void createBackground();
    void createPlayer();
    void createPipes();
    void createPipePair(float xPositon);
    void checkPassingThroughPipes(glm::vec3 birdPosition, Pipe *pipe);
    void checkCollisionTopBottom();
    void renderScore();
    void initialAnimation();
  
  private:
    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
    
    GLuint loadTexture(string texturePath);
    GLFWwindow *window;
    Shader* shader;
    Sprite* background;
    Bird* bird;
    vector<Pipe*> pipes;
    float pipeGap = 130.0;
    float pipeWidth = 65.0;
    float birdWidth = 40.0;
    float birdHeight = 40.0;
    
    double animationLastTime;
    double animationInterval = 0.1;
    double animationMaxTime = 2.0;
    int score = 0;
};