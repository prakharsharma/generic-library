#ifndef _GRAPH_HPP_
#define _GRAPH_HPP_
#include <map>
#include <vector>
#include <queue>
#include <list>
#include <iostream>
#include <limits>
#include <cmath>

enum Color {
    WHITE = 0,
    GRAY,
    BLACK
};

#ifndef _STRUCT_NIL_
#define _STRUCT_NIL_
struct nil {};
#endif

template<typename T=nil>
class Vertex {
    private:
        bool _active;
    public:
        Color clr;
        /*foll 2 fields are for Tarjan's SCC algorithm*/
        unsigned index;
        unsigned lowlink;
        /*application specific data*/
        unsigned id;
        T *meta;
    private:
        Vertex(const Vertex&){};
        const Vertex& operator=(const Vertex&) {};
    public:
        Vertex(unsigned vrtxCnt):meta(NULL), _active(true) {
            id = vrtxCnt;
        }
        Vertex(unsigned vrtxCnt, T *data):meta(data),_active(true) {
            id = vrtxCnt;
        }
        Vertex(unsigned vrtxCnt, T data):_active(true) {
            id = vrtxCnt;
            meta = new T(data);
        }
        ~Vertex() {
            delete meta;
        }
        bool active() { return _active; }
        void active(bool f) { _active = f; }
        T Data() { return *meta; }
        unsigned Id() const { return id; }
};

template<typename T=nil>
class Edge {
    private:
    public:
        unsigned frm;
        unsigned to;
        bool active;
        unsigned id;
        /*application specific data*/
        T *meta;
    private:
        const Edge& operator=(const Edge&){};
    public:
        Edge(unsigned edgCnt):meta(NULL), active(true) {
            id = edgCnt;
        }
        Edge(unsigned edgCnt, unsigned f, unsigned t):frm(f), to(t),
            meta(NULL), active(true) {
            id = edgCnt;
        }
        Edge(unsigned edgCnt, unsigned f, unsigned t, T* data) : frm(f),
            to(t), meta(data), active(true) {
            id = edgCnt;
        }
        Edge(unsigned edgCnt, unsigned f, unsigned t, T data) :frm(f),
            to(t), active(true) {
            id = edgCnt;
            meta = new T(data);
        }
        Edge(unsigned edgCnt, T *data) : meta(data), active(true) {
            id = edgCnt;
        }
        Edge(unsigned edgCnt, T data) : active(true) {
            id = edgCnt;
            meta = new T(data);
        }
        Edge(unsigned edgCnt, const Edge &e) : active(true) {
            id = edgCnt;
            frm = e.From();
            to = e.To();
            meta = new T(e.Data());
        }
        ~Edge() { delete meta; }
        bool isActive() { return active; }
        unsigned From() const { return frm; }
        void From(unsigned _frm) { frm = _frm; }
        unsigned To() const { return to; }
        void To(unsigned to_) { to = to_; }
        bool Active() { return active; }
        void Active(bool f) { active = f; }
        T Data() { return *meta; }
        friend std::ostream operator<<(std::ostream &stream, Edge<T> &obj)
        {
            stream<<"from: "<<obj.frm<<" to: "<<obj.to;
            /*if (obj.meta)
              stream<<" meta: "<<*obj.meta;*/
        }
        unsigned Id() {
            return id;
        }
};

template<typename T1, typename T2>
class AdjMtrx {
    private:
        std::map<unsigned, std::map<unsigned,
            std::list<Edge<T2>*>* >* > *adjMtrx;
    public:
        AdjMtrx() { adjMtrx = new std::map<unsigned, std::map<unsigned,
            std::list<Edge<T2>*>*>*>(); }
        ~AdjMtrx();
        bool isEdge(Vertex<T1> &u, Vertex<T1> &v);
        void addEdge(Edge<T2> &e);
        std::list<Edge<T2>*>* outGoingEdgesList(unsigned vId);
        std::vector<Edge<T2>*>* outGoingEdges(unsigned vId);
        void outGoingEdges(unsigned vId, std::vector<Edge<T2>*> &result);
        void outGoingEdges(unsigned vId, std::list<Edge<T2>*> &result)const;
        void incomingEdges(unsigned vId, std::vector<Edge<T2>*> &result);
        bool isEdge(unsigned, unsigned);
        std::list<Edge<T2>*>* edge(unsigned, unsigned);
        std::vector<Edge<T2>*>* edges();
        void edges(std::vector<Edge<T2>*>&);
        void removeEdge(Edge<T2>*, std::ostream&);
        void removeEdge(unsigned u, unsigned v);
};

