#include "ActorGraph.h"
#include "DisjointSet.h"
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

  char argv2 = argv[4][0];

  DisjointSet set;
  bool temp = set.loadFromFile(argv[1]);
  if(temp==false) return -1;

  out << "Actor1\tActor2\tYear" << endl;
  if(argv2=='u'){
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
      
      DjNode* a1 = set.actor[source];
      DjNode* a2 = set.actor[dest];
      if(a1==NULL||a2==NULL){continue;}
      set.clear();
      bool f = false;
      int a1year = a1->int_year;
      int a2year = a2->int_year;
      int beginyear = (a1year >= a2year) ? a2year : a1year;
      int outyear = 0;
      for(int year = beginyear;year<2016;year++){
	outyear = year;
	for(auto it = set.movie[year].begin();
	    it!=set.movie[year].end();it++){

	  Movie* tempMv = it->second;
	  DjNode* first = tempMv->cast.begin()->second;
	  for(auto it=tempMv->cast.begin();it!=tempMv->cast.end();it++){
	    set.Union(first,it->second);
	  }
	}
	if(set.find(a1)==set.find(a2)){
	  f = true;
	  break;
	}
      }
      if(f){
	out << a1->name << "\t" << a2->name << "\t" << outyear << endl;
      }
    }
  }


  if(argv2=='b'){
    ActorGraph graph(v, m, resetVs);
    graph.loadFromFile(argv[1],argv[2]);
    graph.addEdge('w');
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
	if (start_year <= i) {
	  if (graph.bfsYear(sour, des, 2016 - i))
	    break;
	}
      }
    //  graph.resetEdge();
      out << source << '\t' << dest << '\t' << i << endl;
    }
  }
} 

/*
   int main(int argc, char** argv){
   DisjointSet set;
   string m(argv[4]);
   if(m.compare("ufind")==0){
   bool temp = set.loadFromFile(argv[1]);
   }
   ifstream infile(argv[2]);
   bool have_header=false;
   ofstream outfile(argv[3]);
   outfile << "Actor1" << "\t" << "Actor2" << "\t" <<"Year" << std::endl;
   while (infile) {

   std::string s;

// get the next line
if (!getline(infile, s)) break;

if (!have_header) {
// skip the header
have_header = true;
continue;
}

std::istringstream ss(s);
std::vector <std::string> record;

while (ss) {
std::string next;

// get the next string before hitting a tab character and put it in 
// 'next'
if (!getline(ss, next, '\t')) break;

record.push_back(next);
}

if (record.size() != 2) {
// we should have exactly 2 columns
continue;
}
std::string actor_name_1(record[0]);
std::string actor_name_2(record[1]);

if(m.compare("ufind")==0){
cout<<set.actor[actor_name_1];
DjNode* a1 = set.actor[actor_name_1];
DjNode* a2 = set.actor[actor_name_2];
if(a1==nullptr||a2==nullptr){

continue;
}
set.clear();
bool found = false;
int a1year = a1->int_year;
int a2year = a2->int_year;
cout<<a1year<<a2year<<endl;
int beginyear = (a1year >= a2year) ? a2year : a1year;\
int outyear = 0;
for(int year = beginyear;year<2016;year++){
outyear = year;
for(auto it = set.movie[year].begin();it!=set.movie[year].end();it++){
Movie* tempMV = it->second;
DjNode* first = tempMV->cast.begin()->second;
for(auto it = tempMV->cast.begin();it!=tempMV->cast.end();it++){
set.Union(first,it->second);
}
}
if(set.find(a1)==set.find(a2)){
found = true;
break;
}
}
if(found){
  outfile << a1->name << "\t" << a2->name << "\t"
    << outyear << std::endl;
}
}
}
}
*/		
