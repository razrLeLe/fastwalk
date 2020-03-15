/*

 File Name: util.h
 Author: Yue Yu, razrlele@gmail.com
 Description: Util functions

 */

#pragma once

#include <vector>
#include <string>
#include <functional>
#include <iostream>
#include <ctime>

namespace util {
  class Util{
  public:
    static void hash_combine(size_t &seed, const std::string &str);
    static std::string join_str(const std::vector<std::string> &str_list, const std::string &delimiter);
    static std::vector<std::string> split(const std::string &in_s, const std::string &delimiter) ;
    static void log(const std::string &in_s) ;
  };

  struct graph_hash {
    std::size_t operator()(const std::pair<std::string, std::string> &str_pair) const {
      std::size_t seed = 0;
      Util::hash_combine(seed, str_pair.first);
      Util::hash_combine(seed, str_pair.second);
      return seed;
    }
    std::size_t operator()(const std::string &in_str) const {
      std::size_t seed = 0;
      Util::hash_combine(seed, in_str);
      return seed;
    }
  };

}
