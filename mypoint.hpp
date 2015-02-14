#ifndef TMyPoint_H
#define TMyPoint_H

#include <ostream>
#include <algorithm>
#include <set>
#include <list>



class TMyPoint;

bool turnRight(const TMyPoint &p0, const TMyPoint &p1, const TMyPoint &p2);
std::ostream& operator << (std::ostream & out, std::list<TMyPoint> &l);
long double dist2(const TMyPoint &p1, const TMyPoint &p2);

/**
 * @brief isInsideOfCircle checks if s is inside of a circle wchich is determined by p, q, and r
 * @param p
 * @param q
 * @param r
 * @param s
  */
bool isInsideOfCircle(const TMyPoint &pp, const TMyPoint &qq, const TMyPoint &rr, const TMyPoint &s);

/**
 * @brief det calculates a determinant of 3x3 matrix
 * @param tab - matrix of size 3 x 3
 * @return a value of determinantdist2
 */
long double det(long double tab[][3]);


long double isTurn(const TMyPoint &p0, const TMyPoint &p1, const TMyPoint &p2);


class TMyPoint
{
  friend bool isInsideOfCircle(const TMyPoint &p, const TMyPoint &q, const TMyPoint &r, const TMyPoint &s);
  friend long double det(long double tab[][3], int size);
  friend std::ostream& operator<< (std::ostream& out, const TMyPoint& p);
  friend bool operator< ( const TMyPoint& p1, const TMyPoint& p2);
  friend long double whereTurn(const TMyPoint &p0, const TMyPoint &p1, const TMyPoint &p2);

private:
  long double x,y;
public:
  TMyPoint(long double x = 0, long double y = 0);
  long double getX() const {return x;}
  long double getY() const {return y;}
  void setX(long double newX) {x = newX;}
  void setY(long double newY) {y = newY;}
  bool operator == (const TMyPoint &p) {return x == p.x && y == p.y;}
};

#endif // TMyPoint_H
