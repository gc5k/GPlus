//
//  subcommand_list.cpp
//  gplus
//
//  Created by Zhixiang Zhu on 25/06/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#include <cstring>
#include "base/subcommand.h"
#include "subcommands/profile_subcommand.h"
#include "subcommands/help_subcommand.h"

namespace gplus {

Subcommand * const * GetSubcommandList(int * p_subcmd_cnt) {
  static Subcommand * const subcommand_list[] = {
    new ProfileSubcommand,
    new HelpSubcommand
  };
  *p_subcmd_cnt = sizeof(subcommand_list) / sizeof(Subcommand *);
  return subcommand_list;
}
  
Subcommand * FindSubcommand(const char * name_or_alias) {
  int subcmd_cnt;
  auto subcmd_list = GetSubcommandList(&subcmd_cnt);
  for (int i = 0; i < subcmd_cnt; ++i) {
    auto subcmd = subcmd_list[i];
    if (0 == strcmp(subcmd->GetName(), name_or_alias))
      return subcmd;
    int alias_cnt;
    const char ** aliases = subcmd->GetAliases(&alias_cnt);
    if (aliases) {
      for (int j = 0; j < alias_cnt; ++j) {
        auto alias = aliases[j];
        if (0 == strcmp(alias, name_or_alias))
          return subcmd;
      }
    }
  }
  return nullptr;
}

}
