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

class ScoreFile {
 public:
  struct Variant {
    std::string name;
    std::string ref;
  };

  static std::shared_ptr<ScoreFile> ReadScoreFile();
  
  const std::vector<std::string>& GetScoreNames() const { return score_names_; }
  int GetVariantCount() const { return static_cast<int>(variants_.size()); }
  const std::vector<float>& GetScoreValues(int score_name_idx) const {
    return score_values_[score_name_idx];
  }
  int GetVariantIndex(const std::string& name) const {
    auto iter = variant_index_map_.find(name);
    if (variant_index_map_.end() == iter) {
      GPLUS_LOG << "Cannot find variant " << name << " in score file.";
      exit(EXIT_FAILURE);
    }
    return iter->second;
  }
  Variant GetVariant(int index) const { return variants_[index]; }

 private:
  std::string file_name_;
  std::vector<std::string> score_names_;
  std::vector<Variant> variants_;
  std::map<std::string, int> variant_index_map_;

  // dimension 1: score names
  // dimension 2: score values of all the variants or ref alleles of a score name
  std::vector<std::vector<float>> score_values_;
};

}  // namespace gplus

#endif  // GPLUS_DATA_SCORE_FILE_H_
