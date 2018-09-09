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
    std::vector<std::string> score_names;
    std::vector<Variant> variants;
    std::map<std::string, int> variant_index_map;
    
    // dimension 1: score names
    // dimension 2: score values of all the variants or ref alleles of a score name
    std::vector<std::vector<float>> score_values;

    int GetVariantIndex(const std::string& name) const {
      auto iter = variant_index_map.find(name);
      if (variant_index_map.end() == iter) {
        GPLUS_LOG << "Cannot find variant " << name << " in score file.";
        exit(EXIT_FAILURE);
      }
      return iter->second;
    }
  };
  
  ScoreFile* score_file();
  
  void ReadScoreFile();
}  // namespace gplus

#endif  // GPLUS_DATA_SCORE_FILE_H_
