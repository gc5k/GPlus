//
//  bim_file.cpp
//  gplus
//
//  Created by Zhixiang Zhu on 13/08/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#include "data/bim_file.h"

#include "data/text_file_reader.h"
#include "util/log.h"

using std::string;
using std::vector;

namespace gplus {

std::shared_ptr<BimFile> BimFile::Read(const std::string &file_name) {
  TextFileReader reader("bim", file_name);
  reader.ReadColumns(kColumnCountExact, 6);
  std::shared_ptr<BimFile> bim_file(new BimFile);

  while (reader.ReadColumns()) {
    const vector<string>& columns = reader.GetColumns();
    auto iter = columns.cbegin();

    Snp snp;
    snp.ch = *iter++;
    snp.name = *iter++;
    snp.dist = reader.ReadIntForSnp(snp.name, "dist", *iter++);
    snp.bp = reader.ReadIntForSnp(snp.name, "bp", *iter++);
    snp.allele1 = reader.ReadAlleleForSnp(snp.name, *iter++);
    snp.allele2 = reader.ReadAlleleForSnp(snp.name, *iter++);
    bim_file->snps_.push_back(snp);
  }

  if (bim_file->snps_.empty()) {
    GPLUS_LOG
    << "The bim file '" << file_name
    << "' contains just a title line without any SNP data.";
    exit(EXIT_FAILURE);
  }

  return bim_file;
}

}
