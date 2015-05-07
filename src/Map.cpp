#include "Map.h"
#include "Hero.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <regex>

std::map<std::string, SDL_Surface*> Map::tileImages = {
  
  {"grassSurface", SDL_CreateRGBSurface(0, TILE_SIZE * 4, TILE_SIZE * 4, 32, 0, 0, 0, 0)},
  {"boulder", SDL_CreateRGBSurface(0, TILE_SIZE * 4, TILE_SIZE * 4, 32, 0, 0, 0, 0)},
  {"gold", SDL_CreateRGBSurface(0, TILE_SIZE * 4, TILE_SIZE * 4, 32, 0, 0, 0, 0)}
};

Map::Map(SDL_Window* window, Hero* hero): hero(hero), heroIsHarvesting(false) {

  mapSurface = SDL_GetWindowSurface(window);
}

Map::~Map() {

  destroyTiles();
  destroyObstructions();
  destroyHarvestableTiles();
  SDL_FreeSurface(mapSurface);
}

void Map::destroyTileImages() {

  for(auto iter : tileImages) {
    SDL_FreeSurface(iter.second);
  }

  Map::tileImages.clear();
}

SDL_Surface* Map::getMapSurface() {
  return mapSurface;
}

int Map::getMapSurfaceLowerBound() const {
  return mapSurface->clip_rect.y + mapSurface->clip_rect.h;
}

int Map::getMapSurfaceRightBound() const {
  return mapSurface->clip_rect.x + mapSurface->clip_rect.w;
}

int Map::getMapSurfaceUpperBound() const {
  return mapSurface->clip_rect.y;
}

int Map::getMapSurfaceLeftBound() const {
  return mapSurface->clip_rect.x;
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

  return isObstruction(&destination) || isHarvestableTile(&destination);
}

bool Map::downIsObstruction(Rectangle* location) {
  
  Rectangle destination(location);
  destination.setHeight(destination.getHeight() + 1);

  return isObstruction(&destination) || isHarvestableTile(&destination);
}

bool Map::leftIsObstruction(Rectangle* location) {
  
  Rectangle destination(location);
  destination.setX(destination.getX() - 1);

  return isObstruction(&destination) || isHarvestableTile(&destination);
}

bool Map::rightIsObstruction(Rectangle* location) {
  
  Rectangle destination(location);
  destination.setWidth(destination.getWidth() + 1);

  return isObstruction(&destination) || isHarvestableTile(&destination);
}

bool Map::upLeftIsObstruction(Rectangle* location) {
  
  Rectangle destination(location);
  destination.setX(destination.getX() - 1);
  destination.setY(destination.getY() - 1);

  return isObstruction(&destination) || isHarvestableTile(&destination);
}

bool Map::upRightIsObstruction(Rectangle* location) {
  
  Rectangle destination(location);
  destination.setY(destination.getY() - 1);
  destination.setWidth(destination.getWidth() + 1);

  return isObstruction(&destination) || isHarvestableTile(&destination);
}

bool Map::downLeftIsObstruction(Rectangle* location) {
  
  Rectangle destination(location);
  destination.setX(destination.getX() - 1);
  destination.setHeight(destination.getHeight() + 1);

  return isObstruction(&destination) || isHarvestableTile(&destination);
}

bool Map::downRightIsObstruction(Rectangle* location) {
  
  Rectangle destination(location);
  destination.setWidth(destination.getWidth() + 1);
  destination.setHeight(destination.getHeight() + 1);

  return isObstruction(&destination) || isHarvestableTile(&destination);
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

bool Map::isHarvestableTile(Rectangle* destination) {
  
  for(harvestableTile harvestable : harvestableTiles) {
    int x = destination->getX();
    int y = destination->getY();
    int w = destination->getWidth();
    int h = destination->getHeight();

    Rectangle newDest(x, y, w, h);
    if(newDest.overlaps(harvestable.location->getInternalRect()))
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
  std::regex harvestableBeginRegex("harvestableTile \\{");
  std::regex endOfEntity("^\\}");
  std::regex xValRegex("x = ([0-9]+)");
  std::regex yValRegex("y = ([0-9]+)");
  std::regex wValRegex("w = ([0-9]+)");
  std::regex hValRegex("h = ([0-9]+)");
  std::regex imageValRegex("image = \"([^\"]+)\"");

  std::smatch smatcher;

  bool processingTile = false;
  bool processingObstruction = false;
  bool processingHarvestable = false;

  std::string currentLine;

  int x, y, w, h;
  std::string image;

  while(std::getline(mapFile, currentLine)) {

    tile t;
    obstruction o;
    harvestableTile harvestable;

    if(std::regex_match(currentLine, tileBeginRegex)) 
      processingTile = true;

    if(std::regex_match(currentLine, obstructionBeginRegex)) 
      processingObstruction = true;

    if(std::regex_match(currentLine, harvestableBeginRegex)) 
      processingHarvestable = true;
    
    if(processingTile || processingObstruction || processingHarvestable) {
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

    if((processingObstruction || processingTile || processingHarvestable) &&
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
      else if(processingHarvestable) {
        harvestable.location = new Rectangle(x, y, w, h);
        harvestable.surface = tileImages[image];
        harvestable.tileState = 0;
        harvestableTiles.push_back(harvestable);
        processingHarvestable = false;
      }
    }
  }
  
  mapFile.close();
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

void Map::drawHarvestableTiles() {
  
  for(harvestableTile h : harvestableTiles) {
    drawHarvestableTile(&h);
  }
}

void Map::drawHarvestableTile(harvestableTile* h) {
  
  int color;
  switch(h->tileState) {
    case 0:
      color = 255;
      break;
    case 1:
      color = 230;
      break;
    case 2:
      color = 154;
      break;
    default:
      break;
  }

  SDL_FillRect(h->surface, NULL, SDL_MapRGB(h->surface->format, color, color, 0));
  SDL_BlitSurface(h->surface, NULL, mapSurface, h->location->getInternalRect());
}

void Map::checkHarvesting() {
  
  if(hero->isAttacking() && !heroIsHarvesting) {
    for(auto& harvestable : harvestableTiles) {
      if(hero->getWeaponBoundingBox()->overlaps(harvestable.location->getInternalRect())) {
        heroIsHarvesting = true;
        notifyHarvested(harvestable);
        break;
      } 
    }
  }
}

void Map::notifyHarvested(harvestableTile& harvestable) {

  if(harvestable.tileState == 0 || harvestable.tileState == 1)
    incrementHarvestableTileState(harvestable);
}

void Map::notifyDoneHarvesting() {
  heroIsHarvesting = false;
}

void Map::incrementHarvestableTileState(harvestableTile& hTile) {
  hTile.tileState += 1;
}

void Map::destroyTiles() {
  
  for(tile t : tiles) {
    delete t.location;
  }
}

void Map::destroyObstructions() {
  
  for(obstruction o : obstructions) {
    delete o.location;
  }
}

void Map::destroyHarvestableTiles() {
  
  for(harvestableTile h : harvestableTiles) {
    delete h.location;
  }
}
