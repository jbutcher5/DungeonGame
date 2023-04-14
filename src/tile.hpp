#pragma once
#include <raylib.h>

class Tile {
protected:
  const Vector2 hitbox;
  const Vector2 position;

public:
  Tile(Vector2 hitbox, Vector2 position) : hitbox(hitbox), position(position){};
  void Draw();
};

class GroundTile : protected Tile {
public:
  GroundTile(Vector2 position) : Tile({60, 60}, position){};
};

class WallTile : public Tile {
public:
  WallTile(Vector2 position) : Tile({60, 100}, position){};
};
