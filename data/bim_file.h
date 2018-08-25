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
    std::string allele1;
    std::string allele2;
  };

  static std::shared_ptr<BimFile> Read(const std::string& file_name);

  const std::vector<Variant>& GetVariants() const { return variants_; }
  Variant GetVariant(int index) const { return variants_[index]; }
  int GetVariantCount() const { return static_cast<int>(variants_.size()); }

private:
  std::vector<Variant> variants_;
};

}  // namespace gplus

#endif  // GPLUS_DATA_BIM_FILE_H_
