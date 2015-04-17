#include <iostream>
#include "SDL.h"

bool shouldClose(SDL_Event* event);
bool windowCloseClicked(SDL_Event* event);
bool escapePressed(SDL_Event* event);
bool isWindowEvent(SDL_Event* event);
bool isWindowCloseEvent(SDL_Event* event);
bool isKeyboardEvent(SDL_Event* event);
bool keyPressedIsEscape(SDL_Event* event);
bool inputDetected();
void update(
  SDL_Window* window, 
  SDL_Surface* surface, 
  SDL_Surface* hero,
  SDL_Rect* dest
);
void updateDestination(SDL_Rect* dest);
void updateScreen(
  SDL_Window* window, 
  SDL_Surface* surface, 
  SDL_Surface* hero,
  SDL_Rect* dest
);

int main(int argc, char** argv) {
  
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window* window;
  SDL_Surface* surface;

  window = SDL_CreateWindow(
    "Kirp's Quest",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    320,
    240,
    0
  );
  
  surface = SDL_GetWindowSurface(window);
  
  SDL_Surface* hero = SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);
  
  SDL_FillRect(hero, NULL, SDL_MapRGB(hero->format, 0, 255, 0));

  SDL_Rect dest{0,0,32,32};

  SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));
  SDL_BlitSurface(hero, NULL, surface, &dest);
  SDL_UpdateWindowSurface(window);

  SDL_Event event;

  bool quit = false;

  while(!quit) {
    if(SDL_PollEvent(&event)) 
      quit = shouldClose(&event);
    
    //TODO(Chris): Currently updating even when nothing changes.
    //We only want to update when something on the screen has changed.
    update(window, surface, hero, &dest);

    SDL_Delay(10);
  }

  SDL_FreeSurface(hero);

  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}

bool shouldClose(SDL_Event* event) {
    return windowCloseClicked(event) || escapePressed(event);
}

bool windowCloseClicked(SDL_Event* event) {
  return isWindowEvent(event) && isWindowCloseEvent(event);
}

bool escapePressed(SDL_Event* event) {
  return isKeyboardEvent(event) && keyPressedIsEscape(event);
}

bool isWindowEvent(SDL_Event* event) {
  return event->type == SDL_WINDOWEVENT;
}

bool isWindowCloseEvent(SDL_Event* event) {
  return event->window.event == SDL_WINDOWEVENT_CLOSE;
}

bool isKeyboardEvent(SDL_Event* event) {
  return event->type == SDL_KEYDOWN;
}

bool keyPressedIsEscape(SDL_Event* event) {
  return event->key.keysym.sym == SDLK_ESCAPE;
}

void update(
  SDL_Window* window, 
  SDL_Surface* surface, 
  SDL_Surface* hero,
  SDL_Rect* dest) {

  updateDestination(dest);
  updateScreen(window, surface, hero, dest);
}

void updateDestination(SDL_Rect* dest) {
  
  const Uint8* state = SDL_GetKeyboardState(NULL);

  if(state[SDL_SCANCODE_UP]) 
    dest->y -= 1;
    
  if(state[SDL_SCANCODE_DOWN]) 
    dest->y += 1;
    
  if(state[SDL_SCANCODE_LEFT]) 
    dest->x -= 1;
    
  if(state[SDL_SCANCODE_RIGHT]) 
    dest->x += 1;
}

void updateScreen(
  SDL_Window* window, 
  SDL_Surface* surface, 
  SDL_Surface* hero,
  SDL_Rect* dest) {

  SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));
  SDL_BlitSurface(hero, NULL, surface, dest);
  SDL_UpdateWindowSurface(window);
}