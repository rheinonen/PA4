/* CSE 100: PA4
 * Movie.h
 * Author: Robin Heinonen
 * Date:   10/6/2017
 * Description: class storing movie information for graph (header)
 */

#ifndef MOVIE_H
#define MOVIE_H

#include <string>
#include <set>
#include <vector>
#include "ActorNode.h"

class ActorNode;

class Movie{
  private:
    std::string title; //title of movie
    int year; //year movie released
    int weight; //weight associated with year
    std::set<ActorNode*> cast; //actors appearing in movie
    std::string key; //movie record accessed by this key. (disambiguates common titles by year)

  public:
    Movie(std::string t,int y,bool weighted):title(t),year(y){
      if(weighted)
        weight=2016-y;
      else
        weight=1;
      key=title+"\t"+std::to_string(year);
    }
    /* returns movie title */
    std::string getTitle() const;

    /* returns movie year*/
    int getYear() const;

    /* returns weight associated with movie year */
    int getWeight() const;

    /* returns iterator pointing to beginning of cast*/
    std::set<ActorNode*>::iterator beginCast();

    /* returns iterator pointing to end of cast */
    std::set<ActorNode*>::iterator endCast();

    /* add actor to cast */
    void insertActor(ActorNode* actor);

    /* returns movie's string key */
    std::string getKey() const
    {
      return key;
    }

};



#endif //MOVIE_H
