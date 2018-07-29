//  Copyright © 2018 Guobo Chen. All rights reserved.

#ifndef GPLUS_UTIL_EXECUTABLE_PATH_H_
#define GPLUS_UTIL_EXECUTABLE_PATH_H_

#include <string>

namespace gplus {

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
std::wstring GetExecutablePath();
#else
std::string GetExecutablePath();
#endif

}

#endif  // GPLUS_UTIL_EXECUTABLE_PATH_H_
