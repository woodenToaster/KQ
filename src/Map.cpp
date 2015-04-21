#include "Map.h"

Map::Map(SDL_Window* window) {

  mapSurface = SDL_GetWindowSurface(window);
}

Map::~Map() {

  destroyTiles();
  destroyObstructions();
  SDL_FreeSurface(mapSurface);
}

SDL_Surface* Map::getMapSurface() {
  return mapSurface;
}

bool Map::upIsObstruction(SDL_Rect* location) {
  for(obstruction o : obstructions) {
    SDL_Rect newDest{location->x, location->y - 1, location->w, location->h};
    if(overlaps(&newDest, o.dest) && (dest->y + dest->h) > o.dest->y) 
      return true;
  }
  return false;
}

bool Map::downIsObstruction(SDL_Rect* location) {
  for(obstruction o : obstructions) {
    SDL_Rect newDest{location->x, location->y, location->w, location->h + 1};
    if(overlaps(&newDest, o.dest) && dest->y < o.dest->y) 
      return true;
  }
  return false;
}

bool Map::leftIsObstruction(SDL_Rect* location) {
  for(obstruction o : obstructions) {
    SDL_Rect newDest{location->x - 1, location->y, location->w, location->h};
    if(overlaps(&newDest, o.dest) && dest->x + dest->w > o.dest->x) 
      return true;
  }
  return false;
}

bool Map::rightIsObstruction(SDL_Rect* location) {
  for(obstruction o : obstructions) {
    SDL_Rect newDest{location->x, location->y, location->w + 1, location->h};
    if(overlaps(&newDest, o.dest) && dest->x < o.dest->x + o.dest->w) 
      return true;
  }
  return false;
}


void Map::makeTiles() {
  
  //TODO(Chris): potential memory leak.  Use std::shared_ptr
  SDL_Surface* grassSurface = SDL_CreateRGBSurface(0, TILE_SIZE, TILE_SIZE, 32, 0, 0, 0, 0);

  for(int row = 0; row < (SCREEN_WIDTH / TILE_SIZE); ++row) {
    for(int column = 0; column <  (SCREEN_HEIGHT / TILE_SIZE); ++column) {
      tile t;
      t.surface = grassSurface;
      t.dest = new SDL_Rect{TILE_SIZE * row, TILE_SIZE * column, TILE_SIZE, TILE_SIZE};
      tiles.push_back(t);
    }
  }
}

void Map::makeObstructions() {

  //TODO(Chris): potential memory leak.  Use std::shared_ptr
  SDL_Surface* boulder = SDL_CreateRGBSurface(0, TILE_SIZE * 4, TILE_SIZE * 4, 32, 0, 0, 0, 0);
  for(int i = 0; i < 3; ++i) {
    obstruction obs;
    obs.surface = boulder;
    obs.dest = new SDL_Rect{45 * (i + 1), 72 * (i + 1), TILE_SIZE, TILE_SIZE};
    obstructions.push_back(obs);
  }
}

void Map::drawTiles() {
  for(tile t : tiles) {
    drawTile(mapSurface, &t);
  }
}

void Map::drawTile(tile* t) {
  SDL_FillRect(t->surface, t->dest, SDL_MapRGB(surface->format, 0, 255, 0));
  SDL_BlitSurface(t->surface, NULL, mapSurface, t->dest);
}

void Map::drawObstructions() {
  for(obstruction o : obstructions) {
    drawObstruction(mapSurface, &o);
  }
}

void Map::drawObstruction(obstruction* o) {
  SDL_FillRect(o->surface, NULL, SDL_MapRGB(targetSurface->format, 127, 127, 127));
  SDL_BlitSurface(o->surface, NULL, mapSurface, o->dest);
}

void Map::destroyTiles() {
  for(tile t : tiles) {
    delete t.dest;
  }
  //TODO(Chris): Handle this with std::shared_ptr to avoid memory leaks
  SDL_FreeSurface(tiles[0].surface);
}

void Map::destroyObstructions() {
  for(obstruction o : obstructions) {
    delete o.dest;
  }
  //TODO(Chris): Handle this with std::shared_ptr to avoid memory leaks
  SDL_FreeSurface(obstructions[0].surface);
}

