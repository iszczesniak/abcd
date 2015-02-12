#ifndef TMPoint_H
#define TMPoint_H

#include <ostream>
#include <algorithm>
#include <set>
#include <list>
//#include "tedge.h"

using namespace std;

class TMPoint;

bool turnRight(const TMPoint &p0, const TMPoint &p1, const TMPoint &p2);
ostream& operator << (ostream & out, list<TMPoint> &l);
list<TMPoint> otoczkaWypukla( const set<TMPoint> &zbiorTMPointow );

long double dist2(const TMPoint &p1, const TMPoint &p2);

/**
 * @brief isInsideOfCircle checks if s is inside of a circle wchich is determined by p, q, and r
 * @param p
 * @param q
 * @param r
 * @param s
  */
bool isInsideOfCircle(const TMPoint &pp, const TMPoint &qq, const TMPoint &rr, const TMPoint &s);



/**
 * @brief det calculates a determinant of 3x3 matrix
 * @param tab - matrix of size 3 x 3
 * @return a value of determinantdist2
 */
long double det(long double tab[][3]);

long double isTurn(const TMPoint &p0, const TMPoint &p1, const TMPoint &p2);

class TMPoint
{
  friend bool isInsideOfCircle(const TMPoint &p, const TMPoint &q, const TMPoint &r, const TMPoint &s);
  friend long double det(long double tab[][3], int size);
  long double x,y;
  //string nazwa;
public:
  TMPoint(long double x = 0, long double y = 0);
  void pokaz();
  long double getX() const {return x;}
  long double getY() const {return y;}
  void setX(long double newX) {x = newX;}
  void setY(long double newY) {y = newY;}
  bool operator == (const TMPoint &p) {return x == p.x && y == p.y;}

  friend ostream& operator<< (ostream& out, const TMPoint& p);
  friend bool operator< ( const TMPoint& p1, const TMPoint& p2);
  friend bool turnRight(const TMPoint &p0, const TMPoint &p1, const TMPoint &p2);
  friend long double whereTurn(const TMPoint &p0, const TMPoint &p1, const TMPoint &p2);
};



#endif // TMPoint_H
