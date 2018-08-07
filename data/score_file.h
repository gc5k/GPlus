//  Copyright © 2018 Guobo Chen. All rights reserved.

#ifndef GPLUS_DATA_SCORE_FILE_H_
#define GPLUS_DATA_SCORE_FILE_H_

#include <memory>
#include <string>

namespace gplus {

class ScoreFile {
 public:
  explicit ScoreFile(const std::string& filename) : filename_(filename) {}
  static std::shared_ptr<ScoreFile> ReadScoreFile();

 private:
  std::string filename_;
};

}  // namespace gplus

#endif  // GPLUS_DATA_SCORE_FILE_H_
