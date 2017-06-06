#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
using namespace std;
class Movie;

class DjNode{
public:
	string name;
	int weight;
	DjNode* p;
	int int_year;
	DjNode(string actor_name);
	unordered_map<string,Movie*> played;
	void add_movie(Movie* movie);
};
