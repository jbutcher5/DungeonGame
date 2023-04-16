#include "player.hpp"
#include "wfc.hpp"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <raylib.h>

int main(void) {
  // InitWindow(1820, 980, "Dungeon Game");

  int seed = time(NULL);

  srand(seed);
  srand48(rand() * seed);

  printf("%d\n", seed);

  Player player = Player();

  int **input = (int **)calloc(6, sizeof(int *));
  input[0] = (int *)calloc(6, sizeof(int));
  input[1] = (int *)calloc(6, sizeof(int));
  input[2] = (int *)calloc(6, sizeof(int));
  input[3] = (int *)calloc(6, sizeof(int));
  input[4] = (int *)calloc(6, sizeof(int));
  input[5] = (int *)calloc(6, sizeof(int));

  input[0][0] = 1;
  input[0][1] = 2;
  input[0][2] = 2;
  input[0][3] = 2;
  input[0][4] = 2;
  input[0][5] = 2;

  input[1][0] = 0;
  input[1][1] = 1;
  input[1][2] = 2;
  input[1][3] = 2;
  input[1][4] = 2;
  input[1][5] = 2;

  input[2][0] = 0;
  input[2][1] = 0;
  input[2][2] = 1;
  input[2][3] = 2;
  input[2][4] = 2;
  input[2][5] = 2;

  input[3][0] = 0;
  input[3][1] = 0;
  input[3][2] = 1;
  input[3][3] = 2;
  input[3][4] = 2;
  input[3][5] = 2;

  input[4][0] = 0;
  input[4][1] = 1;
  input[4][2] = 2;
  input[4][3] = 2;
  input[4][4] = 2;
  input[4][5] = 2;

  input[5][0] = 1;
  input[5][1] = 1;
  input[5][2] = 2;
  input[5][3] = 2;
  input[5][4] = 2;
  input[5][5] = 2;

  TileGeneration t = TileGeneration(24, 20, 2, (int **)input, 6, 6);

  int **output = t.Generate();

  puts("OUTPUT:");

  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 24; j++)
      printf("%d ", output[j][i]);
    puts("");
  }

  /*
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    player.GetMovement();
    player.Draw();

    EndDrawing();
  }
  */

  // CloseWindow();
  return 0;
}
