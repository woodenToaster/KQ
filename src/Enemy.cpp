#include "Enemy.h"
#include "Map.h"
#include "SDL_image.h"
#include <iostream>

Enemy::Enemy(): alive(true), life(3) {
	
  recoveringFromHit = false;
  image = IMG_Load("./data/sprites/billy.png");
  SDL_SetSurfaceBlendMode(image, SDL_BLENDMODE_BLEND);
  SDL_SetSurfaceAlphaMod(image, 254);

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

void Enemy::hitHero(Hero* hero) {
  std::cout << "A hit, a very palpable hit.\n";
}

void Enemy::notifyHit() {
  
  if(recoveringFromHit)
    return;

  if(--life <= 0)
    alive = false;

  std::cout << "Gotcha!\n";
  recoveringFromHit = true;
  SDL_SetSurfaceAlphaMod(image, 127);
  SDL_AddTimer(2000, recover, this);
}

bool Enemy::isAlive() const {
  return alive;
}

Uint32 Enemy::recover(Uint32 interval, void* enemyInstance) {

  Enemy* enemy = (Enemy*) enemyInstance;
  enemy->recoveringFromHit = false;
  SDL_SetSurfaceAlphaMod(enemy->getImage(), 254);
  return 0;
}
