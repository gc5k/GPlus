//
//  subcommand.h
//  gplus
//
//  Created by Zhixiang Zhu on 25/06/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#ifndef GPLUS_BASE_SUBCOMMAND_H_
#define GPLUS_BASE_SUBCOMMAND_H_

#include <string>

#include "util/program_options.h"

namespace gplus {

class Subcommand {
 public:
  virtual const char * GetName() const = 0;

  virtual const char ** GetAliases(int * aliases_cnt) const {
    *aliases_cnt = 0;
    return 0;
  }

  virtual const char * GetDescription() const = 0;

  virtual std::string GetLongDescription() const {
    return this->GetDescription();
  }

  // Adds descriptions of all the program options which are
  // used by the command-line parser.
  virtual void AddOptionsDesc(OptionsDesc* opts_desc) const = 0;

  // Adds descriptions of the program options which are shown
  // shown in the help message.
  virtual void AddVisibleOptionsDesc(OptionsDesc* opts_desc) const {
    return AddOptionsDesc(opts_desc);
  }

  virtual void AddPositionalOptionsDesc(PositionalOptionsDesc* pd) {}

  virtual void Execute() = 0;
};

}  // namespace gplus

#endif  // GPLUS_BASE_SUBCOMMAND_H_
