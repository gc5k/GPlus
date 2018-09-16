//  Copyright © 2018 Guobo Chen. All rights reserved.

#include "util/program_options.h"

namespace po = boost::program_options;

using std::string;

namespace gplus {

const char* kArgv0;

po::variables_map* prog_args() {
  static po::variables_map* const ret = new po::variables_map;
  return ret;
}
  
void AddBedOption(OptionsDesc* opts_desc) {
  opts_desc->add_options()
  ("bfile,b", po::value<string>(),
   "filename (no ext) of the plink format input data");
}
void AddOutOption(OptionsDesc* opts_desc) {
  opts_desc->add_options()
  ("out,o", po::value<string>()->default_value("gplus"),
   "filename (no ext) of the output files");
}

}  // namespace gplus
