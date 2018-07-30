//  Copyright © 2018 Guobo Chen. All rights reserved.

#ifndef GPLUS_BASE_EXCEPTION_H_
#define GPLUS_BASE_EXCEPTION_H_

#include <string>
#include <sstream>

namespace gplus {

class Exception {
 public:
  explicit Exception(const std::string& msg) : msg_(msg) {}
  const std::string& GetExceptionMessage() const { return msg_; }

 protected:
  std::string msg_;
};

class MissingProgramOptionException : public Exception {
 public:
  explicit MissingProgramOptionException(const std::string& prog_opt)
      : Exception("Missing --" + prog_opt + " option.") {}
};

}  // namespace gplus

#endif  // GPLUS_BASE_EXCEPTION_H_
