#pragma once
#include <cstdlib>
#include <ctime>
#include <vector>

struct Location {
  int x;
  int y;

  Location(int x, int y) : x(x), y(y){};
  Location() : Location(0, 0){};
};

class WaveFunction {
public:
  std::vector<int> combinations;
  inline int Entropy() { return combinations.size(); };
  inline WaveFunction(int max) {
    for (int n = 0; n <= max; n++)
      combinations.push_back(n);
  }
  bool updated = false;
  Location comparison;
};

class TileGeneration {
protected:
  WaveFunction **buffer;

  int **input;
  const int input_width;
  const int input_height;

  const int output_width;
  const int output_height;

  const int max_id;

  std::vector<Location> *id_locations;

  std::vector<Location> update_queue;

  bool get_random_tile = true;

  inline WaveFunction *GetTile(Location l) { return buffer[l.x] + l.y; }
  int UpdateTile(Location l, Location comparee);
  void CollapseTile();
  void PopulateQueue(Location initial);
  bool GenerationComplete();
  Location GetMinEntropy();

public:
  TileGeneration(int output_width, int output_height, int max_id, int **input,
                 int input_width, int input_height);

  int **Generate();
  ~TileGeneration();
};
