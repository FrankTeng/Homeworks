#include <unordered_map>
#include <string>
using namespace std;
class DjNode;
class Movie{
public:
	string title;
	int year;
	unordered_map<string, DjNode*> cast;
	Movie(){}
	Movie(string title, int year);
        bool operator==(Movie* movie)const;
	void add_actor(DjNode* a);
	~Movie(){}
};
