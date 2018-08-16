//
//  fam_file.cpp
//  gplus
//
//  Created by Zhixiang Zhu on 15/08/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#include "data/fam_file.h"

#include "data/text_file_reader.h"

using std::string;

namespace gplus {

std::shared_ptr<FamFile> FamFile::Read(const string& file_name) {
  TextFileReader reader("fam", file_name);
  std::shared_ptr<FamFile> fam_file(new FamFile);
  const std::vector<string>& columns = reader.GetColumns();
  while (reader.ReadColumns(kColumnCountExact, 6)) {
    auto iter = columns.cbegin();
    Sample sample;
    sample.fam_id = *iter++;
    sample.iid = *iter++;
    string row_name = "Sample " + sample.iid + " in family " + sample.fam_id;
    sample.dad_id = *iter++;
    sample.mom_id = *iter++;
    sample.sex = static_cast<Sex>(
      reader.ReadInt(row_name, "sex", *iter++, Sex::kUnkown, Sex::kMax - 1));
    sample.phenotype_str = *iter++;
    fam_file->samples_.push_back(sample);
  }
  return fam_file;
}

}  // gplus
