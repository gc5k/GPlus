//
//  profile_subcommand.cpp
//  gplus
//
//  Created by Zhixiang Zhu on 22/07/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#include "subcommands/profile_subcommand.h"

#include <string>
#include "data/score_file.h"

using std::string;
namespace po = boost::program_options;

namespace gplus {

void ProfileSubcommand::AddOptionsDesc(OptionsDesc* opts_desc) const {
  opts_desc->add_options()
  ("file,f", po::value<string>(),
   "filename (no ext) of the plink format input data")
  ("score,s", po::value<string>(), "score file")
  ("missing-score",
   po::value<int>()->default_value(-9), "value of missing scores")
  ("keep-atgc", "take palindromic loci into account")
  ("out,o", po::value<string>(),
   "filename (no ext) of the output files");
}

void ProfileSubcommand::Execute() {
  auto score_file = ScoreFile::ReadScoreFile();
}

}  // namespace gplus
