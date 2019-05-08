/* CSE 100: PA4
 * Movie.cpp
 * Author: Robin Heinonen
 * Date:   10/6/2017
 * Description: class storing movie information for graph (implementation)
 */
#include "ActorNode.h"
#include "Movie.h"
#include <vector>
#include <string>

std::string Movie::getTitle() const{
  return title;
}

int Movie::getYear() const{
  return year;
}

int Movie::getWeight() const{
  return weight;
}

std::set<ActorNode*>::iterator Movie::beginCast(){
  return cast.begin();
}

std::set<ActorNode*>::iterator Movie::endCast(){
  return cast.end();
}

void Movie::insertActor(ActorNode* actor){
  cast.emplace(actor);
}
