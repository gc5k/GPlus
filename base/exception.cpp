//
//  exception.cpp
//  gplus
//
//  Created by Zhixiang Zhu on 03/08/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#include "base/exception.h"

#include <cstring>

namespace gplus {

struct ExceptionFormatter {
  const char* what;
  void (*format_func)(const Exception&);
};

// Don't embed a std::string object in an exception object. Thus a static
// char array is used instead.
// https://www.boost.org/community/error_handling.html
static const int kMsgBufSize = 1024;
static char msg_buf[kMsgBufSize];

static ExceptionFormatter formatters[] = { {
  EXCEPTION_MISSING_OPTION,
  [](const Exception& e) {
    if (const char* const* option = boost::get_error_info<ErrorOption>(e)) {
      snprintf(msg_buf, kMsgBufSize, "Missing option '%s'.", *option);
    }
  }
}, {
  EXCEPTION_FILE_NOT_FOUND,
  [](const Exception& e) {
    if (std::string const* filename = boost::get_error_info<ErrorFileName>(e)) {
      snprintf(msg_buf, kMsgBufSize,
               "Cannot find file '%s'.", filename->c_str());
    }
  }
} };

const char* GetFormattedExceptionMessage(const Exception& e) {
  for (auto formatter: formatters) {
    if (strcmp(formatter.what, e.what()) == 0) {
      formatter.format_func(e);
      return msg_buf;
    }
  }
  return e.what();
}

}  // namespace gplus
