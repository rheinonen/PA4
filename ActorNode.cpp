/* CSE 100: PA4
 * ActorNode.cpp
 * Author: Robin Heinonen
 * Date:   10/6/2017
 * Description: class storing actor information for graph (implementation).
 */

#include <vector>
#include <string>
#include "ActorNode.h"
#include "Movie.h"

/* add a new connection given a second actor, a movie key, and the year it came out
* return true if edge didn't previously exist
*/
bool ActorNode::insertEdge(ActorNode* other,Movie* movie){
  if(edges.count(other)==0) //no edge exists currently
  {
    edges.emplace(other,movie);
    actors.push_back(other);
    return true;
  }
  if(movie->getYear()>edges[other]->getYear()) //more current movie
  {
    edges[other]=movie;
  }
  return false;
}


std::string ActorNode::getName() const
{
  return name;
}
