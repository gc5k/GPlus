//
//  log.hpp
//  gplus
//
//  Created by Zhixiang Zhu on 04/08/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#ifndef GPLUS_UTIL_LOG_H_
#define GPLUS_UTIL_LOG_H_

#include "third_party/boost/log/sources/logger.hpp"
#include "third_party/boost/log/sources/global_logger_storage.hpp"

namespace gplus {

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(logger, boost::log::sources::logger)

#define GPLUS_LOG BOOST_LOG(gplus::logger::get())

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(
    diagnostic_logger, boost::log::sources::logger)
  
#define GPLUS_DIAGNOSTIC_LOG BOOST_LOG(gplus::diagnostic_logger::get())

void InitLogging();

}  // namespace gplus

#endif  // GPLUS_UTIL_LOG_H_
