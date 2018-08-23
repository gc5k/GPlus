//
//  bed_file.hpp
//  gplus
//
//  Created by Zhixiang Zhu on 17/08/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#ifndef GPLUS_DATA_BED_FILE_H_
#define GPLUS_DATA_BED_FILE_H_

#include <cmath>
#include <memory>
#include <string>

namespace gplus {

class BedFile {
public:
  static std::shared_ptr<BedFile> Read(const std::string& file_name,
                                       int variant_count,
                                       int sample_count);

  static int GetByteCount(int genotype_count) {
    return (genotype_count + 3) / 4;  // round up
  }
  
private:
  explicit BedFile(int variant_count, int sample_count);
  
  int variant_count_;
  int sample_count_;
  int byte_count_per_variant_;
  std::shared_ptr<std::shared_ptr<char>> genotypes_;
};

}  // namespace gplus

#endif  // GPLUS_DATA_BED_FILE_H_
