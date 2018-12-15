#include<iostream>

namespace cs540 {
  class WrongNumberOfArgs : public std::exception {
  public:
    virtual const char* what() const throw() {
      return "Arguments number does not match number of input characters in input string";
    }
  }
  
  class Interpolate {

  };
}
