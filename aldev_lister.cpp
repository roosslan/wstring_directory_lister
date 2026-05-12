#include "stdafx.h"
#include "func.h"

#pragma comment(lib, "WtsApi32.lib")

namespace fs = std::filesystem;

int main() {
    boost_logger::init_logging();    

    std::cout << "AlDev lister v" << lister_version << "\n";
    std::string model_identity = "";

    /* т.к. у нас обработка кириллических имён файлов происходит на английской версии Windows Server, юзаем UTF-16 */
    _setmode(_fileno(stdout), _O_U16TEXT);

    try {
        /* Iterate recursively through the directory and its subdirectories */
        for (const auto& entry : fs::recursive_directory_iterator("C:\\ProgramData\\Autodesk\\Revit Server 2023\\Projects\\Projects")) {
            std::string path_string{ entry.path().u8string() };

            auto rvt_extension = path_string.substr(path_string.size() - 3);
            boost::algorithm::to_lower(rvt_extension);

            if (fs::is_directory(entry) && rvt_extension == "rvt") {                
                model_identity = get_model_identity(entry.path().wstring());
                boost::replace_all(path_string, "C:\\ProgramData\\Autodesk\\Revit Server 2023\\Projects\\Projects", "RSN://ald-vm-revit01/Projects");
                boost::replace_all(path_string, "\\", "/");

                LOG_SAVE << "INSERT INTO[dbo].[rvt_night_list]([rvt_path], [rvt_name], [guid]) VALUES ('" << path_string << "', '" << entry.path().filename().u8string() << "', '" << model_identity << "')";
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << '\n';
    }

    std::wstring parameters = L"-S ald-vdi-wg0002 -d bimalde -U bimalde -i C:\\rasa\\aldev_lister.sql -o C:\\rasa\\mssql.log -P ";
	parameters += uudecode("uue_пароль_из_argv");

    ShellExecute(nullptr, L"open", L"sqlcmd.exe", parameters.c_str(), nullptr, SW_SHOWNORMAL);
    return 0;
}
