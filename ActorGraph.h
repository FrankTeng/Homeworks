/*
 * ActorGraph.h
 * Author: Yue Teng; Liu Yang
 * Date:   05/22/2017
 * Assignment#: PA4
 * Overview: the header of Actorgraph.
 *
 */

#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H

#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <limits>

// Maybe include some data structures here

using namespace std;
// Declare the Vertex(a kind of node)
struct Vertex
{
  unordered_set<string> movie;
  vector<pair<string,Vertex*>> adj; 
  string name;
  bool visited = false;
  int a = numeric_limits<int>::max();
  Vertex * prev = NULL;

};
class ActorGraph {
    // Maybe add class data structure(s) here

  public:
    vector<Vertex*> resetVs;
    unordered_map<string, Vertex*> vs;
    unordered_map<string, vector<string>> moviemap; 
    ActorGraph(unordered_map<string, Vertex*>, unordered_map<string, vector<string>>,vector<Vertex*>);

    // Maybe add some more methods here

    /** You can modify this method definition as you wish
     *
     * Load the graph from a tab-delimited file of actor->movie relationships.
     *
     * in_filename - input filename
     * use_weighted_edges - if true, compute edge weights as 1 + (2015 - movie_year), otherwise all edge weights will be 1
     *
     * return true if file was loaded sucessfully, false otherwise
     */
    void loadFromFile(const char* in_filename, bool use_weighted_edges);
    void addEdge();
    stack<Vertex*> bfs(Vertex* s, Vertex* d);
//    ~ActorGraph();


};


#endif // ACTORGRAPH_H
