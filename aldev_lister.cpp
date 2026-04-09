#include "stdafx.h"
#include "func.h"

#pragma comment(lib, "WtsApi32.lib")

namespace fs = std::filesystem;

int main() {
    boost_logger::init_logging();

    std::cout << "AlDev listener v" << lister_version << "\n";

    try {
        // Iterate recursively through the directory and its subdirectories
        for (const auto& entry : fs::recursive_directory_iterator("C:\\ProgramData\\Autodesk\\Revit Server 2023\\Projects\\Projects")) {
            std::string path_string{ entry.path().u8string() };
            boost::algorithm::to_lower(path_string);            
            if (fs::is_directory(entry) && path_string.substr(path_string.size() - 3) == "rvt"){
                boost::replace_all(path_string, "c:\\programdata\\autodesk\\revit server 2023\\projects\\projects", "RSN://ald-vm-revit01/Projects");
                boost::replace_all(path_string, "\\", "/");

                LOG_SAVE << "INSERT INTO[dbo].[rvt_night_list]([rvt_path], [rvt_name]) VALUES ('" << path_string << "', '" << entry.path().filename().u8string() << "')";
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << '\n';
    }

    std::wstring parameters = L"-S ald-vdi-wg0002 -d bimalde -U bimalde -i C:\\rasa\\aldev_lister.sql -o C:\\rasa\\mssql.log -P ";
	parameters += uudecode("2S!R,&)K86LP<C!B:V$`");

    ShellExecute(nullptr, L"open", L"sqlcmd.exe", parameters.c_str(), nullptr, SW_SHOWNORMAL);
    return 0;
}
