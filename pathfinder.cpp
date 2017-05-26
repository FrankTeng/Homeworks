#include "ActorGraph.h"
#include <queue>
#include <vector>
#include <utility>
#include <cstring>
using namespace std;

int main(int argc, char* argv[]){
  if(argc!=5) return -1;  

  ifstream pair;
  ofstream out;
  pair.open(argv[3]);
  bool have_header = false;
  out.open(argv[4]);
  Vertex * sour;
  Vertex * des;

  unordered_map<string, Vertex*> v;
  unordered_map<string, vector<string>> m;
  vector<Vertex*> resetVs;
  // Initialize the graph
  ActorGraph graph(v, m, resetVs);
  graph.loadFromFile(argv[1],argv[2]);
  graph.addEdge();
 
  out << "(actor)--[movie#@year]-->(actor)--..." << endl;

  while(pair){
    string s;
    if(!getline(pair,s)) {break;}
    if(!have_header){
      have_header = true;
      continue;
    }
    istringstream ss(s);
    vector <string> toFind;
    while(ss){
      string next;
      if(!getline(ss,next,'\t')) break;
      toFind.push_back(next);
    }
    if(toFind.size()!=2){
      continue;
    }
    string source(toFind[0]);
    string dest(toFind[1]);
    // Load the input file

    // Get the position of the two vertex in our graph
    auto so = graph.vs.find(toFind[0]);
    auto de = graph.vs.find(toFind[1]);
    sour = so->second;
    des = de->second;// Two vertex
    // find their relationships
    stack<Vertex *> st = graph.bfs(sour, des);
    
    // Print them out into the output file in the format.
    Vertex * temp;
    Vertex * next;
    string m;
    while (1) {
      temp = st.top();
      st.pop();
      if (st.empty()) {
        break;
      }
      next = st.top();  
      for (auto it = temp->movie.begin(); it != temp->movie.end(); it++) {
        auto itnext = next->movie.find(*it);
        if(itnext!=next->movie.end()) m = *it;
      }
      char* c = strdup(m.c_str());
      char* c1 = strtok(c,"%");
      char* c2 = strtok(NULL,"%");
      out << "(" << temp->name << ")--[" << c1 << "#@" << c2 << "]-->";       
    }
    out << "(" << next->name << ")" << endl;

  }

}