template<typename T1, typename T2> struct shrstPth {
    T2 wt;
    T2 infinity;
    std::vector<Edge<T1>*> *path;
    shrstPth() {
        if (std::numeric_limits<T2>::has_infinity)
            wt = std::numeric_limits<T2>::infinity();
        else
            wt = (T2)pow((double)2, (int)(sizeof(T2)*8-1)) - (T2)1;
        path = NULL;
    }
    shrstPth(T2 w, Edge<T1>* e) {
        wt = w;
        path = new std::vector<Edge<T1>*>();
        path->push_back(e);
    }
    ~shrstPth() { path->clear(); delete path; }
    void update(shrstPth<T1,T2> *p1, shrstPth<T1,T2> *p2) {
        wt = p1->wt + p2->wt;
        if (!path)
            path = new std::vector<Edge<T1>*>();
        path->clear();
        for (int i = 0; i < p1->path->size(); i++)
            path->push_back(p1->path->at(i));
        for (int i = 0; i < p2->path->size(); i++)
            path->push_back(p2->path->at(i));
    }
};

template<typename T1=nil, typename T2=nil, typename T4 = long,
    typename T3=AdjMtrx<T1,T2> >
class Graph {
    private:
        unsigned vrtxCnt;
        unsigned edgCnt;
        T3 *cnnctvy;
        Graph(const Graph&){};
        const Graph& operator=(const Graph&){};
    public:
        std::map<unsigned, Vertex<T1>*> vMap;

