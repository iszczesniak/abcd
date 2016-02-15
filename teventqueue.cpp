#include "teventqueue.hpp"
#include <list>
#include <fstream>
#include <assert.h>

using namespace std;

/**
 * @brief Gdistance calculates a distance between n1 and o
 * @param n1 - a node
 * @param o - a centre of a circle
 * @return a distance
 */
long double Gdistance(const TNode &n1, const TMyPoint &o)
{
    TMyPoint p1 = n1.getPoint();
    long double rG = sqrt((p1.getX() - o.getX()) * (p1.getX() - o.getX()) + (p1.getY() - o.getY()) * (p1.getY() - o.getY())) ; // sqrt
    return rG;
}

void caclulateGabrielCircle(const TNode &n1, const TNode &n2, TMyPoint &o, long double &rG)
{
    TMyPoint p1 = n1.getPoint();
    TMyPoint p2 = n2.getPoint();
    o.setX( (p1.getX() + p2.getX()) / 2.0 );
    o.setY( (p1.getY() + p2.getY()) / 2.0 );
    rG = sqrt((p1.getX() - p2.getX()) * (p1.getX() - p2.getX()) + (p1.getY() - p2.getY()) * (p1.getY() - p2.getY()))/2.0 ;  // sqrt
}

bool TNode::operator < (const TNode &n) const
{
    if (p.getY() == n.p.getY())
        return p.getX() < n.p.getX();
    else
        return p.getY() < n.p.getY();
}

void TNode::removeEdge(TNode *n)
{
    edges.erase(n);
}

ostream &operator << (ostream &out, const TNode &n)
{
    out << n.getPoint() << " <<-->> ";
    for (set<TNode*>::iterator it = n.edges.begin(); it != n.edges.end(); ++it)
    {
        out << (*it)->getPoint() << " ";
    }
    return out;
}

/**
 * @brief TTringle::isInside checks the location of n against a triangle
 * @param n - node (point) which is tested
 * @return -1 - outside
 *          0 - on edge
 *          1 - inside
 *
 */
int TTriangle::isInside(TNode *n)
{
    TMyPoint p(p1->getPoint());
    TMyPoint q(p2->getPoint());
    TMyPoint r(p3->getPoint());
    TMyPoint s(n->getPoint());
    long double pr1 = whereTurn(p,q, s);
    long double pr2 = whereTurn(p, r, s);
    long double product = pr1 * pr2;
    TMyPoint pmin, pmax;
    if (product == 0)
    {
        if (pr1 == 0)
        {
            pmin.setX(p.getX() <= q.getX()? p.getX() : q.getX());
            pmin.setY(p.getY() <= q.getY()? p.getY() : q.getY());
            pmax.setX(p.getX() >= q.getX()? p.getX() : q.getX());
            pmax.setY(p.getY() >= q.getY()? p.getY() : q.getY());

        }
        else
        {
            pmin.setX(p.getX() <= r.getX()? p.getX() : r.getX());
            pmin.setY(p.getY() <= r.getY()? p.getY() : r.getY());
            pmax.setX(p.getX() >= r.getX()? p.getX() : r.getX());
            pmax.setY(p.getY() >= r.getY()? p.getY() : r.getY());
        }
        if (s.getX() >= pmin.getX() && s.getX() <= pmax.getX() && s.getY() >= pmin.getY() && s.getY() <= pmax.getY())
            return 0;
        else
            return -1;

    }
    if (product > 0)
        return -1;
    pr1 = whereTurn(q,p, s);
    pr2 = whereTurn(q, r, s);
    product = pr1 * pr2;
    if (product == 0)
    {
        pmin.setX(q.getX() <= r.getX()? q.getX() : r.getX());
        pmin.setY(q.getY() <= r.getY()? q.getY() : r.getY());
        pmax.setX(q.getX() >= r.getX()? q.getX() : r.getX());
        pmax.setY(q.getY() >= r.getY()? q.getY() : r.getY());
        if (s.getX() >= pmin.getX() && s.getX() <= pmax.getX() && s.getY() >= pmin.getY() && s.getY() <= pmax.getY())
            return 0;
        else
            return -1;
    }
    if (product > 0)
        return -1;
    pr1 = whereTurn(r,p, s);
    pr2 = whereTurn(r, q, s);
    product = pr1 * pr2;
    if (product == 0)
    {
        cerr << "assert(isInside)";
        exit(-1);
    }
    if (product > 0)
        return -1;
    return 1;
}

