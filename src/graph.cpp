#include "graph.h"
#include <cstdlib>

void error(string msg){ cerr << "Error: " << msg << endl; }
string get_arg(int argc, char *argv[], string def){ return(argc>1)?string(argv[1]):def; }
void file_to_graph(string filename, Graph &G){ ifstream infile(filename); infile>>G; }

// DJS
DisjointSet::DisjointSet(int numVertices) {
    subsets.resize(numVertices);
    for(Vertex i=0; i<numVertices; ++i){ makeSet(i); }
}
void DisjointSet::makeSet(Vertex x){ subsets[x].parent = x; }
Vertex DisjointSet::findSet(Vertex x) {
    if(subsets[x].parent!=x) subsets[x].parent=findSet(subsets[x].parent);
    return subsets[x].parent;
}
void DisjointSet::unionSets(Vertex x, Vertex y){ if(findSet(x)!=findSet(y)) subsets[findSet(y)].parent=findSet(x); }

// Graph
Graph Graph::sort_edges()const{
    Graph s = *this;
    sort(s.begin(), s.end(), [](const Edge &a, const Edge &b){ return a.weight<b.weight; });
    return s;
}
// Kruskals
EdgeList Kruskals(const Graph &G){
    EdgeList mst;
    Graph s=G.sort_edges();
    DisjointSet ds(G.numVertices);
    for (const Edge &e:s) {
        if (ds.findSet(e.u) != ds.findSet(e.v)){
            mst.push_back(e);
            ds.unionSets(e.u, e.v);
        }
    }
    return mst;
}
int sum_weights(EdgeList const &L){ return std::accumulate(L.begin(), L.end(), 0, [](int s, const Edge &e){return s+e.weight;}); }
// Traversals
VertexList Graph::edges_from(Vertex vertex)const{
    VertexList n;
    for(const Edge &e: *this){ if(e.u==vertex) n.push_back(e.v); }
    return n;
}

VertexList dfs(const Graph& graph, Vertex startVertex){
    vector<bool> v(graph.numVertices, false);
    VertexList t;
    stack<Vertex> s({startVertex});
    for(;!s.empty();){
        Vertex u = s.top(); s.pop();
        if (!v[u]){
            v[u] = true;
            t.push_back(u);            
            VertexList n=graph.edges_from(u);
            //sort(n.begin(), n.end());
            //sort(n.rbegin(), n.rend());
            for(auto it=n.begin(); it!=n.end(); ++it){ if(!v[*it]) s.push(*it); }
        }
    }
    return t;
}

VertexList bfs(const Graph& graph, Vertex startVertex){
    vector<bool> v(graph.numVertices, false);
    v[startVertex]=true;
    VertexList t;
    queue<Vertex> q(deque<Vertex>{startVertex});
    for(;!q.empty();){
        Vertex u=q.front(); q.pop();
        t.push_back(u);
        VertexList n=graph.edges_from(u);
        for(Vertex w:n){ if(!v[w]){ v[w]=true; q.push(w);} }
    }
    return t;
}

