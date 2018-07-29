//
//  subcommand_list.h
//  gplus
//
//  Created by Zhixiang Zhu on 25/06/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#ifndef GPLUS_SUBCOMMAND_LIST_H_
#define GPLUS_SUBCOMMAND_LIST_H_

#include "base/subcommand.h"

namespace gplus {

Subcommand * const * GetSubcommandList(int * p_subcmd_cnt);

Subcommand * FindSubcommand(const char * name_or_alias);

}

#endif /* GPLUS_SUBCOMMAND_LIST_H_ */
