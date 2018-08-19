//
//  bed_file.hpp
//  gplus
//
//  Created by Zhixiang Zhu on 17/08/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#ifndef GPLUS_DATA_BED_FILE_H_
#define GPLUS_DATA_BED_FILE_H_

#include <memory>
#include <string>

namespace gplus {

class BedFile {
 public:
  static std::shared_ptr<BedFile> Read(const std::string& file_name,
                                       size_t variant_count,
                                       size_t sample_count);
};

}  // namespace gplus

#endif  // GPLUS_DATA_BED_FILE_H_
