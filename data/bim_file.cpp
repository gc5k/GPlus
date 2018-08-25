//
//  bim_file.cpp
//  gplus
//
//  Created by Zhixiang Zhu on 13/08/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#include "data/bim_file.h"

#include "base/global_variables.h"
#include "data/text_file_reader.h"
#include "util/log.h"

using std::string;
using std::vector;

namespace gplus {

std::shared_ptr<BimFile> BimFile::Read(const std::string &file_name) {
  TextFileReader reader("bim", file_name);
  std::shared_ptr<BimFile> bim_file(new BimFile);

  while (reader.ReadColumns(kColumnCountExact, 6)) {
    const vector<string>& columns = reader.GetColumns();
    auto iter = columns.cbegin();

    Variant variant;
    auto& ch = *iter++;
    variant.name = *iter++;
    variant.chromosome = reader.ReadInt(variant.name, "chromosome", ch);
    variant.dist = reader.ReadInt(variant.name, "dist", *iter++);
    variant.bp = reader.ReadInt(variant.name, "bp", *iter++);
    variant.allele1 = reader.CheckAllele(variant.name, *iter++);
    variant.allele2 = reader.CheckAllele(variant.name, *iter++);
    bim_file->variants_.push_back(variant);
  }

  if (bim_file->variants_.empty()) {
    GPLUS_LOG
    << "The bim file '" << file_name
    << "' contains just a title line without any variant data.";
    exit(EXIT_FAILURE);
  }

  gvar->bim_file = bim_file;
  return bim_file;
}

}
