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
    template <typename U, size_t... Dims> friend class Array;
    using ValueType = T;
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

    class FirstDimensionMajorIterator {
      Array<T, Ds...>* base_ptr;
      size_t index;
      typename Array<T, Ds...>::FirstDimensionMajorIterator current_child_iterator;
    public:
      FirstDimensionMajorIterator() : base_ptr(nullptr) , index(0){} //Default ctor
      // Copy and assignment are implicit
      FirstDimensionMajorIterator(size_t index, Array<T, Ds...> *base_ptr) : base_ptr(base_ptr), index(index) {
        current_child_iterator = base_ptr[index].fmbegin();
      } //ctor with args inde and pointer
      T &operator*() {
        return *current_child_iterator;
      }
      const T &operator*() const {
        return *current_child_iterator;
      }
      FirstDimensionMajorIterator &operator++() {
        ++current_child_iterator;
        if(current_child_iterator == base_ptr[index].fmend()) { //child_iterator has reached its end, increment this iterator and reset child to fmbegin
          index++;
          current_child_iterator = base_ptr[index].fmbegin();
        }
        return *this;
      } // Preincrement
      FirstDimensionMajorIterator operator++(int) {
        ++current_child_iterator;
        FirstDimensionMajorIterator copy(*this);
        if(current_child_iterator == base_ptr[index].fmend()) { //child_iterator has reached its end, increment this iterator and reset child to fmbegin
          index++;
          current_child_iterator = base_ptr[index].fmbegin();
        }
        return copy;
      } // Postincrement


      friend bool operator==(const FirstDimensionMajorIterator &one, const FirstDimensionMajorIterator &two) {
        return one.index == two.index && one.current_child_iterator == two.current_child_iterator;
      } //Equality checker. Free standing.
      friend bool operator!=(const FirstDimensionMajorIterator &one, const FirstDimensionMajorIterator &two) {
        return !(one == two);
      } //Inequality checker. Free standing.

    };

    FirstDimensionMajorIterator fmbegin() {
      return FirstDimensionMajorIterator(0, array);
    } // Pointing at the first sub-array

    FirstDimensionMajorIterator fmend() {
      return FirstDimensionMajorIterator(D, array);
    }

    // TODO LDM
    // Commented for cleanliness
    // class LastDimensionMajorIterator {
    //   Array<T, Ds...>* current_ptr;
    //   size_t index;
    //   typename Array<T, Ds...>::LastDimensionMajorIterator current_child_iterator;
    // public:
    //   LastDimensionMajorIterator() : current_ptr(nullptr), index(0) {} //Default ctor
    //   // Copy and assignment are implicit
    //   LastDimensionMajorIterator(size_t index, Array<T, Ds...> *current_ptr) : current_ptr(current_ptr), index(index) {
    //     current_child_iterator = current_ptr->lmbegin();
    //   } //ctor with args inde and pointer
    //   T &operator*() const {
    //     return *current_child_iterator;
    //   }
    //   LastDimensionMajorIterator &operator++() {
    //     //TODO LDM Iteration
    //     return *this;
    //   } // Preincrement
    //   LastDimensionMajorIterator operator++(int) {
    //     LastDimensionMajorIterator copy(*this);
    //     //TODO LDM Iteration
    //     return copy;
    //   } // Postincrement
    //
    //
    //   friend bool operator==(const LastDimensionMajorIterator &one, const LastDimensionMajorIterator &two) {
    //     return one.current_ptr == two.current_ptr && one.index == two.index && one.current_child_iterator == two.current_child_iterator;
    //   } //Equality checker. Free standing.
    //   friend bool operator!=(const LastDimensionMajorIterator &one, const LastDimensionMajorIterator &two) {
    //     return !(one == two);
    //   } //Inequality checker. Free standing.
    //
    // };
    //
    //
    // LastDimensionMajorIterator lmbegin() {
    //   return LastDimensionMajorIterator(0, array);
    // } // Pointing at the first sub-array
    //
    // LastDimensionMajorIterator lmend() {
    //   return LastDimensionMajorIterator(D, array + D);
    // }



  };

  template <typename T, size_t D>
  class Array<T, D>;

  // Exactly one dimension
  template <typename T, size_t D>
  class Array<T, D> {
  private:
    T elements[D];
  public:
    template <typename U, size_t... Dims> friend class Array;
    using ValueType = T;
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
      return *this;
    }
    template <typename U> Array &operator=(const Array<U, D> &second) {
      for(size_t i=0; i<D; i++) {
        elements[i] = second.elements[i];
      }
      return *this;
    }

    T& operator[](size_t i) {
      if(i>=D) throw OutOfRange();
      return elements[i];
    }

    const T& operator[](size_t i) const {
      if(i>=D) throw OutOfRange();
      return elements[i];
    }

    class FirstDimensionMajorIterator {
      T* base_ptr;
      size_t index;

    public:
      FirstDimensionMajorIterator() : base_ptr(nullptr), index(0){} //default ctor
      // Copy ctor and assignment are implicit
      FirstDimensionMajorIterator(size_t index, T* base_ptr) : base_ptr(base_ptr), index(index) {} //ctor with args index and pointer
      T &operator*() const {
        return base_ptr[index];
      }
      FirstDimensionMajorIterator &operator++() {
        ++index;
        return *this;
      } // Preincrement
      FirstDimensionMajorIterator operator++(int) {
        FirstDimensionMajorIterator copy(*this);
        ++index;
        return copy;
      } // Postincrement
      friend bool operator==(const FirstDimensionMajorIterator &one, const FirstDimensionMajorIterator &two) {
        return one.index == two.index;
      } //Equality checker. Free standing.
      friend bool operator!=(const FirstDimensionMajorIterator &one, const FirstDimensionMajorIterator &two) {
        return !(one == two);
      } //Inequality checker. Free standing.
    };

    FirstDimensionMajorIterator fmbegin() {
      return FirstDimensionMajorIterator(0, elements);
    } // First position

    FirstDimensionMajorIterator fmend() {
      return FirstDimensionMajorIterator(D, elements);
    } // One past the last position


    // TODO uncomment
    // Commented for cleanliness
    // 1D LastDimensionMajorIterator will work like any regular iterator
    // class LastDimensionMajorIterator {
    //   T* current_ptr;
    //   size_t index;
    //
    // public:
    //   LastDimensionMajorIterator() : current_ptr(nullptr), index(0) {} //default ctor
    //   // Copy ctor and assignment are implicit
    //   LastDimensionMajorIterator(size_t index, T* current_ptr) : current_ptr(current_ptr), index(index) {} //ctor with args index and pointer
    //   T &operator*() const {
    //     return *(current_ptr);
    //   }
    //   LastDimensionMajorIterator &operator++() {
    //     index++;
    //     current_ptr++;
    //     return *this;
    //   } // Preincrement
    //   LastDimensionMajorIterator operator++(int) {
    //     LastDimensionMajorIterator copy(*this);
    //     index++;
    //     current_ptr++;
    //     return copy;
    //   } // Postincrement
    //   friend bool operator==(const LastDimensionMajorIterator &one, const LastDimensionMajorIterator &two) {
    //     return one.current_ptr == two.current_ptr && one.index == two.index;
    //   } //Equality checker. Free standing.
    //   friend bool operator!=(const LastDimensionMajorIterator &one, const LastDimensionMajorIterator &two) {
    //     return !(one == two);
    //   } //Inequality checker. Free standing.
    // };
    //
    // LastDimensionMajorIterator lmbegin() {
    //   return LastDimensionMajorIterator(0, (T*) elements);
    // } // First position
    //
    // LastDimensionMajorIterator lmend() {
    //   return LastDimensionMajorIterator(D, ((T*) elements) + D);
    // } // One past the last position
  };

}
