#ifndef TEVENTQUEUE_H
#define TEVENTQUEUE_H
#include "mypoint.hpp"
#include <list>
#include <set>
#include <iostream>
#include <math.h>



class TNode
{
  friend std::ostream &operator << (std::ostream &out, const TNode &n);
private:
  TMyPoint p;
  std::set<TNode*> edges;
public:
  TNode(const TMyPoint &p): p(p) {};
  TNode& addEdge(TNode *n) {edges.insert(n); return *this;}
  void removeEdge(TNode *n);
  bool operator < (const TNode &n) const;
  TMyPoint getPoint() const {return p;}
  std::set<TNode*> getEdges() const {return edges;}
  unsigned int getEdgesNumber() const {return edges.size();}
};

class TTriangle;

void caclulateGabrielCircle(const TNode &n1, const TNode &n2, TMyPoint &o, long double &rG);
long double Gdistance(const TNode &n1, const TMyPoint &o);
void legalizeEdge(TNode *pr, TNode *pi, TNode *pj, TTriangle *T, std::list<TTriangle *> &triangles, TNode *p1, TNode *p2, TNode *p3);

void delaunayTriangulation(std::list<TTriangle *> &triangles, std::list<TNode *> &P);
void determine_M(std::list<TNode*> &PP, TMyPoint &p_1, TMyPoint &p_2, TMyPoint &p_3);
void findTriangles(TTriangle *T, TNode *n, TTriangle **t1, TTriangle **t2 );
void removedoubleNodes(std::list<TNode *> &P);
void convertDelaunay2GabrielGraph(std::list<TNode*> &P);
std::list<TNode *> generate_Nodes(unsigned int w, unsigned int h, unsigned int number);
unsigned int edgeNumber(std::list<TNode*> &P);


class TTriangle
{
    friend void legalizeEdge(TNode *pr, TNode *pi, TNode *pj, TTriangle *T, std::list<TTriangle *> &triangles, TNode *p1, TNode *p2, TNode *p3);
    friend void findTriangles(TTriangle *T, TNode *n, TTriangle **t1, TTriangle **t2 );
    friend std::ostream &operator << (std::ostream &out, TTriangle &t);
    friend void delaunayTriangulation(std::list<TTriangle *> &triangles, std::list<TNode*> &P);
    TNode *p1,
          *p2,
          *p3;
    std::set<TTriangle*> sons;
public:
    TTriangle(TNode *p1, TNode *p2, TNode *p3): p1(p1), p2(p2), p3(p3) {}
    TNode* getNode(int i);
    int isInside(TNode *n);
    void addSon(TTriangle *t) {sons.insert(t);}
};

#endif // TEVENTQUEUE_H
