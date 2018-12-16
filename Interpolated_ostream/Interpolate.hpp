#ifndef CS540_INTERPOLATE_HPP
#define CS540_INTERPOLATE_HPP

#include <iostream>
#include <iomanip>
#include <exception>
#include <string>
#include <tuple>
#include <utility>
#include <type_traits>

namespace cs540 {

  //aliases for convinience
  using String = std::string;
  using StringIter = std::string::const_iterator;
  using Ostream = std::ostream;
  using IosBase = std::ios_base;
  using Streamsize = std::streamsize;

  class WrongNumberOfArgs : public std::exception {
      public:
          virtual const char* what() const throw() {
              return "Number of arguments and number of input chars dont match";
          }
  };

  constexpr auto ffr(Ostream &(*fptr)(Ostream &)) {
      return fptr;
  }

  constexpr auto ffr(Ostream &(*fptr)(IosBase &)) {
      return fptr;
  }

  template <typename... ARGS>
  class Interpolate {
      private:
          String input_str;
          std::tuple<const ARGS &...> arguments;

      public:
          //constructor returns the created object that can be used with << operator
          Interpolate(const String & ip_str, const ARGS &... args) : input_str{ip_str}, arguments{args...} {}

          friend Ostream& operator<<(Ostream &os, const Interpolate<ARGS...> &interpolate) {
              return interpolate.print(os);
          }

          Ostream& print(Ostream &os) const {
              return format<0>(os, arguments, input_str.cbegin());
          }

          template <std::size_t index, typename ARG>
          Ostream& format(Ostream &os, const ARG &arg, StringIter string_iter) const {
              while (string_iter != input_str.cend()){
                  if (*string_iter == '\\' && (string_iter + 1) != input_str.cend()){ //if backslash
                      if (*(string_iter + 1) == '%'){//if next char is a percent
                          string_iter += 2;
                          os << '%';//output percent
                          continue;
                      }
                  }
                  if (*string_iter == '%'){//if percent
                      if constexpr (index < std::tuple_size_v<ARG>){
                          if (post_output(os, std::get<index>(arg))){//post with arg
                              return format<index + 1>(os, arg, string_iter);
                          }
                          return format<index + 1>(os, arg, ++string_iter);
                      } else {
                          throw WrongNumberOfArgs();
                      }
                  }
                  os << *string_iter;
                  string_iter++;
              }

              if constexpr (index != std::tuple_size_v<ARG>){
                  if (post_output(os, std::get<index>(arg))) return format<index+1>(os, arg, string_iter);
                  throw WrongNumberOfArgs();
              }
              return os;
          }

          bool post_output(Ostream &os, IosBase &(*io_manip)(IosBase &)) const {
              os << io_manip;
              return true;
          }

          template <typename ARG>
          bool post_output(Ostream &os, const ARG &arg) const {
              IosBase::fmtflags b4_flags = os.flags();
              char b4_fill = os.fill();
              Streamsize b4_prec = os.precision();
              Streamsize b4_width = os.width();
              os << arg;
              if ((b4_flags != os.flags()) || (b4_fill != os.fill()) || (b4_prec != os.precision()) || (os.width() != 0 && b4_width != os.width())) return true;
              return false;
          }

          bool post_output(Ostream &os, Ostream &(*output_op)(Ostream &)) const {
              os << output_op;
              if (output_op == ffr(std::flush)) return true;
              return false;
          }

  };
}

#endif
