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
  static const int SCREEN_WIDTH = 640;//320;
  static const int SCREEN_HEIGHT = 480;//240;
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

  struct harvestableTile {
    SDL_Surface* surface;
    Rectangle* location;
    int tileState;
  };

  Map(SDL_Window* window, Hero* hero);
  ~Map();

  SDL_Surface* getMapSurface();

  Hero* getHero() const;

  int getMapSurfaceLowerBound() const;
  int getMapSurfaceRightBound() const;
  int getMapSurfaceUpperBound() const;
  int getMapSurfaceLeftBound() const;

  void putHeroOnMap(Hero* hero);
  void putEnemyOnMap(Enemy* enemy);

  void loadTileDataFromFile(std::string& filename);

  void makeTiles();
  void makeObstructions();

  void drawTiles();
  void drawTile(tile* t);

  void drawObstructions();
  void drawObstruction(obstruction* o);

  void drawHarvestableTiles();
  void drawHarvestableTile(harvestableTile* h);

  void checkHarvesting();
  void notifyHarvested(harvestableTile& harvestable);
  void incrementHarvestableTileState(harvestableTile& hTile);

  void destroyTiles();
  void destroyObstructions();
  void destroyHarvestableTiles();

  void destroyTileImages();

  bool isObstruction(Rectangle* destination);
  bool isHarvestableTile(Rectangle* destination);

  void notifyDoneHarvesting();
  
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
  std::vector<harvestableTile> harvestableTiles;

  SDL_Surface* mapSurface;

  //TODO: This is only temporary
  Hero* hero;

  //TODO: Find a better way to do this
  bool heroIsHarvesting;
};

#endif