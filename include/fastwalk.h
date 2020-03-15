/*

 File Name: fastwalk.h
 Author: Yue Yu, razrlele@gmail.com
 Description: Implementation of node2vec randomwalk

 */

#pragma once


#include <string>
#include <vector>

#include "graph.h"
#include "alias.h"
#include "lru_cache.h"
#include "thread_queue.h"

namespace fastwalk {

  class FastWalk
  {
  private:
    Graph node_graph;
    int workers;
    int num_walks;
    int walk_length;
    double p;
    double q;

    LRUCache<std::string, AliasTable> alias_nodes;
    LRUCache<std::pair<std::string, std::string>, AliasTable> alias_edges;

  public:
    FastWalk();
    ~FastWalk();
    void init(Graph &graph,
	     int workers,
	     int num_walks,
	     int walk_length,
	     double p,
	     double q,
       int max_nodes,
       int max_edges);
    std::vector<std::string> walk(std::string &node);
    AliasTable cal_alias_edges(const std::string &t, const std::string &f);
    AliasTable cal_alias_nodes(const std::string &node);
    AliasTable get_alias_nodes(const std::string &node);
    AliasTable get_alias_edges(const std::pair<std::string, std::string> edge);
    
  };// FastWalk

}// namespace fastwalk

