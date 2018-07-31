//  Copyright © 2018 Guobo Chen. All rights reserved.

#include "util/program_options.h"

namespace po = boost::program_options;

namespace gplus {

const char* kArgv0;

po::variables_map& GetSpecifiedOptions() {
  static po::variables_map* const args = new po::variables_map;
  return *args;
}

}  // namespace gplus
