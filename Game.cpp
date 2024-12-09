#include <conio.h>
#include <iostream>
#include <future>
#include <windows.h>
#include <cstdlib>
#include <time.h>
#include <tuple>
#include <chrono>
#include <ctime>
#include <cmath>
#include "Game.h"
#include "draw.h"
#include "bullet.h"
#include "constants.h"

std::vector<Bullet> bullets;

std::string tower_names[] = {"Electric Tower", "Fire Tower", "Poison Tower", "Water Tower", "Ice Tower", "Wind Tower", "Shadow Tower"};

std::string colors[] = {"\033[38;5;214m", "\033[38;5;196m", "\033[38;5;93m", "\033[38;5;39m", "\033[38;5;81m", "\033[38;5;159m", "\033[38;5;23m"};

int number_of_towers = sizeof(tower_names) / sizeof(tower_names[0]);
int placed_towers_count = 0;

std::vector<TowerPositionDataClass> placed_towers;

Draw draw;

int Game::Run()
{
  // PlaySound(Audio::BGM, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
  system("cls");
  draw.game_name();
  hide_cursor();

  float deltaTime = 0.1f;
  float currentSpawnInterval = 3.0f;
  float timeSinceLastSpawn = 0.0f;

  int active_tower = 0, selection_tower = 0;
  bool is_place_mode_active = false;
  int active_grid_x = 0, active_grid_y = 0;

  char input;
  int GRID_SIZE, choice;
  std::tie(GRID_SIZE, choice) = calculate_grid();
  TowerPositionData TowerPosition;

  std::srand(static_cast<unsigned int>(std::time(0)));

  auto last_time = std::chrono::steady_clock::now();

  Draw::m_enemy_type = enemy_type();
  Draw::m_enemy_color = enemy_color(choice);

  int door_x = 2 + 2 * (rand() % (GRID_SIZE / 2 - 1)), door_y = 2 + 2 * (rand() % (GRID_SIZE / 2 - 1));

  int color_code = 91 + (rand() % 6);

  while (1)
  {

    ((rand() % 99) % 2 == 0) ? ((rand() % 101) % 2 == 0) ? color_code = 91 + (rand() % 6) : color_code : color_code;

    auto current_time = std::chrono::steady_clock::now();
    float elapsed_time = std::chrono::duration<float>(current_time - last_time).count();

    last_time = current_time;

    timeSinceLastSpawn += elapsed_time;

    if (timeSinceLastSpawn >= currentSpawnInterval)
    {
      Enemy new_enemy;

      new_enemy.type = enemy_type();
      new_enemy.color = enemy_color(choice);

      if (rand() % 2 == 0)
      {
        new_enemy.x = rand() % ((2 * GRID_SIZE));
        new_enemy.y = (rand() % 2 == 0) ? 0 : ((2 * GRID_SIZE));
      }
      else
      {
        new_enemy.x = (rand() % 2 == 0) ? 0 : ((2 * GRID_SIZE));
        new_enemy.y = rand() % ((2 * GRID_SIZE));
      }

      enemies.push_back(new_enemy);

      timeSinceLastSpawn = 0.0f;

      if (currentSpawnInterval > 0.5f)
        currentSpawnInterval -= 0.1f;
    }

    for (auto &tower : TowerPosition)
    {
      tower.shoot(bullets, enemies);
    }

    for (auto it = bullets.begin(); it != bullets.end();)
    {
      it->update();

      if (it->checkCollision())
      {
        it->applyDamage();
        it = bullets.erase(it);
      }
      else
      {
        if (it->isOutOfBounds(GRID_SIZE))
        {
          it = bullets.erase(it);
        }
        else
        {
          ++it;
        }
      }
    }

    clear_screen(GRID_SIZE);
    draw.grid(GRID_SIZE, tower_names, active_tower, selection_tower, active_grid_x, active_grid_y, is_place_mode_active, TowerPosition, enemies, door_x, door_y, color_code, bullets);

    for (Enemy &enemy : enemies)
    {
      auto current_time = std::chrono::steady_clock::now();
      float elapsed_time = std::chrono::duration<float>(current_time - enemy.last_move_time).count();

      int dx = door_x - enemy.x;
      int dy = door_y - enemy.y;

      if (!enemy.hasMoved)
      {
        if (elapsed_time >= 6.0f)
        {
          if (abs(dx) > abs(dy))
          {
            (dx > 0) ? enemy.x++ : enemy.x--;
          }
          else
          {
            (dy > 0) ? enemy.y++ : enemy.y--;
          }
          enemy.hasMoved = true;
          enemy.last_move_time = current_time;

          if (enemy.x == door_y - 1 && enemy.y == door_y - 1)
          {
            draw.lose_game();
          }
        }
      }
      else
      {
        for (int i = 0; i < placed_towers_count; i++)
        {
          shoot_bullets(placed_towers[i], enemy);
        }
        if (elapsed_time >= 5.0f)
        {
          if (abs(dx) > abs(dy))
          {
            (dx > 0) ? enemy.x++ : enemy.x--;
          }
          else
          {
            (dy > 0) ? enemy.y++ : enemy.y--;

            enemy.last_move_time = current_time;

            if (enemy.x == door_y && enemy.y == door_y)
            {
              draw.lose_game();
            }
          }
        }
      }
    }

    for (auto &tower : TowerPosition)
    {
      time_t current_time = time(0);
      if (tower.getLevel() < 4 && (current_time - tower.getLastUpgradeTime()) >= GameConstants::UPGRADE_TIMES[tower.getIndex()][tower.getLevel()])
      {
        tower.setLevel(tower.getLevel() + 1);
        tower.setLastUpgradeTime(current_time);
      }
    }

    if (_kbhit())
    {
      input = _getch();
      switch (input)
      {
      case KeyBindings::Q_KEY:
      case KeyBindings::q_KEY:
        toggle_state(is_place_mode_active);
        break;
      case KeyBindings::J_KEY:
      case KeyBindings::j_KEY:
      case KeyBindings::W_KEY:
      case KeyBindings::w_KEY:
      case KeyBindings::UP_ARROW:
        Move(is_place_mode_active, "UP", active_grid_y, active_grid_x, selection_tower, number_of_towers, GRID_SIZE);
        break;
      case KeyBindings::K_KEY:
      case KeyBindings::S_KEY:
      case KeyBindings::s_KEY:
      case KeyBindings::DOWN_ARROW:
        Move(is_place_mode_active, "DOWN", active_grid_y, active_grid_x, selection_tower, number_of_towers, GRID_SIZE);
        break;
      case KeyBindings::H_KEY:
      case KeyBindings::h_KEY:
      case KeyBindings::a_KEY:
      case KeyBindings::A_KEY:
      case KeyBindings::LEFT_ARROW:
        Move(is_place_mode_active, "LEFT", active_grid_y, active_grid_x, selection_tower, number_of_towers, GRID_SIZE);
        break;
      case KeyBindings::L_KEY:
      case KeyBindings::D_KEY:
      case KeyBindings::d_KEY:
      case KeyBindings::RIGHT_ARROW:
        Move(is_place_mode_active, "RIGHT", active_grid_y, active_grid_x, selection_tower, number_of_towers, GRID_SIZE);
        break;
      case KeyBindings::COL_KEY:
      case KeyBindings::SPACE_KEY:
        if (!is_place_mode_active)
        {
          active_tower = selection_tower;
        }
        else
        {
          if (!Draw::is_tower_placed(active_grid_x * 2, active_grid_y * 2, TowerPosition))
          {
            calculate_tower_positions(GRID_SIZE, active_tower, active_grid_x, active_grid_y, TowerPosition);
            placed_towers_count++;
          }
        }
        break;
      case KeyBindings::ESC_KEY:
        display_tower_positions(TowerPosition);
        PlaySound(NULL, NULL, 0);
        return 0;
      }
    }
  }
  return 0;
}

