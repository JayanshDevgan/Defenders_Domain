#pragma once

#ifndef GAME_H
#define GAME_H

#include <windows.h>
#include <vector>
#include <chrono>
#include <cmath>


class Enemy {
public:
  int type;
  int health;
  std::string color;
  int x;
  int y;
  int hasMoved = false;
  std::chrono::steady_clock::time_point last_move_time = std::chrono::steady_clock::now();

  Enemy() : type(0), health(type), color("default"), x(x), y(y) {}

  Enemy(int initialHealth, int enemyType)
    : type(enemyType), health(enemyType), color("default"), x(0), y(0) {}

  void takeDamage(int damage) {
    health -= damage;
  }
};

#include "bullet.h"

class Tower {
public:
  Tower(int index)
    : index(index) {}

  void shoot(std::vector<Bullet>& bullets, std::vector<Enemy>& enemies) {
    for (Enemy& enemy : enemies) {
      if (isEnemeyInRange(enemy)) {
        int damage = calculateDamage(enemy.type);
        bullets.emplace_back(x, y, &enemy, damage);
        break;
      }
    }
  }

  void setPosition(int x, int y) {
    this->x = x;
    this->y = y;
  }

private:
  int index;
  int x,y;

  bool isEnemeyInRange(const Enemy& enemy) {
    int range = 5 - index;
    int distance = static_cast<int>(std::hypot(enemy.x - x, enemy.y - y));
    return distance <= range;
  }

  int calculateDamage(int enemyType) {
    switch (index) {
      case 0:
        return (enemyType == 1) ? 10 : 5;
      case 1:
        return (enemyType == 0) ? 0 : (enemyType == 1) ? 5 : 2;
      case 2:
        return (enemyType == 0) ? 10 : (enemyType == 1) ? 7 : 3;
      case 3:
        return (enemyType == 0) ? 5 : (enemyType == 1) ? 10 : 4;
      default:
        return 1;
    }
  }
};

class TowerPositionDataClass {
public:
    TowerPositionDataClass(int idx, int posX, int posY, Tower& tower)
        : index(idx), x(posX), y(posY), level(0), last_upgrade_time(0), tower(tower) {}

    void shoot(std::vector<Bullet>& bullets, std::vector<Enemy>& enemies) {
      tower.shoot(bullets, enemies);
    }

    int getIndex() const { return index; }
    int getX() const { return x; }
    int getY() const { return y; }
    int getLevel() const { return level; }
    time_t getLastUpgradeTime() const { return last_upgrade_time; }

    void setLevel(int lvl) { level = lvl; }
    void setLastUpgradeTime(time_t time) { last_upgrade_time = time; }

private:
    int index;
    int x;
    int y;
    int level;
    time_t last_upgrade_time;
    Tower tower;
};

using TowerPositionData = std::vector<TowerPositionDataClass>;

class Game {
public:
  int Run();
  void toggle_state(bool& is_place_mode_active);
  void Move(bool is_place_mode_active, std::string direction, int& active_grid_y, int &active_grid_x, int &selection_tower, int number_of_towers, int GRID_SIZE);
  static void hide_cursor();
  void clear_screen(int GRID_SIZE);
  std::tuple<int, int> calculate_grid();
  void calculate_tower_positions(int GRID_SIZE, int active_tower, int active_grid_x, int active_grid_y, TowerPositionData& TowerPosition);
  void display_tower_positions(const TowerPositionData& TowerPosition);
  int Game:: calculate_enemy_tower_distance(TowerPositionDataClass& tower,Enemy& enemy );
  void Game:: shoot_bullets(TowerPositionDataClass& tower,Enemy& enemy);

private:
  std::vector<TowerPositionDataClass> placed_towers_list;
  std::vector<Enemy> enemies;
  int enemy_type();
  std::string enemy_color(int choice);
};

#endif
