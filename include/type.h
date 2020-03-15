/*

 File Name: type.h
 Author: Yue Yu, razrlele@gmail.com
 Description: Definition of value type

 */

#pragma once
#include "util.h"

#define  LOG util::Util::log

namespace fastwalk{
  using edge_pair = std::pair<std::string, std::string>;
  using node_type = std::string;
  using edge_set = std::unordered_set<edge_pair, util::graph_hash>;
  using node_set = std::unordered_set<node_type>;
  using node_list = std::vector<node_type>;
  using graph_type = std::unordered_map<node_type, std::unordered_map<node_type, double> >;
} //namespace fastwalk
