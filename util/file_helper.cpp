//
//  file_helper.cpp
//  gplus
//
//  Created by Zhixiang Zhu on 03/08/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#include "util/file_helper.h"

#include <cstdlib>
#include <string>

#include "third_party/boost/algorithm/string/trim.hpp"
#include "util/log.h"

using std::ifstream;

namespace gplus {

std::shared_ptr<ifstream> OpenTextFile(const char* filename) {
  std::shared_ptr<ifstream> ifs(new ifstream(filename, ifstream::in));
  if (!ifs->good()) {
    GPLUS_LOG << "Cannot open file '" << filename << "'." << std::endl;
    exit(EXIT_FAILURE);
  }
  return ifs;
}

std::ifstream& GetNonEmptyLine(
    std::ifstream* in_strm_ptr, std::string* str_ptr) {
  auto& in_strm = *in_strm_ptr;
  auto& str = *str_ptr;
  while (std::getline(in_strm, str)) {
    boost::trim(str);
    if (!str.empty()) {
      return in_strm;
    }
  }
  return in_strm;
}

}  // namespace gplus
