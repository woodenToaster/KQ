#ifndef KQ_ENEMY_H
#define KQ_ENEMY_H

#include "GameEntity.h"
#include "Hero.h"

class Enemy : public GameEntity {
 public:
  Enemy();
  ~Enemy();

  void update(Map* map);
  void draw(Map* map);

  void hitHero(Hero* hero);
  void notifyHit();
  bool isAlive() const;
  
 private:
  bool alive;
};

#endif