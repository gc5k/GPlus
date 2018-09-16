//
//  fam_file.cpp
//  gplus
//
//  Created by Zhixiang Zhu on 15/08/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#include "data/fam_file.h"

#include "data/text_file_reader.h"
#include "util/program_options.h"

using std::string;

namespace gplus {

static const FamFile* ReadFamFile() {
  string file_name = GetOptionValue<string>("bfile") + ".fam";
  TextFileReader reader("fam", file_name);
  auto ret = new FamFile;
  const std::vector<string>& columns = reader.GetColumns();
  while (reader.ReadColumns(kColumnCountExact, 6)) {
    auto iter = columns.cbegin();
    FamFile::Sample sample;
    sample.fam_id = *iter++;
    sample.iid = *iter++;
    string row_name = "Sample " + sample.iid + " in family " + sample.fam_id;
    sample.dad_id = *iter++;
    sample.mom_id = *iter++;
    auto sex = reader.ReadInt(row_name,
                            "sex",
                            *iter++,
                            FamFile::Sex::kUnkown,
                            FamFile::Sex::kMax - 1);
    sample.sex = static_cast<FamFile::Sex>(sex);
    sample.phenotype_str = *iter++;
    ret->samples.push_back(sample);
  }
  auto sample_cnt = ret->samples.size();
  GPLUS_LOG
  << "Read " << sample_cnt << (sample_cnt == 1 ? " sample" : " samples")
  << " from the fam file.";
  return ret;
}

const FamFile* fam_file() {
  static const FamFile* const ret = ReadFamFile();
  return ret;
}

}  // gplus
