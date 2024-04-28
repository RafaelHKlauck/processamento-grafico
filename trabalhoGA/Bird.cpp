#include <iostream>
#include "trabalhoGA/Bird.h"
#include "trabalhoGA/Sprite.h"

Bird::Bird(Sprite *sprite) {
  this -> sprite = sprite;
}

void Bird::draw(bool hasStarted, bool isReadyToPlay, bool isGameOver) {
  if (!hasStarted || !isReadyToPlay || isGameOver) {
    sprite -> draw();
    return;
  }

  double currentTime = glfwGetTime();
  if (currentTime - lastTime >= interval) {
    moveDown();
    lastTime = currentTime;
  }
  sprite -> draw();
}

void Bird::moveUp() {
  sprite -> pos.y += 7;
  rotateUp();
}

void Bird::rotateUp() {
  float currentAngle = sprite -> angle;
  if (currentAngle >= 40) return; 
  if (currentAngle + sprite -> angle > 40) {
    sprite -> angle = 40;
    return;
  }
  sprite -> angle += 20;
}

void Bird::moveDown() {
  if (this->sprite->angle >= 30) sprite -> pos.y -= 10;
  else if (this->sprite->angle >= 20) sprite -> pos.y -= 20;
  else if (this->sprite->angle >= 10) sprite -> pos.y -= 30;
  else if (this->sprite->angle >= 0) sprite -> pos.y -= 35;
  else if (this->sprite->angle <= -10) sprite -> pos.y -= 40;
  else if (this->sprite->angle <= -20) sprite -> pos.y -= 45;
  else if (this->sprite->angle <= -30) sprite -> pos.y -= 50;
  else sprite -> pos.y -= 60;
  rotateDown();
}

void Bird::rotateDown() {
  float currentAngle = sprite -> angle;
  if (currentAngle <= -40) return;
  if (currentAngle - sprite -> angle < -40) {
    sprite -> angle = -40;
    return;
  }
  sprite -> angle -= 7;
}