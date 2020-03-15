/*

 File Name: graph.h
 Author: Yue Yu, razrlele@gmail.com
 Description: Graph

 */

#pragma once

#include <unordered_map>
#include <string>
#include <unordered_set>
#include <stdlib.h>
#include <fstream>

#include "util.h"
#include "type.h"


namespace fastwalk {

  
  class Graph{

  private:
    
    graph_type G;
    edge_set edges;
    node_set nodes;
    std::unordered_map<node_type, node_list> N;

  public:
    Graph();
    ~Graph();
    bool load_edge_list(const std::string &in_f, const std::string &delimiter);
    double get_weight(const node_type &from, const node_type &to, double default_v);
    bool has_edge(const edge_pair &edge);
    node_set get_nodes();
    edge_set get_edges();
    node_list neighbors(const node_type &node);
    std::string graph_info();
  }; //class Graph

} //namespace fastwalk
  
