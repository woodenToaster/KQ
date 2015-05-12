#include <iostream>
#include <vector>
#include "Enemy.h"
#include "Hero.h"
#include "Map.h"
#include "SDL.h"

const int FRAMES_PER_SECOND = 60;
const int MS_PER_FRAME = 1000 / FRAMES_PER_SECOND;

bool shouldClose(SDL_Event* event);
bool windowCloseClicked(SDL_Event* event);
bool escapePressed(SDL_Event* event);
bool isWindowEvent(SDL_Event* event);
bool isWindowCloseEvent(SDL_Event* event);
bool isKeyboardEvent(SDL_Event* event);
bool keyPressedIsEscape(SDL_Event* event);
bool inputDetected();

bool spacebarPressed(SDL_Event* event);
bool heroHitEnemy(Hero* hero, Enemy* enemy);
bool collided(Rectangle* r1, Rectangle* r2);

void update(SDL_Window* window, Map* map, Hero* hero, Enemy* enemy);
void drawWorld(SDL_Window* window, Map* map, Hero* hero, Enemy* enemy);

int main(int argc, char** argv) {
  
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

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
  Enemy enemy;
  Map map(window, &hero);
  map.putHeroOnMap(&hero);
  map.putEnemyOnMap(&enemy);
  std::string mapFile = "1.dat";
  map.loadTileDataFromFile(mapFile);
  SDL_Event event;

  bool quit = false;

  while(!quit) {
    Uint32 start = SDL_GetTicks();
    
    if(!hero.isAlive())
      quit = true;

    if(SDL_PollEvent(&event)) {
      quit = shouldClose(&event);
      if(spacebarPressed(&event))
        hero.attack(&map);
    }
    
    if(collided(hero.getBoundingBox(), enemy.getBoundingBox()))
      hero.notifyCollided(&enemy);

    //TODO(Chris): Currently updating even when nothing changes.
    //We only want to update when something on the screen has changed.
    update(window, &map, &hero, &enemy);
    drawWorld(window, &map, &hero, &enemy);

    Uint32 msToSleep = start + MS_PER_FRAME - SDL_GetTicks();
    if(msToSleep > 0)
      SDL_Delay(msToSleep);
  }

  //SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Hi", "Testing message box.", NULL);
  
  map.destroyTileImages();

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

bool spacebarPressed(SDL_Event* event) {

  return event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_SPACE;
}

bool collided(Rectangle* r1, Rectangle* r2) {
  return r1->overlaps(r2->getInternalRect());
}

void update(SDL_Window* window, Map* map, Hero* hero, Enemy* enemy) {

  hero->update(map);
  if(heroHitEnemy(hero, enemy) && hero->isAttacking())
    enemy->notifyHit();
  if(enemy->isAlive())
    enemy->update(map);
  
}

bool heroHitEnemy(Hero* hero, Enemy* enemy) {
  return collided(hero->getWeaponBoundingBox(), enemy->getBoundingBox());
}

void drawWorld(SDL_Window* window, Map* map, Hero* hero, Enemy* enemy) {

  map->drawTiles();
  map->drawObstructions();
  map->drawHarvestableTiles();
  
  hero->draw(map);
  if(hero->isAttacking())
    hero->drawWeapon(map);
  enemy->draw(map);
  
  //TODO: This should be in update(), but it has to check after the sword has been drawn
  // because that's when the sword's bounding box coordinates are updated.
  map->checkHarvesting();
  
  SDL_UpdateWindowSurface(window);
}

