//
//  test_bed_subcommand.hpp
//  gplus
//
//  Created by Zhixiang Zhu on 2018/09/16.
//  Copyright © 2018年 Guobo Chen. All rights reserved.
//

#ifndef GPLUS_SUBCOMMANDS_TEST_BED_SUBCOMMAND_H_
#define GPLUS_SUBCOMMANDS_TEST_BED_SUBCOMMAND_H_

#include "base/subcommand.h"

namespace gplus {

class TestBedSubcommand : public Subcommand {
 public:
  const char* GetName() const override { return "testbed"; }
  const char* GetDescription() const override {
    return "Just read bed file to test reading performance";
  }
  void AddOptionsDesc(OptionsDesc* opts_desc) const override;
  void Execute() override;
};

}  // namespace gplus

#endif  // GPLUS_SUBCOMMANDS_TEST_BED_SUBCOMMAND_H_
