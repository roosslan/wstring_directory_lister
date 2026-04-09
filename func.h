#pragma once

#include "stdafx.h"

const std::string lister_version = "1.12.2.26";

#define LOG_SAVE BOOST_LOG_SEV(boost::log::trivial::logger::get(), boost::log::trivial::severity_level::trace)	\
	<< boost::log::add_value("Line", __LINE__)

const std::wstring uudecode(const std::string& input);

namespace boost_logger
{
	void init_logging();
	void log_formatter(logging::record_view const& rec, logging::formatting_ostream& strm);
	std::string path_to_filename(const std::string& path);
};