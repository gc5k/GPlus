//
//  log.cpp
//  gplus
//
//  Created by Zhixiang Zhu on 04/08/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//

#include "util/log.h"

#include <iostream>
#include <fstream>

#include "third_party/boost/date_time/posix_time/posix_time_types.hpp"
#include "third_party/boost/core/null_deleter.hpp"
#include "third_party/boost/smart_ptr/shared_ptr.hpp"
#include "third_party/boost/smart_ptr/make_shared_object.hpp"
#include "third_party/boost/log/sinks/sync_frontend.hpp"
#include "third_party/boost/log/sinks/text_ostream_backend.hpp"
#include "third_party/boost/log/attributes.hpp"
#include "third_party/boost/log/utility/setup/common_attributes.hpp"
#include "third_party/boost/log/support/date_time.hpp"
#include "third_party/boost/log/expressions.hpp"
#include "third_party/boost/log/expressions/formatters/date_time.hpp"
#include "util/program_options.h"


namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;

namespace gplus {

void InitLogging() {
  using boost::posix_time::ptime;
  auto formatter = expr::format("[%1%] %2%")
      % expr::format_date_time<ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S")
      % expr::message;

  // Construct the sink
  typedef sinks::synchronous_sink<sinks::text_ostream_backend> text_sink;

  boost::shared_ptr<text_sink> sink = boost::make_shared<text_sink>();

  // Add sink of console. We have to provide an empty deleter to avoid
  // destroying the global stream object.
  boost::shared_ptr<std::ostream> stream(&std::clog, boost::null_deleter());
  sink->locked_backend()->add_stream(stream);

  // Add a stream to write log to
  auto out_file_name = GetOptionValue<std::string>("out");
  auto log_name = out_file_name + ".log";
  auto log_stream = boost::make_shared<std::ofstream>(log_name);
  sink->locked_backend()->add_stream(log_stream);

  sink->set_formatter(formatter);

  // Register the sink in the logging core
  logging::core::get()->add_sink(sink);

  logging::add_common_attributes();
}

}  // namespace gplus
