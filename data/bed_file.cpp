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

BedFile::BedFile(int variant_count, int sample_count)
: variant_count(variant_count)
, sample_count(sample_count)
, byte_count_per_variant(GetByteCount(sample_count))
, genotypes(new char*[variant_count]) {
  for (int i = 0; i < variant_count; i++) {
    auto& genotypes_of_variant = genotypes[i];
    genotypes_of_variant = new char[byte_count_per_variant];
  }
}

const BedFile* ReadBedFile() {
  string file_name = GetOptionValue<string>("bfile") + ".bed";
  std::ifstream in_stream(file_name, std::ios::binary);
  if (!in_stream) {
    GPLUS_LOG << "Cannot open the bed file '" << file_name << "'.";
    exit(EXIT_FAILURE);
  }

  auto variant_count = bim_file()->variants.size();
  auto sample_count = fam_file()->samples.size();
  
  GPLUS_LOG << "Reading the bed file: " << file_name;
  auto timer_start = GetNow();

  auto ret = new BedFile(static_cast<int>(variant_count),
                         static_cast<int>(sample_count));
  
  char magic[3];
  if (!in_stream.read(magic, sizeof(magic))) {
    GPLUS_LOG
    << "Failed to read the first " << sizeof(magic) << " bytes of the bed file '"
    << file_name << "'.";
    exit(EXIT_FAILURE);
  }

  if (0x1 == magic[2]) {
    // variant major
    for (int variant_idx = 0; variant_idx < variant_count; ++variant_idx) {
      if (!in_stream.read(ret->genotypes[variant_idx],
                         ret->byte_count_per_variant)) {
        auto variant = bim_file()->variants[variant_idx];
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
  
  GPLUS_LOG
  << "Reading complete. It took " << PrintDurationSince(timer_start)
  << " to read the bed file.";

  return ret;
}

const BedFile* bed_file() {
  static const BedFile* const ret = ReadBedFile();
  return ret;
}

}  // namespace gplus
