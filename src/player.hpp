#pragma once

#include <raylib.h>

class Player {
protected:
  const Vector2 hitbox;

public:
  int health;
  Vector2 position;

  Player();
  void GetMovement();
  void Draw();
};
