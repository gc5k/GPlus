//
//  gplus.h
//  gplus
//
//  Created by Zhixiang Zhu on 01/07/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//
#ifndef GPLUS_BASE_GPLUS_H_
#define GPLUS_BASE_GPLUS_H_

#include "third_party/boost/program_options/options_description.hpp"
#include "third_party/boost/program_options/positional_options.hpp"
#include "third_party/boost/program_options/variables_map.hpp"

namespace gplus {

extern const char* kArgv0;

namespace program_options = boost::program_options;
typedef program_options::options_description OptionsDesc;
typedef program_options::positional_options_description PositionalOptionsDesc;

program_options::variables_map& GetProgramArguments();

}  // namespace gplus

#endif  // GPLUS_BASE_GPLUS_H_
