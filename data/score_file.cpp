//  Copyright © 2018 Guobo Chen. All rights reserved.

#include "data/score_file.h"

#include <algorithm>  // copy
#include <iterator>  // back_inserter

#include "third_party/boost/format.hpp"
#include "data/text_file_reader.h"
#include "util/math.h"
#include "util/program_options.h"

using std::string;
using std::vector;

namespace gplus {
  ScoreFile* score_file() {
    static ScoreFile* const ret = new ScoreFile;
    return ret;
  }

  void ReadScoreFile() {
    auto file_name = GetOptionValue<std::string>("score");
    auto missing_score = static_cast<float>(GetOptionValue<int>("missing-score"));
    TextFileReader reader("score", file_name);
    const vector<string>& columns = reader.GetColumns();
    
    // file name
    score_file()->file_name = file_name;
    
    // score names (titles)
    reader.ReadColumns(kColumnCountMinimal, 3);
    vector<string>& score_names = score_file()->score_names;
    score_names.reserve(columns.size() - 2);
    const bool no_score_headr = prog_args()->count("no-score-header") > 0;
    bool waiting_to_parse_first_row = no_score_headr;
    if (no_score_headr) {
      for (int i = 2; i < columns.size(); ++i) {
        score_names.push_back(boost::str(boost::format("S%1%") % (i - 1)));
      }
    } else {
      std::copy(columns.cbegin() + 2, columns.cend(), std::back_inserter(score_names));
    }
    
    // variants and score values
    vector<ScoreFile::Variant>& variants = score_file()->variants;
    std::map<string, int>& variant_index_map = score_file()->variant_index_map;
    vector<vector<float>>& score_values = score_file()->score_values;
    score_values.resize(score_names.size());
    const int variant_index_offset = no_score_headr ? 1 : 2;
    while (waiting_to_parse_first_row || reader.ReadColumns()) {
      auto col_iter = columns.cbegin();
      
      // variant name and reference allele
      ScoreFile::Variant variant;
      variant.name = *col_iter++;
      variant.ref = reader.CheckAllele(variant.name, *col_iter++);
      variants.push_back(variant);
      if (variant_index_map.count(variant.name) > 0) {
        GPLUS_LOG << "Variant " << variant.name << " is duplicated in score file.";
        exit(EXIT_FAILURE);
      }
      variant_index_map[variant.name] = reader.GetCurrentRowNumber() - variant_index_offset;
      
      // score values
      auto score_vectors_iter = score_values.begin();
      while (col_iter != columns.cend()) {
        const string& col_val = *col_iter++;
        if (reader.IsMissingValue(col_val)) {
          (score_vectors_iter++)->push_back(0.0f);
        } else {
          try {
            float score_value = stof(col_val);
            if (equals(score_value, missing_score)) {
              score_value = 0.0f;
            }
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
      waiting_to_parse_first_row = false;
    }
    
    if (variants.empty()) {
      GPLUS_LOG
      << "The score file '" << file_name
      << "' contains just a title line without any score data.";
      exit(EXIT_FAILURE);
    }
  }

}  // namespace gplus