        Graph() {
            vrtxCnt = 0;
            edgCnt = 0;
            cnnctvy=new T3();
        }
        ~Graph();
        void addNode(Vertex<T1> &n) {
            if (vMap.find(n.id) == vMap.end())
                vMap.insert(std::pair<unsigned, Vertex<T1>*>(n.id, &n));
        }
        Vertex<T1>* createNode(T1 *data = NULL) {
            Vertex<T1> *v = new Vertex<T1>(vrtxCnt++, data);
            addNode(*v);
            return v;
        }
        Vertex<T1>* createNode(T1 data = NULL) {
            Vertex<T1> *v = new Vertex<T1>(vrtxCnt++, data);
            addNode(*v);
            return v;
        }
        Vertex<T1>* getNode(unsigned vId) const {
            return vMap.find(vId) == vMap.end() ? NULL :
                vMap.find(vId)->second;
        }
        void addEdge(Vertex<T1> &u, Vertex<T1> &v, Edge<T2> &e) {
            addNode(u); addNode(v);
            e.frm = u.id;
            e.to = v.id;
            cnnctvy->addEdge(e);
        }
        void addEdge(Edge<T2> &e) {
            cnnctvy->addEdge(e);
        }
        Edge<T2>* createEdge(unsigned f, unsigned t, T2 *meta = NULL) {
            Edge<T2> *e = new Edge<T2>(edgCnt++, f, t, meta);
            addEdge(*e);
            return e;
        }
        Edge<T2>* createEdge(const Vertex<T1> &u, const Vertex<T1> &v,
                T2 *meta = NULL) {
            Edge<T2> *e = new Edge<T2>(edgCnt++, u.Id(), v.Id(), meta);
            addEdge(*e);
            return e;
        }
        std::list<Edge<T2>*>* outGoingEdgesList(unsigned vId) {
            return (vMap.find(vId) != vMap.end()) ?
                cnnctvy->outGoingEdgesList(vId) : NULL;
        }
        std::vector<Edge<T2>*>* outGoingEdges(unsigned vId) {
            return (vMap.find(vId) != vMap.end()) ?
                cnnctvy->outGoingEdges(vId) : NULL;
        }
        void outGoingEdges(unsigned vId, std::vector<Edge<T2>*> &result) {
            if (vMap.find(vId) != vMap.end())
                cnnctvy->outGoingEdges(vId, result);
            return;
        }
        void outGoingEdges(unsigned vId, std::list<Edge<T2>*> &result)
            const {
                if (vMap.find(vId) != vMap.end())
                    cnnctvy->outGoingEdges(vId, result);
                return;
            }
        void incomingEdges(unsigned vId, std::vector<Edge<T2>*> &result) {
            if (vMap.find(vId) != vMap.end())
                cnnctvy->incomingEdges(vId, result);
            return;
        }
        bool isEdge(unsigned u, unsigned v) {
            return (vMap.find(u) != vMap.end() && vMap.find(v) != vMap.end() &&
                    cnnctvy->isEdge(u,v));
        }
        std::list<Edge<T2>*>* edge(unsigned u, unsigned v) {
            if (vMap.find(u) != vMap.end() && vMap.find(v) != vMap.end())
                return cnnctvy->edge(u, v);
            return NULL;
        }
        void printGraph(std::ostream &out);
        shrstPth<T2,T4>*** allPairsShrtstPth();
        std::vector<Edge<T2>*>* edges() { return cnnctvy->edges(); }
        void edges(std::vector<Edge<T2>*> &res) { cnnctvy->edges(res); }
        void removeNode(Vertex<T1> *u) {
            if (u == NULL)
                return;
            if (vMap.find(u->id) != vMap.end())
                vMap.find(u->id)->second = NULL;
            delete u;
        }
        void removeEdge(Edge<T2> *e, std::ostream &out = std::cout) {
            cnnctvy->removeEdge(e, out); /*delete e;*/
        }
        void removeEdge(unsigned u, unsigned v) {
            cnnctvy->removeEdge(u, v); /*delete e;*/
        }
        unsigned numVertices() {
            return vrtxCnt;
        }
        unsigned numEdges() {
            return edgCnt;
        }
        bool isScc();
        bool tarjan(Vertex<T1>&, unsigned&, std::map<unsigned, bool>&);
        void dijkstra(unsigned src, unsigned dest, std::list<unsigned>&);
        void dfs(std::list<Vertex<T1>*>&);
        bool visitedAll();
        unsigned activeVertices();
};

    template<typename T1, typename T2, typename T4, typename T3 >
Graph<T1,T2,T4,T3>::~Graph()
{
    typename std::map<unsigned, Vertex<T1>*>::iterator i = vMap.begin();
    for (; i != vMap.end(); i++)
        delete i->second;
}

    template<typename T1, typename T2, typename T4, typename T3 >
void Graph<T1,T2,T4,T3>::printGraph(std::ostream &out)
{
    typename std::map<unsigned, Vertex<T1>*>::iterator i = vMap.begin();
    for (; i != vMap.end(); i++) {
        out<<*(i->second->meta);
        std::vector<Edge<T2>*> *outEdges = outGoingEdges(i->first);
        for (int j = 0; outEdges != NULL && j < outEdges->size(); j++) {
            Edge<T2> &e = *(outEdges->at(j));
            out<<" "<<*(vMap[e.to]->meta);
        }
        out<<std::endl;
        if (outEdges) {
            delete outEdges;
            outEdges = NULL;
        }
    }
    return;
}

template<typename T1>
struct path_dijkstra {
    unsigned vId;
    unsigned prev;
    T1 wt;
    path_dijkstra(unsigned id, T1 w) { vId = id; wt = w; prev = 0; }
};

template<typename T1> struct comp_path_dijkstra {
    bool operator()(path_dijkstra<T1> *p1, path_dijkstra<T1> *p2) {
        return p1->wt > p2->wt; /*we need a min-heap*/
    }
};

    template<typename T1, typename T2, typename T4, typename T3 >
