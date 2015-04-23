#ifndef KQ_HERO_H
#define KQ_HERO_H

#include "GameEntity.h"

class Hero : public GameEntity {
public:
  Hero();
  ~Hero();

  void update(Map* map);
  void updateBoundingBox();
  void draw(Map* map);

private:

};

#endif