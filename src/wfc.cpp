#include "wfc.hpp"
#include <cstdlib>
#include <vector>

const Location offsets[4] = {Location(0, 1), Location(1, 0), Location(0, -1),
                             Location(-1, 0)};

Location TileGeneration::GetMinEntropy() {
  Location current_lowest = Location();
  int current;

  for (int i = 0; i < output_width; i++) {
    for (int j = 0; j < output_height; j++) {
      current = buffer[i][j].Entropy();

      int lowest_entropy = GetTile(current_lowest)->Entropy();

      if ((current < lowest_entropy && current > 1) || lowest_entropy == 1)
        current_lowest = Location(i, j);
    }
  }

  return current_lowest;
};

bool TileGeneration::GenerationComplete() {
  for (int i = 0; i < output_width; i++)
    for (int j = 0; j < output_height; j++)
      if (buffer[i][j].Entropy() > 1)
        return false;

  return true;
}

TileGeneration::TileGeneration(int output_width, int output_height, int max_id,
                               int **input, int input_width, int input_height)
    : output_width(output_width), output_height(output_height),
      input_width(input_width), input_height(input_height), input(input) {
  buffer = (WaveFunction **)calloc(output_width, sizeof(void *));

  for (int i = 0; i < output_width; i++) {
    buffer[i] = (WaveFunction *)calloc(output_height, sizeof(WaveFunction));

    for (int j = 0; j < output_height; j++)
      buffer[i][j] = WaveFunction(max_id);
  }

  id_locations = (std::vector<Location> *)calloc(max_id + 1,
                                                 sizeof(std::vector<Location>));

  for (int i = 0; i < input_width; i++) {
    for (int j = 0; j < input_height; j++) {
      id_locations[input[i][j]].push_back(Location(i, j));
    }
  }
}

int TileGeneration::UpdateTile(Location l, const Location comparee,
                               Location direction) {
  if (l.x < 0 || l.x >= output_width || l.y < 0 || l.y >= output_height)
    return 1;

  WaveFunction *current = GetTile(l);
  const WaveFunction *other = GetTile(comparee);

  std::vector<int> new_combination;

  for (int n : other->combinations) {
    for (Location id_location : id_locations[n]) {
      id_location.x += direction.x;
      id_location.y += direction.y;

      if (id_location.x < 0 || id_location.x >= input_width ||
          id_location.y < 0 || id_location.y >= input_height)
        continue;

      int new_state = input[id_location.x][id_location.y];

      bool is_in_new_combinations = false;
      bool is_in_combinations = false;

      // TODO: Replace with sort and binary search

      for (int state : new_combination)
        if (state == new_state) {
          is_in_new_combinations = true;
          break;
        }

      for (int state : current->combinations)
        if (state == new_state) {
          is_in_combinations = true;
          break;
        }

      if (!is_in_new_combinations && is_in_combinations) {
        new_combination.push_back(new_state);
      }
    }
  }

  // TODO: Handle collisions rather than brute forcing them

  if (!new_combination.size())
    return 2;

  current->combinations = new_combination;
  current->updated = true;

  for (int i = 0; i < 4; i++) {
    Location next_tile = l;
    next_tile.x += offsets[i].x;
    next_tile.y += offsets[i].y;

    if (next_tile.x < 0 || next_tile.x >= output_width || next_tile.y < 0 ||
        next_tile.y >= output_height)
      continue;

    WaveFunction *next = GetTile(next_tile);

    if (!next->updated) {
      UpdateTile(next_tile, l, offsets[i]);
    }
  }

  return 0;
}

void TileGeneration::CollapseTile() {
  int i = 0;

  Location l;

  if (!max_entropy)
    l = GetMinEntropy();

  else {
    l.x = rand() % output_width;
    l.y = rand() % output_height;

    max_entropy = false;
  }

  WaveFunction *tile = GetTile(l);
  int n = rand() % tile->Entropy();

  tile->combinations = {tile->combinations[n]};
  tile->updated = true;

RETRY:
  Location next_tile = l;
  next_tile.x += offsets[i].x;
  next_tile.y += offsets[i].y;

  int result = UpdateTile(next_tile, l, offsets[i]);

  for (int i = 0; i < output_width; i++)
    for (int j = 0; j < output_height; j++)
      GetTile(Location(i, j))->updated = false;

  if (result == 1) {
    i = (i + 1) % 4;
    goto RETRY;
  } else if (result == 2) {
    max_entropy = true;
    CollapseTile();
  }
}

int **TileGeneration::Generate() {
  while (!GenerationComplete())
    CollapseTile();

  int **output = (int **)calloc(output_width, sizeof(int *));

  for (int i = 0; i < output_height; i++)
    output[i] = (int *)calloc(output_height, sizeof(int));

  for (int i = 0; i < output_width; i++)
    for (int j = 0; j < output_height; j++)
      output[i][j] = buffer[i][j].combinations[0];

  return output;
}

TileGeneration::~TileGeneration() {
  for (int i = 0; i < output_width; i++) {
    free(buffer[i]);
  }

  free(buffer);
}
