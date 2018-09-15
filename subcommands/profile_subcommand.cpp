//
//  profile_subcommand.cpp
//  gplus
//
//  Created by Zhixiang Zhu on 22/07/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#include "subcommands/profile_subcommand.h"

#include <cassert>
#include <string>
#include <fstream>

#include "third_party/boost/algorithm/string/predicate.hpp"  // iequals
#include "data/score_file.h"
#include "data/bim_file.h"
#include "data/fam_file.h"
#include "data/bed_file.h"

using std::string;
namespace po = boost::program_options;

namespace gplus {

void ProfileSubcommand::AddOptionsDesc(OptionsDesc* opts_desc) const {
  opts_desc->add_options()
  ("bfile,f", po::value<string>(),
   "filename (no ext) of the plink format input data")
  ("score,s", po::value<string>(), "score file")
  ("no-score-header", "specify that score file has no header")
  ("missing-score",
   po::value<int>()->default_value(-9), "value of missing scores")
  ("keep-atgc", "take palindromic loci into account")
  ("out,o", po::value<string>()->default_value("gplus"),
   "filename (no ext) of the output files");
}

void ProfileSubcommand::Execute() {
  string file_name = GetOptionValue<string>("bfile");
  auto bed_file = BedFile::Read(file_name + ".bed",
                                bim_file()->variants.size(),
                                fam_file()->samples.size());
  
  if (score_file()->variants.size() != bim_file()->variants.size()) {
    GPLUS_LOG
    << "The score file contains " << score_file()->variants.size()
    << " variant(s), but the bim file contains " << bim_file()->variants.size()
    << " variant(s).";
    exit(EXIT_FAILURE);
  }
  
  // Create output file.
  auto out_file_name = GetOptionValue<string>("out") + ".profile";
  std::ofstream out_file(out_file_name);
  if (!out_file) {
    GPLUS_LOG << "Failed to create output file '" + out_file_name + "'.";
    exit(EXIT_FAILURE);
  }
  
  // Print headers.
  out_file << "FID\tIID";
  auto& score_names = score_file()->score_names;
  for (auto score_name_iter = score_names.cbegin();
       score_name_iter != score_names.cend(); ++score_name_iter) {
    out_file << "\tSCORE." << *score_name_iter;
  }
  out_file << std::endl;
  
  // Calculate and print scores of each sample.
  auto& samples = fam_file()->samples;
  for (auto sample_iter = samples.cbegin();
       sample_iter != samples.cend(); ++sample_iter) {
    out_file << sample_iter->fam_id << "\t" << sample_iter->iid;
    // Sum up scores of all the variants for each score name.
    for (int score_name_idx = 0; score_name_idx < score_names.size(); ++score_name_idx) {
      auto& scores = score_file()->score_values[score_name_idx];
      float score_sum = 0.0f;
      for (auto variant_iter_of_bim = bim_file()->variants.cbegin();
           variant_iter_of_bim != bim_file()->variants.cend(); ++variant_iter_of_bim) {
        auto variant_idx_in_scores = score_file()->GetVariantIndex(variant_iter_of_bim->name);
        auto score_of_ref = scores[variant_idx_in_scores];
        auto variant_index_in_bim = variant_iter_of_bim - bim_file()->variants.cbegin();
        auto sample_index = sample_iter - samples.cbegin();
        int genotype = bed_file->GetGenotype(variant_index_in_bim, sample_index);
        auto variant_in_scores = score_file()->variants[variant_idx_in_scores];
        bool allele1_is_ref;
        if (boost::iequals(variant_iter_of_bim->allele1, variant_in_scores.ref)) {
          allele1_is_ref = true;
        } else if (boost::iequals(variant_iter_of_bim->allele2, variant_in_scores.ref)) {
          allele1_is_ref = false;
        } else {
          GPLUS_LOG
          << "The reference allele of variant " << variant_iter_of_bim->name
          << " is " << variant_in_scores.ref
          << " in score file, but the alleles of the variant in bim file is "
          << variant_iter_of_bim->allele1 << " and " << variant_iter_of_bim->allele2
          << ". It doesn't match.";
          exit(EXIT_FAILURE);
        }
        switch (genotype) {
          case 0b00:  // Homozygous for first allele in .bim file
            score_sum += allele1_is_ref ? score_of_ref + score_of_ref : 0;
            break;
          case 0b01:  // Missing genotype
            break;
          case 0b10:  // Heterozygous
            score_sum += score_of_ref;
            break;
          case 0b11:  // Homozygous for second allele in .bim file
            score_sum += allele1_is_ref ? 0 : score_of_ref + score_of_ref;
            break;
          default:
            assert(false);
        }
      }
      out_file << "\t" << score_sum;
    }
    out_file << std::endl;
  }
}

}  // namespace gplus
