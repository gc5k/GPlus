//
//  bed_file.cpp
//  gplus
//
//  Created by Zhixiang Zhu on 17/08/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#include "data/bed_file.h"

#include <fstream>

#include "base/global_variables.h"
#include "util/log.h"

namespace gplus {
  
  BedFile::BedFile(int variant_count, int sample_count)
  : variant_count_(variant_count)
  , sample_count_(sample_count)
  , byte_count_per_variant_(GetByteCount(sample_count))
  , genotypes_(new std::shared_ptr<char>[variant_count],
               std::default_delete<std::shared_ptr<char>[]>()) {
    for (int i = 0; i < variant_count; i++) {
      auto& genotypes_of_variant = genotypes_.get()[i];
      genotypes_of_variant.reset(new char[byte_count_per_variant_],
                                 std::default_delete<char[]>());
    }
  }

std::shared_ptr<BedFile> BedFile::Read(const std::string &file_name,
                                       int variant_count,
                                       int sample_count) {
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
  
  std::shared_ptr<BedFile> bed_file(new BedFile(variant_count, sample_count));
  
  if (0x1 == magic[2]) {
    // variant major
    for (int variant_idx = 0; variant_idx < variant_count; ++variant_idx) {
      if (!in_stream.read(bed_file->genotypes_.get()[variant_idx].get(),
                          bed_file->byte_count_per_variant_)) {
        auto variant = gvar->bim_file->GetVariant(variant_idx);
        GPLUS_LOG
        << "Failed to read the genotypes of variant " << variant.name
        << " (variant no. " << (variant_idx + 1) << ").";
        exit(EXIT_FAILURE);
      }
    }
  } else {
    GPLUS_LOG << "Sample major bed file is not supported yet.";
    exit(EXIT_FAILURE);
  }
  
  return bed_file;
}

}  // namespace gplus
