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

#include "base/gplus.h"
#include "third_party/boost/program_options/options_description.hpp"
#include "third_party/boost/program_options/positional_options.hpp"

namespace gplus {

typedef program_options::options_description OptionsDesc;
typedef program_options::positional_options_description PositionalOptionsDesc;

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

  // Gets the program options description that is
  // used by the command-line parser.
  virtual OptionsDesc GetAllOptionsDescription() const = 0;

  // Gets the program options description that is
  // shown in the help message.
  virtual OptionsDesc GetVisibleOptionsDescription() const {
    return GetAllOptionsDescription();
  }

  virtual void AddPositionalOptionsDescription(PositionalOptionsDesc& pd) {}

  virtual void Execute() = 0;
};

}  // namespace gplus

#endif  // GPLUS_BASE_SUBCOMMAND_H_
