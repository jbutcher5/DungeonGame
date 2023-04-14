#include "tile.hpp"
#include <raylib.h>

void Tile::Draw() {
  DrawRectangle(position.x, position.y, hitbox.x, hitbox.y, BLUE);
}
