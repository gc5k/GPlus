//
//  profile_subcommand.cpp
//  gplus
//
//  Created by Zhixiang Zhu on 22/07/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#include "subcommands/profile_subcommand.h"

#include <cassert>
#include <fstream>
#include <string>

#include <boost/algorithm/string/predicate.hpp>  // iequals
#include "data/bed_file.h"
#include "data/bim_file.h"
#include "data/fam_file.h"
#include "data/score_file.h"
#include "util/allele.h"

using std::string;
namespace po = boost::program_options;

namespace gplus {

void ProfileSubcommand::AddOptionsDesc(OptionsDesc* opts_desc) const {
  AddBedOption(opts_desc);
  auto add_opt = opts_desc->add_options();
  add_opt("score,s", po::value<string>(), "score file");
  add_opt("no-score-header", "specify that score file has no header");
  add_opt("missing-score", po::value<int>()->default_value(-9),
          "value of missing scores");
  add_opt("no-weight", "don't divide the score sum by number of variants");
  add_opt("keep-atgc", "take palindromic loci into account");
  AddOutOption(opts_desc);
}

void ProfileSubcommand::Execute() {
  if (score_file()->variants.size() != bim_file()->variants.size()) {
    GPLUS_LOG << "The score file contains " << score_file()->variants.size()
              << " variant(s), but the bim file contains "
              << bim_file()->variants.size() << " variant(s).";
    exit(EXIT_FAILURE);
  }

  auto bed = bed_file();
  auto var_cnt = bed->variant_count;

  // Map variant indexes between the bim file and the score file.
  auto variants = bim_file()->variants;
  int* var_idxs_scr = new int[var_cnt];
  for (int var_idx_bim = 0; var_cnt > var_idx_bim; ++var_idx_bim) {
    var_idxs_scr[var_idx_bim] =
        score_file()->GetVariantIndex(variants[var_idx_bim].name);
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

  const bool is_weighted = 0 == prog_args()->count("no-weight");

  // Calculate and print scores of each sample.
  auto& samples = fam_file()->samples;
  for (auto sample_iter = samples.cbegin(); sample_iter != samples.cend();
       ++sample_iter) {
    out_file << sample_iter->fam_id << "\t" << sample_iter->iid;
    // Sum up scores of all the variants for each score name.
    for (int score_name_idx = 0; score_name_idx < score_names.size();
         ++score_name_idx) {
      auto& scores = score_file()->score_values[score_name_idx];
      float score_sum = 0.0f;
      int allele_count = bed_file()->variant_count << 1;
      for (auto variant_iter_of_bim = bim_file()->variants.cbegin();
           variant_iter_of_bim != bim_file()->variants.cend();
           ++variant_iter_of_bim) {
        auto var_idx_bim = variant_iter_of_bim - bim_file()->variants.cbegin();
        auto var_idx_scr = var_idxs_scr[var_idx_bim];
        auto score_of_ref = scores[var_idx_scr];
        auto sample_index = sample_iter - samples.cbegin();
        int genotype = bed->GetGenotype(var_idx_bim, sample_index);
        auto variant_in_scores = score_file()->variants[var_idx_scr];
        bool allele1_is_ref;
        if (IsMissingAllele(variant_in_scores.ref) ||
            IsMissingAllele(variant_iter_of_bim->allele1) ||
            IsMissingAllele(variant_iter_of_bim->allele2)) {
          continue;
        } else if (boost::iequals(variant_iter_of_bim->allele1,
                                  variant_in_scores.ref)) {
          allele1_is_ref = true;
        } else if (boost::iequals(variant_iter_of_bim->allele2,
                                  variant_in_scores.ref)) {
          allele1_is_ref = false;
        } else {
          GPLUS_LOG << "The reference allele of variant "
                    << variant_iter_of_bim->name << " is "
                    << variant_in_scores.ref
                    << " in score file, but the alleles of the variant in bim "
                       "file is "
                    << variant_iter_of_bim->allele1 << " and "
                    << variant_iter_of_bim->allele2 << ". It doesn't match.";
          exit(EXIT_FAILURE);
        }
        switch (genotype) {
          case 0b00:  // Homozygous for first allele in .bim file
            score_sum += allele1_is_ref ? score_of_ref + score_of_ref : 0;
            break;
          case 0b01:  // Missing genotype
            allele_count -= 2;
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

      // Print score to result file.
      float out_score = score_sum;
      if (is_weighted) {
        if (0 != allele_count)
          out_score /= allele_count;
        else
          out_score = 0.0f;
      }
      out_file << "\t" << out_score;
    }
    out_file << std::endl;
  }
}

}  // namespace gplus
