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

    class LastDimensionMajorIterator {
      size_t index;
      typename Array<T, Ds...>::LastDimensionMajorIterator current_child_iterator;
    public:
      Array<T, Ds...>* base_ptr;
      LastDimensionMajorIterator() : index(0), base_ptr(nullptr) {} //Default ctor
      // Copy and assignment are implicit
      LastDimensionMajorIterator(size_t index, Array<T, Ds...> *base_ptr) : index(index), base_ptr(base_ptr) {
        if(index==D) current_child_iterator = base_ptr[index].lmend();
        else current_child_iterator = base_ptr[index].lmbegin();
      } //ctor with args inde and pointer
      T &operator*() const {
        return *current_child_iterator;
      }
      void shift_ptr() {
        current_child_iterator.base_ptr = base_ptr[index].array;
        current_child_iterator.shift_ptr();
      }
      bool increment() {
        if(index<D) ++index;
        if(index==D) { //reached end, reset to zero, increment child
          index=0;
          shift_ptr();
          if(current_child_iterator.increment()) {
            return true;
          } else return false;
        } else {
          shift_ptr();
          return false;
        }
      }
      LastDimensionMajorIterator &operator++() {
        increment();
        return *this;
      } // Preincrement
      LastDimensionMajorIterator operator++(int) {
        LastDimensionMajorIterator copy(*this);
        increment();
        return copy;
      } // Postincrement


      friend bool operator==(const LastDimensionMajorIterator &one, const LastDimensionMajorIterator &two) {
        return one.index == two.index && one.current_child_iterator == two.current_child_iterator;
      } //Equality checker. Free standing.
      friend bool operator!=(const LastDimensionMajorIterator &one, const LastDimensionMajorIterator &two) {
        return !(one == two);
      } //Inequality checker. Free standing.

    };


    LastDimensionMajorIterator lmbegin() {
      return LastDimensionMajorIterator(0, array);
    } // Pointing at the first sub-array

    LastDimensionMajorIterator lmend() {
      return LastDimensionMajorIterator(D, array);
    }



  };

  template <typename T, size_t D>
  class Array<T, D>;

  // Exactly one dimension
  template <typename T, size_t D>
  class Array<T, D> {
  private:
    T array[D];
  public:
    template <typename U, size_t... Dims> friend class Array;
    using ValueType = T;
    Array() {
      static_assert(D>0, "Dimension should be greater than zero");
    }
    Array(const Array &second) {
      for(size_t i=0; i<D; i++) {
        array[i] = second.array[i];
      }
    }
    template <typename U> Array(const Array<U, D> &second) {
      for(size_t i=0; i<D; i++) {
        array[i] = second.array[i];
      }
    }
    Array &operator=(const Array &second) {
      if(this == &second) return *this;
      for(size_t i=0; i<D; i++) {
        array[i] = second.array[i];
      }
      return *this;
    }
    template <typename U> Array &operator=(const Array<U, D> &second) {
      for(size_t i=0; i<D; i++) {
        array[i] = second.array[i];
      }
      return *this;
    }

    T& operator[](size_t i) {
      if(i>=D) throw OutOfRange();
      return array[i];
    }

    const T& operator[](size_t i) const {
      if(i>=D) throw OutOfRange();
      return array[i];
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
      return FirstDimensionMajorIterator(0, array);
    } // First position

    FirstDimensionMajorIterator fmend() {
      return FirstDimensionMajorIterator(D, array);
    } // One past the last position


    // 1D LastDimensionMajorIterator will work like any regular iterator
    class LastDimensionMajorIterator {
      size_t index;

    public:
      T* base_ptr;
      LastDimensionMajorIterator() : index(0), base_ptr(nullptr) {} //default ctor
      // Copy ctor and assignment are implicit
      LastDimensionMajorIterator(size_t index, T* base_ptr) : index(index), base_ptr(base_ptr){} //ctor with args index and pointer
      T &operator*() const {
        return base_ptr[index];
      }
      void shift_ptr() {}
      bool increment() { //return true if at end
        if(index<D) ++index;
        if(index==D) return true;
        else return false;
      }
      LastDimensionMajorIterator &operator++() {
        increment();
        return *this;
      } // Preincrement
      LastDimensionMajorIterator operator++(int) {
        LastDimensionMajorIterator copy(*this);
        increment();
        return copy;
      } // Postincrement
      friend bool operator==(const LastDimensionMajorIterator &one, const LastDimensionMajorIterator &two) {
        return one.index == two.index;
      } //Equality checker. Free standing.
      friend bool operator!=(const LastDimensionMajorIterator &one, const LastDimensionMajorIterator &two) {
        return !(one == two);
      } //Inequality checker. Free standing.
    };

    LastDimensionMajorIterator lmbegin() {
      return LastDimensionMajorIterator(0, array);
    } // First position

    LastDimensionMajorIterator lmend() {
      return LastDimensionMajorIterator(D, array);
    } // One past the last position
  };

}
