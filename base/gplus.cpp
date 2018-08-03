//
//  gplus.cpp
//  gplus
//
//  Created by Zhixiang Zhu on 18/06/2018.
//  Copyright © 2018 Guobo Chen. All rights reserved.
//
#include <sys/types.h>
#include <sys/stat.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "base/exception.h"
#include "base/subcommand_list.h"
#include "util/executable_path.h"
#include "util/program_options.h"
#include "third_party/boost/log/trivial.hpp"
#include "third_party/boost/log/utility/setup/common_attributes.hpp"
#include "third_party/boost/log/utility/setup/console.hpp"
#include "third_party/boost/log/utility/setup/file.hpp"
#include "third_party/boost/program_options/parsers.hpp"
#include "third_party/crashpad/client/crashpad_client.h"
#include "third_party/crashpad/client/crash_report_database.h"
#include "third_party/crashpad/client/settings.h"

namespace po = boost::program_options;
namespace logging = boost::log;

namespace gplus {

static bool StartCrashHandler() {
  std::map<std::string, std::string> annotations;
  std::vector<std::string> arguments;
  crashpad::CrashpadClient client;
  bool rc;
  /*
   * ENSURE THIS VALUE IS CORRECT.
   *
   * This is the directory you will use to store and queue crash data.
   */
#if defined(OS_WIN)
  std::wstring db_path(L"./crashpad/db");
#else
  std::string db_path("./crashpad/db");
#endif
  /*
   * ENSURE THIS VALUE IS CORRECT.
   *
   * Crashpad has the ability to support crashes both in-process and out-of-process.
   * The out-of-process handler is significantly more robust than traditional in-process
   * crash handlers. This path may be relative.
   */
#if defined(OS_WIN)
#ifdef _WIN64
  std::wstring handler_path(L"./crashpad/win64_tools/crashpad_handler");
#else
  std::wstring handler_path(L"./crashpad/win32_tools/crashpad_handler");
#endif  // _WIN64
#else
  std::string handler_path("./crashpad/mac_tools/crashpad_handler");
#endif
  /*
   * YOU MUST CHANGE THIS VALUE.
   *
   * This should point to your server dump submission port (labeled as "http/writer"
   * in the listener configuration pane. Preferrably, the SSL enabled port should
   * be used. If Backtrace is hosting your instance, the default port is 6098.
   */
  std::string url("");
  /*
   * THE FOLLOWING ANNOTATIONS MUST BE SET.
   *
   * Backtrace supports many file formats. Set format to minidump so it knows
   * how to process the incoming dump.
   */
  annotations["format"] = "minidump";
  /*
   * REMOVE THIS FOR ACTUAL BUILD.
   *
   * We disable crashpad rate limiting for this example.
   */
  arguments.push_back("--no-rate-limit");

  base::FilePath db(db_path);
  base::FilePath handler(handler_path);

  std::unique_ptr<crashpad::CrashReportDatabase>
  database = crashpad::CrashReportDatabase::Initialize(db);

  if (database == nullptr || database->GetSettings() == NULL)
    return false;
  /* Enable automated uploads. */
  database->GetSettings()->SetUploadsEnabled(true);
  rc = client.StartHandler(handler,
                           db,
                           db,
                           url,
                           annotations,
                           arguments,
                           true,
                           true);
  if (rc == false)
    return false;
#if defined(OS_WIN) || DOXYGEN
  /* Optional, wait for Crashpad to initialize. */
  rc = client.WaitForHandlerStart(INFINITE);
  if (rc == false)
    return false;
#endif
  return true;
}

static void InitLogging() {
  logging::add_console_log();
  logging::add_file_log(
      // file name pattern
      logging::keywords::file_name = "gplus.log",
      // log record format
      logging::keywords::format = "[%TimeStamp%]: %Message%");
  logging::add_common_attributes();
}

}  // namespace gplus

int main(int argc, const char* argv[]) {
  if (argc < 2) {
    std::cout << "Type '" << argv[0] << " help' for usage." << std::endl;
    return 0;
  }
  gplus::kArgv0 = argv[0];
  gplus::InitLogging();
  gplus::StartCrashHandler();
  auto subcmd_name = argv[1];
  auto subcmd = gplus::FindSubcommand(subcmd_name);
  if (nullptr == subcmd) {
    BOOST_LOG_TRIVIAL(error)
    << "No subcommand named as '" << subcmd_name << "'." << std::endl;
    return 1;
  }
  try {
    // Parse and store the command-line arguments.
    std::unique_ptr<const char *[]> subcmd_argv(new const char *[argc - 1]);
    std::copy(argv + 1, argv + argc, subcmd_argv.get());
    po::options_description opts_desc;
    subcmd->AddOptionsDesc(&opts_desc);
    gplus::PositionalOptionsDesc pd;
    subcmd->AddPositionalOptionsDesc(&pd);
    auto parser = po::command_line_parser(argc - 1, subcmd_argv.get());
    auto parsed_options = parser.options(opts_desc).positional(pd).run();
    auto& prog_args = gplus::GetSpecifiedOptions();
    po::store(parsed_options, prog_args);
    po::notify(prog_args);
    
    subcmd->Execute();
  } catch (const std::exception& e) {
    auto gplus_exception = dynamic_cast<const gplus::Exception*>(&e);
    auto msg = gplus_exception == nullptr ?
        e.what() : gplus::GetFormattedExceptionMessage(*gplus_exception);
    BOOST_LOG_TRIVIAL(error) << msg << std::endl;
  }

  return 0;
}
