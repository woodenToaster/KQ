#ifndef KQ_MAP_H
#define KQ_MAP_H

class Map {
public:
  const int SCREEN_WIDTH = 320;
  const int SCREEN_HEIGHT = 240;
  const int TILE_SIZE = 8;

  struct tile {
    SDL_Surface* surface;
    SDL_Rect* dest;
  };

  struct obstruction {
    SDL_Surface* surface;
    SDL_Rect* dest;
  };

private:
  std::vector<tile> tiles;
  std::vector<obstruction> obstructions;

  SDL_Surface* mapSurface;
};

#endif