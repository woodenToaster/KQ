#include "Map.h"
#include "Hero.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <regex>

std::map<std::string, SDL_Surface*> Map::tileImages = {
  
  {"grassSurface", SDL_CreateRGBSurface(0, TILE_SIZE * 4, TILE_SIZE * 4, 32, 0, 0, 0, 0)},
  {"boulder", SDL_CreateRGBSurface(0, TILE_SIZE * 4, TILE_SIZE * 4, 32, 0, 0, 0, 0)}
};

Map::Map(SDL_Window* window, Hero* hero): hero(hero) {

  mapSurface = SDL_GetWindowSurface(window);
  //makeTiles();
  //makeObstructions();
}

Map::~Map() {

  destroyTiles();
  destroyObstructions();
  SDL_FreeSurface(mapSurface);

  for(auto iter : tileImages) {
    SDL_FreeSurface(iter.second);
  }
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

void Map::loadTileDataFromFile(std::string& filename) {

  std::string path = "./data/maps/";
  std::string fullFilePath = path + filename;
  std::ifstream mapFile;
  mapFile.open(fullFilePath);

  //TODO(Chris): figure out assertion pattern for debugging
  assert(mapFile != nullptr);

  std::regex tileBeginRegex("tile \\{");
  std::regex obstructionBeginRegex("obstruction \\{");
  std::regex endOfEntity("^\\}");
  std::regex xValRegex("x = ([0-9]+)");
  std::regex yValRegex("y = ([0-9]+)");
  std::regex wValRegex("w = ([0-9]+)");
  std::regex hValRegex("h = ([0-9]+)");
  std::regex imageValRegex("image = \"([^\"]+)\"");

  std::smatch smatcher;

  bool processingTile = false;
  bool processingObstruction = false;

  std::string currentLine;

  // obstruction o;

  int x, y, w, h;
  std::string image;

  while(std::getline(mapFile, currentLine)) {

    tile t;
    obstruction o;

    if(std::regex_match(currentLine, tileBeginRegex)) 
      processingTile = true;

    if(std::regex_match(currentLine, obstructionBeginRegex)) 
      processingObstruction = true;
    
    if(processingTile || processingObstruction) {
      if(std::regex_search(currentLine, smatcher, xValRegex)) 
        x = stoi(smatcher.str(1));
      
      else if(std::regex_search(currentLine, smatcher, yValRegex)) 
        y = stoi(smatcher.str(1));
      
      else if(std::regex_search(currentLine, smatcher, wValRegex)) 
        w = stoi(smatcher.str(1));
      
      else if(std::regex_search(currentLine, smatcher, hValRegex)) 
        h = stoi(smatcher.str(1));
      
      else if(std::regex_search(currentLine, smatcher, imageValRegex)) 
        image = smatcher.str(1);
      
    }

    if((processingObstruction == true || processingTile == true) &&
        std::regex_match(currentLine, endOfEntity)) {
      
      if(processingTile) {
        t.location = new Rectangle(x, y, w, h);
        t.surface = tileImages[image];
        tiles.push_back(t);
        processingTile = false;
      }
      else if(processingObstruction) {
        o.location = new Rectangle(x, y, w, h);
        o.surface = tileImages[image];
        obstructions.push_back(o);
        processingObstruction = false;
      }
    }
  }
  
  mapFile.close();
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

