//
//  test_bed_subcommand.cpp
//  gplus
//
//  Created by Zhixiang Zhu on 2018/09/16.
//  Copyright © 2018年 Guobo Chen. All rights reserved.
//

#include "subcommands/test_bed_subcommand.h"

#include "data/bed_file.h"
#include "util/program_options.h"

namespace gplus {

void TestBedSubcommand::AddOptionsDesc(OptionsDesc* opts_desc) const {
  AddBedOption(opts_desc);
  AddOutOption(opts_desc);
}

void TestBedSubcommand::Execute() {
  auto bed = bed_file();
  // Just to prevent the reading invocation from
  // being eliminated by compiler optimization.
  GPLUS_LOG << "Byte count per variant: " << bed->byte_count_per_variant;
}

}  // namespace gplus
