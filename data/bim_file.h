//
//  bim_file.hpp
//  gplus
//
//  Created by Zhixiang Zhu on 13/08/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#ifndef GPLUS_DATA_BIM_FILE_H_
#define GPLUS_DATA_BIM_FILE_H_

#include <memory>
#include <string>
#include <vector>

namespace gplus {

  struct BimFile {
    struct Variant {
      int chromosome;
      std::string name;
      int dist;
      int bp;
      std::string allele1;
      std::string allele2;
    };
    
    std::vector<Variant> variants;
  };
  
  const BimFile* bim_file();

}  // namespace gplus

#endif  // GPLUS_DATA_BIM_FILE_H_
