//  Copyright © 2018 Guobo Chen. All rights reserved.

#ifndef GPLUS_UTIL_PROGRAM_OPTIONS_H_
#define GPLUS_UTIL_PROGRAM_OPTIONS_H_

#include "third_party/boost/program_options/options_description.hpp"
#include "third_party/boost/program_options/positional_options.hpp"
#include "third_party/boost/program_options/variables_map.hpp"

namespace gplus {

extern const char* kArgv0;

namespace program_options = boost::program_options;
typedef program_options::options_description OptionsDesc;
typedef program_options::positional_options_description PositionalOptionsDesc;

program_options::variables_map& GetSpecifiedOptions();

}  // namespace gplus

#endif  // GPLUS_UTIL_PROGRAM_OPTIONS_H_
