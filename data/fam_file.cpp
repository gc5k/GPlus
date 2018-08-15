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
  while (reader.ReadColumns(kColumnCountExact, 6)) {
    
  }
  return fam_file;
}

}  // gplus
