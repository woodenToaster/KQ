#ifndef KQ_ENEMY_H
#define KQ_ENEMY_H

#include "GameEntity.h"
#include "Hero.h"

class Enemy : public GameEntity {
 public:
  Enemy();
  ~Enemy();

  virtual void update(Map* map);
  virtual void draw(Map* map);

  void hitHero(Hero* hero);
  void notifyHit();
  bool isAlive() const;
  
  void decrementLife();

  static Uint32 recover(Uint32 interval, void* enemyInstance);

 private:
  bool alive;
  int life;
};

#endif