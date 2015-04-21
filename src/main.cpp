#include <iostream>
#include <vector>
#include "Hero.h"
#include "Map.h"
#include "SDL.h"

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
  Map* map, 
  Hero* hero,
  SDL_Surface* enemy,
  SDL_Rect* enemyDest
);
void updateEnemy(SDL_Rect* heroLocation, SDL_Rect* enemyDest, SDL_Surface* surface);
void updateScreen(
  SDL_Window* window, 
  Map* map, 
  Hero* hero,
  SDL_Surface* enemy,
  SDL_Rect* enemyDest
);

bool heroAndEnemyOverlap(SDL_Rect* heroLocation, SDL_Rect* enemyLocation);
point center(SDL_Rect* rect);

bool overlaps(SDL_Rect* r1, SDL_Rect* r2);
bool xOverlaps(SDL_Rect* r1, SDL_Rect* r2);
bool yOverlaps(SDL_Rect* r1, SDL_Rect* r2);

int main(int argc, char** argv) {
  
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window* window;

  window = SDL_CreateWindow(
    "Kirp's Quest",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    Map::SCREEN_WIDTH,
    Map::SCREEN_HEIGHT,
    0
  );

  Hero hero;  
  Map map(window);

  SDL_Surface* enemy = SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);
  SDL_FillRect(enemy, NULL, SDL_MapRGB(enemy->format, 255, 0, 0));
  SDL_Rect enemyDest{Map::SCREEN_WIDTH - 32, Map::SCREEN_HEIGHT - 32, 32, 32};

  SDL_Event event;

  bool quit = false;

  while(!quit) {
    if(SDL_PollEvent(&event)) 
      quit = shouldClose(&event);
    
    if(heroAndEnemyOverlap(hero.getLocation(), &enemyDest))
      break;

    //TODO(Chris): Currently updating even when nothing changes.
    //We only want to update when something on the screen has changed.
    update(window, &map, &hero, enemy, &enemyDest);

    SDL_Delay(10);
  }

  SDL_FreeSurface(enemy);

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
  Map* map, 
  Hero* hero,
  SDL_Surface* enemy,
  SDL_Rect* enemyDest) {

  hero->update(map);

  //updateEnemy(dest, enemyDest, surface);

  updateScreen(window, map, hero, enemy, enemyDest);
}

//TODO: Normalize speed for player and enemy
// void updateEnemy(SDL_Rect* heroLocation, SDL_Rect* enemyDest, SDL_Surface* surface) {

//   point heroCenter = center(heroLocation);
//   point enemyCenter = center(enemyDest);

//   if(heroCenter.x < enemyCenter.x) {
//     if(canMoveLeft(enemyDest, surface))
//       enemyDest->x -= 1;
//   }

//   if(heroCenter.x > enemyCenter.x) {
//     if(canMoveRight(enemyDest, surface))
//       enemyDest->x += 1;
//   }

//   if(heroCenter.y < enemyCenter.y) {
//     if(canMoveUp(enemyDest, surface))
//       enemyDest->y -= 1;
//   }

//   if(heroCenter.y > enemyCenter.y) {
//     if(canMoveDown(enemyDest, surface))
//       enemyDest->y += 1;
//   }
// }

void updateScreen(
  SDL_Window* window, 
  Map* map, 
  Hero* hero,
  SDL_Surface* enemy,
  SDL_Rect* enemyDest) {

  map->drawTiles();
  map->drawObstructions();
  
  hero->draw(map);

  SDL_BlitSurface(enemy, NULL, map->getMapSurface(), enemyDest);
  SDL_UpdateWindowSurface(window);
}

bool overlaps(SDL_Rect* r1, SDL_Rect* r2) {
  return xOverlaps(r1, r2) && yOverlaps(r1, r2);
}

bool xOverlaps(SDL_Rect* r1, SDL_Rect* r2) {
  int x1 = r1->x;
  int x2 = r1->x + r1->w;
  int x3 = r2->x;
  int x4 = r2->x + r2->w;

  return x1 < x4 && x2 > x3;
}

bool yOverlaps(SDL_Rect* r1, SDL_Rect* r2) {
  int y1 = r1->y;
  int y2 = r1->y + r1->h;
  int y3 = r2->y;
  int y4 = r2->y + r2->h;

  return y1 < y4 && y2 > y3;
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
