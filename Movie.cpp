#include "Movie.h"
#include "DjNode.h"
Movie::Movie(string title, int year){
	title = title;
	year = year;
}
void Movie::add_actor(DjNode* a){
	cast.insert(make_pair(a->name,a));
}

bool Movie::operator==(Movie* movie)const{
	if(movie->title.compare(this->title)&&
		movie->year==this->year){
		return true;
	}
	else{
		return false;
	}
}


