/*

 File Name: thread_queue.h
 Author: Yue Yu, razrlele@gmail.com
 Description: A thread-safe queue

 */

#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <initializer_list>

namespace fastwalk{
  template<typename T>
  class ThreadQueue{
   private:
    mutable std::mutex mut;
    mutable std::condition_variable data_cond;
    using queue_type = std::queue<T>;
    queue_type data_queue;

   public:
    using value_type = typename queue_type::value_type;
    using container_type = typename queue_type::container_type;

    ThreadQueue()=default;
    ThreadQueue(const ThreadQueue&)=delete;
    ThreadQueue& operator=(const ThreadQueue&)=delete;

    template<typename _InputIterator>
      ThreadQueue(_InputIterator first, _InputIterator last){
      for(auto itor=first; itor != last; ++itor){
        data_queue.push(*itor);
      }
    }

    explicit ThreadQueue(const container_type &c):data_queue(c){}

    ThreadQueue(std::initializer_list<value_type> list):ThreadQueue(list.begin(), list.end()){}

    void push(const value_type &new_value){
      std::lock_guard<std::mutex> lk(mut);
      data_queue.push(std::move(new_value));
      data_cond.notify_one();
    }

    value_type wait_and_pop(){
      std::unique_lock<std::mutex> lk(mut);
      data_cond.wait(lk, [this]{return !this->data_queue.empty();});
      auto value = std::move(data_queue.front());
      data_queue.pop();
      return value;
    }

    bool try_pop(value_type& value) {
      std::lock_guard<std::mutex> lk(mut);
      if(data_queue.empty())
        return false;
      value = std::move(data_queue.front());
      data_queue.pop();
      return true;
    }

    auto empty() const->decltype(data_queue.empty()) {
      std::lock_guard<std::mutex> lk(mut);
      return data_queue.empty();
    }

    auto size() const->decltype(data_queue.size()){
      std::lock_guard<std::mutex> lk(mut);
      return data_queue.size();
    }

  }; // ThreadQueue
  
} //namespace fastwalk
