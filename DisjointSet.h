#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include "DjNode.h"
#include "Movie.h"
#include <utility>
using namespace std;
class DisjointSet{
public:
	unordered_map<string, DjNode*> actor;
	unordered_map<int,unordered_map<string,Movie*>> movie;
	DisjointSet(void){}
	~DisjointSet();
	bool loadFromFile(const char* in);
	void clear();
	void Union(DjNode* a1,DjNode* a2);
	DjNode* find(DjNode* actor);
};

