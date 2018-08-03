//  Copyright © 2018 Guobo Chen. All rights reserved.

#include "data/score_file.h"

#include <string>
#include "base/exception.h"
#include "util/file_helper.h"
#include "util/program_options.h"

namespace gplus {

std::shared_ptr<ScoreFile> ScoreFile::ReadScoreFile() {
  auto filename = GetOptionValue<std::string>("score");
  auto file = OpenTextFile(filename.c_str());
  std::shared_ptr<ScoreFile> score_file(new ScoreFile);
  return score_file;
}

}  // namespace gplus
