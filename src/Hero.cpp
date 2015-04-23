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
  location = new Rectangle(0, 0, 24, 24);
  boundingBox = new Rectangle(0, 0, 24, 24);
  locationInSpriteSheet = new Rectangle(0, 136, 24, 24);
}

Hero::~Hero() {
  delete location;
  delete locationInSpriteSheet;
  delete boundingBox;
}

//TODO: Speed for diagonal movement should be speed/sqrt(2)
void Hero::update(Map* map) {

  const Uint8* state = SDL_GetKeyboardState(NULL);

  if(state[SDL_SCANCODE_UP] && canMoveUp(map)) 
    setY(getY() - 1);
    
  if(state[SDL_SCANCODE_DOWN] && canMoveDown(map)) 
    setY(getY() + 1);
    
  if(state[SDL_SCANCODE_LEFT] && canMoveLeft(map)) 
    setX(getX() - 1);
    
  if(state[SDL_SCANCODE_RIGHT] && canMoveRight(map)) 
    setX(getX() + 1);

  updateBoundingBox();
}

void Hero::updateBoundingBox() {
  setBBx(getX() + 4);
  setBBy(getY() + 1);
  setBBw(getWidth() - 8);
  setBBh(getHeight() - 2);
}

void Hero::draw(Map* map) {
  SDL_BlitSurface(
    image, 
    locationInSpriteSheet->getInternalRect(),
    map->getMapSurface(), 
    boundingBox->getInternalRect()
  );
}