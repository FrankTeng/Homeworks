#ifndef ACTORNODE_H
#define ACTORNODE_H

#include <string>
#include <unordered_set>

using namespace std;

class ActorNode{
private:
  std::string name;
  unordered_set<std::string> movie;
public:
  ActorNode(string name, unordered_set<std::string> movie);
  bool makeEdge(ActorNode a1, ActorNode a2);
}
