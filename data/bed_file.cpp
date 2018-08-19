//
//  bed_file.cpp
//  gplus
//
//  Created by Zhixiang Zhu on 17/08/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#include "data/bed_file.h"

#include <fstream>

#include "util/log.h"

namespace gplus {

std::shared_ptr<BedFile> BedFile::Read(const std::string &file_name,
                                       size_t variant_count,
                                       size_t sample_count) {
  std::ifstream in_stream(file_name, std::ios::binary);
  if (!in_stream) {
    GPLUS_LOG << "Cannot open the bed file '" << file_name << "'.";
    exit(EXIT_FAILURE);
  }
  
  char magic[3];
  if (!in_stream.read(magic, sizeof(magic))) {
    GPLUS_LOG
    << "Failed to read the first " << sizeof(magic) << " bytes of the bed file '"
    << file_name << "'.";
    exit(EXIT_FAILURE);
  }
  
  if (0x1 == magic[2]) {
    // variant major
  } else {
    GPLUS_LOG << "Sample major bed file is not supported yet.";
    exit(EXIT_FAILURE);
  }
  
  std::shared_ptr<BedFile> bed_file(new BedFile);
  return bed_file;
}

}  // namespace gplus
