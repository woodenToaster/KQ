#include "Enemy.h"
#include "Map.h"
#include "SDL_image.h"

Enemy::Enemy() {
	
  image = IMG_Load("./data/billy.png");
  startingLocation = new Rectangle(Map::SCREEN_WIDTH - 32, Map::SCREEN_HEIGHT - 32, 32, 32);
  boundingBox = new Rectangle(6, 4, 20, 28);
  locationInSpriteSheet = new Rectangle(0, 96, 32, 32);
}

Enemy::~Enemy() {
	
  SDL_FreeSurface(image);
  delete startingLocation;
}

//TODO: Normalize speed for player and enemy
void Enemy::update(Map* map) {
	
  // point heroCenter = center(heroLocation);
  // point enemyCenter = center(enemyDest);

  int heroX = map->getHero()->getBBx();
  int heroY = map->getHero()->getBBy();

  if(heroX < getBBx()) {
    if(canMoveLeft(map))
      setBBx(getBBx() - 1);
  }

  if(heroX > getBBx()) {
    if(canMoveRight(map))
      setBBx(getBBx() + 1);
  }

  if(heroY < getBBy()) {
    if(canMoveUp(map))
      setBBy(getBBy() - 1);
  }

  if(heroY > getBBy()) {
    if(canMoveDown(map))
      setBBy(getBBy() + 1);
  }
}

void Enemy::draw(Map* map) {
	SDL_BlitSurface(
    image,
    locationInSpriteSheet->getInternalRect(), 
    map->getMapSurface(), 
    startingLocation->getInternalRect()
  );
}