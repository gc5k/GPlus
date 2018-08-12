//  Copyright © 2018 Guobo Chen. All rights reserved.

#include "data/score_file.h"

#include <algorithm>
#include <iterator>
#include "data/text_file_reader.h"
#include "util/program_options.h"

using std::string;
using std::vector;

namespace gplus {

std::shared_ptr<ScoreFile> ScoreFile::ReadScoreFile() {
  auto file_name = GetOptionValue<std::string>("score");
  TextFileReader reader("score", file_name);
  const vector<string>& columns = reader.GetColumns();

  // file name
  std::shared_ptr<ScoreFile> score_file(new ScoreFile);
  score_file->file_name_ = file_name;

  // score names (titles)
  reader.ReadColumns(3);
  vector<string>& score_names = score_file->score_names_;
  score_names.reserve(columns.size() - 2);
  std::copy(columns.cbegin() + 2, columns.cend(),
            std::back_inserter(score_names));

  return score_file;
}

}  // namespace gplus