void Graph<T1,T2,T4,T3>::dijkstra(unsigned src, unsigned dest,
        std::list<unsigned> &path)
{
    T4 infinity;
    if (std::numeric_limits<T4>::has_infinity)
        infinity = std::numeric_limits<T4>::infinity();
    else
        infinity = (T4)pow((double)2, (int)(sizeof(T4)*8-1));
    std::map<unsigned, path_dijkstra<T4>*> shortPaths;
    std::priority_queue<path_dijkstra<T4>*, std::vector<path_dijkstra<T4>*>,
        comp_path_dijkstra<T4> > queue;
    path_dijkstra<T4> *tmp = NULL;
    typename std::map<unsigned, Vertex<T1>*>::iterator i = vMap.begin();
    for (; i != vMap.end(); i++) {
        if (i->first == src)
            tmp = new path_dijkstra<T4>(i->first, 0);
        else
            tmp = new path_dijkstra<T4>(i->first, infinity);
        shortPaths.insert(std::pair<unsigned, path_dijkstra<T4>*>(i->first,
                    tmp));
        queue.push(tmp);
    }

    while (!queue.empty()) {
        path_dijkstra<T4> *top = queue.top();
        if (top->wt == infinity)
            break;
        queue.pop();
        if(top->vId == dest)
            break;
        std::vector<Edge<T2>*> *outEdges = outGoingEdges(top->vId);
        if (!outEdges)
            continue;
        for (unsigned i = 0; i < outEdges->size(); i++) {
            T4 alt = top->wt + outEdges->at(i)->meta->cost;
            T4 &dist_v = shortPaths.find(outEdges->at(i)->to)->second->wt;
            if (alt < dist_v) {
                dist_v = alt;
                shortPaths.find(outEdges->at(i)->to)->second->prev = top->vId;
            }
        }
        delete outEdges; outEdges = NULL;
    }
    path_dijkstra<T4> *u = shortPaths.find(dest)->second;
    while (u->prev) {
        path.push_front(u->vId);
        u = shortPaths.find(u->prev)->second;
    }

    return;
}

    template<typename T1, typename T2>
void AdjMtrx<T1, T2>::addEdge(Edge<T2> &e)
{
    typename std::map<unsigned, std::map<unsigned,
             std::list<Edge<T2>*>* >* >::iterator i = adjMtrx->find(e.frm);
    if (i == adjMtrx->end()) {
        std::map<unsigned, std::list<Edge<T2>*>*> *tmp =
            new std::map<unsigned, std::list<Edge<T2>*>*>();
        adjMtrx->insert(std::pair<unsigned, std::map<unsigned,
                std::list<Edge<T2>*>*>*>(e.frm, tmp));
    }
    typename std::map<unsigned, std::list<Edge<T2>*>*>::iterator j =
        adjMtrx->find(e.frm)->second->find(e.to);
    if (j == adjMtrx->find(e.frm)->second->end())
        adjMtrx->find(e.frm)->second->insert(std::pair<unsigned,
                std::list<Edge<T2>*>*>(e.to, new std::list<Edge<T2>*>()));
    adjMtrx->find(e.frm)->second->find(e.to)->second->push_back(&e);

    return;
}

    template<typename T1, typename T2>
std::vector<Edge<T2>* >* AdjMtrx<T1,T2>::outGoingEdges(unsigned vId)
{
    if (adjMtrx->find(vId) == adjMtrx->end())
        return NULL;
    std::vector<Edge<T2>*> *retVal = new std::vector<Edge<T2>*>();
#ifdef DEBUG
    //std::vector<Edge<T2>*> *tmp = edges();
#endif
    typename std::map<unsigned, std::list<Edge<T2>*>*>::iterator i =
        adjMtrx->find(vId)->second->begin();
    for (; i != adjMtrx->find(vId)->second->end(); i++) {
#ifdef DEBUG
        //Edge<T2> *eTmp = i->second;
#endif
        if (i->second && i->second->size()) {
            for (typename std::list<Edge<T2>*>::iterator k =
                    i->second->begin(); k != i->second->end(); k++)
                if (*k && (*k)->Active())
                    retVal->push_back(*k);
        }
    }
    if (!retVal->size()) {
        delete retVal;
        retVal = NULL;
    }
    return retVal;
}

    template<typename T1, typename T2>
