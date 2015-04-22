#include "Hero.h"
#include "Map.h"
#include "Rectangle.h"

Hero::Hero() {
	
  image = SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);
  location = new Rectangle(0,0,32,32);

  SDL_FillRect(image, NULL, SDL_MapRGB(image->format, 0, 0, 255));
}

Hero::~Hero() {
  delete location;
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
}

void Hero::draw(Map* map) {
  SDL_BlitSurface(image, NULL, map->getMapSurface(), location->getInternalRect());
}