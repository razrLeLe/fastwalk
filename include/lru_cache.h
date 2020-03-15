/*

 File Name: lru_cache.h
 Author: Yue Yu, razrlele@gmail.com
 Description: A thread-safe LRU

 */

#pragma once

#include <unordered_map>
#include <mutex>
#include <condition_variable>

#include "util.h"

#include <iostream>

namespace fastwalk{
  template<typename KT, typename VT>
  class LRUCache
  {

  private:
    struct Node{
      KT key;
      VT value;
      Node* prev;
      Node* next;
      Node(){
        prev = NULL;
        next = NULL;
      }
    };
  private:
    mutable std::mutex mut;
    mutable std::condition_variable data_cond;
    
  private:
    std::unordered_map<KT, Node*, util::graph_hash>  ht;
    Node* head;
    Node* tail;
    int capacity;
    int count;

  public:
    ~LRUCache(){
      delete head;
      delete tail;
    }
    
    LRUCache(){}

    void init(int capacity){
      this->capacity = capacity;
      this->count = 0;
      head = new Node();
      tail = new Node();
      head->next = tail;
      tail->prev = head;
    }
    bool get(KT key, VT &res){
      std::lock_guard<std::mutex> lk(mut);

      if(ht.find(key) == ht.end()){
        return false;
      } else {
        Node* node = ht[key];
        detach_node(node);
        insert_to_front(node);
        res = node->value;
        return true;
      }
    }
    void set(KT key, VT value){
      std::lock_guard<std::mutex> lk(mut);

      if(ht.find(key) == ht.end()){
        Node* node = new Node();
        if(count == capacity){
          remove_node();
        }
        node->key = key;
        node->value = value;
        ht[key] = node;
        insert_to_front(node);
        ++count;
      } else {
        Node* node = ht[key];
        detach_node(node);
        node->value = value;
        insert_to_front(node);
      }
      data_cond.notify_one();
    }
    
    int get_count(){
      return count;
    }

  private:
    void remove_node(){
      Node* node = tail->prev;
      detach_node(node);
      ht.erase(node->key);
      delete node;
      --count;
    }
    void detach_node(Node* node){
      node->prev->next = node->next;
      node->next->prev = node->prev;
    }
    void insert_to_front(Node* node){
      node->next = head->next;
      node->prev = head;
      head->next = node;
      node->next->prev = node;
    }

  }; // LRUCache
  
}// namespace fastwalk
