#include "func.h"

void boost_logger::init_logging()
{
	std::string roaming_directory;

	char* appdata = nullptr;
	size_t sz = 0;
	if (_dupenv_s(&appdata, &sz, "APPDATA") == 0 && appdata != nullptr)
	{
		/* Convert the Windows path type to a C++ path */
		roaming_directory = appdata;
		free(appdata);
	}

	boost::log::add_common_attributes();

	const auto console_sink = boost::log::add_console_log(std::clog);
	console_sink->set_formatter(&log_formatter);
	logging::core::get()->add_sink(console_sink);

	const auto fs_sink = boost::log::add_file_log(
		boost::log::keywords::file_name = "C:\\rasa\\_lister.sql",
		keywords::format = "% Message %",
		boost::log::keywords::rotation_size = 10 * 1024 * 1024,
		boost::log::keywords::min_free_space = 30 * 1024 * 1024,
		boost::log::keywords::open_mode = std::ios_base::out | std::ios_base::trunc);

	fs_sink->locked_backend()->auto_flush(true);
	LOG_SAVE << "USE[lister_db]";
	LOG_SAVE << "GO";
	LOG_SAVE << "TRUNCATE TABLE rvt_night_list;";
}

void boost_logger::log_formatter(logging::record_view const& rec, logging::formatting_ostream& strm)
{
	strm << rec[expr::smessage];
}

const std::wstring uudecode(const std::string& input) {
	std::wstring decoded;
	int len = input.length();

	for (int i = 0; i < len; i += 4) {
		unsigned int combined = 0;
		for (int j = 0; j < 4; ++j) {
			unsigned char c = input[i + j];
			c = (c == '`') ? 0 : (c - 32) & 0x3F;  // decode character
			combined = (combined << 6) | c;
		}

		decoded += (combined >> 16) & 0xFF;
		if (i + 2 < len) decoded += (combined >> 8) & 0xFF;
		if (i + 3 < len) decoded += combined & 0xFF;
	}

	return decoded;
}

std::string get_model_identity(const std::wstring& xml_path)
{
	std::ifstream ifs(xml_path + L"\\Model.rvt", std::ios::binary);
	if (!ifs.is_open())
		return "";

	/* грузим файл целиком */
	std::stringstream buffer;
	buffer << ifs.rdbuf();
	std::string xml = buffer.str();

	std::regex rgx(R"(<ModelIdentity>(.*?)</ModelIdentity>)", std::regex::icase);
	std::smatch match;

	if (std::regex_search(xml, match, rgx))
	{
		ifs.close();
		return match[1].str();
	}

	return "";
}
