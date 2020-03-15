
#include <thread>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#include "fastwalk.h"
#include "graph.h"
#include "type.h"
#include "util.h"

struct opt {
  int workers;
  int num_walks;
  int walk_length;
  int max_nodes;
  int max_edges;
  double p;
  double q;
  std::string edge_list;
  std::string output_f;
  std::string delimiter;
  opt(){
    workers=1;
    num_walks=10;
    walk_length=80;
    max_nodes=10000;
    max_edges=10000;
    p=1.0;
    q=1.0;
    edge_list = "";
    output_f = "";
    delimiter = "\t";
  }
};

static struct option long_options[] =
  {
    { "workers", required_argument, NULL, 'w'},
    { "num_walks", required_argument, NULL, 'n'},
    { "walk_length", required_argument, NULL, 'l'},
    { "p", required_argument, NULL, 'p'},
    { "q", required_argument, NULL, 'q'},
    { "edge_list", required_argument, NULL, 'e'},
    { "max_nodes", required_argument, NULL, 'm'},
    { "max_edges", required_argument, NULL, 'g'},
    { "output", required_argument, NULL, 'o'},
    { "delimiter", required_argument, NULL, 'd'},
    { "help", no_argument, NULL, 'h'}
  };

void print_help()
{
  std::cout << std::string(
      "Usage:\n"
      "  --workers: Number of threads to executing walking, 1 as default.\n"
      "  --num_walks: Number of rounds to walk from all nodes, 10 as default.\n"
      "  --walk_length: Length of walk per source, 10 as default.\n" 
      "  --p: Return hyperparameter, 1.0 as default.\n"
      "  --q: In-out hyperparameter, 1.0 as default.\n"
      "  --edge_list: Input graph path.\n"
      "  --max_nodes: Maximum number of nodes stored in pre-computed alias table, 10000 as default.\n"
      "  --max_edges: Maximum number of nodes stored in pre-computed alias table, 10000 as default.\n"
      "  --delimiter: Delimiter of input and output data, tab or space, tab as default.\n"
      "  --output: Walk result output path.\n"
      " \nSample:\n"
      "   ./fastwalk --edge_list <path_to_edgelist> --output <path_to_output> --delimiter space --p 10 --q 0.01\n"
      );
  return ;
}

void init_args(int argc, char *argv[], opt &walk_opt)
{
  int index = 0, opt = 0;
  char tab[] = "tab";
  char space[] = "space";
 
  while((opt = getopt_long_only(argc, argv, "w:n:l:p:q:e:m:g:o:d:h", long_options, &index)) != -1){
    switch(opt) {
    case 'w':
      walk_opt.workers = atoi(optarg);
      break;
    case 'n':
      walk_opt.num_walks = atoi(optarg);
      break;
    case 'l':
      walk_opt.walk_length = atoi(optarg);
      break;
    case 'p':
      walk_opt.p = atof(optarg);
      break;
    case 'q':
      walk_opt.q = atof(optarg);
      break;
    case 'e':
      walk_opt.edge_list = std::string(optarg);
      break;
    case 'd':
      if(strcmp(optarg, space) == 0){
        walk_opt.delimiter = " ";
      } else if (strcmp(optarg, tab) == 0){
        walk_opt.delimiter = "\t";
      } else {
        printf("unsupported delimiter: %s, use tab or space\n", optarg);
        print_help();
        exit(0);
      }
      break;
    case 'm':
      walk_opt.max_nodes = atoi(optarg);
      break;
    case 'g':
      walk_opt.max_edges = atoi(optarg);
      break;
    case 'o':
      walk_opt.output_f = std::string(optarg);
      break;
    case 'h':
      print_help();
      exit(0); 
      break;
    case '?':
      printf("unknown arg: %s\n", optarg);
      print_help();
      exit(0); 
      break;
    default:
      break;
    }
  }
}
void walker(fastwalk::FastWalk &fast_walk,
    fastwalk::ThreadQueue<std::string> &in_data,
    fastwalk::ThreadQueue<std::string> &out_data, 
    const std::string &delimiter)
{
  std::string node;
  while(1){
    if(in_data.try_pop(node)){
      if(node == "DONE"){
        out_data.push("DONE");
        break;
      } else {
        std::vector<std::string> walk = fast_walk.walk(node);
        std::string s = util::Util::join_str(walk, delimiter);
        out_data.push(s);
      }
    }
    
  }
  return ;
}


void writer(fastwalk::ThreadQueue<std::string> &in_data,
    const std::string &out_f,
    int process_num){
  std::ofstream outfile;
  outfile.open(out_f);
  std::string out_s;
  int count = 0;
  while(1){
    if(in_data.try_pop(out_s)){
      if(out_s == "DONE"){
        count += 1;
        if(count == process_num){
          break;
        }
      } else {
        outfile << out_s << std::endl;
      }
    }
  }
  outfile.close();
  return ;
}


int main(int argc, char* argv[]){
  opt walk_opt;
  init_args(argc, argv, walk_opt);
  fastwalk::Graph graph;
  LOG("Loading file: "+walk_opt.edge_list);
  graph.load_edge_list(walk_opt.edge_list, walk_opt.delimiter);
  LOG(graph.graph_info());
  
  LOG("Graph loaded.");

  fastwalk::FastWalk fast_walk;
  fast_walk.init(graph,
      walk_opt.workers,
      walk_opt.num_walks,
      walk_opt.walk_length,
      walk_opt.p,
      walk_opt.q,
      walk_opt.max_nodes,
      walk_opt.max_edges);
  fastwalk::ThreadQueue<std::string> in_data_q;
  fastwalk::ThreadQueue<std::string> out_data_q;

  std::vector<std::thread> jobs(walk_opt.workers+1);
  jobs[0] = std::thread(writer, std::ref(out_data_q), std::ref(walk_opt.output_f), walk_opt.workers);

  LOG("Starting to walk...");
  for(int i=1; i <= walk_opt.workers; i++){
    jobs[i] = std::thread(walker, std::ref(fast_walk), std::ref(in_data_q), std::ref(out_data_q), walk_opt.delimiter);
  }
  std::unordered_set<std::string> nodes = graph.get_nodes();
  for(int i=0; i < walk_opt.num_walks; i++ ){
    for(auto &node: nodes){
      in_data_q.push(node);
    }
  }
  LOG("Done feeding nodes.");
  for(int i = 0; i < walk_opt.workers; i++){
    in_data_q.push("DONE");
  }
  for(auto &x: jobs){
    x.join();
  }
  LOG("All done.");
  return 0;
}
