#include "Enemy.h"
#include "Map.h"
#include "SDL_image.h"

Enemy::Enemy(): alive(true) {
	
  image = IMG_Load("./data/billy.png");
  startingLocation = new Rectangle(Map::SCREEN_WIDTH - 32, Map::SCREEN_HEIGHT - 32, 32, 32);
  boundingBox = new Rectangle(
    startingLocation->getX(),
    startingLocation->getY(), 
    20, 
    28
  );
  locationInSpriteSheet = new Rectangle(6, 100, 20, 28);
}

Enemy::~Enemy() {
	
  SDL_FreeSurface(image);
  delete startingLocation;
  delete boundingBox;
  delete locationInSpriteSheet;
}

//TODO: Normalize speed for player and enemy
void Enemy::update(Map* map) {
	
  int heroX = map->getHero()->getBBx();
  int heroY = map->getHero()->getBBy();

  if(heroX < getBBx() && canMoveLeft(map)) {
    setBBx(getBBx() - 1);
  }

  if(heroX > getBBx() && canMoveRight(map)) {
    setBBx(getBBx() + 1);
  }

  if(heroY < getBBy() && canMoveUp(map)) {
    setBBy(getBBy() - 1);
  }

  if(heroY > getBBy() && canMoveDown(map)) {
    setBBy(getBBy() + 1);
  }
}

void Enemy::draw(Map* map) {
	SDL_BlitSurface(
    image,
    locationInSpriteSheet->getInternalRect(), 
    map->getMapSurface(), 
    boundingBox->getInternalRect()
  );
}

void Enemy::notifyHit() {
  alive = false;
}

bool Enemy::isAlive() const {
  return alive;
}