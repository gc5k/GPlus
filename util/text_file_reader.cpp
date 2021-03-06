//
//  file_helper.cpp
//  gplus
//
//  Created by Zhixiang Zhu on 03/08/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#include "util/text_file_reader.h"

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

// This function handles all three line endings ("\r", "\n" and "\r\n").
// Copied from https://stackoverflow.com/a/6089413/444860
static std::istream& SafeGetLine(std::istream& is, std::string& t) {
  t.clear();
  
  // The characters in the stream are read one-by-one using a std::streambuf.
  // That is faster than reading them one-by-one using the std::istream.
  // Code that uses streambuf this way must be guarded by a sentry object.
  // The sentry object performs various tasks,
  // such as thread synchronization and updating the stream state.
  
  std::istream::sentry se(is, true);
  std::streambuf* sb = is.rdbuf();
  
  for(;;) {
    int c = sb->sbumpc();
    switch (c) {
      case '\n':
        return is;
      case '\r':
        if(sb->sgetc() == '\n')
          sb->sbumpc();
        return is;
      case std::streambuf::traits_type::eof():
        // Also handle the case when the last line has no line ending
        if(t.empty())
          is.setstate(std::ios::eofbit);
        return is;
      default:
        t += (char)c;
    }
  }
}

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
  while (SafeGetLine(in_stream_, line_)) {
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

int TextFileReader::ReadInt(const std::string& row_name,
                            const std::string& column_name,
                            const std::string& value,
                            int min, int max) const {
  try {
    int int_val = std::stoi(value);
    if (int_val < min || int_val > max) {
      GPLUS_LOG << row_name << " has an invalid " << column_name << " in line "
                << line_no_ << " of the " << file_desc_ << ". A " << column_name
                << " must be no less than " << min << " and no greater than "
                << max;
      exit(EXIT_FAILURE);
    }
    return int_val;
  } catch (std::invalid_argument) {
    GPLUS_LOG << row_name << " has an invalid " << column_name << " '" << value
              << "' in line " << line_no_ << " of the " << file_desc_ << ". A "
              << column_name << " must be an integer.";
    exit(EXIT_FAILURE);
  } catch (std::out_of_range) {
    GPLUS_LOG << row_name << " has a " << column_name << " value "
              << value << " which is out the range of program integers.";
    exit(EXIT_FAILURE);
  }
}

}  // namespace gplus
