#include "Map.h"
#include "Hero.h"

Map::Map(SDL_Window* window, Hero* hero): hero(hero) {

  mapSurface = SDL_GetWindowSurface(window);
  makeTiles();
  makeObstructions();
}

Map::~Map() {

  destroyTiles();
  destroyObstructions();
  SDL_FreeSurface(mapSurface);
}

SDL_Surface* Map::getMapSurface() {
  return mapSurface;
}

Hero* Map::getHero() {
  return hero;
}

bool Map::upIsObstruction(Rectangle* location) {
  for(obstruction o : obstructions) {
    int x = location->getX();
    int y = location->getY() - 1;
    int w = location->getWidth();
    int h = location->getHeight();

    Rectangle dest(x, y, w, h);
    if(dest.overlaps(o.location->getInternalRect()))
      return true;
  }
  return false;
}

bool Map::downIsObstruction(Rectangle* location) {
  for(obstruction o : obstructions) {
    int x = location->getX();
    int y = location->getY();
    int w = location->getWidth();
    int h = location->getHeight() + 1;

    Rectangle newDest(x, y, w, h);
    if(newDest.overlaps(o.location->getInternalRect())) 
      return true;
  }
  return false;
}

bool Map::leftIsObstruction(Rectangle* location) {
  for(obstruction o : obstructions) {
    int x = location->getX() - 1;
    int y = location->getY();
    int w = location->getWidth();
    int h = location->getHeight();

    Rectangle newDest(x, y, w, h);
    if(newDest.overlaps(o.location->getInternalRect()))
      return true;
  }
  return false;
}

bool Map::rightIsObstruction(Rectangle* location) {
  for(obstruction o : obstructions) {
    int x = location->getX();
    int y = location->getY();
    int w = location->getWidth() + 1;
    int h = location->getHeight();

    Rectangle newDest(x, y, w, h);
    if(newDest.overlaps(o.location->getInternalRect()))
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
      t.location = new Rectangle(TILE_SIZE * row, TILE_SIZE * column, TILE_SIZE, TILE_SIZE);
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
    obs.location = new Rectangle(45 * (i + 1), 72 * (i + 1), TILE_SIZE, TILE_SIZE);
    obstructions.push_back(obs);
  }
}

void Map::drawTiles() {
  for(tile t : tiles) {
    drawTile(&t);
  }
}

void Map::drawTile(tile* t) {
  SDL_FillRect(t->surface, t->location->getInternalRect(), SDL_MapRGB(t->surface->format, 0, 255, 0));
  SDL_BlitSurface(t->surface, NULL, mapSurface, t->location->getInternalRect());
}

void Map::drawObstructions() {
  for(obstruction o : obstructions) {
    drawObstruction(&o);
  }
}

void Map::drawObstruction(obstruction* o) {
  SDL_FillRect(o->surface, NULL, SDL_MapRGB(o->surface->format, 127, 127, 127));
  SDL_BlitSurface(o->surface, NULL, mapSurface, o->location->getInternalRect());
}

void Map::destroyTiles() {
  for(tile t : tiles) {
    delete t.location;
  }
  //TODO(Chris): Handle this with std::shared_ptr to avoid memory leaks
  SDL_FreeSurface(tiles[0].surface);
}

void Map::destroyObstructions() {
  for(obstruction o : obstructions) {
    delete o.location;
  }
  //TODO(Chris): Handle this with std::shared_ptr to avoid memory leaks
  SDL_FreeSurface(obstructions[0].surface);
}

