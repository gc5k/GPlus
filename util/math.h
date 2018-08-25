//
//  math.h
//  gplus
//
//  Created by Zhixiang Zhu on 25/08/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#ifndef GPLUS_UTIL_MATH_H_
#define GPLUS_UTIL_MATH_H_

#include <cmath>

namespace gplus {
  bool equals(float a, float b) {
    return fabs(a - b) <= 1.0e-6;
  }
}  // namespace gplus

#endif  // GPLUS_UTIL_MATH_H_
