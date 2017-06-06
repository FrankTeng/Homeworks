#include "DisjointSet.h"
#include <utility>
DisjointSet::~DisjointSet(){
  for(auto it = actor.begin();it!=actor.end();it++){
    if(it->second!=NULL){
      delete it->second;
      it->second = NULL;
    }
  }
  for(auto it = movie.begin();it!=movie.end();it++){
    int i = it->first;
    for(auto it = movie[i].begin();it!=movie[i].end();it++){
      if(it->second!=NULL){
	delete it->second;
	it->second = NULL;
      }
    }
  }
}
void DisjointSet::Union(DjNode* a1, DjNode* a2){
  DjNode * r1 = find(a1);
  DjNode * r2 = find(a2);
  if(r1==r2) return;
  if(r1->weight<r2->weight){
    r2->weight += r1->weight+r2->weight;
    r1->p = r2;
  }
  else{
    r1->weight += r2->weight;
    r2->p = r1;
  }
}
DjNode* DisjointSet::find(DjNode* a){
  if(a!=a->p) a->p = find(a->p);
  return a->p;
}
bool DisjointSet::loadFromFile(const char* in){
  // Initialize the file stream
  std::ifstream infile(in);

  bool have_header = false;

  // keep reading lines until the end of file is reached
  // keep reading lines until the end of file is reached
  while (infile) {
    std::string s;

    // get the next line
    if (!getline(infile, s)) break;
    if (!have_header) {
      // skip the header
      have_header = true;
      continue;
    }

    std::istringstream ss(s);
    std::vector <std::string> record;

    while (ss) {
      std::string next;

      // get next string before hitting a tab character and put it in
      // 'next'
      if (!getline(ss, next, '\t')) break;

      record.push_back(next);
    }


    if (record.size() != 3) {
      // we should have exactly 3 columns
      continue;
    }
    std::string actor_name(record[0]);
    std::string movie_title(record[1]);
    int movie_year = stoi(record[2]);
    DjNode* a;
    if(actor.find(actor_name)==actor.end()){
      
      a = new DjNode(actor_name);
      actor.insert(make_pair(actor_name,a));
    }
    else{
      a = actor[actor_name];
    }
    if(a->int_year>movie_year) a->int_year = movie_year;
    Movie* mv;
    if(movie[movie_year].find(movie_title+
	  to_string(movie_year))==movie[movie_year].end()){

      mv = new Movie(movie_title,movie_year);
      movie[movie_year].insert
	(make_pair(movie_title+to_string(movie_year),mv));
    }
    else{
      mv = movie[movie_year][movie_title+to_string(movie_year)];
    }
    mv->add_actor(a);
    a->add_movie(mv);
  }
  if(!infile.eof()) return false;
  infile.close();
  return true;
}

void DisjointSet::clear(){
  for(auto it = actor.begin();it!=actor.end();it++){
    if(it->second!=NULL) it->second->p = it->second;
  }
  return;
}


