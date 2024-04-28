#pragma once
#include "trabalhoGA/Sprite.h"

class Bird {
  public: 
    Bird(Sprite *sprite); 
    ~Bird();

    Sprite *sprite;

    void moveUp();
    void moveDown();
    void rotateUp();
    void rotateDown();
    void draw(bool hasStarted, bool isReadyToPlay,bool isGameOver);
    glm::vec3 getPosition() const { return this->sprite->pos; }

  private:
    double lastTime;
    double interval = 0.1;
};