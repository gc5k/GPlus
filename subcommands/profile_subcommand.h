//
//  profile_subcommand.h
//  gplus
//
//  Created by Zhixiang Zhu on 22/07/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#ifndef GPLUS_SUBCOMMANDS_PROFILE_SUBCOMMAND_H_
#define GPLUS_SUBCOMMANDS_PROFILE_SUBCOMMAND_H_

#include "base/subcommand.h"

namespace gplus {

class ProfileSubcommand : public Subcommand {
 public:
  const char* GetName() const override { return "profile"; }

  const char** GetAliases(int * aliases_cnt) const override {
    static const char * aliases[] = { "prof" };
    *aliases_cnt = sizeof(aliases) / sizeof(aliases[0]);
    return aliases;
  }

  const char* GetDescription() const override {
    return "Calculate the risk profile scores";
  }

  void AddOptionsDesc(OptionsDesc* opts_desc) const override;

  void Execute() override;
};

}  // namespace gplus

#endif  // GPLUS_SUBCOMMANDS_PROFILE_SUBCOMMAND_H_
