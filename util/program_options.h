//  Copyright © 2018 Guobo Chen. All rights reserved.

#ifndef GPLUS_UTIL_PROGRAM_OPTIONS_H_
#define GPLUS_UTIL_PROGRAM_OPTIONS_H_

#include "base/exception.h"
#include "third_party/boost/program_options/options_description.hpp"
#include "third_party/boost/program_options/positional_options.hpp"
#include "third_party/boost/program_options/variables_map.hpp"

namespace gplus {

extern const char* kArgv0;

namespace program_options = boost::program_options;
typedef program_options::options_description OptionsDesc;
typedef program_options::positional_options_description PositionalOptionsDesc;

program_options::variables_map& GetSpecifiedOptions();

template<typename T>
T GetOptionValue(const char* option_name) {
  auto& prog_opts = GetSpecifiedOptions();
  if (prog_opts.count(option_name) <= 0) {
    throw MissingProgramOptionException(option_name);
  }
  return prog_opts[option_name].as<T>();
}

}  // namespace gplus

#endif  // GPLUS_UTIL_PROGRAM_OPTIONS_H_
