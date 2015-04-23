#ifndef KQ_GAME_ENTITY_H
#define KQ_GAME_ENTITY_H

#include "Rectangle.h"
#include "SDL.h"

class Map;

class GameEntity {
public:
  virtual ~GameEntity() {}
  virtual void update(Map* map) = 0;
  virtual void draw(Map* map) = 0;

  Rectangle* getLocation();
  SDL_Surface* getImage();

  int getX();
  int getY();
  int getWidth();
  int getHeight();

  void setX(int newX);
  void setY(int newY);

  void setBBx(int newX);
  void setBBy(int newY);
  void setBBw(int newW);
  void setBBh(int newH);
  
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
  Rectangle* location;
  Rectangle* locationInSpriteSheet;
  Rectangle* boundingBox;
};

#endif