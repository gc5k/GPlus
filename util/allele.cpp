//
//  allele.cpp
//  gplus
//
//  Created by Zhixiang Zhu on 2018/09/30.
//  Copyright © 2018年 Guobo Chen. All rights reserved.
//

#include "util/allele.h"

#include "util/log.h"

using std::string;

namespace gplus {
  
bool IsMissingAllele(const std::string& allele) {
  return "0" == allele;
}
  
string CheckAllele(const TextFileReader& reader,
                   const string& variant_name,
                   const string& allele) {
  if (!IsMissingAllele(allele) &&
      "A" != allele && "T" != allele &&
      "C" != allele && "G" != allele &&
      "a" != allele && "t" != allele &&
      "c" != allele && "g" != allele) {
    GPLUS_LOG
    << "Variant " << variant_name << " has an invalid allele '" << allele
    << "' at line " << reader.GetLineNo() << " of the " << reader.GetFileDescription()
    << ". An allele must be A, T, C, or G.";
    exit(EXIT_FAILURE);
  }
  return allele;
}

}  // namespace gplus
