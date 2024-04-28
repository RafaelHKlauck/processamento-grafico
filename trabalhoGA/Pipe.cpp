#include "trabalhoGA/Pipe.h"

Pipe::Pipe(Sprite *topSprite, Sprite *bottomSprite) {
  this -> topSprite = topSprite;
  this -> bottomSprite = bottomSprite;
}

Pipe::~Pipe() {
  delete topSprite;
  delete bottomSprite;
}

void Pipe::draw(int currentScore, bool isGameOver) {
  if (isGameOver) {
    topSprite -> draw();
    bottomSprite -> draw();
    return;
  }
  double currentTime = glfwGetTime();
  if (currentTime - lastTime >= interval) {
    movePipeLeft(currentScore);
    lastTime = currentTime;
  }
  topSprite -> draw();
  bottomSprite -> draw();
  if (currentScore > 3) this->interval = 0.6;
  if (currentScore > 6) this->interval = 0.5;
  if (currentScore > 9) this->interval = 0.4;
}

void Pipe::movePipeLeft(int currentScore) {
  if (currentScore > 12) {
    topSprite -> pos.x -= 60;
    bottomSprite -> pos.x -= 60;
    return;
  }
  if (currentScore > 15) {
    topSprite -> pos.x -= 70;
    bottomSprite -> pos.x -= 70;
    return;
  }
  topSprite -> pos.x -= 50;
  bottomSprite -> pos.x -= 50;
}