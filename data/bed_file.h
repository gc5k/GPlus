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

class BedFile {
 public:
  static const BedFile* ReadBedFile();
  const char* GetGenotypes() const { return genotypes_; }
  char GetByte(size_t byte_idx) const { return genotypes_[byte_idx]; }
  /*
  int GetGenotype(std::size_t dim1_index,
                  std::size_t dim2_genotype_index) const {
    auto byte_index = dim1_index * dim2_byte_cnt + (dim2_genotype_index >> 2);
    char byte = genotypes_[byte_index];
    int bit_index = static_cast<int>((dim2_genotype_index & 0x3) << 1);
    int genotype = (byte >> bit_index) & 0x3;
    return genotype;
  }
   */

  size_t GetDim1Size() const { return dim1_size_; }
  size_t GetDim2GenotypeCount() const { return dim2_genotype_cnt_; }
  size_t GetDim2ByteCount() const { return GetByteCount(dim2_genotype_cnt_); }
  size_t GetTotalByteCount() const { return dim1_size_ * GetDim2ByteCount(); }
  
  bool IsVariantMajor() const { return is_variant_major_; }

 private:
  static size_t GetByteCount(size_t genotype_count) {
    return (genotype_count + 3) >> 2;  // round up
  }
  
  bool is_variant_major_;
  
  // variant major
  // dimension 1: variants
  // dimension 2: genotypes of all the samples of a variant
  //
  // sample major
  // dimension 1: samples
  // dimension 2: genotypes of all the variant of a sample
  char* genotypes_;
  size_t dim1_size_;
  size_t dim2_genotype_cnt_;
};

const BedFile* bed_file();

}  // namespace gplus

#endif  // GPLUS_DATA_BED_FILE_H_
