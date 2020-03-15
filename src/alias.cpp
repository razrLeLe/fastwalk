
#include "alias.h"

#include <iostream>
namespace fastwalk {
  
  AliasTable::AliasTable(){}

  AliasTable::~AliasTable(){}

  
  void AliasTable::init(std::vector<double> area_ratio){
    size_t l = area_ratio.size();
    std::vector<int> tmp_l(l, 0);
    accept.resize(tmp_l.size());
    alias.resize(tmp_l.size());
    std::vector<double> area_ratio_;
    std::vector<int>small, large;
    
    for(size_t i=0; i < l; i++){
      area_ratio_.push_back(area_ratio[i]*l);
    }
    for(size_t i=0; i < l; i++){
      if(area_ratio_[i] < 1.0){
        small.push_back(i);
      } else {
        large.push_back(i);
      }
    }
    while(!small.empty() && !large.empty()){
      size_t small_idx = small.back();
      size_t large_idx = large.back();
      small.pop_back();
      large.pop_back();
      accept[small_idx] = area_ratio_[small_idx];
      alias[small_idx] = large_idx;
      area_ratio_[large_idx] = area_ratio_[large_idx] - \
        (1 - area_ratio_[small_idx]);
      if(area_ratio_[large_idx] < 1.0){
        small.push_back(large_idx);
      } else {
        large.push_back(large_idx);
      }
    }
    while(!large.empty()){
      size_t large_idx = large.back();
      large.pop_back();
      accept[large_idx] = 1;
    }
    while(!small.empty()){
      size_t small_idx = small.back();
      small.pop_back();
      accept[small_idx] = 1;
    }

    N = accept.size();
    return ;
  }

  int AliasTable::sample(){

    std::random_device rd;
    unsigned seed = rd();
    std::mt19937 mt(seed);
    std::uniform_real_distribution<double> dis(0, 1.0);

    double rand1 = dis(mt);
    double rand2 = dis(mt);
    double r = rand1;
    int64_t i = static_cast<int64_t>(rand2 * N);
    if(r < accept[i]){
      return i;
    } else {
      return alias[i];
    }
  }

} // namespace fastwalk
