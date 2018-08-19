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

class BimFile {
 public:
  struct Variant {
    int chromosome;
    std::string name;
    int dist;
    int bp;
    char allele1;
    char allele2;
  };

  static std::shared_ptr<BimFile> Read(const std::string& file_name);

  size_t GetVariantCount() const { return variants_.size(); }

private:
  std::vector<Variant> variants_;
};

}  // namespace gplus

#endif  // GPLUS_DATA_BIM_FILE_H_
