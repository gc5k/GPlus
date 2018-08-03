//  Copyright © 2018 Guobo Chen. All rights reserved.

#ifndef GPLUS_BASE_EXCEPTION_H_
#define GPLUS_BASE_EXCEPTION_H_

#include <exception>
#include <string>
#include <sstream>
#include "third_party/boost/exception/all.hpp"

namespace gplus {

// Define short descriptions of all the exceptions
#define EXCEPTION_MISSING_OPTION "missing option"
#define EXCEPTION_FILE_NOT_FOUND "file not found"

// Define detailed error info related to exceptions
typedef boost::error_info<struct TagErrorOption, const char*> ErrorOption;
typedef boost::error_info<struct TagErrorFileName, std::string> ErrorFileName;

class Exception
    : virtual public std::exception, virtual public boost::exception {
 public:
  explicit Exception(const char* what) : what_(what) {}
  const char* what() const noexcept override { return what_; }

 private:
  // Don't embed a std::string object in an exception object.
  // Thus a plain pointer is used instead.
  // https://www.boost.org/community/error_handling.html
  const char* what_;
};

const char* GetFormattedExceptionMessage(const Exception& e);

}  // namespace gplus

#endif  // GPLUS_BASE_EXCEPTION_H_
