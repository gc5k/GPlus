//  Copyright © 2018 Guobo Chen. All rights reserved.

#ifndef GPLUS_DATA_SCORE_FILE_H_
#define GPLUS_DATA_SCORE_FILE_H_

#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "util/log.h"

namespace gplus {
  
  struct ScoreFile {
    
    struct Variant {
      std::string name;
      std::string ref;
    };
    
    std::string file_name;
    std::vector<std::string> trait_names;
    std::vector<Variant> variants;
    std::map<std::string, int> variant_index_map;
    
    // dimension 1: variants
    // dimension 2: score values of all the score columns of a given variant
    std::vector<std::vector<float>> score_rows;

    int GetVariantIndex(const std::string& name) const {
      auto iter = variant_index_map.find(name);
      return variant_index_map.end() == iter ? -1 : iter->second;
    }
  };
  
  const ScoreFile* score_file();
}  // namespace gplus

#endif  // GPLUS_DATA_SCORE_FILE_H_