std::list<Edge<T2>* >* AdjMtrx<T1,T2>::outGoingEdgesList(unsigned vId)
{
    if (adjMtrx->find(vId) == adjMtrx->end())
        return NULL;
    std::list<Edge<T2>*> *retVal = new std::list<Edge<T2>*>();
#ifdef DEBUG
    //std::vector<Edge<T2>*> *tmp = edges();
#endif
    typename std::map<unsigned, std::list<Edge<T2>*>*>::iterator i =
        adjMtrx->find(vId)->second->begin();
    for (; i != adjMtrx->find(vId)->second->end(); i++) {
#ifdef DEBUG
        //Edge<T2> *eTmp = i->second;
#endif
        if (i->second && i->second->size()) {
            for (typename std::list<Edge<T2>*>::iterator k =
                    i->second->begin(); k != i->second->end(); k++)
                if (*k && (*k)->Active())
                    retVal->push_back(*k);
        }
    }
    if (!retVal->size()) {
        delete retVal;
        retVal = NULL;
    }
    return retVal;
}

    template<typename T1, typename T2>
void AdjMtrx<T1,T2>::outGoingEdges(unsigned vId,
        std::vector<Edge<T2>*> &result)
{
    if (adjMtrx->find(vId) == adjMtrx->end())
        return;
#ifdef DEBUG
    //std::vector<Edge<T2>*> *tmp = edges();
#endif
    typename std::map<unsigned, std::list<Edge<T2>*>*>::iterator i =
        adjMtrx->find(vId)->second->begin();
    for (; i != adjMtrx->find(vId)->second->end(); i++) {
#ifdef DEBUG
        //Edge<T2> *eTmp = i->second;
#endif
        if (i->second && i->second->size())
            for (typename std::list<Edge<T2>*>::iterator k =
                    i->second->begin(); k != i->second->end(); k++)
                if (*k && (*k)->Active())
                    result.push_back(*k);
    }
    return;
}

template<typename T1, typename T2>
void AdjMtrx<T1,T2>::outGoingEdges(unsigned vId,
        std::list<Edge<T2>*> &result) const
{
    if (adjMtrx->find(vId) == adjMtrx->end())
        return;
#ifdef DEBUG
    //std::vector<Edge<T2>*> *tmp = edges();
#endif
    typename std::map<unsigned, std::list<Edge<T2>*>*>::iterator i =
        adjMtrx->find(vId)->second->begin();
    for (; i != adjMtrx->find(vId)->second->end(); i++) {
#ifdef DEBUG
        //Edge<T2> *eTmp = i->second;
#endif
        if (i->second && i->second->size())
            for (typename std::list<Edge<T2>*>::iterator k =
                    i->second->begin(); k != i->second->end(); k++)
                if (*k && (*k)->Active())
                    result.push_back(*k);
    }
    return;
}

template<typename T1, typename T2>
void AdjMtrx<T1,T2>::incomingEdges(unsigned vId, std::vector<Edge<T2>*>
        &retVal)
{
    std::vector<Edge<T2>*> allEdges;
    edges(allEdges);
    for (unsigned i = 0; i < allEdges.size(); i++) {
        if (allEdges[i]->to == vId)
            retVal.push_back(allEdges[i]);
    }
    return;
}

template<typename T1, typename T2> bool AdjMtrx<T1,T2>::isEdge(unsigned u,
        unsigned v) {
    return (adjMtrx->find(u) != adjMtrx->end() &&
            adjMtrx->find(u)->second->find(v) != adjMtrx->find(u)->second->end() &&
            adjMtrx->find(u)->second->find(v)->second->size());//isActive());
}

    template<typename T1, typename T2>
std::list<Edge<T2>*>* AdjMtrx<T1,T2>::edge(unsigned u, unsigned v)
{
    return (adjMtrx->find(u) != adjMtrx->end() &&
            adjMtrx->find(u)->second->find(v) != adjMtrx->find(u)->second->end() &&
            adjMtrx->find(u)->second->find(v)->second->size()) ?
        adjMtrx->find(u)->second->find(v)->second : NULL;
}

    template<typename T1, typename T2>
