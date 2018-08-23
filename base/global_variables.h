//
//  global_status.h
//  gplus
//
//  Created by Zhixiang Zhu on 06/08/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#ifndef GPLUS_BASE_GLOBAL_VARIABLES_H_
#define GPLUS_BASE_GLOBAL_VARIABLES_H_

#include <memory>

#include "data/bim_file.h"

namespace  gplus {

struct GlobalVariables {
  std::shared_ptr<BimFile> bim_file;
};

extern GlobalVariables* gvar;

}  // namespace gplus

#endif  // GPLUS_BASE_GLOBAL_VARIABLES_H_
