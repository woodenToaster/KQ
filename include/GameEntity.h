#ifndef KQ_GAME_ENTITY_H
#define KQ_GAME_ENTITY_H

#include "SDL.h"

class GameEntity {
public:
  virtual ~GameEntity() = 0;
  virtual void update(Map* map) = 0;
  virtual void draw(Map* map) = 0;

  inline SDL_Rect* getLocation() {return location;}
  inline SDL_Surface* getImage() {return image;}

  bool canMoveUp(Map* map);
  bool canMoveDown(Map* map);
  bool canMoveLeft(Map* map);
  bool canMoveRight(Map* map);

  bool upIsMap(Map* map);
  bool downIsMap(Map* map);
  bool leftIsMap(Map* map);
  bool rightIsMap(Map* map);


protected:
  SDL_Surface* image; 
  SDL_Rect* location;
};

#endif