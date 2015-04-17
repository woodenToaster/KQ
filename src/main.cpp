#include <iostream>
#include <vector>
#include "SDL.h"

const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;
const int TILE_SIZE = 8;

struct tile {
  SDL_Surface* surface;
  SDL_Rect* dest;
};

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
void updateDestination(SDL_Rect* dest, SDL_Surface* surface);
void updateScreen(
  SDL_Window* window, 
  SDL_Surface* surface, 
  SDL_Surface* hero,
  SDL_Rect* dest
);

bool canMoveUp(SDL_Rect* dest, SDL_Surface* surface);
bool canMoveDown(SDL_Rect* dest, SDL_Surface* surface);
bool canMoveLeft(SDL_Rect* dest, SDL_Surface* surface);
bool canMoveRight(SDL_Rect* dest, SDL_Surface* surface);

void makeTiles();
void drawTiles(SDL_Surface* surface);
void drawTile(SDL_Surface* surface, tile* t);



std::vector<tile> tiles;

int main(int argc, char** argv) {
  
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window* window;
  SDL_Surface* surface;

  

  window = SDL_CreateWindow(
    "Kirp's Quest",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    0
  );
  
  surface = SDL_GetWindowSurface(window);
  
  SDL_Surface* hero = SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);
  
  SDL_FillRect(hero, NULL, SDL_MapRGB(hero->format, 0, 0, 255));

  SDL_Rect dest{0,0,32,32};

  SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));
  SDL_BlitSurface(hero, NULL, surface, &dest);
  SDL_UpdateWindowSurface(window);

  makeTiles();
  drawTiles(surface);

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

  //TODO: Manage resources properly
  SDL_FreeSurface(tiles[0].surface);
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

  updateDestination(dest, surface);
  updateScreen(window, surface, hero, dest);
}

void updateDestination(SDL_Rect* dest, SDL_Surface* surface) {
  
  const Uint8* state = SDL_GetKeyboardState(NULL);

  if(state[SDL_SCANCODE_UP] && canMoveUp(dest, surface)) 
    dest->y -= 1;
    
  if(state[SDL_SCANCODE_DOWN] && canMoveDown(dest, surface)) 
    dest->y += 1;
    
  if(state[SDL_SCANCODE_LEFT] && canMoveLeft(dest, surface)) 
    dest->x -= 1;
    
  if(state[SDL_SCANCODE_RIGHT] && canMoveRight(dest, surface)) 
    dest->x += 1;
}

void updateScreen(
  SDL_Window* window, 
  SDL_Surface* surface, 
  SDL_Surface* hero,
  SDL_Rect* dest) {

  drawTiles(surface);
  SDL_BlitSurface(hero, NULL, surface, dest);
  SDL_UpdateWindowSurface(window);
}

bool canMoveUp(SDL_Rect* dest, SDL_Surface* surface) {
  return dest->y > surface->clip_rect.y;
}

bool canMoveDown(SDL_Rect* dest, SDL_Surface* surface) {
  return (dest->y + dest->h) < (surface->clip_rect.y + surface->clip_rect.h);
}

bool canMoveLeft(SDL_Rect* dest, SDL_Surface* surface) {
  return dest->x > surface->clip_rect.x;
}

bool canMoveRight(SDL_Rect* dest, SDL_Surface* surface) {
  return (dest->x + dest->w) < (surface->clip_rect.x + surface->clip_rect.w);
}

void makeTiles() {
  //TODO: potential memory leak.  Use std::shared_ptr
  SDL_Surface* grassSurface = SDL_CreateRGBSurface(0, TILE_SIZE, TILE_SIZE, 32, 0, 0, 0, 0);

  for(int row = 0; row < (SCREEN_WIDTH / TILE_SIZE); ++row) {
    for(int column = 0; column <  (SCREEN_HEIGHT / TILE_SIZE); ++column) {
      tile t;
      t.surface = grassSurface;
      t.dest = new SDL_Rect{TILE_SIZE * row, TILE_SIZE * column, TILE_SIZE, TILE_SIZE};
      tiles.push_back(t);
    }
  }
}

void drawTiles(SDL_Surface* surface) {
  for(tile t : tiles) {
    drawTile(surface, &t);
  }
}

void drawTile(SDL_Surface* surface, tile* t) {
  SDL_FillRect(t->surface, t->dest, SDL_MapRGB(surface->format, 0, 255, 0));
  SDL_BlitSurface(t->surface, NULL, surface, t->dest);
}