#include "Rectangle.h"

Rectangle::Rectangle() {
  internal_rect = new SDL_Rect{0, 0, 0, 0};
}

Rectangle::Rectangle(int x, int y, int w, int h) {
	
  internal_rect = new SDL_Rect{x, y, w, h};
}

Rectangle::Rectangle(Rectangle* other) {
  
  int x = other->getX();
  int y = other->getY();
  int w = other->getWidth();
  int h = other->getHeight();

  internal_rect = new SDL_Rect{x, y, w, h};
}

Rectangle::~Rectangle() {
  delete internal_rect;
}
 
SDL_Rect* Rectangle::getInternalRect() {
  return internal_rect;
}

int Rectangle::getX() {
  return internal_rect->x;
}

int Rectangle::getY() {
  return internal_rect->y;
}

int Rectangle::getWidth() {
  return internal_rect->w;
}

int Rectangle::getHeight() {
  return internal_rect->h;
}

void Rectangle::setX(int newX) {
  internal_rect->x = newX;
}

void Rectangle::setY(int newY) {
  internal_rect->y = newY;
}

void Rectangle::setWidth(int newW) {
  internal_rect->w = newW;
}

void Rectangle::setHeight(int newH) {
  internal_rect->h = newH;
}

bool Rectangle::overlaps(SDL_Rect* rect) {
  return xOverlaps(rect) && yOverlaps(rect);
}

bool Rectangle::xOverlaps(SDL_Rect* rect) {
  int x1 = internal_rect->x;
  int x2 = internal_rect->x + internal_rect->w;
  int x3 = rect->x;
  int x4 = rect->x + rect->w;

  return x1 < x4 && x2 > x3;
}

bool Rectangle::yOverlaps(SDL_Rect* rect) {
  int y1 = internal_rect->y;
  int y2 = internal_rect->y + internal_rect->h;
  int y3 = rect->y;
  int y4 = rect->y + rect->h;

  return y1 < y4 && y2 > y3;
}
