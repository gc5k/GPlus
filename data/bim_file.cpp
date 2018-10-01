//
//  bim_file.cpp
//  gplus
//
//  Created by Zhixiang Zhu on 13/08/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#include "data/bim_file.h"

#include "data/text_file_reader.h"
#include "util/allele.h"
#include "util/log.h"
#include "util/program_options.h"

using std::string;
using std::vector;

namespace gplus {

static const BimFile* ReadBimFile() {
  auto file_name = GetOptionValue<string>("bfile") + ".bim";
  TextFileReader reader("bim", file_name);
  auto ret = new BimFile;
  while (reader.ReadColumns(kColumnCountExact, 6)) {
    const vector<string>& columns = reader.GetColumns();
    auto iter = columns.cbegin();
    
    BimFile::Variant variant;
    auto& ch = *iter++;
    variant.name = *iter++;
    variant.chromosome = reader.ReadInt(variant.name, "chromosome", ch);
    variant.dist = reader.ReadInt(variant.name, "dist", *iter++);
    variant.bp = reader.ReadInt(variant.name, "bp", *iter++);
    variant.allele1 = CheckAllele(reader, variant.name, *iter++);
    variant.allele2 = CheckAllele(reader, variant.name, *iter++);
    ret->variants.push_back(variant);
  }
  if (ret->variants.empty()) {
    GPLUS_LOG
    << "The bim file '" << file_name
    << "' contains just a title line without any variant data.";
    exit(EXIT_FAILURE);
  }
  auto variant_cnt = ret->variants.size();
  GPLUS_LOG
  << "Read " << variant_cnt << (variant_cnt == 1 ? " variant" : " variants")
  << " from the bim file.";
  return ret;
}

const BimFile* bim_file() {
  static const BimFile* const ret = ReadBimFile();
  return ret;
}

}
