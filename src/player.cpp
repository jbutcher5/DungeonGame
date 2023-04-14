#include "player.hpp"
#include <raymath.h>

Player::Player() : hitbox({40, 80}) {
  health = 100;
  position = {0, 0};
}

void Player::GetMovement() {
  Vector2 player_velocity = {0, 0};

  player_velocity.y -= IsKeyDown(KEY_W);
  player_velocity.y += IsKeyDown(KEY_S);

  player_velocity.x += IsKeyDown(KEY_D);
  player_velocity.x -= IsKeyDown(KEY_A);

  player_velocity.x *= 200 * GetFrameTime();
  player_velocity.y *= 200 * GetFrameTime();

  position = Vector2Add(position, player_velocity);
}

void Player::Draw() {
  DrawRectangle(position.x, position.y, hitbox.x, hitbox.y, RED);
}
