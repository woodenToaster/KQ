#ifndef KQ_MAP_H
#define KQ_MAP_H

#include <map>
#include <string>
#include <vector>
#include "Enemy.h"
#include "Hero.h"
#include "Rectangle.h"
#include "SDL.h"

class Map {
public:
  static const int SCREEN_WIDTH = 320;
  static const int SCREEN_HEIGHT = 240;
  static const int TILE_SIZE = 8;
  static const int NUM_X_TILES = SCREEN_WIDTH / TILE_SIZE;
  static const int NUM_Y_TILES = SCREEN_HEIGHT / TILE_SIZE;

  struct tile {
    SDL_Surface* surface;
    Rectangle* location;
  };

  struct obstruction {
    SDL_Surface* surface;
    Rectangle* location;
  };

  Map(SDL_Window* window, Hero* hero);
  ~Map();

  SDL_Surface* getMapSurface();

  Hero* getHero() const;

  void putHeroOnMap(Hero* hero);
  void putEnemyOnMap(Enemy* enemy);

  void loadTileDataFromFile(std::string& filename);

  void makeTiles();
  void makeObstructions();

  void drawTiles();
  void drawTile(tile* t);

  void drawObstructions();
  void drawObstruction(obstruction* o);

  void destroyTiles();
  void destroyObstructions();

  bool isObstruction(Rectangle* destination);

  bool upIsObstruction(Rectangle* location);
  bool downIsObstruction(Rectangle* location);
  bool leftIsObstruction(Rectangle* location);
  bool rightIsObstruction(Rectangle* location);

  bool upLeftIsObstruction(Rectangle* location);
  bool upRightIsObstruction(Rectangle* location);
  bool downLeftIsObstruction(Rectangle* location);
  bool downRightIsObstruction(Rectangle* location);

private:
  static std::map<std::string, SDL_Surface*> tileImages;

  std::vector<tile> tiles;
  std::vector<obstruction> obstructions;

  SDL_Surface* mapSurface;

  //TODO: This is only temporary
  Hero* hero;

};

#endif