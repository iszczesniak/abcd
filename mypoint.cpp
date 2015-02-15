#include "mypoint.hpp"
#include <iostream>

using namespace std;

/**
 * @brief dist2 caclulates squere between the points p1 and p2
 * @param p1 - the first point
 * @param p2 - the second point
 * @return sqered distance between p1 and p2
 */
long double dist2(const TMyPoint &p1, const TMyPoint &p2)
{
    return (p1.getX() - p2.getX()) * (p1.getX() - p2.getX()) + (p1.getY() - p2.getY()) * (p1.getY() - p2.getY());
}

/**
 * @brief isInsideOfCircle checks if the s point is inside of a circle which is determined by points: p, q, r
 * @param p - a point
 * @param q - a point
 * @param r - a point
 * @param s - a point
 * @return true if the s point is inside of a circle which is determined by points: p, q, r, otherwise false
 */
bool isInsideOfCircle(const TMyPoint &pp, const TMyPoint &qq, const TMyPoint &rr, const TMyPoint &s)
{
    // we set the points: p, q, r in the clockwise direction
    TMyPoint p(pp), q, r;
    if (dist2(p, qq) < dist2(p, rr) )
    {
        q = rr;
        r = qq;
    }
    else
    {
        q = qq;
        r = rr;
    }
    if (dist2(p, q) < dist2(q, r) )
    {
        TMyPoint temp(p);
        p = r;
        r = temp;
    }
    if (whereTurn(p, q, r) > 0)
    {
        TMyPoint temp(p);
        p = q;
        q = temp;
    }
    const int size = 3;
    long double tab[size][size] =
    {
        p.x - s.x, p.y - s.y, p.x * p.x + p.y * p.y - s.x * s.x - s.y * s.y,
        q.x - s.x, q.y - s.y, q.x * q.x + q.y * q.y - s.x * s.x - s.y * s.y,
        r.x - s.x, r.y - s.y, r.x * r.x + r.y * r.y - s.x * s.x - s.y * s.y,
    };
    long double d = det(tab);
    return ( d > 0);
}

/**
 * @brief det calculates the determinant of a matrix
 * @param tab - a matrix
 * @return value of the determinant
 */
long double det(long double tab[][3])
{
   return tab[0][0] * tab[1][1] * tab[2][2] +
          tab[0][1] * tab[1][2] * tab[2][0] +
          tab[0][2] * tab[1][0] * tab[2][1] -
          tab[0][2] * tab[1][1] * tab[2][0] -
          tab[1][0] * tab[0][1] * tab[2][2] -
          tab[0][0] * tab[2][1] * tab[1][2];
}

std::ostream& operator<< (std::ostream& out, const TMyPoint& p)
{
 return out << "(" << p.x <<"," << p.y << ")" ;
}

bool operator< (const TMyPoint& p1, const TMyPoint& p2)
{

  if(p1.y == p2.y)
      return p1.x < p2.x;
  return p1.y < p2.y;
}


TMyPoint::TMyPoint(long double x, long double y) {
  this->x = x;
  this->y = y;

}

/**
 * @brief whereTurn determines a turn direction, when one goes from p0 through p1 to p2
 * @param p0
 * @param p1
 * @param p2
 * @return  < 0 turns left
 *          0 - no turn,
 *          > 0 - turns right
 */
long double whereTurn(const TMyPoint &p0, const TMyPoint &p1, const TMyPoint &p2)
{
  TMyPoint vP0P1(p1.x-p0.x, p1.y-p0.y);
  TMyPoint vP0P2(p2.x-p0.x, p2.y-p0.y);
  return (vP0P1.y * vP0P2.x ) - (vP0P1.x * vP0P2.y);
}

ostream& operator << (ostream & out, list<TMyPoint> &l)
{
    out << "<";
    for( list<TMyPoint>::iterator it = l.begin(); it != l.end(); ++it )
    {
        if (it == l.begin())
        out << *it;
    else
     out << ", " << *it;
    }
  return out << ">";
}
