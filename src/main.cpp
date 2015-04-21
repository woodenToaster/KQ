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

struct obstruction {
  SDL_Surface* surface;
  SDL_Rect* dest;
};

struct point {
  double x;
  double y;
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
  SDL_Rect* dest,
  SDL_Surface* enemy,
  SDL_Rect* enemyDest
);
void updateHero(SDL_Rect* dest, SDL_Surface* surface);
void updateEnemy(SDL_Rect* heroLocation, SDL_Rect* enemyDest, SDL_Surface* surface);
void updateScreen(
  SDL_Window* window, 
  SDL_Surface* surface, 
  SDL_Surface* hero,
  SDL_Rect* dest,
  SDL_Surface* enemy,
  SDL_Rect* enemyDest
);

bool heroAndEnemyOverlap(SDL_Rect* heroLocation, SDL_Rect* enemyLocation);
point center(SDL_Rect* rect);

bool canMoveUp(SDL_Rect* dest, SDL_Surface* surface);
bool canMoveDown(SDL_Rect* dest, SDL_Surface* surface);
bool canMoveLeft(SDL_Rect* dest, SDL_Surface* surface);
bool canMoveRight(SDL_Rect* dest, SDL_Surface* surface);

bool upIsInMap(SDL_Rect* dest, SDL_Surface* surface);
bool downIsInMap(SDL_Rect* dest, SDL_Surface* surface);
bool leftIsInMap(SDL_Rect* dest, SDL_Surface* surface);
bool rightIsInMap(SDL_Rect* dest, SDL_Surface* surface);

bool targetIsObstruction(SDL_Rect* dest);
bool upIsObstruction(SDL_Rect* dest);
bool downIsObstruction(SDL_Rect* dest);
bool leftIsObstruction(SDL_Rect* dest);
bool rightIsObstruction(SDL_Rect* dest);

bool overlaps(SDL_Rect* r1, SDL_Rect* r2);

void makeTiles();
void drawTiles(SDL_Surface* surface);
void drawTile(SDL_Surface* surface, tile* t);

void makeObstructions();
void drawObstructions(SDL_Surface* surface);
void drawObstruction(SDL_Surface* surface, obstruction* o);

void destroyTiles();
void destroyObstructions();

std::vector<tile> tiles;
std::vector<obstruction> obstructions;

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
  SDL_Surface* enemy = SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);

  SDL_FillRect(hero, NULL, SDL_MapRGB(hero->format, 0, 0, 255));
  SDL_FillRect(enemy, NULL, SDL_MapRGB(enemy->format, 255, 0, 0));

  SDL_Rect dest{0,0,32,32};
  SDL_Rect enemyDest{SCREEN_WIDTH - 32, SCREEN_HEIGHT - 32, 32, 32};

  makeTiles();
  drawTiles(surface);

  makeObstructions();
  drawObstructions(surface);

  SDL_Event event;

  bool quit = false;

  while(!quit) {
    if(SDL_PollEvent(&event)) 
      quit = shouldClose(&event);
    
    if(heroAndEnemyOverlap(&dest, &enemyDest))
      break;

    //TODO(Chris): Currently updating even when nothing changes.
    //We only want to update when something on the screen has changed.
    update(window, surface, hero, &dest, enemy, &enemyDest);

    SDL_Delay(10);
  }

  SDL_FreeSurface(hero);
  SDL_FreeSurface(enemy);

  destroyTiles();
  destroyObstructions();

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
  SDL_Rect* dest,
  SDL_Surface* enemy,
  SDL_Rect* enemyDest) {

  updateHero(dest, surface);
  //updateEnemy(dest, enemyDest, surface);

  updateScreen(window, surface, hero, dest, enemy, enemyDest);
}

