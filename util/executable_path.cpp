//
//  executable_path.cpp
//  gplus
//
//  Created by Zhixiang Zhu on 21/07/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//
//  This file implement the function GetExecutablePath() which returns the path
//  of the gplus executable file. The implementation is mainly copied from
//  https://github.com/mirror/boost/blob/master/libs/log/src/process_name.cpp

#include <climits>  // PATH_MAX
#include <string>
#include <boost/filesystem/path.hpp>

using namespace std;

namespace gplus {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <Windows.h>

wstring GetExecutablePath() {
  wstring buf;
  buf.resize(MAX_PATH);
  do {
    unsigned int len = GetModuleFileNameW(
        NULL, &buf[0], static_cast<unsigned int>(buf.size()));
    if (len < buf.size()) {
      buf.resize(len);
      break;
    }

    buf.resize(buf.size() * 2);
  } while (buf.size() < 65536);

  return boost::filesystem::path(buf).wstring();
}
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#include <mach-o/dyld.h>

string GetExecutablePath() {
  string buf;
  buf.resize(PATH_MAX);
  while (true) {
    uint32_t size = static_cast<uint32_t>(buf.size());
    if (_NSGetExecutablePath(&buf[0], &size) == 0) {
      buf.resize(strlen(&buf[0]));
      break;
    }
    buf.resize(size);
  }
  return boost::filesystem::path(buf).string();
}
#else  // Linux
// TODO(Zhixiang Zhu): Linux
#endif
}  // namespace gplus
