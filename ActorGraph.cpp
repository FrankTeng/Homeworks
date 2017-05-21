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
#include "ActorGraph.h"

using namespace std;

struct Vertex
{
  unordered_set<string> movie;
  vector<std::pair<string,Vertex*>> adj; 
  string name;
};

ActorGraph::ActorGraph(void) {
  vector<Vertex*> vs;
}

ActorGraph* ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {
    // Initialize the file stream
    ifstream infile(in_filename);
    ActorGraph * graph = new Actorgraph(); 
    bool have_header = false;
    int i = 0;
  
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

        if(graph->vs[i] && graph->vs[i]->name!=actor_name) i++;
        // we have an actor/movie relationship, now what
        if(!graph->vs[i]){
          vertex * vp = new vertex;
          vp->name = actor_name;
          string temp;
          temp.append(movie_title);
          temp.append('\t');
          temp.append(movie_year);
          vp->movie.inset(temp);
          vs[i] = vp;
        }
        if(graph->vs[i]->name == actor_name){
          string temp;
          temp.append(movie_title);
          temp.append('\t');
          temp.append(movie_year);
          vs[i]->vp->movie.insert(temp);
        }
    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();
    for(int j=0;j<graph->vs.size();j++){
        
    }

    return graph;
}
