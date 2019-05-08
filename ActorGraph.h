/* CSE 100: PA4
 * ActorGraph.h
 * Author: Robin Heinonen
 * Date:   10/6/2017
 * Description: class encoding graph of actors connected by movies (header)
 */

#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H

#include <iostream>
#include "ActorNode.h"
#include "Movie.h"
#include <unordered_map>
#include <vector>
#include <set>

using namespace std;

class ActorGraph {
    private:
      bool weighted; //weighted or unweighted edges
      set<string> movie_keys; //movies contained in database
      set<string> actor_keys; //actors contained in database
      int year_start=9999; //earliest movie year in database

    public:
      std::unordered_map<std::string,Movie*> movies; //maps title of movie+year to corresponding Movie object

      std::unordered_map<std::string,ActorNode*> actors; //maps name of actor to corresponding ActorNode object

      /* class constructor */
      ActorGraph(bool w): weighted(w){}

      /* returns year of earliest movie in database */
      int getFirstYear()
      {
        return year_start;
      }

      /* add edges into graph corresponding to movies released between two given years
       * return a vector of pairs of actors that were not previously connected that are now connected
       */
      std::vector<std::pair<ActorNode*,ActorNode*>> addEdges(int year1,int year2);

      /**
       * Load the graph from a tab-delimited file of actor->movie relationships.
       *
       * in_filename - input filename
       * return true if file was loaded sucessfully, false otherwise
       */
      bool loadFromFile(const char* in_filename);

      /* class destructor */
      ~ActorGraph();



};


#endif // ACTORGRAPH_H
