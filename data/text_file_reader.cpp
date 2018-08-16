//
//  file_helper.cpp
//  gplus
//
//  Created by Zhixiang Zhu on 03/08/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#include "data/text_file_reader.h"

#include <cctype>  // toupper
#include <cstdlib>

#include "third_party/boost/algorithm/string/classification.hpp"  // is_any_of
#include "third_party/boost/algorithm/string/split.hpp"
#include "third_party/boost/algorithm/string/trim.hpp"
#include "third_party/boost/format.hpp"
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
      row_no_(0),
      missing_value_marks_({".", "-", "N/A", "NA", "n/a", "na"}) {
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

bool TextFileReader::ReadColumns(ColumnCountRequirement req, size_t col_cnt) {
  columns_.clear();
  if (ReadNonEmptyLine()) {
    boost::algorithm::split(columns_, line_, boost::is_any_of(" \t"),
                            boost::token_compress_on);
  }

  // Check column count.
  if (columns_.empty()) {
    return false;
  }
  switch (req) {
    case kColumnCountExact:
      if (columns_.size() != col_cnt) {
        GPLUS_LOG
        << file_desc_u_ << " should have exactly " << col_cnt
        << " column(s) at row " << row_no_ << ", but actually it contains "
        << columns_.size() << " column(s) now.";
        exit(EXIT_FAILURE);
      }
      break;
    case kColumnCountMinimal:
      if (columns_.size() < col_cnt) {
        GPLUS_LOG
        << file_desc_u_ << " should have at least " << col_cnt
        << " column(s) at " << GetRowLocationForLog() << ", but now only "
        << columns_.size()
        << (columns_.size() <= 1 ? " column is read." : " columns are read.");
        exit(EXIT_FAILURE);
      }
      break;
    default:
      assert(false);
  }
  if (column_count_required_ == 0) {
    column_count_required_ = columns_.size();
  }
  return true;
}

string TextFileReader::GetRowLocationForLog() const {
  return (boost::format("row %1% (line %2% of the file)")
          % row_no_ % line_no_).str();
}

int TextFileReader::ReadIntForSnp(const std::string& snp_name,
                                  const std::string& column_name,
                                  const std::string& column_value) const {
  try {
    return std::stoi(column_value);
  } catch (std::invalid_argument) {
    GPLUS_LOG
    << "SNP " << snp_name << " has an invalid " << column_name << " '"
    << column_value << "' in line " << line_no_ << " of the " << file_desc_
    << ". A " << column_name << " must be an integer.";
    exit(EXIT_FAILURE);
  } catch (std::out_of_range) {
    GPLUS_LOG
    << "SNP " << snp_name << " has a " << column_name << " value "
    << column_value << " which is out the range of program integers.";
    exit(EXIT_FAILURE);
  }
}

char TextFileReader::ReadAlleleForSnp(const std::string& snp_name,
                                      const std::string& column_value) const {
  if ("A" != column_value && "T" != column_value &&
      "C" != column_value && "G" != column_value &&
      "a" != column_value && "t" != column_value &&
      "c" != column_value && "g" != column_value) {
    GPLUS_LOG
    << "SNP " << snp_name << " has an invalid allele '" << column_value
    << "' at line " << line_no_ << " of the " << file_desc_
    << ". An allele must be A, T, C, or G.";
    exit(EXIT_FAILURE);
  }
  return toupper(column_value[0]);
}

}  // namespace gplus