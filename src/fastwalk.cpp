
#include "fastwalk.h"

namespace fastwalk {
  
  FastWalk::FastWalk()
  {
  }

  void FastWalk::init(Graph &graph,
      int workers,
      int num_walks,
      int walk_length,
      double p,
      double q,
      int max_nodes,
      int max_edges)
  {
    this->node_graph = graph;
    this->workers = workers;
    this->num_walks = num_walks;
    this->walk_length = walk_length;
    this->p = p;
    this->q = q;
    alias_nodes.init(max_nodes);
    alias_edges.init(max_edges);
  }

  FastWalk::~FastWalk()
  {
  }

  std::vector<std::string> FastWalk::walk(std::string &node)
  {
    std::vector<std::string> path;
    path.push_back(node);

    while(path.size() < walk_length){
      std::string cur = path.back();
      std::vector<std::string> cur_nbrs = node_graph.neighbors(cur);
      if(cur_nbrs.size() > 0) {
        if(path.size() == 1){
          AliasTable a_nodes = get_alias_nodes(cur);
          int sample_idx = a_nodes.sample();
          path.push_back(cur_nbrs[sample_idx]);
        } else{
          std::string prev = path[path.size()-2];
          AliasTable a_edges = get_alias_edges(std::make_pair(prev, cur));
          int sample_idx = a_edges.sample();
          path.push_back(cur_nbrs[sample_idx]);
        }
      } else {
        break;
      }//if 
    }//while
    return path;
  }


  AliasTable FastWalk::cal_alias_edges(const std::string &t, const std::string &v)
  {
    std::vector<double> unnormed_probs;
    
    for(auto &x: node_graph.neighbors(v)){
      
      double wei = node_graph.get_weight(v, x, 1.0);
      if(x == t) {
        unnormed_probs.push_back(wei/p);
      } else if(node_graph.has_edge(std::make_pair(x, t))){
        unnormed_probs.push_back(wei);
      } else {
        unnormed_probs.push_back(wei/q);
      }
    }
    double norm_const = 0;
    for(auto &x : unnormed_probs){
      norm_const += x;
    }

    std::vector<double> normed_probs;
    for(auto &x : unnormed_probs){
      normed_probs.push_back(x/norm_const);
    }
    AliasTable alias_table;
    alias_table.init(normed_probs);
    return alias_table;
  }
  

  AliasTable FastWalk::cal_alias_nodes(const std::string &node)
  {
    std::vector<double> unnormed_probs;
    for(auto &x: node_graph.neighbors(node)){
      double wei = node_graph.get_weight(node, x, 1.0);
      unnormed_probs.push_back(wei);
    }
    double norm_const = 0;
    for(auto &x : unnormed_probs){
      norm_const += x;
    }

    std::vector<double> normed_probs;
    for(auto &x : unnormed_probs){
      normed_probs.push_back(x/norm_const);
    }
    AliasTable alias_table;
    alias_table.init(normed_probs);
    return alias_table;
  }

  AliasTable FastWalk::get_alias_nodes(const std::string &node)
  {
    AliasTable res;
    if(!alias_nodes.get(node, res)){
      res = cal_alias_nodes(node);
      alias_nodes.set(node, res);
    }
    return res;
  }

  AliasTable FastWalk::get_alias_edges(const std::pair<std::string, std::string> edge)
  {
    AliasTable res;
    if(!alias_edges.get(edge, res)){
      res = cal_alias_edges(edge.first, edge.second);
      alias_edges.set(edge, res);
    }
    return res;
  }
} // namespace fastwalk
