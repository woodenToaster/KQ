#ifndef KQ_RECTANGLE_H
#define KQ_RECTANGLE_H

#include "SDL.h"

class Rectangle {
public:
  Rectangle(int x, int y, int w, int h);
  ~Rectangle();

  SDL_Rect* getInternalRect();

  int getX();
  int getY();
  int getWidth();
  int getHeight();
  
  bool overlaps(SDL_Rect* rect);
  bool xOverlaps(SDL_Rect* rect);
  bool yOverlaps(SDL_Rect* rect);

private:
  SDL_Rect* internal_rect;
};

#endif