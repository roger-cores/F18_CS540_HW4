#include "Array.hpp"
#include<iostream>
#include <assert.h>

int main() {

  // Testing 1-D FDM Iter
  cs540::Array<int, 4> a1;
  for(int i=0; i<4; i++) {
    a1[i] = i;
  }
  auto it = a1.fmbegin();
  for(int i=0; i<4; i++) {++it;}
  assert(it == a1.fmend());

  it = a1.fmbegin();
  for(;it!=a1.fmend();++it) {
    // std::cout<<*it<<std::endl;
  }

  cs540::Array<int, 3, 5> a2;
  int a[3][5];
  int* ap = (int *) a;
  int x = 0;
  for(int i=0; i<3; i++) {
    for(int j=0; j<5; j++) {
      a[i][j] = x;
      a2[i][j] = x;
      x++;
    }
  }
  // Testing access
  for(int i=0; i<3; i++) {
    for(int j=0; j<5; j++) {
      std::cout<<a2[i][j]<<std::endl;
    }
    std::cout<<std::endl;
  }
  std::cout<<std::endl;

  //Testing fdm
  //post incr
  size_t i=0;
  auto it2=a2.fmbegin();
  for(;it2!=a2.fmend();++it2) {
    assert((*it2) == ap[i++]);
  }
  //pre incr
  i=0;
  it2=a2.fmbegin();
  do {
    assert((*it2) == ap[i++]);
  } while(++it2 != a2.fmend());

}