std::vector<Edge<T2>*>* AdjMtrx<T1,T2>::edges()
{
    std::vector<Edge<T2>*> *retVal = new std::vector<Edge<T2>*>();
    typename std::map<unsigned,
             std::map<unsigned, std::list<Edge<T2>*>*>*>::iterator i =
                 adjMtrx->begin();
    for (; i != adjMtrx->end(); i++) {
        typename std::map<unsigned, std::list<Edge<T2>*>*>::iterator j =
            i->second->begin();
        for (; j != i->second->end(); j++) {
            if (j->second) {
                for (typename std::list<Edge<T2>*>::iterator k =
                        j->second->begin(); k != j->second->end(); k++)
                    if (*k && (*k)->Active())
                        retVal->push_back(*k);
            }
        }
    }
    return retVal;
}

template<typename T1, typename T2>
void AdjMtrx<T1,T2>::edges(std::vector<Edge<T2>*> &retVal) {
    typename std::map<unsigned, std::map<unsigned,
             std::list<Edge<T2>*>*>*>::iterator i = adjMtrx->begin();
    for (; i != adjMtrx->end(); i++) {
        typename std::map<unsigned, std::list<Edge<T2>*>*>::iterator j =
            i->second->begin();
        for (; j != i->second->end(); j++) {
            if (j->second) {
                for (typename std::list<Edge<T2>*>::iterator k =
                        j->second->begin(); k != j->second->end(); k++)
                    if (*k && (*k)->Active())
                        retVal.push_back(*k);
            }
        }
    }
    return;
}

    template<typename T1, typename T2>
void AdjMtrx<T1,T2>::removeEdge(Edge<T2> *e, std::ostream &out)
{
    if (adjMtrx->find(e->frm) == adjMtrx->end() ||
            adjMtrx->find(e->frm)->second->find(e->to) ==
            adjMtrx->find(e->frm)->second->end())
        return;
#ifdef DEBUG
    out<<"Removing edge from "<<e->frm<<" to "<<e->to<<std::endl;
#endif
    /*adjMtrx->find(e->frm)->second->erase(
      adjMtrx->find(e->frm)->second->find(e->to));*/
    typename std::list<Edge<T2>*>::iterator i =
        adjMtrx->find(e->frm)->second->find(e->to)->second->begin();
    for (;i != adjMtrx->find(e->frm)->second->find(e->to)->second->end();) {
        if (*i == e) {
            i = adjMtrx->find(e->frm)->second->find(e->to)->second->
                erase(i);
            break;
        } else
            i++;
    }
    if (adjMtrx->find(e->frm)->second->find(e->to)->second->size() == 0) {
        delete adjMtrx->find(e->frm)->second->find(e->to)->second;
        adjMtrx->find(e->frm)->second->erase(e->to);
    }
    return;
}

    template<typename T1, typename T2>
void AdjMtrx<T1,T2>::removeEdge(unsigned u, unsigned v)
{
    if (adjMtrx->find(u) == adjMtrx->end())
        return;
    if (adjMtrx->find(u)->second->find(v) ==
            adjMtrx->find(u)->second->end())
        return;
    delete adjMtrx->find(u)->second->find(v)->second;
    adjMtrx->find(u)->second->erase(v);
    return;
}

    template<typename T1, typename T2>
AdjMtrx<T1,T2>::~AdjMtrx()
{
    typename std::map<unsigned, std::map<unsigned,
             std::list<Edge<T2>*>* >* >::iterator i = adjMtrx->begin();
    for (; i != adjMtrx->end(); i++) {
        typename std::map<unsigned, std::list<Edge<T2>*>*>::iterator j =
            i->second->begin();
        for (typename std::list<Edge<T2>*>::iterator k =
                j->second->begin(); k != j->second->end(); k++)
            delete *k;
        i->second->clear();
    }
    adjMtrx->clear();
}

    template<typename T1, typename T2, typename T4, typename T3 >
