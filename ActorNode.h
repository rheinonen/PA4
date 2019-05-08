/* CSE 100: PA4
 * ActorNode.h
 * Author: Robin Heinonen
 * Date:   10/6/2017
 * Description: class storing actor information for graph (header)
 */

#ifndef ACTORNODE_H
#define ACTORNODE_H

#include <vector>
#include <string>
#include <unordered_map>

class Movie;

class ActorNode{
  private:
    std::string name; //actor's name
    std::vector<ActorNode*> actors; //actors appearing in at least one common movie

  public:

    /* constructor */
    ActorNode(std::string n): name(n){}

    //associates a connected actor to the most recent movie they both appeared in
    std::unordered_map<ActorNode*,Movie*> edges;

    /* add a new connection given a second actor, a movie key, and the year it came out
    * return true if edge didn't previously exist
    */
    bool insertEdge(ActorNode* other,Movie* movie);

    /* returns iterator pointing to beginning of vector of directly connected actors */
    std::vector<ActorNode*>::iterator beginActors()
    {
      return actors.begin();
    }

    /* returns iterator pointing to end vector of directly connected actors */
    std::vector<ActorNode*>::iterator endActors()
    {
      return actors.end();
    }

    /*returns actor's name*/
    std::string getName() const;

};



#endif //ACTORNODE_H
