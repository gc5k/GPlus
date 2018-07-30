//  Copyright © 2018 Guobo Chen. All rights reserved.

#ifndef GPLUS_DATA_SCORE_FILE_H_
#define GPLUS_DATA_SCORE_FILE_H_

#include <memory>

namespace gplus {

namespace data {

class ScoreFile {
 public:
  static std::shared_ptr<ScoreFile> ReadScoreFile();
};

}  // namespace data

}  // namespace gplus

#endif  // GPLUS_DATA_SCORE_FILE_H_
