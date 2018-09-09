//
//  help_subcommand.cpp
//  gplus
//
//  Created by Zhixiang Zhu on 25/06/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#include "subcommands/help_subcommand.h"

#include <iostream>
#include <sstream>

#include "base/subcommand_list.h"
#include "util/program_options.h"

using std::cout;
using std::cerr;
using std::endl;
namespace po = boost::program_options;

namespace gplus {

std::string HelpSubcommand::GetLongDescription() const {
  std::stringstream ss;
  ss << "Describe the usage of this program or its subcommands." << endl
     << "Usage: " << kArgv0 << " help [subcommand]";
  return ss.str();
}

void HelpSubcommand::AddOptionsDesc(OptionsDesc* opts_desc) const {
  opts_desc->add_options()
  ("subcmd", po::value<std::string>(), "subcommand name");
}

void HelpSubcommand::AddPositionalOptionsDesc(PositionalOptionsDesc* pd) {
  pd->add("subcmd", 1);
}

static void PrintSubcommand(const Subcommand * subcmd) {
  std::stringstream name_stream;
  name_stream << subcmd->GetName();

  // Print aliases
  int alias_cnt;
  auto aliases = subcmd->GetAliases(&alias_cnt);
  if (alias_cnt > 0) {
    name_stream << " (" << aliases[0];
    for (int j = 1; j < alias_cnt; ++j) {
      name_stream << ", " << aliases[j];
    }
    name_stream << ")";
  }

  const auto name = name_stream.str();
  cout << name;

  // Print spaces between name (aliases) and description.
  const int kNameWidth = 40;
  for (size_t j = name.length(); j < kNameWidth; ++j) {
    cout << " ";
  }

  cout << subcmd->GetDescription() << std::endl;
}

static void PrintSubcommandList() {
  int subcmd_cnt;
  auto subcmd_list = GetSubcommandList(&subcmd_cnt);
  for (int i = 0; i < subcmd_cnt; ++i) {
    PrintSubcommand(subcmd_list[i]);
  }
}

void HelpSubcommand::Execute() {
  if (prog_args()->count("subcmd")) {
    // Pring help message of the subcommand.
    auto subcmd_name = (*prog_args())["subcmd"].as<std::string>();
    auto subcmd = FindSubcommand(subcmd_name.c_str());
    if (subcmd == nullptr) {
      cerr << "Cannot find subcommand named '" << subcmd_name << "'." << endl;
    } else {
      OptionsDesc opts_desc;
      subcmd->AddVisibleOptionsDesc(&opts_desc);
      cout << subcmd->GetLongDescription() << endl
           << endl
           << opts_desc << endl;
    }
  } else {
    // Show all subcommands.
    cout
    << "Usage: " << kArgv0 << " <subcommand> [options]" << endl
    << "Type '" << kArgv0 << " help <subcommand>' for help on a specific "
    << "subcommnd." << endl
    << endl
    << "Available subcommands:" << endl;
    PrintSubcommandList();
  }
}

}  // namespace gplus
