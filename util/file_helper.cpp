//
//  file_helper.cpp
//  gplus
//
//  Created by Zhixiang Zhu on 03/08/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#include "util/file_helper.h"

#include <cstdlib>
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

}  // namespace gplus
