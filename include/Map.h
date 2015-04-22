#ifndef KQ_MAP_H
#define KQ_MAP_H

#include <vector>
#include "Hero.h"
#include "Rectangle.h"
#include "SDL.h"

class Map {
public:
  static const int SCREEN_WIDTH = 320;
  static const int SCREEN_HEIGHT = 240;
  static const int TILE_SIZE = 8;

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

  Hero* getHero();

  void makeTiles();
  void makeObstructions();

  void drawTiles();
  void drawTile(tile* t);

  void drawObstructions();
  void drawObstruction(obstruction* o);

  void destroyTiles();
  void destroyObstructions();

  bool upIsObstruction(Rectangle* location);
  bool downIsObstruction(Rectangle* location);
  bool leftIsObstruction(Rectangle* location);
  bool rightIsObstruction(Rectangle* location);

private:
  std::vector<tile> tiles;
  std::vector<obstruction> obstructions;

  SDL_Surface* mapSurface;

  //TODO: This is only temporary
  Hero* hero;
};

#endif