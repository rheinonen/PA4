/* CSE 100: PA4
 * Dijkstra.h
 * Author: Robin Heinonen
 * Date:   10/6/2017
 * Description: data structures useful for Dijkstra's Algorithm
 */

#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include<string>
#include<queue>
#include<array>
#include "Movie.h"
#include "ActorNode.h"

using namespace std;

//class designed to be thrown into priority queue for Dijkstra's algorithm
class DijkstraNode
{
public:
  int weight;
  ActorNode* node;

  DijkstraNode(int w,ActorNode* n): weight(w),node(n){}

  bool operator<(const DijkstraNode other) const
  {
    if(weight==other.weight)
      return (node->getName())>(other.node->getName());
    return weight>other.weight;
  }

};

//container storing Dijkstra's algorithm data associated with a node
class DijkstraData
{
public:
  int dist;
  bool done;
  ActorNode* prev;
  Movie* prev_movie; //movie linking to this node

  DijkstraData(){
    dist=std::numeric_limits<int>::max(); //initialize dist to "infinity"
    done=false;
    prev=nullptr;
  }
};


#endif //DIJKSTRA_NODE