void determine_M(list<TNode*> &P, TMyPoint &p_1, TMyPoint & p_2, TMyPoint &p_3)
{
   if (P.size() < 1)
       cerr << "ERROR: list has no elements." << endl;
   TMyPoint p;
   long double M = 0;
   for (list<TNode*>::iterator it = P.begin(); it != P.end(); ++it)
   {
      p = (*it)->getPoint();
      if (fabs(p.getX()) > M)
          M = fabs(p.getX());
      if (fabs(p.getY()) > M)
          M = fabs(p.getY());
   }
   p_1.setX(3 * M);
   p_1.setY(0);
   p_2.setX(0);
   p_2.setY(3 * M);
   p_3.setX(-3 * M);
   p_3.setY(-3 * M);
}

/**
 * @brief findTriangles finds a trinangle or two triangles in a graph T that includes a node n (when n lies on a edge the function return the references to 2 triangles)
 * @param T - pointer to a main node in a graph T
 * @param n - a node
 * @param t1 - reference to a triangle that include a node n (or 0)
 * @param t2 - reference to a triangle that include a node n (or 0)
 */
void findTriangles(TTriangle *T, TNode *n, TTriangle* *t1, TTriangle* *t2 )
{
    int ans = T->isInside(n);
    switch (ans) {
    case -1:      // wierzchołek leży na zewnątrz trójkąta T
        return;
        break;
    case 0:       // wierzchołek leży na krawędzi wspólnej dwóch trójkątów

    case 1:      // wierzchołek leży wewnątrz trójkąta T
        if (T->sons.size() == 0)  // liść - znaleźliśmy jeden trójkąt, który zawiera szukany wierzchołek n
        {
            if (*t1 == 0)
                *t1 = T;
            else
                if (*t1 != T)
                   *t2 = T;
        }
        else
        {
           for (set<TTriangle *>::iterator it = T->sons.begin(); it != T->sons.end(); ++it)
           {
              findTriangles(*it, n, t1, t2);
           }
        }
        break;
    }
}

ostream &operator << (ostream &out, TTriangle &tr)
{
    /*
       ofstream file;
       if (fileName.endsWith(".txt") == false)
           fileName = fileName + ".txt";

       file.open((fileName.toStdString()).c_str(), ios::out);
       file.clear();
       for (set<TNode>::iterator it = nodes.begin(); it != nodes.end(); ++it)
       {
           file << it->getPoint().getX() << " " << it->getPoint().getY() << endl;
       }
       file.close();
       this->fileName = fileName;
     */

    out << "OUTPUT to file: Triangles.txt" << endl;
    list<TTriangle*> q;
    q.push_back(&tr);
    ofstream file;
    file.open("triangles.txt", ios::out);
    while (q.size() > 0)
    {
      TTriangle* t = *(q.begin());
      q.pop_front();

      file << "Triangle " << t << ": " << t->p1->getPoint() << ", " << t->p2->getPoint() << ", " << t->p3->getPoint() << " >> ";

      //file << "sons: " << &t << ": " << endl;
      if (t->sons.size() == 0 )
          file << "leaf" << endl;
      else
      {

          for (set<TTriangle *>::iterator it = t->sons.begin(); it != t->sons.end(); ++it)
          {
              q.push_back(*it);
              file << (*it) << " ";
          }
          file << endl;
      }
    }
    file.close();
    return out;
}

