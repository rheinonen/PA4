/* CSE 100: PA4
 * DisjointSet.h
 * Author: Robin Heinonen
 * Date:   10/6/2017
 * Description: disjoint set data structure (header)
 */

#ifndef DISJOINTSET_H
#define DISJOINTSET_H

#include<string>
#include<vector>
#include<unordered_map>

class DisjointSetNode
{
public:
  DisjointSetNode* parent;
  std::string actor;
  int rank;
  DisjointSetNode(std::string a):actor(a),parent(this),rank(0){}
};

class DisjointSet{
  private:
    std::unordered_map<std::string,DisjointSetNode*> nodes;
    std::vector<std::string> names;

    /* given a pointer to a node, returns pointer to representative node of the
    * parent set
    */
    DisjointSetNode* find(DisjointSetNode* actor);

  public:
    /* constructor for the disjoint set. creates a node for each actor */
    DisjointSet(const std::vector<std::string> & actors);

    /* class destructor */
    ~DisjointSet();

    /* given two actors connected by a common film,
    *  performs a union_by_rank on their parent sets (attaching the shorter set to the root
    * of the taller set)
    */
    void insertConnection(std::string node1, std::string node2);

    /* given two actors, determines if they are in the same set
    */
    bool connected(std::string node1,std::string node2);

};

#endif //DISJOINTSET_H
