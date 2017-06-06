#include "ActorGraph.h"
#include <queue>
#include <vector>
#include <utility>
#include <cstring>
#include <algorithm>
using namespace std;
/*
//template <string>
bool string::operator<(string s1, string s2){
  int yearOfs1 = stoi(s1.substr(s1.find("^") + 1));
  int yearOfs2 = stoi(s2.substr(s2.find("^") + 1));
  return yearOfs1 < yearOfs2;
}
*/
//functor for the std::sort,use it for the customized string
//comparison
struct VCpr
{
  inline bool operator() (string s1, string s2)
  {
    int yearOfs1 = stoi(s1.substr(s1.find("^") + 1));
    int yearOfs2 = stoi(s2.substr(s2.find("^") + 1));
    return yearOfs1 > yearOfs2;
  }
};

int main(int argc, char* argv[]){
  if(argc!=5) return -1;  

  ifstream pair;
  ofstream out;
  pair.open(argv[3]);
  bool have_header = false;
  out.open(argv[4]);
  Vertex * sour;
  Vertex * des;
   
  char argv2 = argv[2][0];
  
  unordered_map<string, Vertex*> v;
  unordered_map<string, vector<string>> m;
  vector<Vertex*> resetVs;
  // Initialize the graph
  ActorGraph graph(v, m, resetVs);
  graph.loadFromFile(argv[1],argv[2]);
  graph.addEdge(argv2);
 
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
    // find their relationships either bfs od Dijkstra
    stack<Vertex *> st;
    if(argv2=='u') st = graph.bfs(sour, des);
    if(argv2=='w') st = graph.Dijkstra(sour, des);

    // Print them out into the output file in the format based on the
    // weight input
    Vertex * temp;
    Vertex * next;
    string m;
    if(argv2=='u'){
      while (1) {
        temp = st.top();
        st.pop();
        if (st.empty()) {
          break;
        }
        next = st.top();  
        for (auto it = temp->movie.begin(); it != temp->movie.end(); it++) {
          auto itnext = next->movie.find(*it);
          if(itnext!=next->movie.end()) m=*it;
        }
        char* c = strdup(m.c_str());
        char* c1 = strtok(c,"^");
        char* c2 = strtok(NULL,"^");
        out << "(" << temp->name << ")--[" << c1 << "#@" << c2 << "]-->";
        free(c);       
        }
    }
    if(argv2=='w'){
      while(1){
        vector<string> vc;
        temp=st.top();
        st.pop();
        if(st.empty()){
          break;
        }
        next=st.top();
        //push same movies between actor A and B into a vecotr
        for(auto it = temp->movie.begin();it!=temp->movie.end();it++){
          auto itnext = next->movie.find(*it);
          if(itnext!=next->movie.end()) vc.push_back(*it);
        }
        //use the customized sort to finde the earliest movie
        std::sort(vc.begin(),vc.end(),VCpr());
        m=vc.front();
        char* c = strdup(m.c_str());
        char* c1 = strtok(c,"^");
        char* c2 = strtok(NULL,"^");
        out << "(" << temp->name << ")--[" << c1 << "#@" << c2 << "]-->";
        free(c);
      }   
    }

    out << "(" << next->name << ")" << endl;
  }

}