TNode* TTriangle::getNode(int i)
{
    switch (i) {
    case 1:
        return p1;
        break;
    case 2:
        return p2;
        break;
    default:
        return p3;
        break;
    }
}

void legalizeEdge(TNode *pr, TNode *pi, TNode *pj, TTriangle *T, list<TTriangle*> &triangles, TNode *p1, TNode *p2, TNode *p3)
{
   if ( (pi == p1 || pi == p2 || pi == p3) && (pj == p1 || pj == p2 || pj == p3) )   // przypadek (i) - i, j są ujemne, legalna
       return;

   // szukamy trójkąta sąsiadującego (o wspólnej krawędzi pi, pj
   TTriangle* t1 = 0;
   TTriangle* t2 = 0;
   TNode *pk;
   TNode n( TMyPoint((pi->getPoint().getX() + pj->getPoint().getX())/2.0, (pi->getPoint().getY() + pj->getPoint().getY())/2.0) ); // punkt będący środkiem krawędzi pi, pj
   findTriangles(T, &n, &t1, &t2);
   if (t2 != 0 && t2 != 0)
   {
       TNode *nodes_ij[3] = {t1->getNode(1), t1->getNode(2), t1->getNode(3)};
       set<TNode *> setT1(nodes_ij, nodes_ij + 3);
       setT1.insert(t2->getNode(1));
       setT1.insert(t2->getNode(2));
       setT1.insert(t2->getNode(3));
       set<TNode *>::iterator it = setT1.find(pr);
       setT1.erase(it);
       it = setT1.find(pi);
       setT1.erase(it);
       it = setT1.find(pj);
       setT1.erase(it);
       it = setT1.begin();
       pk = *it;
   }
   else
   {
       cerr << "Somethig wrong LegalizeEdge" << endl;
       cerr << "pi: " << *pi << endl;
       cerr << "pj: " << *pj << endl;
       cerr << "pr: " << *pr << endl;
       cerr << "n : " << n << endl;
       exit(-1);
   }
 // przypadek (iV - jeden z indeksów i, j oraz jeden z indeksów k, l jest ujemny)
 // zmieniamy, jeżeli ujemny indeks pary i, j jest mniejszy od ujemnego indeksu pary l, k

   if ( ((pi == p1 || pi == p2 || pi == p3 || pj == p1 || pj == p2 || pj == p3) && ( pr == p1 || pr == p2 || pr == p3 )) ||
        ((pi == p1 || pi == p2 || pi == p3 || pj == p1 || pj == p2 || pj == p3) && ( pk == p1 || pk == p2 || pk == p3)) )
   {
       int index_ij = -3;
       if (pi == p1 || pj == p1)
           index_ij = -1;
       if (pi == p2 || pj == p2)
           index_ij = -2;
       int index_lk = -3;
       if (pr == p1 || pk == p1)
           index_lk = -1;
       if (pr == p2 || pk == p2)
           index_lk = -2;
       if (index_ij < index_lk)
           ;
       else
       {  // nielegalne,

           TMyPoint p( pr->getPoint() );
           TMyPoint q( pj->getPoint() );
           TMyPoint r( pi->getPoint() ) ;
           TMyPoint s( pk->getPoint() );
           if (isInsideOfCircle(p, q, r, s) == true)
           {
               pj->removeEdge(pi);
               pi->removeEdge(pj);
               pr->addEdge(pk);
               pk->addEdge(pr);
               TTriangle *tson = new TTriangle(pr, pi, pk);
               t1->addSon(tson);
               t2->addSon(tson);
               triangles.push_back(tson);
               tson = new TTriangle(pr, pj, pk);
               t1->addSon(tson);
               t2->addSon(tson);
               triangles.push_back(tson);
               legalizeEdge(pr, pi, pk, T, triangles, p1, p2, p3);
               legalizeEdge(pr, pj, pk, T, triangles, p1, p2, p3);
           }
       }
       return;
   }

   if ( ((pi == p1 || pi == p2 || pi == p3) || (pj == p1 || pj == p2 || pj == p3)) &&
        (  !( (pr == p1 || pr == p2 || pr == p3) || (pk == p1 || pk == p2 || pk == p3) )   ) )
   { // przypadek (iii - i lub j jest ujemne, natomiast l, k są dodatnie) - nielegalna, zmieniamy
       TMyPoint p( pr->getPoint() );
       TMyPoint q( pj->getPoint() );
       TMyPoint r( pi->getPoint() ) ;
       TMyPoint s( pk->getPoint() );
       if (isInsideOfCircle(p, q, r, s) == true)
       {
           pj->removeEdge(pi);
           pi->removeEdge(pj);
           pr->addEdge(pk);
           pk->addEdge(pr);
           TTriangle *tson = new TTriangle(pr, pi, pk);
           triangles.push_back(tson);
           t1->addSon(tson);
           t2->addSon(tson);
           tson = new TTriangle(pr, pj, pk);
           triangles.push_back(tson);
           t1->addSon(tson);
           t2->addSon(tson);
           legalizeEdge(pr, pi, pk, T, triangles, p1, p2, p3);
           legalizeEdge(pr, pj, pk, T, triangles, p1, p2, p3);
       }
       return;
   }
   TMyPoint p( pr->getPoint() );
   TMyPoint q( pj->getPoint() );
   TMyPoint r( pi->getPoint() ) ;
   TMyPoint s( pk->getPoint() );
   // przypadek (ii) - wszystkie indeksy są dodatnie (nie ma punktów specjalnych)
   if (isInsideOfCircle(p, q, r, s) == true)
   { // przekręcenie przekątnych
     pj->removeEdge(pi);
     pi->removeEdge(pj);
     pr->addEdge(pk);
     pk->addEdge(pr);
     TTriangle *tson = new TTriangle(pr, pi, pk);
     triangles.push_back(tson);
     t1->addSon(tson);
     t2->addSon(tson);
     tson = new TTriangle(pr, pj, pk);
     triangles.push_back(tson);
     t1->addSon(tson);
     t2->addSon(tson);
     legalizeEdge(pr, pi, pk, T, triangles, p1, p2, p3);
     legalizeEdge(pr, pj, pk, T, triangles, p1, p2, p3);
   }
}

