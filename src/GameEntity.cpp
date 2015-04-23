#include "GameEntity.h"
#include "Map.h"

Rectangle* GameEntity::getLocation() {
  return location;
}

SDL_Surface* GameEntity::getImage() {
  return image;
}

int GameEntity::getX() {
  return location->getInternalRect()->x;
}

int GameEntity::getY() {
  return location->getInternalRect()->y;
}

int GameEntity::getWidth() {
  return location->getInternalRect()->w;
}

int GameEntity::getHeight() {
  return location->getInternalRect()->h;
}

void GameEntity::setX(int newX) {
  location->getInternalRect()->x = newX;
}

void GameEntity::setY(int newY) {
  location->getInternalRect()->y = newY;
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
  return upIsInMap(map) && !map->upIsObstruction(location);
}

bool GameEntity::canMoveDown(Map* map) {
  return downIsInMap(map) && !map->downIsObstruction(location);
}

bool GameEntity::canMoveLeft(Map* map) {
  return leftIsInMap(map) && !map->leftIsObstruction(location);
}

bool GameEntity::canMoveRight(Map* map) {
  return rightIsInMap(map) && !map->rightIsObstruction(location); 
}

bool GameEntity::upIsInMap(Map* map) {
  return getY() > map->getMapSurface()->clip_rect.y;
}

bool GameEntity::downIsInMap(Map* map) {
  return (getY() + getHeight()) < (map->getMapSurface()->clip_rect.y + map->getMapSurface()->clip_rect.h);
}

bool GameEntity::leftIsInMap(Map* map) {
  return getX() > map->getMapSurface()->clip_rect.x;
}

bool GameEntity::rightIsInMap(Map* map) {
  return (getX() + getWidth()) < (map->getMapSurface()->clip_rect.x + map->getMapSurface()->clip_rect.w);
}


