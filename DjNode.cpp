#include "DjNode.h"
#include "Movie.h"

DjNode::DjNode(string actor_name){
	name = actor_name;
	weight = 0;
	p = NULL;
	int_year = 0; 
}
void DjNode::add_movie(Movie* movie){
	played.insert(make_pair(movie->title + to_string(movie->year),movie));
}
