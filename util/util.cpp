
#include "util.h"

namespace util {

  void Util::hash_combine(size_t &seed, const std::string &str){
    seed ^= std::hash<std::string>{}(str) + 0x9e3779b9 + (seed << 6) + (seed >> 2); 
    return ;
  }
  
  std::string Util::join_str(const std::vector<std::string> &str_list, const std::string &delimiter) {
    std::string ret_s ;
    for(int i = 0; i < str_list.size(); i++){
      if(i) ret_s += delimiter;
      ret_s += str_list[i];
    }
    return ret_s;
  }
  
  void Util::log(const std::string &in_s){
    std::time_t now = std::time(0);
    std::tm *ltm = std::localtime(&now);
    char buf[128];
    strftime(buf, 128, "%Y-%m-%d %H:%M:%S", ltm);
    std::cout 
      << "[" << buf << "] "
      << in_s 
      << std::endl;
  }
  
  std::vector<std::string> Util::split(const std::string &in_s, const std::string &delimiter) {
    std::vector<std::string> res;
    size_t last = 0;
    size_t idx = in_s.find_first_of(delimiter, last);
    while(idx != std::string::npos){
      res.push_back(in_s.substr(last, idx-last));
      last = idx + 1;
      idx = in_s.find_first_of(delimiter, last);
    }
    if(idx - last > 0){
      res.push_back(in_s.substr(last, idx-last));
    }
    return res;
  }

  
} //namespace fastwalk
