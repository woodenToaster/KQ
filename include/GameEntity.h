#ifndef KQ_GAME_ENTITY_H
#define KQ_GAME_ENTITY_H

#include "Map.h"
#include "SDL.h"

class GameEntity {
public:
  virtual ~GameEntity() = 0;
  virtual void update(Map* map) = 0;
  virtual void draw(Map* map) = 0;

  SDL_Rect* getLocation();
  SDL_Surface* getImage();

  bool canMoveUp(Map* map);
  bool canMoveDown(Map* map);
  bool canMoveLeft(Map* map);
  bool canMoveRight(Map* map);

  bool upIsInMap(Map* map);
  bool downIsInMap(Map* map);
  bool leftIsInMap(Map* map);
  bool rightIsInMap(Map* map);

protected:
  SDL_Surface* image; 
  SDL_Rect* location;
};

#endif