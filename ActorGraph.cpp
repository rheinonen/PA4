/* CSE 100: PA4
 * ActorGraph.cpp
 * Author: Robin Heinonen
 * Date:   10/6/2017
 * Description: class encoding graph of actors connected by movies (implementation)
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include "ActorGraph.h"
#include "ActorNode.h"
#include "Movie.h"

using namespace std;

bool ActorGraph::loadFromFile(const char* in_filename) {
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
        int movie_year = stoi(record[2]);
        string movie_key=movie_title+"\t"+to_string(movie_year);
        if(movie_year<year_start)
        {
          year_start=movie_year; //update earliest movie year if necessary
        }


        //add actor and movie to database if necessary and update database to reflect this movie-actor pair
        if(actor_keys.count(actor_name)==0)
        {
          actor_keys.emplace(actor_name);
          actors.emplace(actor_name,new ActorNode(actor_name));
        }
        if(movie_keys.count(movie_key)==0)
        {
          movie_keys.emplace(movie_key);
          movies.emplace(movie_key,new Movie(movie_title,movie_year,weighted));
        }
          movies[movie_key]->insertActor(actors[actor_name]);

    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();

    return true;
}

std::vector<std::pair<ActorNode*,ActorNode*>> ActorGraph::addEdges(int year1,int year2)
{
  std::vector<std::pair<ActorNode*,ActorNode*>> out;
  for(std::string nextmovie : movie_keys) //loop through movies
  {
    if(movies[nextmovie]->getYear() <= year2 && year1 < movies[nextmovie]->getYear() ) //if year in correct bounds
    {
      for(auto actor1=movies[nextmovie]->beginCast();actor1!=movies[nextmovie]->endCast();++actor1)
      { //loop through the cast of each movie and pick out pairs of actors in the same movie
        for(auto actor2=movies[nextmovie]->beginCast();actor2!=movies[nextmovie]->endCast();++actor2)
        {
          if(*actor1!=*actor2)
          {
            bool isnew=(*actor1)->insertEdge(*actor2,movies[nextmovie]);
            (*actor2)->insertEdge(*actor1,movies[nextmovie]);
            if(isnew) //add actor pair to return vector if first time actors connected
            {
              out.push_back(std::make_pair(*actor1,*actor2));
            }
          }
        }
      }
    }
  }
  return out;
}

ActorGraph::~ActorGraph(){
  for(auto it=actor_keys.begin();it!=actor_keys.end();++it)
    delete actors[*it];
  for(auto it=movie_keys.begin();it!=movie_keys.end();++it)
    delete movies[*it];

}
