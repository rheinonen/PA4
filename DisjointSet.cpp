/* CSE 100: PA4
 * DisjointSet.h
 * Author: Robin Heinonen
 * Date:   10/6/2017
 * Description: disjoint set data structure (implementation)
 */

#include "DisjointSet.h"
#include <string>
#include <unordered_map>
#include <vector>

DisjointSet::DisjointSet(const std::vector<std::string>& actors){
  names=actors;
  for(auto it=names.begin();it!=names.end();++it)
    nodes.emplace(*it,new DisjointSetNode(*it));
}

DisjointSet::~DisjointSet(){
  for(auto it=names.begin();it!=names.end();++it)
    delete nodes[*it];
}

void DisjointSet::insertConnection(std::string node1, std::string node2)
{
  DisjointSetNode* root1=find(nodes[node1]);
  DisjointSetNode* root2=find(nodes[node2]);

  if(root1==root2)
    return;

  if(root1->rank < root2->rank)
    root1->parent = root2;
  else if(root1->rank > root2->rank)
    root2->parent = root1;
  else
    {
      root2->parent=root1;
      root1->rank+=1;
    }
}

DisjointSetNode* DisjointSet::find(DisjointSetNode* actor){
  if(actor->parent!=actor)
    actor->parent=find(actor->parent); //path compression
  return actor->parent;
}

bool DisjointSet::connected(std::string node1,std::string node2)
{
  DisjointSetNode* root1=find(nodes[node1]);
  DisjointSetNode* root2=find(nodes[node2]);
  return root1==root2;
}