shrstPth<T2,T4>*** Graph<T1,T2,T4,T3>::allPairsShrtstPth()
{
    unsigned sz = vMap.size();
    shrstPth<T2,T4> ***arr2D = new shrstPth<T2, T4>**[sz];
    for (unsigned i = 0; i < sz; i++)
        arr2D[i] = new shrstPth<T2, T4>*[sz];
    for (unsigned i = 0; i < sz; i++)
        for (unsigned j = 0; j < sz; j++)
            arr2D[i][j] = NULL;
    /*Step1: intialize*/
    for (unsigned i = 0; i < sz; i++) {
        for (unsigned j = 0; j < sz; j++) {
            Edge<T2> *e;// = edge(i+1,j+1);
            if (!e)
                arr2D[i][j] = new shrstPth<T2,T4>();
            else
                arr2D[i][j] = new shrstPth<T2,T4>(e->meta->weight(), e);
        }
    }
    /*Step2: 3 loops of Floyd-Warshall's algo*/
    for (int k = 0; k < sz; k++) {
        for (int i = 0; i < sz; i++) {
            for (int j = 0; j < sz; j++) {
                if (i==j && j==k)
                    continue;
                if (!arr2D[i][k]->path || !arr2D[k][j]->path)
                    continue;
                T4 tmp = arr2D[i][k]->wt + arr2D[k][j]->wt;
                if (tmp < arr2D[i][j]->wt)
                    arr2D[i][j]->update(arr2D[i][k], arr2D[k][j]);
            }
        }
    }
    return arr2D;
}

template<typename T1, typename T2, typename T4, typename T3 > bool
Graph<T1,T2,T4,T3>::isScc() {
    /*implementation of Robert Tarjan's SCC algo*/
    typename std::map<unsigned, Vertex<T1>*>::iterator j = vMap.begin();
    for (; j != vMap.end(); j++) {
        j->second->clr = WHITE;
        j->second->index = 0;
        j->second->lowlink = 0;
    }
    typename std::map<unsigned, Vertex<T1>*>::iterator i = vMap.begin();
    unsigned idx = 0;
    for (; i != vMap.end(); i++) {
        std::map<unsigned, bool> predecessor;
        if (i->second->clr == WHITE)
            if (!tarjan(*i->second, idx, predecessor))
                return false;
    }
    return true;
}

template<typename T> T _min_(T a, T b) {
    return (a <= b) ? a : b;
}

    template<typename T1, typename T2, typename T4, typename T3 > bool
Graph<T1,T2,T4,T3>::tarjan(Vertex<T1> &u, unsigned &index, std::map<unsigned,
        bool>& predecessor)
{
    u.index = index;
    u.lowlink = index;
    ++index;
    u.clr = GRAY;
    predecessor.insert(std::pair<unsigned, bool>(u.id, true));
    std::vector<Edge<T2>*> *outEdges = outGoingEdges(u.id);
    if (!outEdges && vMap.size() > 1)
        return false;
    for (unsigned i = 0; i < outEdges->size(); i++) {
        Vertex<T1> &v = *(vMap.find(outEdges->at(i)->to)->second);
        if (v.clr == WHITE) {
            if (!tarjan(v, index, predecessor))
                return false;
            u.lowlink = _min_<unsigned>(u.lowlink, v.lowlink);
        } else if (predecessor.find(v.id) != predecessor.end())
            u.lowlink = _min_<unsigned>(u.lowlink, v.index);
    }
    delete outEdges; outEdges = NULL;
    return u.index ? u.lowlink < u.index : (u.lowlink == u.index);
}

    template<typename T1, typename T2, typename T3, typename T4 >
void Graph<T1,T2,T3,T4>::dfs(std::list<Vertex<T1>*> &visited)
{
    return;
}

    template<typename T1, typename T2, typename T3, typename T4 >
bool Graph<T1,T2,T3,T4>::visitedAll()
{
    typename std::map<unsigned, Vertex<T1>*>::iterator i =
        vMap.begin();
    for (; i != vMap.end(); i++)
        if (i->second->clr == WHITE)
            return false;
    return true;
}


template<typename T1, typename T2, typename T4, typename T3 >
unsigned Graph<T1,T2,T4,T3>::activeVertices() {
    unsigned num = 0;
    for (typename std::map<unsigned, Vertex<T1>*>::iterator i =
            vMap.begin(); i != vMap.end(); i++) {
        if (i->second->active())
            ++num;
    }
    return num;
}

#endif

