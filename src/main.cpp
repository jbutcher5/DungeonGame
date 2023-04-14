#include "player.hpp"
#include <cstdlib>
#include <ctime>
#include <raylib.h>

int main(void) {
  InitWindow(1820, 980, "Dungeon Game");
  srand48(time(NULL));

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