void Game::toggle_state(bool &is_place_mode_active) { is_place_mode_active = !is_place_mode_active; }

void Game::Move(bool is_place_mode_active, std::string direction, int &active_grid_y, int &active_grid_x, int &selection_tower, int number_of_towers, int GRID_SIZE)
{
  if (is_place_mode_active)
  {
    if (direction == "UP")
    {
      (active_grid_y > 0) ? active_grid_y-- : active_grid_y = GRID_SIZE - 1;
    }
    if (direction == "DOWN")
    {
      active_grid_y++;
      active_grid_y = active_grid_y % GRID_SIZE;
    }
    if (direction == "LEFT")
    {
      (active_grid_x > 0) ? active_grid_x-- : active_grid_x = GRID_SIZE - 1;
    }
    if (direction == "RIGHT")
    {
      active_grid_x++;
      active_grid_x = active_grid_x % GRID_SIZE;
    }
  }
  else
  {
    if (direction == "UP")
    {
      selection_tower = selection_tower % number_of_towers;
      (selection_tower > 0) ? selection_tower-- : selection_tower = number_of_towers - 1;
    }
    if (direction == "DOWN")
    {
      selection_tower++;
      selection_tower = selection_tower % number_of_towers;
    }
  }
}

int Game::enemy_type()
{
  return (rand() % (GameConstants::MAX_ENEMIES - 63) + 64);
}

