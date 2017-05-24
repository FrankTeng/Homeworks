/*
 * ActorGraph.cpp
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
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

using namespace std;


ActorGraph::ActorGraph(unordered_map<string, Vertex*> vs, unordered_map<string, vector<string>> moviemap, vector<Vertex*> resetVs) {
  vs = vs;
  resetVs = resetVs; 
  moviemap = moviemap;
}

void ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {
  // Initialize the file stream
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

      // get the next string before hitting a tab character and put it in 'next'
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
    string mv = movie_title + "%" + movie_year;

 


    // we have an actor/movie relationship, now what

    auto it = moviemap.find(mv);

    if (moviemap.empty()) {
      vector<string> v;
      v.push_back(actor_name);
      pair<string, vector<string>> pr = make_pair(mv,v);
      moviemap.insert(pr);
    } 

    else if (it != moviemap.end()) {
      it->second.push_back(actor_name);
    }

    else {
      vector<string> v;
      v.push_back(actor_name);
      pair<string,vector<string>> pr = make_pair(mv,v);
      moviemap.insert(pr);
    }
    
    auto ac = vs.find(actor_name);

    if (vs.empty()) {
      Vertex * vp = new Vertex;
      vp->name = actor_name;
      string temp;
      temp.append(movie_title);
      temp.append("%");
      temp.append(movie_year);
      vp->movie.insert(temp);
      resetVs.push_back(vp);
      pair<string, Vertex*> pr  = make_pair(actor_name,vp);
      vs.insert(pr);
    }

    else if(ac != vs.end()){  
      string temp;
      temp.append(movie_title);
      temp.append("%");
      temp.append(movie_year);
      ac->second->movie.insert(temp);
    }

    else {
      Vertex * vp = new Vertex;
      vp->name = actor_name;
      string temp;
      temp.append(movie_title);
      temp.append("%");
      temp.append(movie_year);
      vp->movie.insert(temp);
      pair<string, Vertex*> pr  = make_pair(actor_name,vp);
      vs.insert(pr);
     // cout<<vp->name<<endl;
    }
    //cout<<vs.size()<<endl; 
  }

  if (!infile.eof()) {
    cerr << "Failed to read " << in_filename << "!\n";
    return;
  }
  infile.close();
  return;
}

void ActorGraph::addEdge() {

  /*for(unsigned int i=0;i<this->vs.size();i++)
  {
    cout << "a" << endl;
    for(const auto& elem:this->vs[i]->movie)
    {
      unsigned int temp = i+1;
      while(temp!=this->vs.size())
      {
        auto it = this->vs[temp]->movie.find(elem);
        if(it !=v this->vs[temp]->movie.end())
        {
          pair<string,Vertex*> pr = make_pair(elem,this->vs[temp]);
          vs[i]->adj.push_back(pr);
        }
        temp++;
      }
    }
  }*/
  for(auto i = vs.begin(); i != vs.end(); i++)
  {
    resetVs.push_back(i->second);
   
    for(const auto& elem: i->second->movie)
    {
      auto it = moviemap.find(elem);
      for (unsigned int j = 0; j < it->second.size(); j++) {
        auto vtx = vs.find(it->second[j]);
        if (i->second->name != it->second[j]) {
          pair<string, Vertex*> pr = make_pair(elem, vtx->second);
          i->second->adj.push_back(pr);
        }
      }
    }
  }
 
}

stack<Vertex*> ActorGraph::bfs(Vertex* s, Vertex* d){
  
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
      if(get<1>(curr->adj[i])->visited==false){
        get<1>(curr->adj[i])->visited = true;
        temp = get<1>(curr->adj[i]);
        temp->prev = curr;

        if (temp->name == d->name) {
          while(temp->prev){
            cout<<temp->prev->name<<endl;
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
