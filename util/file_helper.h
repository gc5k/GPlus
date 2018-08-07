//
//  file_helper.h
//  gplus
//
//  Created by Zhixiang Zhu on 03/08/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#ifndef GPLUS_UTIL_FILE_HELPER_H_
#define GPLUS_UTIL_FILE_HELPER_H_

#include <memory>
#include <iostream>
#include <fstream>
#include <string>

namespace gplus {

std::shared_ptr<std::ifstream> OpenTextFile(const char* filename);

std::ifstream& GetNonEmptyLine(
    std::ifstream* in_strm_ptr, std::string* str_ptr);

}  // namespace gplus

#endif  // GPLUS_UTIL_FILE_HELPER_H_