std::string Game::enemy_color(int choice)
{
  return (choice > 5) ? colors[rand() % sizeof(colors) / sizeof(colors[0])] : (choice > 1) ? colors[rand() % (choice - 1)]
                                                                                           : colors[0];
}

void Game::hide_cursor()
{
  HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

  CONSOLE_CURSOR_INFO cursor_info;

  GetConsoleCursorInfo(out, &cursor_info);
  cursor_info.bVisible = false;
  SetConsoleCursorInfo(out, &cursor_info);
}

void Game::clear_screen(int GRID_SIZE)
{
  static bool is_cleared = false;
  std::cout << "\033[26;H";
  if (!is_cleared)
  {
    std::cout << "\033[J";
    is_cleared = true;
  }
  std::cout << '\r';
}

std::tuple<int, int> Game::calculate_grid()
{
  int choice;
  int GRID_SIZE;
  std::cout << "\n\n\n\nENTER THE GRID SIZE \n\t(0 -> 2x2)\n\t(1 -> 3x3)\n\t(2 -> 4x4)\n\t(3 -> 5x5)\n\t(4 -> 6x6)\n\t(5 -> 8x8)\n\t(6 -> 10x10)\n\t(7 -> CUSTOM GRID SIZE)\n\t(8 -> EXIT)\n";
  std::cout << "Enter Your Choice: ";
  std::cin >> choice;

  int grid_options[] = {2, 3, 4, 5, 6, 8, 10};

  if (choice >= 0 && choice <= 6)
    GRID_SIZE = grid_options[choice];

  else if (choice == 7)
  {
    int size;
    std::cout << "Enter Grid Size: ";
    std::cin >> size;
    GRID_SIZE = size;
  }

  else
    return std::make_tuple(0, choice);

  // system("cls");

  return std::make_tuple(GRID_SIZE, choice);
}

void Game::calculate_tower_positions(int GRID_SIZE, int active_tower, int active_grid_x, int active_grid_y, TowerPositionData &TowerPosition)
{
  Tower tower(active_tower);
  TowerPositionDataClass newTowerPosition(active_tower, active_grid_x, active_grid_y, tower);
  newTowerPosition.setLastUpgradeTime(time(0));

  TowerPosition.push_back(newTowerPosition);
  placed_towers_list.push_back(newTowerPosition);
}

void Game::display_tower_positions(const TowerPositionData &TowerPosition)
{
  std::cout << "Tower Posistions: \n";
  for (const auto &tower : TowerPosition)
  {
    std::cout << "Tower: " << tower.getIndex() << ", Coord: [" << tower.getX() << ", " << tower.getY() << "]" << std::endl;
  }
}

int Game::calculate_enemy_tower_distance(TowerPositionDataClass &tower, Enemy &enemy)
{
  int distance_x = tower.getX() - enemy.x;
  int distance_y = tower.getY() - enemy.y;
  return sqrt((pow(distance_x, 2)) + pow(distance_y, 2)); // Euclidean distance formula
}

void Game::shoot_bullets(TowerPositionDataClass &tower, Enemy &enemy)
{
  if (calculate_enemy_tower_distance(tower, enemy) <= GameConstants::RANGE)
  {
    for (int i = 0; i < 5; i++)
    {
      Bullet bullet(tower.getX(), tower.getY(), &enemy, *(&enemy.health));
      bullet.update();

      if (bullet.checkCollision())
      {
        bullet.applyDamage();
        break;
      }
    }
  }
}
