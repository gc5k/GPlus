//
//  file_helper.cpp
//  gplus
//
//  Created by Zhixiang Zhu on 03/08/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#include "data/text_file_reader.h"

#include <cassert>
#include <cstdlib>

#include "third_party/boost/algorithm/string/classification.hpp"  // is_any_of
#include "third_party/boost/algorithm/string/split.hpp"
#include "third_party/boost/algorithm/string/trim.hpp"
#include "util/log.h"

using std::ifstream;
using std::string;

namespace gplus {

TextFileReader::TextFileReader(const std::string& file_description,
                               const std::string& file_name)
    : file_desc_("the " + file_description + " file '" + file_name + "'"),
      file_desc_u_("The " + file_description + " file '" + file_name + "'"),
      file_name_(file_name),
      in_stream_(file_name),
      line_no_(0),
      column_count_required_(0),
      row_no_(0) {
  if (!in_stream_) {
    GPLUS_LOG << "Cannot open " << file_desc_ << "." << std::endl;
    exit(EXIT_FAILURE);
  }
}

bool TextFileReader::ReadNonEmptyLine() {
  while (std::getline(in_stream_, line_)) {
    ++line_no_;
    boost::trim(line_);
    if (!line_.empty()) {
      ++row_no_;
      return true;
    }
  }
  if (row_no_ <= 0) {
    GPLUS_LOG << file_desc_u_ << " is empty." << std::endl;
    exit(EXIT_FAILURE);
  }
  return false;
}

bool TextFileReader::ReadColumns(int minimal_column_count_required) {
  columns_.clear();
  if (ReadNonEmptyLine()) {
    boost::algorithm::split(columns_, line_, boost::is_any_of(" \t"),
                            boost::token_compress_on);
  }
  if (columns_.size() < minimal_column_count_required) {
    GPLUS_LOG
    << file_desc_u_ << " should have at least " << minimal_column_count_required
    << " column(s) at row " << row_no_
    << " (line " << line_no_ << " of the file, but now only " << columns_.size()
    << (columns_.size() <= 1 ? " column is read." : " columns are read.");
    exit(EXIT_FAILURE);
  }
  if (column_count_required_ == 0) {
    column_count_required_ = columns_.size();
  } else if (column_count_required_ != columns_.size()) {
    GPLUS_LOG
    << "Row 1 of " << file_desc_ << " has " << column_count_required_
    << " column(s), but row " << row_no_ << " (line " << line_no_
    << " of the file) has " << columns_.size() << " column(s)." << std::endl;
    exit(EXIT_FAILURE);
  }
  return !columns_.empty();
}

}  // namespace gplus
