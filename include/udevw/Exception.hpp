#ifndef UDEVW_EXCEPTION_HPP
#define UDEVW_EXCEPTION_HPP

#include <stdexcept>

namespace udevw {

  class Exception : public std::exception {
  public:
    Exception(int error)
      : _error(error)
    {
    }

    int get_error() const {
      return _error;
    }

  private:
    int _error;
  };

}

#endif //UDEVW_EXCEPTION_HPP
