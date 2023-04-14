#include "wfc.hpp"
#include <cstdlib>
#include <ctime>
#include <vector>

class WaveFunction {
public:
  std::vector<int> combinations;
  int Entropy() { return combinations.size(); };
  WaveFunction(int max) {
    for (int n = 0; n <= max; n++)
      combinations[n] = n;
  }
};

struct Location {
  int x;
  int y;

  Location(int x, int y) : x(x), y(y){};
  Location() : Location(0, 0){};
};

class TileGeneration {
protected:
  WaveFunction **buffer;

  int **input;
  const int input_width;
  const int input_height;

  const int output_width;
  const int output_height;

  std::vector<Location> *id_locations;

  bool generation_complete = false;

  WaveFunction *GetTile(Location l) { return buffer[l.x] + l.y; }

  Location GetMinEntropy() {
    Location current_lowest = Location();
    int current;

    bool found_index = false;

    for (int i = 0; i < output_width; i++) {
      for (int j = 0; j < output_height; j++) {
        current = buffer[i][j].Entropy();

        if (current == 2)
          return Location(i, j);
        else if (current < GetTile(current_lowest)->Entropy() && current > 1) {
          current_lowest = Location(i, j);

          found_index = true;
        }
      }
    }

    generation_complete = !found_index;
    return current_lowest;
  };

public:
  TileGeneration(int output_width, int output_height, int max_id, int **input,
                 int input_width, int input_height)
      : output_width(output_width), output_height(output_height),
        input_width(input_width), input_height(input_height), input(input) {
    buffer = (WaveFunction **)calloc(output_width, sizeof(void *));

    for (int i = 0; i < output_width; i++) {
      buffer[i] = (WaveFunction *)calloc(output_height, sizeof(WaveFunction));

      for (int j = 0; j < output_height; j++)
        buffer[i][j] = WaveFunction(max_id);
    }

    id_locations = (std::vector<Location> *)calloc(
        max_id + 1, sizeof(std::vector<Location>));

    for (int i = 0; i < input_width; i++) {
      for (int j = 0; j < input_height; j++) {
        id_locations[input[i][j]].push_back(Location(i, j));
      }
    }
  }

  void UpdateAdjTiles(Location l) {
    std::vector<int> new_combinations[4] = {{}, {}, {}, {}};
    Location offset[4] = {Location(0, 1), Location(0, -1), Location(1, 0),
                          Location(-1, 0)};

    WaveFunction *tile = GetTile(l);
    std::vector<int> c = tile->combinations;
    std::vector<Location> input_id_locations = {};

    for (int i = 0; i < c.size(); i++)
      for (Location id_l : id_locations[c[i]])
        input_id_locations.push_back(id_l);

    for (int adj_tile = 0; adj_tile < 4; adj_tile++) {
      for (int i = 0; i < input_id_locations.size(); i++) {
        Location adjacent_location = input_id_locations[i];
        adjacent_location.x += offset[adj_tile].x;
        adjacent_location.y += offset[adj_tile].y;

        if (adjacent_location.x < 0 || adjacent_location.x >= input_width ||
            adjacent_location.y < 0 || adjacent_location.y >= input_height)
          continue;

        new_combinations[adj_tile].push_back(
            input[adjacent_location.x][adjacent_location.y]);
      }

      // TODO: Maybe remove new_combinations and just have a local variabled in
      // the for loop

      GetTile(Location(l.x + offset[adj_tile].x, l.y + offset[adj_tile].y))
          ->combinations = new_combinations[adj_tile];
    }
  }

  void CollapseTile() {
    Location l = GetMinEntropy();
    WaveFunction *tile = GetTile(l);
    int i = rand() % tile->Entropy();
    tile->combinations = {tile->combinations[i]};

    UpdateAdjTiles(l);
  }

  int **Generate() {
    while (!generation_complete)
      CollapseTile();

    int **output = (int **)calloc(output_width, sizeof(int *));

    for (int i = 0; i < output_height; i++)
      output[i] = (int *)calloc(output_height, sizeof(int));

    for (int i = 0; i < output_width; i++)
      for (int j = 0; j < output_height; j++)
        output[i][j] = buffer[i][j].combinations[0];

    return output;
  }

  ~TileGeneration() {
    for (int i = 0; i < output_width; i++) {
      free(buffer[i]);
    }

    free(buffer);
  }
};
