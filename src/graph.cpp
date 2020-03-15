
#include "graph.h"

namespace fastwalk {
  Graph::Graph()
  {}

  Graph::~Graph()
  {}

  node_set Graph::get_nodes()
  {
    return nodes;
  }

  edge_set Graph::get_edges()
  {
    return edges;
  }
  
  std::string Graph::graph_info(){
    int node_num = nodes.size();
    int edge_num = edges.size();
    return std::string("Graph node num: " + std::to_string(node_num)
        +" edge num: " + std::to_string(edge_num));
  }
  
  bool Graph::load_edge_list(const std::string &in_f, const std::string &delimiter)
  {
    std::ifstream edge_file(in_f.c_str());
    if(!edge_file){
      return false;
    }
    std::vector<std::string> lines;
    std::string line;
    while(getline(edge_file, line)){
      lines.push_back(line);
    }
    LOG("Done reading edgelist files, starting to build graph...");
    for(auto &each_line: lines){
      std::vector<std::string> vec = util::Util::split(each_line, delimiter);
      if(vec.size() < 2){
        return false;
      }
      node_type from, to;
      from = vec[0];
      to = vec[1];
      double wei = 1.0;
      if (vec.size() == 3){
        wei = atof(vec[2].c_str());
      }
      G[from][to] = wei;
      nodes.insert(from);
      nodes.insert(to);
      edges.insert(std::make_pair(from, to));
    }
    for(auto &node: G){
      node_list nbrs;
      for(auto &x: G[node.first]){
        nbrs.push_back(x.first) ;
      }
      N.insert(std::make_pair(node.first, nbrs));
    }
    LOG("Done building graph.");
    return true;
  }

  double Graph::get_weight(const node_type &from, const node_type &to, double default_v=1.0)
  {
    if(G.find(from) != G.end() and G[from].find(to) != G[from].end()){
      return G[from][to];
    }
    return default_v;
  }

  bool Graph::has_edge(const edge_pair &edge)
  {
    return edges.find(edge) != edges.end();
  }

  node_list Graph::neighbors(const node_type &node)
  {
    node_list nbrs;
    if(N.find(node) != N.end()){
      nbrs = N[node];
    }
    return nbrs;
  }

} // namespace fastwalk
