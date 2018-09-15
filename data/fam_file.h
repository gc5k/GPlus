//
//  fam_file.hpp
//  gplus
//
//  Created by Zhixiang Zhu on 15/08/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#ifndef GPLUS_DATA_FAM_FILE_H_
#define GPLUS_DATA_FAM_FILE_H_

#include <memory>
#include <string>
#include <vector>

namespace gplus {

  // PLINK fam file
  // https://www.cog-genomics.org/plink2/formats#fam
  struct FamFile {
    enum Sex {
      kUnkown = 0,
      kMale,
      kFemale,
      kMax
    };
    
    struct Sample {
      std::string fam_id;  // Family ID
      std::string iid;  // Within-family ID ('IID'; cannot be '0')
      std::string dad_id;  // Within-family ID of father ('0' if father isn't in dataset)
      std::string mom_id;  // Within-family ID of mother ('0' if mother isn't in dataset)
      Sex sex;  // Sex code ('1' = male, '2' = female, '0' = unknown)
      std::string phenotype_str;
    };
    
    std::vector<Sample> samples;
  };
  
  const FamFile* fam_file();

}  // namespace gplus

#endif  // GPLUS_DATA_FAM_FILE_H_
