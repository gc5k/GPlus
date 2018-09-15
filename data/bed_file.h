//
//  bed_file.hpp
//  gplus
//
//  Created by Zhixiang Zhu on 17/08/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#ifndef GPLUS_DATA_BED_FILE_H_
#define GPLUS_DATA_BED_FILE_H_

#include <cstddef>

namespace gplus {

  struct BedFile {
    BedFile(int variant_count, int sample_count);

    static int GetByteCount(int genotype_count) {
      return (genotype_count + 3) / 4;  // round up
    }
    
    int GetGenotype(std::size_t variant_index, std::size_t sample_index) const {
      auto genotypes_of_variant = genotypes[variant_index];
      auto byte_index = sample_index >> 2;
      char byte = genotypes_of_variant[byte_index];
      int bit_index = static_cast<int>((sample_index & 0x3) << 1);
      int genotype = (byte >> bit_index) & 0x3;
      return genotype;
    }

    int variant_count;
    int sample_count;
    int byte_count_per_variant;
    
    // dimension 1: variants
    // dimension 2: genotypes of all the samples of a variant
    char** genotypes;
  };
  
  const BedFile* bed_file();

}  // namespace gplus

#endif  // GPLUS_DATA_BED_FILE_H_
