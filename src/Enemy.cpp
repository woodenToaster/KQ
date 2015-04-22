#include "Enemy.h"
#include "Map.h"

Enemy::Enemy() {
	
  image = SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);
  location = new Rectangle(Map::SCREEN_WIDTH - 32, Map::SCREEN_HEIGHT - 32, 32, 32);
  SDL_FillRect(image, NULL, SDL_MapRGB(image->format, 255, 0, 0));
}

Enemy::~Enemy() {
	
  SDL_FreeSurface(image);
  delete location;
}

//TODO: Normalize speed for player and enemy
void Enemy::update(Map* map) {
	
  // point heroCenter = center(heroLocation);
  // point enemyCenter = center(enemyDest);

  int heroX = map->getHero()->getX();
  int heroY = map->getHero()->getY();

  if(heroX < getX()) {
    if(canMoveLeft(map))
      setX(getX() - 1);
  }

  if(heroX > getX()) {
    if(canMoveRight(map))
      setX(getX() + 1);
  }

  if(heroY < getY()) {
    if(canMoveUp(map))
      setY(getY() - 1);
  }

  if(heroY > getY()) {
    if(canMoveDown(map))
      setY(getY() + 1);
  }
}

void Enemy::draw(Map* map) {
	SDL_BlitSurface(image, NULL, map->getMapSurface(), location->getInternalRect());
}