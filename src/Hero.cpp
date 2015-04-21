#include "Hero.h"

Hero::Hero() {
	
  image = SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);
  location = new SDL_Rect{0,0,32,32};

  SDL_FillRect(image, NULL, SDL_MapRGB(image->format, 0, 0, 255));
}

Hero::~Hero() {
  delete location;
}

void Hero::update(Map* map) {

  const Uint8* state = SDL_GetKeyboardState(NULL);

  if(state[SDL_SCANCODE_UP] && canMoveUp(map)) 
    location->y -= 1;
    
  if(state[SDL_SCANCODE_DOWN] && canMoveDown(map)) 
    location->y += 1;
    
  if(state[SDL_SCANCODE_LEFT] && canMoveLeft(map)) 
    location->x -= 1;
    
  if(state[SDL_SCANCODE_RIGHT] && canMoveRight(map)) 
    location->x += 1;
}

void Hero::draw(Map* map) {
  SDL_BlitSurface(image, NULL, map->getMapSurface(), location);
}