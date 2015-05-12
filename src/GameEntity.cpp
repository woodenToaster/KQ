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

bool GameEntity::canMoveUpLeft(Map* map) {
  return upLeftIsInMap(map) && !map->upLeftIsObstruction(boundingBox);
}

bool GameEntity::canMoveUpRight(Map* map) {
  return upRightIsInMap(map) && !map->upRightIsObstruction(boundingBox);
}

bool GameEntity::canMoveDownLeft(Map* map) {
  return downLeftIsInMap(map) && !map->downLeftIsObstruction(boundingBox);
}

bool GameEntity::canMoveDownRight(Map* map) {
  return downRightIsInMap(map) && !map->downRightIsObstruction(boundingBox);
}

bool GameEntity::upIsInMap(Map* map) {
  return getBBy() > map->getMapSurfaceUpperBound();
}

bool GameEntity::downIsInMap(Map* map) {
  return (getBBy() + getBBh()) < map->getMapSurfaceLowerBound();
}

bool GameEntity::leftIsInMap(Map* map) {
  return getBBx() > map->getMapSurfaceLeftBound();
}

bool GameEntity::rightIsInMap(Map* map) {
  return (getBBx() + getBBw()) < map->getMapSurfaceRightBound();
}

bool GameEntity::upLeftIsInMap(Map* map) {
  return leftIsInMap(map) && upIsInMap(map);
}

bool GameEntity::upRightIsInMap(Map* map) {
  return rightIsInMap(map) && upIsInMap(map);
}

bool GameEntity::downLeftIsInMap(Map* map) {
  return leftIsInMap(map) && downIsInMap(map);
}

bool GameEntity::downRightIsInMap(Map* map) {
  return downIsInMap(map) && rightIsInMap(map);
}
