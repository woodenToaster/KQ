#include "GameEntity.h"

bool GameEntity::canMoveUp(Map* map) {
  return upIsInMap(map) && !upIsObstruction(map);
}

bool GameEntity::canMoveDown(Map* map) {
  return downIsInMap(map) && !downIsObstruction(map);
}

bool GameEntity::canMoveLeft(Map* map) {
  return leftIsInMap(map) && !leftIsObstruction(map);
}

bool GameEntity::canMoveRight(Map* map) {
  return rightIsInMap(map) && !rightIsObstruction(map); 
}

bool GameEntity::upIsInMap(Map* map) {
  return location->y > map->getMapSurface()->clip_rect.y;
}

bool GameEntity::downIsInMap(Map* map) {
  return (location->y + location->h) < (map->getMapSurface()->clip_rect.y + map->getMapSurface()->clip_rect.h);
}

bool GameEntity::leftIsInMap(Map* map) {
  return location->x > map->getMapSurface()->clip_rect.x;
}

bool GameEntity::rightIsInMap(Map* map) {
  return (location->x + location->w) < (map->getMapSurface()->clip_rect.x + map->getMapSurface()->clip_rect.w);
}