//TODO: Speed for diagonal movement should be speed/sqrt(2)
void updateHero(SDL_Rect* dest, SDL_Surface* surface) {
  
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

//TODO: Normalize speed for player and enemy
void updateEnemy(SDL_Rect* heroLocation, SDL_Rect* enemyDest, SDL_Surface* surface) {

  point heroCenter = center(heroLocation);
  point enemyCenter = center(enemyDest);

  if(heroCenter.x < enemyCenter.x) {
    if(canMoveLeft(enemyDest, surface))
      enemyDest->x -= 1;
  }

  if(heroCenter.x > enemyCenter.x) {
    if(canMoveRight(enemyDest, surface))
      enemyDest->x += 1;
  }

  if(heroCenter.y < enemyCenter.y) {
    if(canMoveUp(enemyDest, surface))
      enemyDest->y -= 1;
  }

  if(heroCenter.y > enemyCenter.y) {
    if(canMoveDown(enemyDest, surface))
      enemyDest->y += 1;
  }
}


void updateScreen(
  SDL_Window* window, 
  SDL_Surface* surface, 
  SDL_Surface* hero,
  SDL_Rect* dest,
  SDL_Surface* enemy,
  SDL_Rect* enemyDest) {

  drawTiles(surface);
  drawObstructions(surface);
  SDL_BlitSurface(hero, NULL, surface, dest);
  SDL_BlitSurface(enemy, NULL, surface, enemyDest);
  SDL_UpdateWindowSurface(window);
}

bool canMoveUp(SDL_Rect* dest, SDL_Surface* surface) {
  return upIsInMap(dest, surface) && !upIsObstruction(dest);
}

bool canMoveDown(SDL_Rect* dest, SDL_Surface* surface) {
  return downIsInMap(dest, surface) && !downIsObstruction(dest);
}

bool canMoveLeft(SDL_Rect* dest, SDL_Surface* surface) {
  return leftIsInMap(dest, surface) && !leftIsObstruction(dest);
}

bool canMoveRight(SDL_Rect* dest, SDL_Surface* surface) {
  return rightIsInMap(dest, surface) && !rightIsObstruction(dest); 
}

bool upIsInMap(SDL_Rect* dest, SDL_Surface* surface) {
  return dest->y > surface->clip_rect.y;
}

bool downIsInMap(SDL_Rect* dest, SDL_Surface* surface) {
  return (dest->y + dest->h) < (surface->clip_rect.y + surface->clip_rect.h);
}

bool leftIsInMap(SDL_Rect* dest, SDL_Surface* surface) {
  return dest->x > surface->clip_rect.x;
}

bool rightIsInMap(SDL_Rect* dest, SDL_Surface* surface) {
  return (dest->x + dest->w) < (surface->clip_rect.x + surface->clip_rect.w);
}

void makeTiles() {
  //TODO(Chris): potential memory leak.  Use std::shared_ptr
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

void makeObstructions() {
  SDL_Surface* boulder = SDL_CreateRGBSurface(0, TILE_SIZE * 4, TILE_SIZE * 4, 32, 0, 0, 0, 0);
  for(int i = 0; i < 3; ++i) {
    obstruction obs;
    obs.surface = boulder;
    obs.dest = new SDL_Rect{45 * (i + 1), 72 * (i + 1), TILE_SIZE, TILE_SIZE};
    obstructions.push_back(obs);
  }
}

void drawObstructions(SDL_Surface* targetSurface) {
  for(obstruction o : obstructions) {
    drawObstruction(targetSurface, &o);
  }
}

void drawObstruction(SDL_Surface* targetSurface, obstruction* o) {
  SDL_FillRect(o->surface, NULL, SDL_MapRGB(targetSurface->format, 127, 127, 127));
  SDL_BlitSurface(o->surface, NULL, targetSurface, o->dest);
}

bool overlaps(SDL_Rect* r1, SDL_Rect* r2) {
  int x1 = r1->x;
  int x2 = r1->x + r1->w;
  int x3 = r2->x;
  int x4 = r2->x + r2->w;

  bool xOverlaps = x1 < x4 && x2 > x3;

  int y1 = r1->y;
  int y2 = r1->y + r1->h;
  int y3 = r2->y;
  int y4 = r2->y + r2->h;

  bool yOverlaps = y1 < y4 && y2 > y3;

  return xOverlaps && yOverlaps;
}

bool heroAndEnemyOverlap(SDL_Rect* heroLocation, SDL_Rect* enemyLocation) {
  return overlaps(heroLocation, enemyLocation);
}

point center(SDL_Rect* rect) {
  point p;
  
  p.x = rect->x + rect->w / 2.0;
  p.y = rect->y + rect->h / 2.0;

  return p;
}

bool upIsObstruction(SDL_Rect* dest) {

  for(obstruction o : obstructions) {
    SDL_Rect newDest{dest->x, dest->y - 1, dest->w, dest->h};
    if(overlaps(&newDest, o.dest) && (dest->y + dest->h) > o.dest->y) 
      return true;
  }
  return false;
}

bool downIsObstruction(SDL_Rect* dest) {
 
  for(obstruction o : obstructions) {
    SDL_Rect newDest{dest->x, dest->y, dest->w, dest->h + 1};
    if(overlaps(&newDest, o.dest) && dest->y < o.dest->y) 
      return true;
  }
  return false;
}

bool leftIsObstruction(SDL_Rect* dest) {

  for(obstruction o : obstructions) {
    SDL_Rect newDest{dest->x - 1, dest->y, dest->w, dest->h};
    if(overlaps(&newDest, o.dest) && dest->x + dest->w > o.dest->x) 
      return true;
  }
  return false;
}

bool rightIsObstruction(SDL_Rect* dest) {

  for(obstruction o : obstructions) {
    SDL_Rect newDest{dest->x, dest->y, dest->w + 1, dest->h};
    if(overlaps(&newDest, o.dest) && dest->x < o.dest->x + o.dest->w) 
      return true;
  }
  return false;
}

void destroyTiles() {
  for(tile t : tiles) {
    delete t.dest;
  }
  //TODO(Chris): Handle this with std::shared_ptr to avoid memory leaks
  SDL_FreeSurface(tiles[0].surface);
}

void destroyObstructions() {
  for(obstruction o : obstructions) {
    delete o.dest;
  }
  //TODO(Chris): Handle this with std::shared_ptr to avoid memory leaks
  SDL_FreeSurface(obstructions[0].surface);
}


  

