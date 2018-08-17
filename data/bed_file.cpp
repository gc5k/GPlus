//
//  bed_file.cpp
//  gplus
//
//  Created by Zhixiang Zhu on 17/08/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#include "data/bed_file.h"

namespace gplus {

std::shared_ptr<BedFile> BedFile::Read(const std::string &file_name,
                                       size_t snp_count,
                                       size_t sample_count) {
  std::shared_ptr<BedFile> bed_file(new BedFile);
  return bed_file;
}

}  // namespace gplus
