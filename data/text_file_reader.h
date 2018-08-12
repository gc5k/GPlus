//
//  file_helper.h
//  gplus
//
//  Created by Zhixiang Zhu on 03/08/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#ifndef GPLUS_DATA_TEXT_FILE_READER_
#define GPLUS_DATA_TEXT_FILE_READER_

#include <algorithm>
#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace gplus {

class TextFileReader {
 public:
  explicit TextFileReader(const std::string& file_description,
                          const std::string& file_name);
  const std::vector<std::string>& GetColumns() const { return columns_; }
  bool ReadColumns(int minimal_column_count_required = 0);
  std::string GetRowLocationForLog() const;
  bool IsMissingValue(const std::string& val) const {
    return std::find(missing_value_marks_.begin(),
                     missing_value_marks_.end(),
                     val) != missing_value_marks_.end();
  }

 private:
  bool ReadNonEmptyLine();

  std::string file_desc_;
  std::string file_desc_u_;  // upper case in first letter
  std::string file_name_;
  std::ifstream in_stream_;
  int line_no_;
  std::string line_;
  int row_no_;
  std::vector<std::string> columns_;
  size_t column_count_required_;
  std::vector<std::string> missing_value_marks_;
};

}  // namespace gplus

#endif  // GPLUS_DATA_TEXT_FILE_READER_
