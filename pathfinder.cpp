/* CSE 100: PA4
 * pathfinder.cpp
 * Author: Robin Heinonen
 * Date:   10/6/2017
 * Description: finds shortest paths (weighted or unweighted) between actors
 */

#include "ActorGraph.h"
#include "ActorNode.h"
#include "Movie.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <queue>
#include <unordered_map>
#include <stack>
#include "Dijkstra.h"

using namespace std;

int main(int argc,char** argv)
{
  if(argc!=5)
  {
    cout << "Error: wrong number of input arguments." << endl;
    return -1;
  }

  char* castfile=argv[1];
  bool weighted;
  if(*argv[2]=='w')
    weighted=true;
  else if(*argv[2]=='u')
    weighted=false;
  else
    {
      cout << "Error: please enter 'w' or 'u' for second argument." << endl;
      return -1;
    }

  char* pairfile=argv[3];
  char* outfile=argv[4];

  ActorGraph graph(weighted);
  graph.loadFromFile(castfile); //build graph info
  graph.addEdges(0,2017); //add all edges

  ifstream pairstream;
  pairstream.open(pairfile);

  if(!pairstream.is_open())
  {
      cout<< "Error: invalid actor pair list file. No file was opened." << endl;
      return -1;
  }


  ofstream out;
  out.open(outfile);
  out << "(actor)--[movie#@year]-->(actor)--..." << endl; //header

  string actor1;
  string actor2;
  getline(pairstream,actor1); //skip first line of file

  while(pairstream.peek()!=EOF)
  {
    getline(pairstream,actor1,'\t');
    getline(pairstream,actor2);

    if(actor1==actor2) //edge case where both actors the same
    {
      out << actor1 << endl;
      continue;
    }

    cout << "pathfinding " << actor1 << " and " << actor2 << "..." << endl;

    //run Dijkstra's algorithm on actor1 and actor2
    priority_queue<DijkstraNode> pq;
    unordered_map<ActorNode*,DijkstraData> data;
    ActorNode* node1=graph.actors[actor1];
    ActorNode* node2=graph.actors[actor2];
    pq.emplace(0,node1);
    data.emplace(node1,DijkstraData());
    data[node1].dist=0;

    string finalactor;

    while(!pq.empty())
    {
      ActorNode* nextNode=pq.top().node;
      finalactor=nextNode->getName();
      pq.pop();

      if(nextNode==node2) //terminate loop if path to terminal node is found
        break;
      if(!data[nextNode].done)
      {
        data[nextNode].done=true;
        for(auto it=nextNode->beginActors();it!=nextNode->endActors();++it) //explore edges connected to node
        {
          ActorNode* exploreNode=*it;
          Movie* nextmovie=nextNode->edges[*it];


          if(data[exploreNode].done==true) //don't bother adding an actor to queue if done already
          {
            continue;
          }

          if(data.count(exploreNode)==0) //create data for actor if not already encountered
            data.emplace(exploreNode,DijkstraData());
          int c=data[nextNode].dist+nextmovie->getWeight();
            if(c<data[exploreNode].dist){ //if new distance < old, update dijkstra data for the node and push to queue
              data[exploreNode].prev=nextNode;
              data[exploreNode].dist=c;
              data[exploreNode].prev_movie=nextmovie;
              pq.emplace(c,exploreNode);
            }
        }
      }
    }

    if(finalactor!=actor2) //if no path found
    {
      out << actor1 << " and " << actor2 << " not connected" << endl;
    }

    //trace from actor2 back to actor 1
    stack<ActorNode*> actor_stack;
    ActorNode* curr=node2;

    while(curr!=nullptr)
    {
      actor_stack.push(curr);
      curr=data[curr].prev;
    }

    while(!actor_stack.empty())
    {
      curr=actor_stack.top();
      actor_stack.pop();
      ActorNode* peek;
      if(!actor_stack.empty())
        peek=actor_stack.top();

      //output result to file
      out << "(" << curr->getName() << ")";
      if(!actor_stack.empty())
        out << "--["<<data[peek].prev_movie->getTitle() << "#@" << data[peek].prev_movie->getYear() << "]-->";
    }
    out << endl;
  }

  pairstream.close();
  out.close();

  return 0;

}
