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

Hero* Map::getHero() const {
  return hero;
}

void Map::putHeroOnMap(Hero* hero) {
  
  SDL_BlitSurface(
    hero->getImage(),
    hero->getLocationInSpriteSheet()->getInternalRect(), 
    mapSurface, 
    hero->getStartingLocation()->getInternalRect()
  );
}

void Map::putEnemyOnMap(Enemy* enemy) {
  
  SDL_BlitSurface(
    enemy->getImage(),
    enemy->getLocationInSpriteSheet()->getInternalRect(), 
    mapSurface, 
    enemy->getStartingLocation()->getInternalRect()
  );
}

bool Map::upIsObstruction(Rectangle* location) {
  
  Rectangle destination(location);
  destination.setY(destination.getY() - 1);

  return isObstruction(&destination);
}

bool Map::downIsObstruction(Rectangle* location) {
  
  Rectangle destination(location);
  destination.setHeight(destination.getHeight() + 1);

  return isObstruction(&destination);
}

bool Map::leftIsObstruction(Rectangle* location) {
  
  Rectangle destination(location);
  destination.setX(destination.getX() - 1);

  return isObstruction(&destination);
}

bool Map::rightIsObstruction(Rectangle* location) {
  
  Rectangle destination(location);
  destination.setWidth(destination.getWidth() + 1);

  return isObstruction(&destination);
}

bool Map::upLeftIsObstruction(Rectangle* location) {
  
  Rectangle destination(location);
  destination.setX(destination.getX() - 1);
  destination.setY(destination.getY() - 1);

  return isObstruction(&destination);
}

bool Map::upRightIsObstruction(Rectangle* location) {
  
  Rectangle destination(location);
  destination.setY(destination.getY() - 1);
  destination.setWidth(destination.getWidth() + 1);

  return isObstruction(&destination);
}

bool Map::downLeftIsObstruction(Rectangle* location) {
  
  Rectangle destination(location);
  destination.setX(destination.getX() - 1);
  destination.setHeight(destination.getHeight() + 1);

  return isObstruction(&destination);
}

bool Map::downRightIsObstruction(Rectangle* location) {
  
  Rectangle destination(location);
  destination.setWidth(destination.getWidth() + 1);
  destination.setHeight(destination.getHeight() + 1);

  return isObstruction(&destination);
}

bool Map::isObstruction(Rectangle* destination) {
  for(obstruction o : obstructions) {
    int x = destination->getX();
    int y = destination->getY();
    int w = destination->getWidth();
    int h = destination->getHeight();

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
  SDL_FillRect(
    t->surface, 
    t->location->getInternalRect(), 
    SDL_MapRGB(t->surface->format, 0, 255, 0)
  );
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

