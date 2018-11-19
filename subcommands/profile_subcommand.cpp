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
using std::vector;
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

// temp variables
static const ScoreFile* scr_file;
static size_t trait_cnt;
static size_t var_cnt;
static const vector<BimFile::Variant>* vars_in_bim;
static int* var_idxs_in_scr;
static const vector<ScoreFile::Variant>* vars_in_scr;
static const char* genotype_ptr;
static size_t dim2_genotype_cnt;
static size_t dim2_byte_cnt;
static vector<vector<float>>* score_sums;
static size_t dim2_genotype_idx;
static vector<size_t>* allele_cnts;

namespace variant_major {

static inline void DoForRefIsAllele1(const vector<float>& scrs_of_a_var) {
  while (dim2_genotype_idx < dim2_genotype_cnt) {
    char byte = *genotype_ptr++;
    for (int bit_idx = 0;
         bit_idx < 8 && dim2_genotype_idx < dim2_genotype_cnt;
         bit_idx += 2, ++dim2_genotype_idx) {
      auto& scr_sums_of_a_sample = score_sums->at(dim2_genotype_idx);
      int genotype = (byte >> bit_idx) & 0b11;
      switch (genotype) {
        case 0b00:  // Homozygous for first allele in .bim file
          for (int trait_idx = 0; trait_idx < trait_cnt; ++trait_idx) {
            auto score = scrs_of_a_var[trait_idx];
            scr_sums_of_a_sample[trait_idx] += score + score;
          }
          break;
        case 0b01:  // Missing genotype
          allele_cnts->at(dim2_genotype_idx) -= 2;
          break;
        case 0b10:  // Heterozygous
          for (int trait_idx = 0; trait_idx < trait_cnt; ++trait_idx) {
            scr_sums_of_a_sample[trait_idx] += scrs_of_a_var[trait_idx];
          }
          break;
        case 0b11:  // Homozygous for second allele in .bim file
          break;
        default:
          assert(false);
      }
    }
  }
}

static void DoForRefIsAllele2(const vector<float>& scrs_of_a_var) {
  while (dim2_genotype_idx < dim2_genotype_cnt) {
    char byte = *genotype_ptr++;
    for (int bit_idx = 0;
         bit_idx < 8 && dim2_genotype_idx < dim2_genotype_cnt;
         bit_idx += 2, ++dim2_genotype_idx) {
      auto& scr_sums_of_a_sample = score_sums->at(dim2_genotype_idx);
      int genotype = (byte >> bit_idx) & 0b11;
      switch (genotype) {
        case 0b00:  // Homozygous for first allele in .bim file
          break;
        case 0b01:  // Missing genotype
          allele_cnts->at(dim2_genotype_idx) -= 2;
          break;
        case 0b10:  // Heterozygous
          for (int trait_idx = 0; trait_idx < trait_cnt; ++trait_idx) {
            scr_sums_of_a_sample[trait_idx] += scrs_of_a_var[trait_idx];
          }
          break;
        case 0b11:  // Homozygous for second allele in .bim file
          for (int trait_idx = 0; trait_idx < trait_cnt; ++trait_idx) {
            auto score = scrs_of_a_var[trait_idx];
            scr_sums_of_a_sample[trait_idx] += score + score;
          }
          break;
        default:
          assert(false);
      }
    }
  }
}

static void Execute() {
  for (size_t var_idx_in_bim = 0; var_idx_in_bim < var_cnt; ++var_idx_in_bim) {
    auto& var_in_bim = vars_in_bim->at(var_idx_in_bim);
    auto& var_idx_in_scr = var_idxs_in_scr[var_idx_in_bim];
    auto ref = vars_in_scr->at(var_idx_in_scr).ref;
    if (IsMissingAllele(ref) ||
        IsMissingAllele(var_in_bim.allele1) ||
        IsMissingAllele(var_in_bim.allele2)) {
      genotype_ptr += dim2_byte_cnt;
      continue;
    }
    
    const vector<float>& scrs_of_a_var = scr_file->score_rows[var_idx_in_scr];
    dim2_genotype_idx = 0;
    
    if (boost::iequals(var_in_bim.allele1, ref)) {
      DoForRefIsAllele1(scrs_of_a_var);
    } else if (boost::iequals(var_in_bim.allele2, ref)) {
      DoForRefIsAllele2(scrs_of_a_var);
    } else {
      GPLUS_LOG
      << "The reference allele of variant "
      << var_in_bim.name << " is " << ref
      << " in score file, but the alleles of the variant in bim file is "
      << var_in_bim.allele1 << " and "
      << var_in_bim.allele2 << ". It doesn't match.";
      exit(EXIT_FAILURE);
    }
  }
}

}  // namespace variant_major

