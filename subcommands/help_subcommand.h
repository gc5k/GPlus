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
  const char * GetName() const { return "help"; }

  const char * GetDescription() const { return "Print help information"; }

  std::string GetLongDescription() const;

  boost::program_options::options_description GetAllOptionsDescription() const;

  boost::program_options::options_description
  GetVisibleOptionsDescription() const {
    boost::program_options::options_description desc;
    return desc;
  }

  void AddPositionalOptionsDescription(PositionalOptionsDesc* pd);

  void Execute();
};

}  // namespace gplus

#endif  // GPLUS_SUBCOMMANDS_HELP_SUBCOMMAND_H_
