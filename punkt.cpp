#include "punkt.h"
#include <iostream>


/**
 * @brief dist2 caclulates squere
 *
 */
long double dist2(const TMPoint &p1, const TMPoint &p2)
{
    return (p1.getX() - p2.getX()) * (p1.getX() - p2.getX()) + (p1.getY() - p2.getY()) * (p1.getY() - p2.getY());
}


/**
 * @brief isInsideOfCircle
 * @param p
 * @param q
 * @param r
 * @param s
 * @return
 */
bool isInsideOfCircle(const TMPoint &pp, const TMPoint &qq, const TMPoint &rr, const TMPoint &s)
{
    // ustawiamy punkty p, q, r na okręgu w porządku zgodnym z ruchem wskazówek zegara
    TMPoint p(pp), q, r;
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
        TMPoint temp(p);
        p = r;
        r = temp;
    }
    if ( turnRight(p, r, q) == false )
    {
        TMPoint temp(p);
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


long double det(long double tab[][3])
{
   return tab[0][0] * tab[1][1] * tab[2][2] +
          tab[0][1] * tab[1][2] * tab[2][0] +
          tab[0][2] * tab[1][0] * tab[2][1] -
          tab[0][2] * tab[1][1] * tab[2][0] -
          tab[1][0] * tab[0][1] * tab[2][2] -
          tab[0][0] * tab[2][1] * tab[1][2];
}



ostream& operator<< (ostream& out, const TMPoint& p) {
 return out << "(" << p.x <<"," << p.y << ")" ;
}

bool operator< ( const TMPoint& p1, const TMPoint& p2)
{

  if(p1.y == p2.y)
      return p1.x < p2.x;
  return p1.y < p2.y;
}

// porownanie wspolrzednych TMPointow
// A<B gdy gdy x od TMPointu A jest mniejszy  //
// jezeli wspolrzedne x sa takie same to wiekszy jest ten co ma wieksza wspolrzedna y // ten co jest wyzej wiekszy
//jezeli y rwne zwraca false

TMPoint::TMPoint(long double x, long double y) {
  this->x = x;
  this->y = y;

}

bool turnRight(const TMPoint &p0, const TMPoint &p1, const TMPoint &p2)
{
  TMPoint vP0P1(p1.x-p0.x, p1.y-p0.y);
  TMPoint vP0P2(p2.x-p0.x, p2.y-p0.y);
  if ( (vP0P1.x * vP0P2.y) - (vP0P1.y * vP0P2.x ) <= 0)
    return true;
  else
    return false;
}

/**
 * @brief whereTurn determines a turn direction, when one goes from p0 through p1 to p2
 * @param p0
 * @param p1
 * @param p2
 * @return  < 0 skręt w lewo
 *          0 - brak skrętu, punkty są współliniowe
 *          > 0 - skręt w prawo
 */
long double whereTurn(const TMPoint &p0, const TMPoint &p1, const TMPoint &p2)
{
  TMPoint vP0P1(p1.x-p0.x, p1.y-p0.y);
  TMPoint vP0P2(p2.x-p0.x, p2.y-p0.y);
  return (vP0P1.y * vP0P2.x ) - (vP0P1.x * vP0P2.y);
}



ostream& operator << (ostream & out, list<TMPoint> &l)
{
    out << "<";
    for( list<TMPoint>::iterator it = l.begin(); it != l.end(); ++it )
    {
        if (it == l.begin())
        out << *it;
    else
     out << ", " << *it;
    }
  return out << ">";
}

list<TMPoint> otoczkaWypukla( const set<TMPoint> &zbiorTMPointow )
{
  std::cout << zbiorTMPointow.size() << std::endl;
  list<TMPoint> lista(zbiorTMPointow.begin(), zbiorTMPointow.end());
  lista.sort();
  list<TMPoint> lista_gorna;
  list<TMPoint>::iterator it = lista.begin(); //pokazuje na pierwszy elem listy
  lista_gorna.push_back(*it);
  ++it; //wskazuje na 2 elem listy
  lista_gorna.push_back(*it); //dwa pierwsze TMPointy sa na liscie otoczki gornej
  ++it; //wskazuje na 3. elem listy
  for( ; it != lista.end(); ++it)
  {
    lista_gorna.push_back(*it); //wstawiamy TMPoint na liste otoczki gornej
    // odczytać z listy górnej trzy ostatnie TMPointy i sprawdzić warunek, czy skręt jest w prawo
    list<TMPoint>::iterator preIt = lista_gorna.end();
    --preIt;  // preIt wskazuje ostatni TMPoint na liście
    TMPoint P3 = *preIt;
    --preIt; //  preIt wskazuje przedostatni TMPoint na liściesony nex3n
    TMPoint P2 = *preIt;
    --preIt; //  preIt wskazuje na trzeci od końca TMPoint na liście
    TMPoint P1 = *preIt;
    while (lista_gorna.size() > 2 && turnRight(P1, P2, P3) == false ) //jezeli lista gorna ma wiecej niz 3 elem i trzy ost elem nie skrecaja w praow
    {
      //usunac przedostatni element lista gorna
      ++preIt; // wskazuje na przedostatni el. na liście górnej
      lista_gorna.erase(preIt);
      if (lista_gorna.size() > 2)
      {
        preIt = lista_gorna.end();
        --preIt;  // preIt wskazuje ostatni TMPoint na liście
        P3 = *preIt;
        --preIt; //  preIt wskazuje przedostatni TMPoint na liście
        P2 = *preIt;
        --preIt; //  preIt wskazuje na trzeci od końca TMPoint na liście
        P1 = *preIt;
      }
    }
  }
  list<TMPoint> lista_dolna;
  list<TMPoint>::reverse_iterator rIt = lista.rbegin();
  lista_dolna.push_back(*rIt);
  ++rIt; //wskazuje na 2 elem listy
  lista_dolna.push_back(*rIt); //dwa pierwsze TMPointy sa na liscie otoczki gornej
  ++rIt; //wskazuje na 3. elem listy
  for( ; rIt != lista.rend(); ++rIt)
  {
    lista_dolna.push_back(*rIt); //wstawiamy TMPoint na liste otoczki gornej
    // odczytać z listy górnej trzy ostatnie TMPointy i sprawdzić warunek, czy skręt jest w prawo
    it = lista_dolna.end();
    --it;  // preIt wskazuje ostatni TMPoint na liście
    TMPoint P3 = *it;
    --it; //  preIt wskazuje przedostatni TMPoint na liście
    TMPoint P2 = *it;
    --it; //  preIt wskazuje na trzeci od końca TMPoint na liście
    TMPoint P1 = *it;
    //cout << "dolna: " << P1 << ", " << P2 << ", " << P3 << ": " << czyWPrawo(P1, P2, P3) << endl;
    while (lista_dolna.size() > 2 && turnRight(P1, P2, P3) == false ) //jezeli lista gorna ma wiecej niz 3 elem i trzy ost elem nie skrecaja w praow
    {
      //usunac przedostatni element lista gorna
      ++it; // wskazuje na przedostatni el. na liście górnej
      lista_dolna.erase( it );
      if (lista_dolna.size() > 2)
      {
        it = lista_dolna.end();
        --it;
        P3 = *it;
        --it; //  preIt wskazuje przedostatni TMPoint na liście
        P2 = *it;
        --it; //  preIt wskazuje na trzeci od końca TMPoint na liście
        P1 = *it;
      }
   }
 }
 std::cout << lista_dolna.size() << " LISTA DOLNA: " << lista_dolna << std::endl << std::endl;
  //if (lista_dolna)
 it = lista_dolna.end();
 --it;
 lista_dolna.erase(it);
 it = lista_dolna.begin();
// cout << "LISTA DOLNA przed insert: " << lista_dolna << endl << endl;
 lista_gorna.insert(lista_gorna.end(), ++(lista_dolna.begin()), lista_dolna.end());
// cout << "LISTA po INSERT: " << lista_gorna << endl << endl;
 return lista_gorna;
}
