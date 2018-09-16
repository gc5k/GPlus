//
//  log.hpp
//  gplus
//
//  Created by Zhixiang Zhu on 04/08/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#ifndef GPLUS_UTIL_LOG_H_
#define GPLUS_UTIL_LOG_H_

#include <chrono>
#include <string>

#include "third_party/boost/log/sources/logger.hpp"
#include "third_party/boost/log/sources/global_logger_storage.hpp"
#include "third_party/boost/log/sources/record_ostream.hpp"

namespace gplus {

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(logger, boost::log::sources::logger)

#define GPLUS_LOG BOOST_LOG(gplus::logger::get())

void InitLogging();

std::chrono::steady_clock::time_point GetNow();
std::string PrintDurationSince(std::chrono::steady_clock::time_point tp);

}  // namespace gplus

#endif  // GPLUS_UTIL_LOG_H_
