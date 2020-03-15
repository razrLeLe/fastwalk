/*

 File Name: alias.h
 Author: Yue Yu, razrlele@gmail.com
 Description: Alias sample class 

 */

#pragma once

#include <stdio.h>
#include <random>
#include <vector>
#include <chrono>

namespace fastwalk {

  class AliasTable
  {

  private:
    std::vector<double> accept;
    std::vector<int> alias;
    size_t N;
  public:
    AliasTable();
    ~AliasTable();
    void init(std::vector<double> area_ration);
    int sample();
  }; // AliasTable
  
  
} //namespace fastwalk
