#include<exception>
#include<iostream>
namespace cs540 {
  class OutOfRange : public std::exception {
      public:
          virtual const char* what() const throw() {
              return "Out of valid range";
          }
  };

  template <typename T, size_t... Dims>
  class Array;

  // More than one dimension
  template <typename T, size_t D, size_t... Ds>
  class Array<T, D, Ds...> {
  private:
    // Recursively create array of type T, dimension Ds and size D
    Array<T, Ds...> array[D];
  public:
    Array() {
      static_assert(D > 0, "Dimension must be greater than zero");
    }
    Array(const Array &second) {
      for(size_t i=0; i<D; i++){
        array[i] = second.array[i]; //Assign each nested array
      }
    }
    template <typename U> Array(const Array<U, D, Ds...> &second) {
      for(size_t i=0; i<D; i++){
        array[i] = second.array[i]; //Assign each nested array
      }
    }
    Array &operator=(const Array &second) {
      if(this == &second) return *this;
      for(size_t i=0; i<D; i++) {
        array[i] = second.array[i];
      }
      return *this;
    }
    template <typename U> Array &operator=(const Array<U, D, Ds...> &second) {
      for(size_t i=0; i<D; i++) {
        array[i] = second.array[i];
      }
      return *this;
    }
    Array<T, Ds...>& operator[](size_t i) {
      if(i>=D) throw OutOfRange();
      return array[i];
    }
    const Array<T, Ds...>& operator[](size_t i) const {
      if(i>=D) throw OutOfRange();
      return array[i];
    }

  };

  // Exactly one dimension
  template <typename T, size_t D>
  class Array<T, D> {
  private:
    T elements[D];
  public:
    Array() {
      static_assert(D>0, "Dimension should be greater than zero");
    }
    Array(const Array &second) {
      for(size_t i=0; i<D; i++) {
        elements[i] = second.elements[i];
      }
    }
    template <typename U> Array(const Array<U, D> &second) {
      for(size_t i=0; i<D; i++) {
        elements[i] = second.elements[i];
      }
    }
    Array &operator=(const Array &second) {
      if(this == &second) return *this;
      for(size_t i=0; i<D; i++) {
        elements[i] = second.elements[i];
      }
    }
    template <typename U> Array &operator=(const Array<U, D> &second) {
      for(size_t i=0; i<D; i++) {
        elements[i] = second.elements[i];
      }
    }
    const T& operator[](size_t i) const {
      if(i>=D) throw OutOfRange();
      return elements[i];
    }
  };

}
