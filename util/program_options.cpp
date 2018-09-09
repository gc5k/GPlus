//  Copyright © 2018 Guobo Chen. All rights reserved.

#include "util/program_options.h"

namespace po = boost::program_options;

namespace gplus {

const char* kArgv0;

po::variables_map* prog_args() {
  static po::variables_map* const ret = new po::variables_map;
  return ret;
}

}  // namespace gplus
