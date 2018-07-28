//
//  gplus.h
//  gplus
//
//  Created by Zhixiang Zhu on 01/07/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//
#ifndef GPLUS_BASE_GPLUS_H_
#define GPLUS_BASE_GPLUS_H_

#include "third_party/boost/program_options/variables_map.hpp"

namespace gplus {

extern const char* kArgv0;

namespace program_options = boost::program_options;

program_options::variables_map& GetProgramArguments();

}

#endif  // GPLUS_BASE_GPLUS_H_
