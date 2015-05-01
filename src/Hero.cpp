#include <iostream>
#include "Hero.h"
#include "Map.h"
#include "Rectangle.h"
#include "SDL_image.h"

const uint16_t Hero::direction_masks[4] = {
  0x0001, //right
  0x0002, //up
  0x0004, //left
  0x0008  //down
};

const int Hero::direction_values[16] = {
  -1, // none: stop
   0, // right 
   2, // up 
   1, // right + up
   4, // left 
  -1, // left + right: stop
   3, // left + up
  -1, // left + right + up: stop 
   6, // down
   7, // down + right
  -1, // down + up: stop
  -1, // down + right + up: stop
   5, // down + left
  -1, // down + left + right: stop
  -1, // down + left + up: stop
  -1  // down + left + right + up: stop
};

Hero::Hero(): facing(DOWN), attacking(false) {
	
  image = IMG_Load("./data/walking.tunic.png");
  weaponImage = SDL_CreateRGBSurface(0, 16, 16, 32, 0, 0, 0, 0);
  startingLocation = new Rectangle(0, 0, 0, 0);
  locationInSpriteSheet = new Rectangle(4, 137, 16, 22);
  boundingBox = new Rectangle(
    startingLocation->getX(),
    startingLocation->getY(),
    16, 
    22
  );
  weaponBoundingBox = new Rectangle(
    boundingBox->getX(),
    boundingBox->getY(),
    8,
    16
  );

  direction_images[UP] = new Rectangle(4, 41, 16, 22);
  direction_images[DOWN] = new Rectangle(4, 137, 16, 22);
  direction_images[LEFT] = new Rectangle(5, 105, 16, 23);
  direction_images[RIGHT] = new Rectangle(3, 9, 16, 23);
  direction_images[UPPER_RIGHT] = new Rectangle(197, 10, 14, 22);
  direction_images[UPPER_LEFT] = new Rectangle(197, 42, 14, 22);
  direction_images[LOWER_RIGHT] = new Rectangle(197, 137, 14, 22);
  direction_images[LOWER_LEFT] = new Rectangle(197, 105, 14, 22);
}

Hero::~Hero() {
  
  SDL_FreeSurface(image);

  delete startingLocation;
  delete locationInSpriteSheet;
  delete boundingBox;
  
  for(int i = 0; i < 8; i++) {
    delete direction_images[0];
  }
}

//TODO: Speed for diagonal movement should be speed/sqrt(2)
void Hero::update(Map* map) {

  const Uint8* state = SDL_GetKeyboardState(NULL);
  uint16_t direction_mask = 0x0000;

  if(state[SDL_SCANCODE_UP]) 
    direction_mask |= 0x0002;
    
  if(state[SDL_SCANCODE_DOWN]) 
    direction_mask |= 0x0008;
    
  if(state[SDL_SCANCODE_LEFT]) 
    direction_mask |= 0x0004;
    
  if(state[SDL_SCANCODE_RIGHT]) 
    direction_mask |= 0x0001;

  int direction_value = direction_values[direction_mask];

  if(direction_value >= 0) {
    
    facing = direction_value;

    switch(direction_value) {
      case 0: // right
        if(canMoveRight(map))
          setBBx(getBBx() + 1);
        break;
      case 1: // right + up
        if(canMoveUpRight(map)) {
          setBBx(getBBx() + 1);
          setBBy(getBBy() - 1);
        }
        break;
      case 2: // up 
        if(canMoveUp(map))
          setBBy(getBBy() - 1);
        break;
      case 3: // left + up
        if(canMoveUpLeft(map)) {
          setBBx(getBBx() - 1);
          setBBy(getBBy() - 1);
        }
        break;
      case 4: // left
        if(canMoveLeft(map)) 
          setBBx(getBBx() - 1);
        break;
      case 5: // down + left
        if(canMoveDownLeft(map)) {
          setBBx(getBBx() - 1);
          setBBy(getBBy() + 1);
        }
        break;
      case 6: // down
        if(canMoveDown(map))
          setBBy(getBBy() + 1);
        break;
      case 7: // down + right
        if(canMoveDownRight(map)) {
          setBBy(getBBy() + 1);
          setBBx(getBBx() + 1);
        }
        break;
      default:
        break;
    }
  }
}

void Hero::draw(Map* map) {
  SDL_BlitSurface(
    image, 
    getFacingDirection()->getInternalRect(),
    map->getMapSurface(), 
    boundingBox->getInternalRect()
  );
}

void Hero::attack(Map* map) {
  attacking = true;
  SDL_AddTimer(200, doneAttacking, this);
}

bool Hero::isAttacking() const {
  return attacking;
}

void Hero::drawWeapon(Map* map) {
  
  Rectangle r{0, 0, 0, 0};

  switch(facing) {
    case 0: // Right
      drawSwordRight(map);
      break;
    case  2: // Up
      drawSwordUp(map);
      break;
    case  4: // Left
      drawSwordLeft(map);
      break;
    case  6: // Down
      drawSwordDown(map);
      break;
    default:
      break;
  }
}

void Hero::drawSwordRight(Map* map) {
  weaponBoundingBox->setX(boundingBox->getX() + boundingBox->getWidth());
  weaponBoundingBox->setY(boundingBox->getY() + boundingBox->getHeight() / 2);
  Rectangle r = getHorizontalSword();
  drawSword(r, map);
}

void Hero::drawSwordLeft(Map* map) {
  weaponBoundingBox->setX(boundingBox->getX() - 16);
  weaponBoundingBox->setY(boundingBox->getY() + boundingBox->getHeight() / 2);
  Rectangle r = getHorizontalSword();
  drawSword(r, map);
}

void Hero::drawSwordUp(Map* map) {
  weaponBoundingBox->setX(boundingBox->getX() + boundingBox->getWidth() / 2);
  weaponBoundingBox->setY(boundingBox->getY() - 16);
  Rectangle r = getVerticalSword();
  drawSword(r, map);
}

void Hero::drawSwordDown(Map* map) {
  weaponBoundingBox->setX(boundingBox->getX() + boundingBox->getWidth() / 2);
  weaponBoundingBox->setY(boundingBox->getY() + boundingBox->getHeight());
  Rectangle r = getVerticalSword();
  drawSword(r, map);
}

void Hero::drawSword(Rectangle& dest, Map* map) {
  SDL_FillRect(weaponImage, NULL, SDL_MapRGB(weaponImage->format, 127, 127, 127));
  SDL_BlitSurface(weaponImage, dest.getInternalRect(), map->getMapSurface(), weaponBoundingBox->getInternalRect());
}

Uint32 Hero::doneAttacking(Uint32 interval, void* heroInstance) {
  Hero* h = (Hero*) heroInstance;
  h->attacking = false;
  return 0;
}

Rectangle Hero::getHorizontalSword() {
  weaponBoundingBox->setWidth(16);
  weaponBoundingBox->setHeight(4);
  Rectangle r{0, 0, 16, 4};
  
  return r;
}

Rectangle Hero::getVerticalSword() {
  weaponBoundingBox->setWidth(4);
  weaponBoundingBox->setHeight(16);
  Rectangle r{0, 0, 4, 16};
  
  return r;
}

Rectangle* Hero::getWeaponBoundingBox() const {
  return weaponBoundingBox;
}

Rectangle* Hero::getFacingDirection() const {
  return direction_images[(int)facing];
}