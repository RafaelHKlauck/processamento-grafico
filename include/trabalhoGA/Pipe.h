#pragma once
#include "trabalhoGA/Sprite.h"

class Pipe {
  public:
    Pipe(Sprite *topSprite, Sprite *bottomSprite);
    ~Pipe();

    Sprite *topSprite;
    Sprite *bottomSprite;

    void draw(int currentScore, bool isGameOver);
    void movePipeLeft(int currentScore);

    glm::vec3 getTopPipePosition() { return this->topSprite->pos; }
    glm::vec3 getBottomPipePosition() { return this->bottomSprite->pos; }
    
    bool getHasBirdPassed() const { return hasBirdPassed; }
    void setHasBirdPassed(bool value) { hasBirdPassed = value; }

  private:
    double lastTime;
    double interval = 0.8;
    bool hasBirdPassed = false;
};