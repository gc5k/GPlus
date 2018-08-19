//  Copyright © 2018 Guobo Chen. All rights reserved.

#include "data/score_file.h"

#include <algorithm>  // copy
#include <iterator>  // back_inserter
#include "data/text_file_reader.h"
#include "util/program_options.h"

using std::string;
using std::vector;

namespace gplus {

std::shared_ptr<ScoreFile> ScoreFile::ReadScoreFile() {
  auto file_name = GetOptionValue<std::string>("score");
  auto missing_score = static_cast<float>(GetOptionValue<int>("missing-score"));
  TextFileReader reader("score", file_name);
  const vector<string>& columns = reader.GetColumns();

  // file name
  std::shared_ptr<ScoreFile> score_file(new ScoreFile);
  score_file->file_name_ = file_name;

  // score names (titles)
  reader.ReadColumns(kColumnCountMinimal, 3);
  vector<string>& score_names = score_file->score_names_;
  score_names.reserve(columns.size() - 2);
  std::copy(columns.cbegin() + 2, columns.cend(),
            std::back_inserter(score_names));

  // variants and score values
  vector<Variant>& variants = score_file->variants_;
  vector<vector<float>>& score_values = score_file->score_values_;
  score_values.resize(score_names.size());
  while (reader.ReadColumns()) {
    auto col_iter = columns.cbegin();

    // variant name and reference allele
    Variant variant;
    variant.name = *col_iter++;
    variant.ref = reader.ReadAlleleForVariant(variant.name, *col_iter++);
    variants.push_back(variant);

    // score values
    auto score_vectors_iter = score_values.begin();
    while (col_iter != columns.cend()) {
      const string& col_val = *col_iter++;
      if (reader.IsMissingValue(col_val)) {
        (score_vectors_iter++)->push_back(missing_score);
      } else {
        try {
          float score_value = stof(col_val);
          (score_vectors_iter++)->push_back(score_value);
        } catch (std::invalid_argument) {
          GPLUS_LOG
          << "Variant " << variant.name << " has an invalid score value '"
          << col_val << "'. A score value must be a floating point number.";
          exit(EXIT_FAILURE);
        } catch (std::out_of_range) {
          GPLUS_LOG
          << "Variant " << variant.name << " has a score value " << col_val
          << " which is out of the range of floating point numbers.";
          exit(EXIT_FAILURE);
        }
      }
    }
  }

  if (variants.empty()) {
    GPLUS_LOG
    << "The score file '" << file_name
    << "' contains just a title line without any score data.";
    exit(EXIT_FAILURE);
  }

  return score_file;
}

}  // namespace gplus
