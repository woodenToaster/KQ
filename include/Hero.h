#ifndef KQ_HERO_H
#define KQ_HERO_H

#include "GameEntity.h"

class Hero : public GameEntity {
 public:
  Hero();
  ~Hero();

  enum Direction {
    RIGHT,
    UPPER_RIGHT,
    UP,
    UPPER_LEFT,
    LEFT,
    LOWER_LEFT,
    DOWN,
    LOWER_RIGHT
  };

  virtual void update(Map* map);
  virtual void draw(Map* map);

  void updateBoundingBox();

  void attack(Map* map);
  void drawWeapon(Map* map);
  void clearWeapon();

  void drawSwordRight(Map* map);
  void drawSwordLeft(Map* map);
  void drawSwordUp(Map* map);
  void drawSwordDown(Map* map);
  void drawSword(Rectangle& dest, Map* map);

  Rectangle getHorizontalSword();
  Rectangle getVerticalSword();

  Rectangle* getWeaponBoundingBox() const;

  bool isAttacking() const;
  bool isAlive() const;

  //TODO: virtual method on base class?
  void notifyCollided(GameEntity* entity);

  static Uint32 doneAttacking(Uint32 interval, void* mapInstance);
  static Uint32 recover(Uint32 interval, void* heroInstance);

  Rectangle* getFacingDirection() const;

 private:
  static const uint16_t direction_masks[4];
  static const int direction_values[16];

  uint16_t facing;
  bool attacking;
  bool alive;
  int life;

  Rectangle* direction_images[8];

  SDL_Surface* weaponImage;
  Rectangle* weaponBoundingBox;

};

#endif