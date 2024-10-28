#pragma once

class Draw {
public:
  void grid(int GRID_SIZE, std::string tower_names[], int active_tower, int selection_tower, int active_grid_x, int active_grid_y, bool is_place_mode_active);
private:
  void top_grid(int i, int GRID_SIZE, bool is_place_mode_active, int active_grid_x, int active_grid_y);
  void bottom_grid(int i, int GRID_SIZE, bool is_place_mode_active, int active_grid_x, int active_grid_y);
  void grid_tower_names(int name_index, int selection_tower, int active_tower, bool is_place_mode_active, std::string tower_names[]);
};

