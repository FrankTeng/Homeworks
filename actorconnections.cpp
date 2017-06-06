#include "ActorGraph.h"
#include <queue>
#include <vector>
#include <utility>
#include <cstring>
#include <algorithm>
using namespace std;

struct VCpr
{
  inline bool operator() (string s1, string s2)
  {
    int yearOfs1 = stoi(s1.substr(s1.find("^") + 1));
    int yearOfs2 = stoi(s2.substr(s2.find("^") + 1));
    return yearOfs1 > yearOfs2;
  }
};

struct YCpr
{
  inline bool operator() (string s1, string s2)
  {
    int yearOfs1 = stoi(s1.substr(s1.find("^") + 1));
    int yearOfs2 = stoi(s2.substr(s2.find("^") + 1));
    return yearOfs1 < yearOfs2;
  }
};

int main(int argc, char* argv[]){
  if(argc!=5) return -1;

  ifstream pair;
  ofstream out;
  pair.open(argv[2]);
  bool have_header = false;
  out.open(argv[3]);
  Vertex * sour;
  Vertex * des;
  unordered_map<string, Vertex*> v;
  unordered_map<string, vector<string>> m;
  vector<Vertex*> resetVs;
      // Initialize the graph
  ActorGraph graph(v, m, resetVs);
  graph.loadFromFile(argv[1],argv[2]);

  char argv2 = argv[4][0];
  out << "Actor1\tActor2\tYear" << endl;

  if(argv2=='b'){
    //graph.addEdge(argv2);
    while(pair){


      string s;
      if(!getline(pair,s)) break;
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
      // Get the position of the two vertex in our graph
      auto so = graph.vs.find(toFind[0]);
      auto de = graph.vs.find(toFind[1]);
      sour = so->second;
      des = de->second;// Two vertex
      /*
      int syear=9999,dyear=9999,inity=0;
      for(auto i = sour->movie.begin();i!=sour->movie.end();i++){
	if(syear>stoi((*i).substr((*i).find("^") + 1))){
	  syear = stoi((*i).substr((*i).find("^") + 1));
	}
      }
      for(auto i = des->movie.begin();i!=des->movie.end();i++){
	if(dyear>stoi((*i).substr((*i).find("^") + 1))){
	  dyear = stoi((*i).substr((*i).find("^") + 1));
	}
      }
      if(syear>dyear){inity=syear;}
      else{inity=dyear;}
      */

      unsigned int start_year;
      int s_start_year = 2016;
      int d_start_year = 2016;
      for (auto it = sour->movie.begin(); it != sour->movie.end(); it++) {
        if (stoi((*it).substr((*it).find("^") + 1)) < s_start_year) 
          s_start_year = stoi((*it).substr((*it).find("^") + 1));
      }

      for (auto it = des->movie.begin(); it != des->movie.end(); it++) {
        if (stoi((*it).substr((*it).find("^") + 1)) < d_start_year) 
          d_start_year = stoi((*it).substr((*it).find("^") + 1));
      }

      if (s_start_year > d_start_year) start_year = (unsigned int)s_start_year;
      else start_year = (unsigned int)d_start_year;

      unsigned int i;
     for (i = graph.eyear; i < 2016; i++) {
	graph.addYearEdge(i);
        if (start_year <= i) {
	  if (graph.bfsYear(sour, des))
	    break;
        }
      }
      graph.resetEdge();
      out << source << '\t' << dest << '\t' << i << endl;
    }
  } 
}


