#include <iostream>
#include "Hero.h"
#include "Map.h"
#include "Rectangle.h"
#include "SDL_image.h"


Hero::Hero() {
	
  image = IMG_Load("./data/walking.tunic.png");
  if(!image) {
    std::cout << "Image not loaded";
  }
  startingLocation = new Rectangle(0, 0, 0, 0);
  locationInSpriteSheet = new Rectangle(4, 137, 16, 22);
  boundingBox = new Rectangle(
    startingLocation->getX(),
    startingLocation->getY(),
    16, 
    22
  );
}

Hero::~Hero() {
  
  SDL_FreeSurface(image);
  delete startingLocation;
  delete locationInSpriteSheet;
  delete boundingBox;
}

//TODO: Speed for diagonal movement should be speed/sqrt(2)
void Hero::update(Map* map) {

  const Uint8* state = SDL_GetKeyboardState(NULL);

  if(state[SDL_SCANCODE_UP] && canMoveUp(map)) 
    setBBy(getBBy() - 1);
    
  if(state[SDL_SCANCODE_DOWN] && canMoveDown(map)) 
    setBBy(getBBy() + 1);
    
  if(state[SDL_SCANCODE_LEFT] && canMoveLeft(map)) 
    setBBx(getBBx() - 1);
    
  if(state[SDL_SCANCODE_RIGHT] && canMoveRight(map)) 
    setBBx(getBBx() + 1);
}

void Hero::draw(Map* map) {
  SDL_BlitSurface(
    image, 
    locationInSpriteSheet->getInternalRect(),
    map->getMapSurface(), 
    boundingBox->getInternalRect()
  );
}