//  Copyright © 2018 Guobo Chen. All rights reserved.

#include "data/score_file.h"

#include <string>
#include "base/exception.h"
#include "util/program_options.h"

namespace gplus {

std::shared_ptr<ScoreFile> ScoreFile::ReadScoreFile() {
  auto& prog_args = GetSpecifiedOptions();
  if (prog_args.count("score") <= 0) {
    throw MissingProgramOptionException("score");
  }
  auto filename = prog_args["score"].as<std::string>();
  std::shared_ptr<ScoreFile> score_file(new ScoreFile);
  return score_file;
}

}  // namespace gplus
