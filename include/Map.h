#ifndef KQ_MAP_H
#define KQ_MAP_H

#include <vector>
#include "SDL.h"

class Map {
public:
  static const int SCREEN_WIDTH = 320;
  static const int SCREEN_HEIGHT = 240;
  static const int TILE_SIZE = 8;

  struct tile {
    SDL_Surface* surface;
    SDL_Rect* dest;
  };

  struct obstruction {
    SDL_Surface* surface;
    SDL_Rect* dest;
  };

  Map(SDL_Window* window);
  ~Map();

  SDL_Surface* getMapSurface();

  void makeTiles();
  void makeObstructions();

  void drawTiles();
  void drawTile(tile* t);

  void drawObstructions();
  void drawObstruction(obstruction* o);

  void destroyTiles();
  void destroyObstructions();

  bool upIsObstruction(SDL_Rect* location);
  bool downIsObstruction(SDL_Rect* location);
  bool leftIsObstruction(SDL_Rect* location);
  bool rightIsObstruction(SDL_Rect* location);

private:
  std::vector<tile> tiles;
  std::vector<obstruction> obstructions;

  SDL_Surface* mapSurface;
};

#endif