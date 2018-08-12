//  Copyright © 2018 Guobo Chen. All rights reserved.

#ifndef GPLUS_DATA_SCORE_FILE_H_
#define GPLUS_DATA_SCORE_FILE_H_

#include <memory>
#include <string>
#include <vector>

namespace gplus {

class ScoreFile {
 public:
  struct Snp {
    std::string name;
    char ref;
  };

  static std::shared_ptr<ScoreFile> ReadScoreFile();

 private:
  std::string file_name_;
  std::vector<std::string> score_names_;
  std::vector<Snp> snps_;
  std::vector<std::vector<float>> score_values_;
};

}  // namespace gplus

#endif  // GPLUS_DATA_SCORE_FILE_H_
