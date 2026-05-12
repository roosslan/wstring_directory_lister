#pragma once

#include "stdafx.h"

const std::string lister_version = "2.12.5.26";

#define LOG_SAV BOOST_LOG_SEV(boost::log::trivial::logger::get(), boost::log::trivial::severity_level::trace)
#define LOG_SAVE BOOST_LOG_SEV(boost::log::trivial::logger::get(), boost::log::trivial::severity_level::trace)	\
	<< boost::log::add_value("Line", __LINE__)

const std::wstring uudecode(const std::string& input);
std::string get_model_identity(const std::wstring& xml_path);

namespace boost_logger
{
	void init_logging();
	void log_formatter(logging::record_view const& rec, logging::formatting_ostream& strm);	
};