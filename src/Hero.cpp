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

Hero::Hero(): facing(DOWN) {
	
  image = IMG_Load("./data/walking.tunic.png");
  startingLocation = new Rectangle(0, 0, 0, 0);
  locationInSpriteSheet = new Rectangle(4, 137, 16, 22);
  boundingBox = new Rectangle(
    startingLocation->getX(),
    startingLocation->getY(),
    16, 
    22
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

  if(state[SDL_SCANCODE_UP] && canMoveUp(map)) {
    setBBy(getBBy() - 1);
    direction_mask |= 0x0002;
  }
    
  if(state[SDL_SCANCODE_DOWN] && canMoveDown(map)) {
    setBBy(getBBy() + 1);
    direction_mask |= 0x0008;
  }
    
  if(state[SDL_SCANCODE_LEFT] && canMoveLeft(map)) {
    setBBx(getBBx() - 1);
    direction_mask |= 0x0004;
  }
    
  if(state[SDL_SCANCODE_RIGHT] && canMoveRight(map)) {
    setBBx(getBBx() + 1);
    direction_mask |= 0x0001;
  }

  if(direction_values[direction_mask] >= 0)
    facing = direction_values[direction_mask];
}

void Hero::draw(Map* map) {
  SDL_BlitSurface(
    image, 
    getFacingDirection()->getInternalRect(),
    map->getMapSurface(), 
    boundingBox->getInternalRect()
  );
}

Rectangle* Hero::getFacingDirection() const {
  return direction_images[(int)facing];
}