/**
 * @brief delaunayTriangulation - realizuje triangulacje Delaunaya
 * @param triangles - pamięta wskaźniki do wszystkich trójkątów utworzonych w funkcji - trzeba zwolnić im pamięć !!!!! Należy zastąpić to rozwiązanie efektywniejszym !!!!!
 * @param P - lista wierzchołków, ktre należy strianguluwać
 */

void delaunayTriangulation(list<TTriangle*> &triangles, list<TNode*> &P)
{
    removedoubleNodes(P);
    if (P.size() <= 0)
        return;
    TMyPoint p_1, p_2, p_3;
    determine_M(P, p_1, p_2, p_3);
    TNode* p1 = new TNode(p_1);
    TNode* p2 = new TNode(p_2);
    TNode* p3 = new TNode(p_3);
    P.push_front(p1);
    P.push_front(p2);
    P.push_front(p3);
    p1->addEdge(p2);
    p1->addEdge(p3);
    p2->addEdge(p1);
    p2->addEdge(p3);
    p3->addEdge(p1);
    p3->addEdge(p2);
    TTriangle* T = new TTriangle(p1, p2, p3);
    triangles.push_back(T);
    TNode *pr;
    list<TNode*>::iterator it = P.begin();
    if (it != P.end())
       ++++++it; // pomijamy 3 pierwsze wierzchołki p1, p2, p3
    unsigned int count = 0;
    for (; it != P.end(); ++it)
    {
        ++count;
        TTriangle* t1 = 0;
        TTriangle* t2 = 0;
        pr = *it;
        findTriangles(T, pr, &t1, &t2);
        if (t2 != 0)
        {  // czterej synowie
            TNode *pi = 0, *pj = 0, *pk = 0, *pl = 0;

            set<TNode*> T1;
            T1.insert(t1->getNode(1));
            T1.insert(t1->getNode(2));
            T1.insert(t1->getNode(3));

            set<TNode*> T2;
            T2.insert(t2->getNode(1));
            T2.insert(t2->getNode(2));
            T2.insert(t2->getNode(3));

            set<TNode*> SI;

            TNode *n1 = t1->getNode(1);
            TNode *n2 = t1->getNode(2);
            TNode *n3 = t1->getNode(3);
            TNode *n11 = t2->getNode(1);
            TNode *n22 = t2->getNode(2);
            TNode *n33 = t2->getNode(3);
            if (n1 == n11 || n1 == n22 || n1 == n33)
                pi = n1;
            else
                pl = n1;
            if (n2 == n11 || n2 == n22 || n2 == n33)
            {
                if (pi == 0)
                    pi = n2;
                else
                    pj = n2;
            }
            else
                pl = n2;
            if (n3 == n11 || n3 == n22 || n3 == n33)
            {
                if (pi == 0)
                    pi = n3;
                else
                    pj = n3;
            }
            else
                pl = n3;
            if (n1 == n11 || n2 == n11 || n3 == n11)
                ;
            else
                pk = n11;
            if (n1 == n22 || n2 == n22 || n3 == n22)
                ;
            else
                pk = n22;
            if (n1 == n33 || n2 == n33 || n3 == n33)
                ;
            else
                pk = n33;
            if (pl == 0 || pk == 0)
            {
                cerr << "assert(4 sons), count: " << count << endl;
                cout << *T << endl;
                exit(-1);
            }
            TTriangle *Tson = new TTriangle(pi, pl, pr);
            pi->addEdge(pr);
            pr->addEdge(pi);
            pl->addEdge(pr);
            pr->addEdge(pl);
            t1->addSon(Tson);
            triangles.push_back(Tson);

            Tson = new TTriangle(pl, pj, pr);
            pj->addEdge(pr);
            pr->addEdge(pj);
            t1->addSon(Tson);
            triangles.push_back(Tson);

            Tson = new TTriangle(pi, pk, pr);
            pk->addEdge(pr);
            pr->addEdge(pk);
            t2->addSon(Tson);
            triangles.push_back(Tson);

            Tson = new TTriangle(pj, pk, pr);
            t2->addSon(Tson);
            triangles.push_back(Tson);

            legalizeEdge(pr, pi, pl, T, triangles, p1, p2, p3);
            legalizeEdge(pr, pl, pj, T, triangles, p1, p2, p3);
            legalizeEdge(pr, pi, pk, T, triangles, p1, p2, p3);
            legalizeEdge(pr, pj, pk, T, triangles, p1, p2, p3);
        }
        else
        {
            if(t1 != 0)
            {   // trzej synowie
                TNode *pi = t1->getNode(1);
                TNode *pj = t1->getNode(2);
                TNode *pk = t1->getNode(3);
                TTriangle *Tson = new TTriangle(pi, pj, pr);
                pi->addEdge(pr);
                pr->addEdge(pi);
                pj->addEdge(pr);
                pr->addEdge(pj);
                t1->addSon(Tson);
                triangles.push_back(Tson);

                Tson = new TTriangle(pj, pk, pr);
                pk->addEdge(pr);
                pr->addEdge(pk);
                t1->addSon(Tson);
                triangles.push_back(Tson);

                Tson = new TTriangle(pi, pk, pr);
                t1->addSon(Tson);
                triangles.push_back(Tson);

                legalizeEdge(pr, pi, pj, T, triangles, p1, p2, p3);
                legalizeEdge(pr, pj, pk, T, triangles, p1, p2, p3);
                legalizeEdge(pr, pi, pk, T, triangles, p1, p2, p3);
            }
            else
              cerr << "NO triangle" << endl;
        }
    }
    // usuwamy wierzchołki p1, p2, p3 z listy P wraz krawędziami incydentnymi, itp.
    int n = 3;
    for (list<TNode*>::iterator it = P.begin(); it != P.end() && n > 0; ++it, --n)
    {
        TNode *p1 = *it;
        set<TNode*> edges = p1->getEdges();
        for (set<TNode*>::iterator itEdge =  edges.begin(); itEdge != edges.end(); ++itEdge)
        {
            (*itEdge)->removeEdge(p1);
        }
    }
    // usunięcie P1, P2, P3 z listy
    P.erase(P.begin());
    P.erase(P.begin());
    P.erase(P.begin());
}

