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

  void update(Map* map);
  void updateBoundingBox();
  void draw(Map* map);

  Rectangle* getFacingDirection() const;

private:
  static const uint16_t direction_masks[4];
  static const int direction_values[16];

  uint16_t facing;
  Rectangle* direction_images[8];

};

#endif