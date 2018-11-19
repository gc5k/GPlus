//
//  allele.h
//  gplus
//
//  Created by Zhixiang Zhu on 2018/09/30.
//  Copyright © 2018年 Guobo Chen. All rights reserved.
//

#ifndef GPLUS_UTIL_ALLELE_H_
#define GPLUS_UTIL_ALLELE_H_

#include <string>

#include "util/text_file_reader.h"

namespace gplus {

inline bool IsMissingAllele(const std::string& allele) { return "0" == allele; }
  
std::string CheckAllele(const TextFileReader& reader,
                        const std::string& variant_name,
                        const std::string& allele);
  
}  // namespace gplus

#endif  // GPLUS_UTIL_ALLELE_H_