void ProfileSubcommand::Execute() {
  scr_file = score_file();
  if (scr_file->variants.size() != bim_file()->variants.size()) {
    GPLUS_LOG << "The score file contains " << score_file()->variants.size()
              << " variant(s), but the bim file contains "
              << bim_file()->variants.size() << " variant(s).";
    exit(EXIT_FAILURE);
  }

  // Map variant indexes between the bim file and the score file.
  vars_in_bim = &bim_file()->variants;
  var_cnt = vars_in_bim->size();
  var_idxs_in_scr = new int[var_cnt];
  for (int var_idx_bim = 0; var_cnt > var_idx_bim; ++var_idx_bim) {
    var_idxs_in_scr[var_idx_bim] =
        score_file()->GetVariantIndex(vars_in_bim->at(var_idx_bim).name);
  }
  
  // Construct the container of output result.
  auto fam = fam_file();
  auto sample_cnt = fam->samples.size();
  score_sums = new vector<vector<float>>(sample_cnt);
  trait_cnt = scr_file->trait_names.size();
  for (auto& score_sums_of_a_sample : *score_sums) {
    score_sums_of_a_sample.resize(trait_cnt);
  }
  
  // Initialize temporary variables.
  auto bed = bed_file();
  genotype_ptr = bed->GetGenotypes();
  dim2_genotype_cnt = bed->GetDim2GenotypeCount();
  dim2_byte_cnt = bed->GetDim2ByteCount();
  vars_in_scr = &scr_file->variants;
  allele_cnts = new vector<size_t>(sample_cnt, var_cnt << 1);
  
  GPLUS_LOG << "Profiling scores.";
  auto timer_start = GetNow();

  // Sum up scores.
  if (bed->IsVariantMajor()) {
    variant_major::Execute();
  } else {
    GPLUS_LOG << "Profile is not implemented for sample major bed yet.";
    exit(1);
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
  auto& score_names = score_file()->trait_names;
  for (auto score_names_iter = score_names.cbegin();
       score_names_iter != score_names.cend(); ++score_names_iter) {
    out_file << "\tSCORE." << *score_names_iter;
  }
  out_file << std::endl;

  const bool is_weighted = 0 == prog_args()->count("no-weight");

  // Print score results of each sample.
  const auto& samples = fam->samples;
  for (size_t sample_idx = 0; sample_idx < sample_cnt; ++sample_idx) {
    const auto& sample = samples[sample_idx];
    out_file << sample.fam_id << "\t" << sample.iid;
    const auto& scr_sums_of_a_sample = score_sums->at(sample_idx);
    float allele_cnt = allele_cnts->at(sample_idx);
    for (auto scr_sums_iter = scr_sums_of_a_sample.cbegin();
         scr_sums_iter != scr_sums_of_a_sample.cend(); ++scr_sums_iter) {
      // Print score to result file.
      float scr_result = *scr_sums_iter;
      if (is_weighted) {
        if (0 != allele_cnt)
          scr_result /= allele_cnt;
        else
          scr_result = 0.0f;
      }
      out_file << "\t" << scr_result;
    }
    out_file << std::endl;
  }

  GPLUS_LOG << "Profiling complete. It took "
            << PrintDurationSince(timer_start) << " to profile scores.";
}

}  // namespace gplus
