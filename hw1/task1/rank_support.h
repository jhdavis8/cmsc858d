// -*- mode: cpp -*-
#ifndef RANK_SUPPORT_H
#define RANK_SUPPORT_H

#include <iostream>
#include <string>
#include <cmath>

class bit_vector_ref {
  uint64_t* loc;
  uint64_t offset;

  bit_vector_ref(uint64_t* l, uint64_t o) {
    loc = l;
    offset = o;
  }

  void operator=(bool b) {
    b ? loc* |= (1 << offset) : loc* &= ~(1 << offset);
  }

  operator bool() const {
    return loc* & (1 << offset) == 1 << offset;
  }
}

class bit_vector {
  uint64_t* vec;
  uint64_t size;

 public:
  bit_vector(std::size_t len) {
    size = len;
    vec = new uint64_t[(len / 64) + 1]();
  }

  ~bit_vector() {
    delete[] vec;
  }
  
  bit_vector(std::string& state) : bit_vector(state.size()) {
    for (int i = 0; i < state.size(); i++) {
      stoi(state[i]) ? this[i] = 1 : (void)0;
      // this[i] = stoi(state[i]);
    }
  }
  
  bit_vector_ref operator[](std::size_t i) {
    return bit_vector_ref(&vec[i / 64], i % 64);
  }
}

class rank_support {
 public:
  rank_support(bit_vector& b);
  uint64_t rank1(uint64_t i);
  uint64_t operator()(uint64_t i);
  uint64_t overhead();
  void save(std::string& fname);
  void load(std::string& fname);
}

#endif
