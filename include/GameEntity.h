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

  Rectangle* getStartingLocation();
  SDL_Surface* getImage();
  Rectangle* getBoundingBox();
  Rectangle* getLocationInSpriteSheet();

  int getBBx();
  int getBBy();
  int getBBw();
  int getBBh();

  void setBBx(int newX);
  void setBBy(int newY);
  void setBBw(int newW);
  void setBBh(int newH);
  
  bool canMoveUp(Map* map);
  bool canMoveDown(Map* map);
  bool canMoveLeft(Map* map);
  bool canMoveRight(Map* map);

  bool canMoveUpLeft(Map* map);
  bool canMoveUpRight(Map* map);
  bool canMoveDownLeft(Map* map);
  bool canMoveDownRight(Map* map);

  bool upIsInMap(Map* map);
  bool downIsInMap(Map* map);
  bool leftIsInMap(Map* map);
  bool rightIsInMap(Map* map);

  bool upLeftIsInMap(Map* map);
  bool upRightIsInMap(Map* map);
  bool downLeftIsInMap(Map* map);
  bool downRightIsInMap(Map* map);

protected:
  SDL_Surface* image; 
  Rectangle* startingLocation;
  Rectangle* locationInSpriteSheet;
  Rectangle* boundingBox;

  bool recoveringFromHit;
};

#endif