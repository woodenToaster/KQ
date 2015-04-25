#include "GameEntity.h"
#include "Map.h"

Rectangle* GameEntity::getStartingLocation() {
  return startingLocation;
}

SDL_Surface* GameEntity::getImage() {
  return image;
}

Rectangle* GameEntity::getBoundingBox() {
  return boundingBox;
}

Rectangle* GameEntity::getLocationInSpriteSheet() {
  return locationInSpriteSheet;
}

int GameEntity::getBBx() {
  return boundingBox->getX();
}

int GameEntity::getBBy() {
  return boundingBox->getY();
}

int GameEntity::getBBw() {
  return boundingBox->getWidth();
}

int GameEntity::getBBh() {
  return boundingBox->getHeight();
}

void GameEntity::setBBx(int newX) {
  boundingBox->setX(newX);
}

void GameEntity::setBBy(int newY) {
  boundingBox->setY(newY);
}

void GameEntity::setBBw(int newW) {
  boundingBox->setWidth(newW);
}

void GameEntity::setBBh(int newH) {
  boundingBox->setHeight(newH);
}

bool GameEntity::canMoveUp(Map* map) {
  return upIsInMap(map) && !map->upIsObstruction(boundingBox);
}

bool GameEntity::canMoveDown(Map* map) {
  return downIsInMap(map) && !map->downIsObstruction(boundingBox);
}

bool GameEntity::canMoveLeft(Map* map) {
  return leftIsInMap(map) && !map->leftIsObstruction(boundingBox);
}

bool GameEntity::canMoveRight(Map* map) {
  return rightIsInMap(map) && !map->rightIsObstruction(boundingBox); 
}

bool GameEntity::upIsInMap(Map* map) {
  return getBBy() > map->getMapSurface()->clip_rect.y;
}

bool GameEntity::downIsInMap(Map* map) {
  return (getBBy() + getBBh()) < (map->getMapSurface()->clip_rect.y + map->getMapSurface()->clip_rect.h);
}

bool GameEntity::leftIsInMap(Map* map) {
  return getBBx() > map->getMapSurface()->clip_rect.x;
}

bool GameEntity::rightIsInMap(Map* map) {
  return (getBBx() + getBBw()) < (map->getMapSurface()->clip_rect.x + map->getMapSurface()->clip_rect.w);
}


