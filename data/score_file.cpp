//  Copyright © 2018 Guobo Chen. All rights reserved.

#include "data/score_file.h"

#include <cstdlib>
#include <string>

#include "base/global_variables.h"
#include "util/file_helper.h"
#include "util/program_options.h"

using std::string;

namespace gplus {

std::shared_ptr<ScoreFile> ScoreFile::ReadScoreFile() {
  auto filename = GetOptionValue<std::string>("score");
  auto file_ptr = OpenTextFile(filename.c_str());

  string line;

  // Read title line.
  GetNonEmptyLine(file_ptr.get(), &line);
  if (line.empty()) {
    GPLUS_LOG << "Score file '" << filename << "' is empty." << std::endl;
    exit(EXIT_FAILURE);
  }
  string titleLine = line;

  std::shared_ptr<ScoreFile> score_file(new ScoreFile(filename));
  return score_file;
}

}  // namespace gplus
