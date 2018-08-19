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
#include "data/bim_file.h"
#include "data/fam_file.h"
#include "data/bed_file.h"

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
  string file_name = GetOptionValue<string>("file");
  auto bim_file = BimFile::Read(file_name + ".bim");
  auto fam_file = FamFile::Read(file_name + ".fam");
  auto bed_file = BedFile::Read(file_name + ".bed",
                                bim_file->GetVariantCount(),
                                fam_file->GetSampleCount());
}

}  // namespace gplus
