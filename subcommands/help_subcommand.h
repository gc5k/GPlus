//
//  help_subcommand.h
//  gplus
//
//  Created by Zhixiang Zhu on 25/06/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#ifndef GPLUS_SUBCOMMANDS_HELP_SUBCOMMAND_H_
#define GPLUS_SUBCOMMANDS_HELP_SUBCOMMAND_H_

#include <string>
#include "base/subcommand.h"

namespace gplus {

class HelpSubcommand : public Subcommand {
 public:
  const char* GetName() const override { return "help"; }

  const char* GetDescription() const override {
    return "Print help information";
  }

  std::string GetLongDescription() const override;

  void AddOptionsDesc(OptionsDesc* opts_desc) const override;

  void AddVisibleOptionsDesc(OptionsDesc* opts_desc) const override {}

  void AddPositionalOptionsDesc(PositionalOptionsDesc* pd) override;

  void Execute() override;
};

}  // namespace gplus

#endif  // GPLUS_SUBCOMMANDS_HELP_SUBCOMMAND_H_
