#ifndef TEVENTQUEUE_H
#define TEVENTQUEUE_H
#include "punkt.h"
#include <list>
#include <set>
#include <iostream>
#include <math.h>

using namespace  std;



class TNode
{
    friend ostream &operator << (ostream &out, const TNode &n);
private:
  TMPoint p;
  set<TNode*> edges;
public:
  TNode(const TMPoint &p): p(p) {};
  TNode& addEdge(TNode *n) {edges.insert(n); return *this;}
  void removeEdge(TNode *n);
  bool operator < (const TNode &n) const;
  TMPoint getPoint() const {return p;}
  set<TNode*> getEdges() const {return edges;}

};

class TTriangle;

void caclulateGabrielCircle(const TNode &n1, const TNode &n2, TMPoint &o, long double &rG);
long double Gdistance(const TNode &n1, const TMPoint &o);
void legalizeEdge(TNode *pr, TNode *pi, TNode *pj, TTriangle *T, list<TTriangle *> &triangles, TNode *p1, TNode *p2, TNode *p3);

void delaunayTriangulation(list<TTriangle *> &triangles, list<TNode *> &P);
void determine_M(list<TNode*> &PP, TMPoint &p_1, TMPoint &p_2, TMPoint &p_3);
void findTriangles(TTriangle *T, TNode *n, TTriangle **t1, TTriangle **t2 );
void removedoubleNodes(list<TNode *> &P);
void makeGabrielGraph(list<TNode*> &P);
list<TNode *> generate_Nodes(unsigned int w, unsigned int h, unsigned int number);

class TTriangle
{
    friend void legalizeEdge(TNode *pr, TNode *pi, TNode *pj, TTriangle *T, list<TTriangle *> &triangles, TNode *p1, TNode *p2, TNode *p3);
    friend void findTriangles(TTriangle *T, TNode *n, TTriangle **t1, TTriangle **t2 );
    friend ostream &operator << (ostream &out, TTriangle &t);
    friend void delaunayTriangulation(list<TTriangle *> &triangles, list<TNode*> &P);
    TNode *p1,
          *p2,
          *p3;
    set<TTriangle*> sons;
public:
    TTriangle(TNode *p1, TNode *p2, TNode *p3): p1(p1), p2(p2), p3(p3) {}
    TNode* getNode(int i);
    int isInside(TNode *n);
    void addSon(TTriangle *t) {sons.insert(t);}
    //bool operator == (const TTriangle &t) const;
};






void removeWrongAspiredEdges(list<TNode*> &aspiredEdges, const TNode &n);

class TEventQueue
{
private:
    set<TNode> q_events;
public:
    TEventQueue();
//    void addEvent(TEvent &e);
friend ostream &operator << (ostream &out, const TEventQueue &eq);
};


class TState
{
private:
   // set<TEdgeState> state;
public:
    /*void add(const TEdge &e);
    void remove(const TEdge &e);
    TEdge *onLeft(const TEdge &e);
    TEdge *onRight(const TEdge &e);
*/
};

#endif // TEVENTQUEUE_H
