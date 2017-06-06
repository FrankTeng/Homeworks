/*
 * ActorGraph.cpp
 * Author: Yue Teng; Liu Yang
 * Date:   05/22/2017
 * Assignment#: PA4
 * Overview: A container of graph which contains Vertex of actors
 *           and edges of their relationships by coorperate movies
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <unordered_map>
#include "ActorGraph.h"
#include <cstring>
#include <utility>

using namespace std;



//Destructor

ActorGraph::~ActorGraph() {
  for(auto it = vs.begin();it!=vs.end();it++){
    if(it->second!=nullptr){
      delete it->second;
      it->second=nullptr;
    }
  }
  resetVs.clear();
  vs.clear();
  moviemap.clear();
}

// Constructor
ActorGraph::ActorGraph(unordered_map<string, Vertex*> vs, unordered_map<string, vector<string>> moviemap, vector<Vertex*> resetVs) {
  eyear = 2016;
  vs = vs;
  resetVs = resetVs; 
  moviemap = moviemap;
}

// Load the input file and create vertex of actors and string of movies.
void ActorGraph::loadFromFile(const char* in_filename,bool use_weighted_edges){
  // Initialize the file streamertex*& l, Vertex*& r) const {

  ifstream infile(in_filename); 
  bool have_header = false;
  // keep reading lines until the end of file is reached
  while (infile) {
    string s;

    // get the next line
    if (!getline( infile, s )) break;

    if (!have_header) {
      // skip the header
      have_header = true;
      continue;
    }

    istringstream ss( s );
    vector <string> record;

    while (ss) {
      string next;

      // get the next string before hitting a tab character 
      // and put it in 'next'
      if (!getline( ss, next, '\t' )) break;

      record.push_back( next );
    }

    if (record.size() != 3) {
      // we should have exactly 3 columns
      continue;
    }

    string actor_name(record[0]);
    string movie_title(record[1]);
    string movie_year(record[2]);
    //Merge the movie title and movie year
    //Distinguish by a sign "^"
    if(stoi(movie_year)<eyear) eyear = stoi(movie_year);
    string mv = movie_title + "^" + movie_year;




    //Use iterator to locate the movie in the map
    auto it = moviemap.find(mv);
    //Case 1: if the movie map is empty
    if (moviemap.empty()) {
      vector<string> v;
      v.push_back(actor_name);
      //Create new vector and insert it into the map
      pair<string, vector<string>> pr = make_pair(mv,v);
      moviemap.insert(pr);
    } 

    //Case 2: We find the movie in the map, then push the actor's name
    //        into the second of map element
    else if (it != moviemap.end()) {
      it->second.push_back(actor_name);
    }

    //Case 3: Did not find the movie in the map
    else {
      vector<string> v;
      v.push_back(actor_name);
      pair<string,vector<string>> pr = make_pair(mv,v);
      moviemap.insert(pr);// Like Case 1
    }

    //Use iterator to locate the vertex of actor, as above
    auto ac = vs.find(actor_name);

    if (vs.empty()) {
      Vertex * vp = new Vertex;
      vp->name = actor_name;// Create a new vertex
      string temp;
      temp.append(movie_title);
      temp.append("^");
      temp.append(movie_year);
      vp->movie.insert(temp);

      pair<string, Vertex*> pr  = make_pair(actor_name,vp);
      vs.insert(pr);// Insert to the map and the vector
    }

    else if(ac != vs.end()){  
      string temp;
      temp.append(movie_title);
      temp.append("^");
      temp.append(movie_year);
      ac->second->movie.insert(temp);// Insert to the map only
    }

    else {
      Vertex * vp = new Vertex;
      vp->name = actor_name;
      string temp;
      temp.append(movie_title);
      temp.append("^");
      temp.append(movie_year);
      vp->movie.insert(temp);
      pair<string, Vertex*> pr  = make_pair(actor_name,vp);
      vs.insert(pr);// As above
      // cout<<vp->name<<endl;
    }
    //cout<<vs.size()<<endl; 
  }

  if (!infile.eof()) {
    cerr << "Failed to read " << in_filename << "!\n";
  }
  infile.close();
}

/*
   void ActorGraph::addEdgeCon(unsigned int year){
   for(auto i = vs.begin(); i != vs.end(); i++)
   {
   resetVs.push_back(i->second);// add all the Vertex in to resetVs for bfs
// Use the movie set we store in the vertex to establish edges(adj)
for(const auto& elem: i->second->movie)
{
int tempY = stoi(elem.substr(elem.find("^") + 1));
if(tempY == year){
auto it = moviemap.find(elem);
for (unsigned int j = 0; j < it->second.size(); j++) {
auto vtx = vs.find(it->second[j]); // get the vertex
if (i->second->name != it->second[j]) { // get adjs except itself
//char* c = strdup(elem.c_str());
//char* c1 = strtok(c,"^");
//char* c2 = strtok(NULL,"^");
//int weight;
//if(ch == 'u') weight = 0;
//if(ch == 'w') weight = 1 + (2015 - c2);
int weight = 0;
//if unweighted, assign 0 weight to each edge
//make edge and insert it
tuple<string, Vertex*,int> tp = 
make_tuple(elem, vtx->second, weight);
i->second->adj.push_back(tp);
}
}
}
}
}
}*/
//function to add edges to the graph
void ActorGraph::addEdge(char ch) {

  for(auto i = vs.begin(); i != vs.end(); i++)
  {
    resetVs.push_back(i->second);// add all the Vertex in to resetVs for bfs
    // Use the movie set we store in the vertex to establish edges(adj)
    for(const auto& elem: i->second->movie)
    {
      auto it = moviemap.find(elem);
      for (unsigned int j = 0; j < it->second.size(); j++) {
	auto vtx = vs.find(it->second[j]); // get the vertex
	if (i->second->name != it->second[j]) { // get adjs except itself
	  //char* c = strdup(elem.c_str());
	  //char* c1 = strtok(c,"^");
	  //char* c2 = strtok(NULL,"^");
	  //int weight;
	  //if(ch == 'u') weight = 0;
	  //if(ch == 'w') weight = 1 + (2015 - c2);
	  int weight,year;
	  //if unweighted, assign 0 weight to each edge
	  if(ch=='u') weight = 0;
	  if(ch=='w'){
	    //otherwise calculate the weight and insert it
	    year = stoi(elem.substr(elem.find("^")+1));
	    weight = 2016 - year;
	  }
	  //make edge and insert it
	  tuple<string, Vertex*,int> tp = 
	    make_tuple(elem, vtx->second, weight);
	  i->second->adj.push_back(tp);
	}
      }
    }
  }
}

