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

po::options_description ProfileSubcommand::GetAllOptionsDescription() const {
  po::options_description desc;
  desc.add_options()
  ("file,f", po::value<string>(),
   "filename (no ext) of the plink format input data")
  ("score,s", po::value<string>(), "score file")
  ("keep-atgc", "take palindromic loci into account")
  ("out,o", po::value<string>(),
   "filename (no ext) of the output files");
  return desc;
}

void ProfileSubcommand::Execute() {
  auto score_file = ScoreFile::ReadScoreFile();
}

}  // namespace gplus
