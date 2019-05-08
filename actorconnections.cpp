/* CSE 100: PA4
 * actorconnections.cpp
 * Author: Robin Heinonen
 * Date:   10/6/2017
 * Description: program that determines if two actors are connected, and what year they became connected
 */

#include "ActorGraph.h"
#include "Movie.h"
#include "ActorNode.h"
#include "DisjointSet.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string.h>
#include <stdio.h>
#include <unordered_set>

using namespace std;

int main(int argc,char** argv)
{
char* castfile=argv[1];
char* pairfile=argv[2];
char* outfile=argv[3];
bool disjoint;
if(argc==4)
{
  disjoint=true;
}
else if(argc==5)
{
  if(strcmp(argv[4],"ufind")==0)
    disjoint=true;
  else if(strcmp(argv[4],"bfs")==0)
    disjoint=false;
  else{
    cout << "Error: bad search method" << endl;
    return -1;
  }
}
else
{
  cout << "Error: wrong number of inputs."<<endl;
  return -1;
}

ActorGraph graph(false);
graph.loadFromFile(castfile); //build graph

ifstream pairstream;
pairstream.open(pairfile);

if(!pairstream.is_open())
{
    cout<< "Error: invalid actor pair list file. No file was opened." << endl;
    return -1;
}


ofstream out;
out.open(outfile);
out << "Actor1\tActor2\tYear" << endl; //header

vector<pair<string,string>> actor_list;

string actor1;
string actor2;
getline(pairstream,actor1); //skip first line of file

while(pairstream.peek()!=EOF)
{
  getline(pairstream,actor1,'\t');
  getline(pairstream,actor2);
  actor_list.push_back(make_pair(actor1,actor2));
}

pairstream.close();

if(disjoint)
{
  vector<string> all_actors; //compile list of all actors in database
  for(auto it=graph.actors.begin();it!=graph.actors.end();++it)
  {
    all_actors.push_back((*it).first);
  }

  int yearlist[actor_list.size()];
  for(int i=0;i<actor_list.size();i++)
  {
    yearlist[i]=9999;
  }

  DisjointSet djs(all_actors);

  for(int currYear=graph.getFirstYear();currYear<2017;currYear++)
  {

    cout << "checking year "<<currYear << endl;

    std::vector<std::pair<ActorNode*,ActorNode*>> newEdges=graph.addEdges(currYear-1,currYear); //add edges into graph up to current year
    for(auto it=newEdges.begin();it!=newEdges.end();++it) //update disjoint set
    {
      djs.insertConnection(it->first->getName(),it->second->getName());
    }
    int index=0;
    for(auto nextpair=actor_list.begin(); nextpair!=actor_list.end();++nextpair) //for each pair of actors
    {
      if(yearlist[index]!=9999) //skip pair if already determined to be connected
      {
        index++;
        continue;
      }
      actor1=(*nextpair).first;
      actor2=(*nextpair).second;

      if(actor1==actor2) //edge case where both actors the same
      {
        yearlist[index]=0;
        index++;
        continue;
      }

      if(djs.connected(actor1,actor2))
      {
        yearlist[index]=currYear; //update year if connected
      }
      index++;
    }
    //check if done
    bool done=true;
    for(int i=0;i<actor_list.size();i++) //check if all actor pairs have been found to be connected
    {
      if(yearlist[i]==9999){
        done=false;
        break;
      }
    }
    if(done)
      break;
  }
  int index=0;
  for(auto nextpair=actor_list.begin(); nextpair!=actor_list.end();++nextpair) //print result to file
  {
    if(yearlist[index]==0) //both actors the same
    {
      out << nextpair->first << " is trivially connected to " << nextpair->second << ", you fool" << endl;
      continue;
    }
    out << nextpair->first << "\t" << nextpair->second << "\t" << yearlist[index] << endl;
    index++;
  }
}
else{ //bfs case

  int yearlist[actor_list.size()];

  for(int i=0;i<actor_list.size();i++)
  {
    yearlist[i]=9999;
  }

  for(int currYear=graph.getFirstYear();currYear<2017;currYear++) //loop over increasing years until done
  {
    cout << "checking year "<<currYear << endl;

    graph.addEdges(currYear-1,currYear); //add edges into graph up to current year
    int index=0;
    for(auto nextpair=actor_list.begin(); nextpair!=actor_list.end();++nextpair) //loop over actor pairs
    {
      if(yearlist[index]!=9999) //skip pair if already determined to be connected
      {
        index++;
        continue;
      }

      actor1=(*nextpair).first;
      actor2=(*nextpair).second;

      if(actor1==actor2) //edge case where both actors the same
      {
        yearlist[index]=0;
        index++;
        continue;
      }

      //breadth first search
      queue<ActorNode*> q;
      ActorNode* node1=graph.actors[actor1];
      ActorNode* node2=graph.actors[actor2];
      q.push(node1);
      unordered_set<ActorNode*> visited;
      bool done=false;

      while(!q.empty()) //breadth first search
      {
        ActorNode* nextNode=q.front();
        q.pop();

        for(auto it=nextNode->beginActors();it!=nextNode->endActors();++it)
        {
          if(*it==node2) //path found
          {
            done=true;
            yearlist[index]=currYear;
            break;
          }

          if(visited.count(*it)==0) //push to queue if not already visited
          {
            visited.emplace(*it);
            q.push(*it);
          }

        }
        if(done)
          break;
      }
      index++;
    }
    bool done=true;
    for(int i=0;i<actor_list.size();i++) //check if all actor pairs have been found to be connected
    {
      if(yearlist[i]==9999){
        done=false;
        break;
      }
    }
    if(done)
      break;
  }
  int index=0;
  for(auto nextpair=actor_list.begin(); nextpair!=actor_list.end();++nextpair) //print result to file
  {
    if(yearlist[index]==0) //both actors the same
    {
      out << nextpair->first << " is trivially connected to " << nextpair->second << ", you fool" << endl;
      continue;
    }
    out << nextpair->first << "\t" << nextpair->second << "\t" << yearlist[index] << endl;
    index++;
  }
}

out.close();

return 0;

}