// To get the shortest path of two actors by adj
stack<Vertex*> ActorGraph::bfs(Vertex* s, Vertex* d){

  //Refresh the visit status of every Vertex
  for(unsigned int j=0;j<resetVs.size();j++){
    resetVs[j]->visited = false;
    resetVs[j]->prev = NULL;
  }
  queue<Vertex*> toExplore;
  stack<Vertex*> result;
  Vertex* temp;
  s->visited = true;
  toExplore.push(s);
  while(!toExplore.empty()){

    Vertex * curr = toExplore.front();
    toExplore.pop();
    for(unsigned int i=0;i<curr->adj.size();i++){
      // Go through only the unvisited vertex
      if(get<1>(curr->adj[i])->visited==false){
	get<1>(curr->adj[i])->visited = true;
	temp = get<1>(curr->adj[i]);
	temp->prev = curr;

	// When we find it, use prev to get the path
	if (temp->name == d->name) {
	  while(temp->prev){
	    //cout<<temp->prev->name<<endl;
	    //temp->visited = false;
	    result.push(temp);

	    temp = temp->prev;
	  }
	  //s->visited = false;
	  result.push(s);
	  return result;
	}

	else {
	  toExplore.push(temp);
	}
      }
    }
  }
  return result;
}
//The Dijkstra algorithm to find the shortest path for weighted edge
stack<Vertex*> ActorGraph::Dijkstra(Vertex* s, Vertex* d){
  //reset everything
  for(unsigned int j=0;j<resetVs.size();j++){
    resetVs[j]->visited = false;
    resetVs[j]->prev = NULL;
    resetVs[j]->dist = numeric_limits<int>::max();
  }
  stack<Vertex*> result;
  int temp_dist = 0;
  s->dist=0;
  //pq for the algorithm
  priority_queue<Vertex*,vector<Vertex*>,Cpr> pq;
  pq.push(s);
  //finding the shortest path between vertex s and d, and push
  //the path to the pq
  while(!pq.empty()){
    Vertex * temp = pq.top();
    pq.pop();
    if(temp->visited==false){
      temp->visited=true;
      for(auto nb:temp->adj){
	temp_dist = temp->dist + get<2>(nb);
	//relaxation 
	if (temp_dist < get<1>(nb)->dist) {
	  get<1>(nb)-> prev = temp;
	  get<1>(nb)-> dist = temp_dist;
	  pq.push(get<1>(nb));
	}
      }
    }
  }
  //using the d node to backtrace the path, and push them to the
  //returning stack
  Vertex* temp = d;
  while (temp->prev) {
    result.push(temp);
    temp = temp->prev;
  }
  if (result.size() == 0) return result;
  result.push(s);
  return result;
}
/*
void ActorGraph::addYearEdge(unsigned int year) {
  for(auto i = vs.begin(); i != vs.end(); i++)
  {
    //resetVs.push_back(i->second);// add all the Vertex in to resetVs for bfs
    // Use the movie set we store in the vertex to establish edges(adj)
    for(const auto& elem: i->second->movie)
    {
      unsigned int y = stoi(elem.substr(elem.find("^")+1));
      if (y == year) {

	auto it = moviemap.find(elem);
	for (unsigned int j = 0; j < it->second.size(); j++) {
	  auto vtx = vs.find(it->second[j]); // get the vertex
	  if (i->second->name != it->second[j]) { // get adjs except itself
	    tuple<string, Vertex*,int> tp = 
	      make_tuple(elem, vtx->second, 0);
	    i->second->adj.push_back(tp);
	  }
	}
      }
    }
  }
}

void ActorGraph::resetEdge() {
  for (unsigned int i = 0; i < resetVs.size(); i++) {
    resetVs[i]->adj.clear();
  }

}
*/

bool ActorGraph::bfsYear(Vertex* s, Vertex* d, unsigned int weight) {

  //Refresh the visit status of every Vertex
  for(unsigned int j=0;j<resetVs.size();j++){
    resetVs[j]->visited = false;
    resetVs[j]->prev = NULL;
  }

  queue<Vertex*> toExplore;
  Vertex* temp;
  s->visited = true;
  toExplore.push(s);
  while(!toExplore.empty()){

    Vertex * curr = toExplore.front();
    toExplore.pop();
    for(unsigned int i = 0;i < curr->adj.size() ; i++){
      // Go through only the unvisited vertex
      if(get<1>(curr->adj[i])->visited==false&&(unsigned int)get<2>(curr->adj[i]) <= weight){
	get<1>(curr->adj[i])->visited = true;
	temp = get<1>(curr->adj[i]);
	temp->prev = curr;

	// When we find it, use prev to get the path
	if (temp->name == d->name) {
	  return true;
	}
	toExplore.push(temp);
      }
    }
  }
  return false;


}




