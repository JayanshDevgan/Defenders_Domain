#include <iostream>
#include "draw.h"
#include "constants.h"

void Draw::grid(int GRID_SIZE, std::string tower_names[], int active_tower, int selection_tower, int active_grid_x, int active_grid_y, bool is_place_mode_active) {
    int name_index = 0;
    std::cout << "\n\n\n\n\n";

    for (int i = 0; i <= 2 * GRID_SIZE; i++) {
        if (i <= 1) {
            top_grid(i, GRID_SIZE, is_place_mode_active, active_grid_x, active_grid_y);
        }
        else {
            if (i % 2 != 0) {
                if (!tower_names[name_index].empty()) {
                    if (name_index == selection_tower && !(is_place_mode_active)) {
                        std::cout << "\t\t\t\033[34m" << tower_names[name_index] << "\t\t\t\t\033[37m";
                        name_index++;
                    }
                    else if (name_index == active_tower) {
                        std::cout << "\t\t\t\033[32m" << tower_names[name_index] << "\t\t\t\t\033[37m";
                        name_index++;
                    }
                    else {
                        std::cout << "\t\t\t" << tower_names[name_index] << "\t\t\t\t";
                        name_index++;
                    }
                }
                else {
                    std::cout << "\t\t\t\t\t\t\t\t";
                }

                for (int j = 0; j <= 2 * GRID_SIZE; j++) {
                    if (j % 2 == 0) {
                        if (is_place_mode_active && ((j / 2 == active_grid_x && i / 2 == active_grid_y) || (j / 2 == active_grid_x + 1 && i / 2 == active_grid_y))) {
                            std::cout << "\033[31m" << BORDER_WALL_Y << "\033[37m";
                        } else {
                            std::cout << BORDER_WALL_Y;
                        }
                    }
                    else {
                        std::cout << EMPTY;
                    }
                }
            }
            else {
                bottom_grid(i, GRID_SIZE, is_place_mode_active, active_grid_x, active_grid_y);
            }
        }
        std::cout << '\n';
    }
}

void Draw::top_grid(int i, int GRID_SIZE, bool is_place_mode_active, int active_grid_x, int active_grid_y) {
    std::cout << "\t\t\t\t\t\t\t\t";

    if (i % 2 != 0) {
        for (int j = 0; j <= 2 * GRID_SIZE; j++) {
            if (j % 2 == 0) {
                if (is_place_mode_active && ((j / 2 == active_grid_x && i / 2 == active_grid_y) || (j / 2 == active_grid_x + 1 && i / 2 == active_grid_y))) {
                    std::cout << "\033[31m" << BORDER_WALL_Y << "\033[37m"; 
                } else {
                    std::cout << BORDER_WALL_Y;
                }
            } else {
                std::cout << EMPTY;
            }
        }
    } else {
        for (int j = 0; j <= 2 * GRID_SIZE; j++) {
            if (j % 2 == 0) {
                if (is_place_mode_active && ((j / 2 == active_grid_x && i / 2 == active_grid_y) || (j / 2 == active_grid_x + 1 && i / 2 == active_grid_y + 1) || (j / 2 == active_grid_x && i / 2 == active_grid_y + 1) || (j / 2 == active_grid_x + 1 && i / 2 == active_grid_y))) {
                    std::cout << "\033[31m" << BORDER_CORNER << "\033[37m";
                } else {
                    std::cout << BORDER_CORNER;
                }
            } else {
                if (is_place_mode_active && ((j / 2 == active_grid_x && i / 2 == active_grid_y) || (j / 2 == active_grid_x && i / 2 == active_grid_y + 1))) {
                    std::cout << "\033[31m" << BORDER_WALL_X << "\033[37m";
                } else {
                    std::cout << BORDER_WALL_X;
                }
            }
        }
    }
}

void Draw::bottom_grid(int i, int GRID_SIZE, bool is_place_mode_active, int active_grid_x, int active_grid_y) {
    std::cout << "\t\t\t\t\t\t\t\t";

    for (int j = 0; j <= 2 * GRID_SIZE; j++) {
        if (j % 2 == 0) {
            if (is_place_mode_active && ((j / 2 == active_grid_x && i / 2 == active_grid_y) || (j / 2 == active_grid_x + 1 && i / 2 == active_grid_y + 1) || (j / 2 == active_grid_x && i / 2 == active_grid_y + 1) || (j / 2 == active_grid_x + 1 && i / 2 == active_grid_y + 1))) {
                std::cout << "\033[31m" << BORDER_CORNER << "\033[37m"; 
            } else {
                std::cout << BORDER_CORNER;
            }
        } else {
            if (is_place_mode_active && ((j / 2 == active_grid_x && i / 2 == active_grid_y) || (j / 2 == active_grid_x && i / 2 == active_grid_y + 1))) {
                std::cout << "\033[31m" << BORDER_WALL_X << "\033[37m"; 
            } else {
                std::cout << BORDER_WALL_X;
            }
        }
    }
}
