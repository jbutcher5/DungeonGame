#include <raylib.h>
#include <raymath.h>

#include "player.hpp"

int main(void) {
  InitWindow(1820, 980, "Dungeon Game");

  Player player = Player();

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    player.GetMovement();
    player.Draw();

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
