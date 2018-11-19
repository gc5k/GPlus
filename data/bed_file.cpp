//
//  bed_file.cpp
//  gplus
//
//  Created by Zhixiang Zhu on 17/08/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#include "data/bed_file.h"

#include <fstream>

#include "data/bim_file.h"
#include "data/fam_file.h"
#include "util/log.h"
#include "util/program_options.h"

using std::string;

namespace gplus {

const BedFile* BedFile::ReadBedFile() {
  string file_name = GetOptionValue<string>("bfile") + ".bed";
  std::ifstream in_stream(file_name, std::ios::binary);
  if (!in_stream) {
    GPLUS_LOG << "Cannot open the bed file '" << file_name << "'.";
    exit(EXIT_FAILURE);
  }

  auto ret = new BedFile;

  auto variant_count = bim_file()->variants.size();
  auto sample_count = fam_file()->samples.size();
  
  GPLUS_LOG << "Reading the bed file: " << file_name;
  auto timer_start = GetNow();

  char magic[3];
  if (!in_stream.read(magic, sizeof(magic))) {
    GPLUS_LOG << "Failed to read the first " << sizeof(magic)
              << " bytes of the bed file '" << file_name << "'.";
    exit(EXIT_FAILURE);
  }

  ret->is_variant_major_ = 0x1 == magic[2];
  if (ret->is_variant_major_) {
    // variant major
    ret->dim1_size_ = variant_count;
    ret->dim2_genotype_cnt_ = sample_count;
  } else {
    ret->dim1_size_ = sample_count;
    ret->dim2_genotype_cnt_ = variant_count;
  }
  
  ret->genotypes_ = new char[ret->GetTotalByteCount()];
  if (!in_stream.read(ret->genotypes_, ret->GetTotalByteCount())) {
    GPLUS_LOG << "Failed to read the remaining " << ret->GetTotalByteCount()
              << " bytes of the bed file.";
    exit(EXIT_FAILURE);
  }

  GPLUS_LOG << "Reading bed complete. It took "
            << PrintDurationSince(timer_start) << " to read the bed file.";

  return ret;
}

const BedFile* bed_file() {
  static const BedFile* const ret = BedFile::ReadBedFile();
  return ret;
}

}  // namespace gplus