/**
 * @brief removelong doubleNodes usuwa z listy powtarzające się wierzchołki, zostawia jedno wystąpienie wierzchołka
 * @param P - lista wierzchołków do weryfikacji
 */
void removedoubleNodes(list<TNode *> &P)
{
    set<TMyPoint> nodes;
    unsigned int nodeSize;
    list<TNode *>::iterator it = P.begin();
    while ( it != P.end() )
    {
            nodeSize = nodes.size();
            TMyPoint p = (*it)->getPoint();
            nodes.insert(p);
            if ( nodeSize == nodes.size() )
            {
                delete *it;
                it = P.erase(it);

            }
            else
                ++it;
    }
}

/**
 * @brief makeGabrielGraph removes wrong edges from Delaunay Triangulation to make Gabriel graph
 * @param P - list of nodes (it is a Delaunays graph)
 */
void convertDelaunay2GabrielGraph(list<TNode*> &P)
{
    TMyPoint o(0, 0);
    long double rG;
    set<TNode*> edges;
    for (list<TNode*>::iterator it = P.begin(); it != P.end(); ++it)
    {
       edges = (*it)->getEdges();

       for (set<TNode*>::iterator itEdge = edges.begin(); itEdge != edges.end(); ++itEdge)  // for each edge where *(*it) is one of its ends
       {
           set<TNode*> toCheck = (*itEdge)->getEdges();      // The nodes to check
           toCheck.insert(edges.begin(), edges.end());
           caclulateGabrielCircle( *(*itEdge), *(*it), o, rG);
           for (set<TNode*>::iterator it2Check = toCheck.begin(); it2Check != toCheck.end(); ++it2Check)
           {
              if (Gdistance( *(*it2Check), o) < rG)
              {  // remove the edge (it, it2Check)
                  (*it)->removeEdge(*itEdge);
                  (*itEdge)->removeEdge(*it);
                  break;
              }
           }

       }
    }
}

list<TNode *> generate_Nodes(unsigned int w, unsigned int h, unsigned int number)
{
    set<TNode *> P;
    set<TMyPoint> testedPoints;
    unsigned int size;
    while (testedPoints.size() < number)
    {
      int x = random() %  w;
      int y = random() % h;
      TMyPoint p(x, y);
      testedPoints.insert(p);
    }
    list<TNode *> nodeList;
    for (set<TMyPoint>::iterator it = testedPoints.begin(); it != testedPoints.end(); ++it)
    {
      nodeList.push_back(new TNode(*it));
    }
    return nodeList;
}

/**
 * @brief edgeNumber counts edges
 * @param P - a list of nodes
 * @return
 */
unsigned int edgeNumber(list<TNode*> &P)
{
    unsigned int count = 0;
    for (list<TNode*>::iterator it = P.begin(); it != P.end(); ++it)
        count += (*it)->getEdgesNumber();
    assert(count % 2 == 0);
    return count / 2;
}
