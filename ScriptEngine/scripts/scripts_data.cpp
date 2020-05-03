#include "scripts_data.h"
#include "amg_string.h"
#include "picojson.h"
#include <windows.h>
#include <fstream>

namespace {
    constexpr auto EMPTY_STR = "";
    constexpr auto EMPTY_WSTR = L"";
    constexpr auto DELIMITER = ", ";
}

namespace amg
{
    bool ScriptsData::LoadJson(const char* path)
    {
        // UTF-8 BOM無し Json file
        std::ifstream ifs(path);
        picojson::value json_value;

        ifs >> json_value;

        const auto err = picojson::get_last_error();

        if (!err.empty()) {
            return false;
        }

        if (scripts == nullptr) {
            scripts.reset(new std::vector<std::string>());
        }

        auto root = json_value.get<picojson::array>();
        auto object = root[0].get<picojson::object>();
        const auto array = object["scripts"].get<picojson::array>();

        // UTF-8 -> Wide(UTF-16) -> MultiByte と文字コードを変換しながらスクリプト文字を取得
        for (auto i = array.begin(); i != array.end(); ++i) {
            const auto utf8 = (*i).get<std::string>();
            const auto utf16 = ConvertUTF8ToWide(utf8);
            const auto mbs = ConvertWideToMultiByte(utf16);

            scripts->push_back(mbs);
        }

        return true;
    }

    std::wstring ScriptsData::ConvertUTF8ToWide(const std::string& utf8) const
    {
        if (utf8.empty()) {
            return EMPTY_WSTR;
        }

        const auto in_length = static_cast<int>(utf8.length());
        const auto out_length = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), in_length, 0, 0);

        if (out_length <= 0) {
            return EMPTY_WSTR;
        }

        std::vector<wchar_t> buffer(out_length);

        MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), in_length, &(buffer[0]), out_length);

        std::wstring utf16(buffer.begin(), buffer.end());

        return std::move(utf16);
    }

    std::string ScriptsData::ConvertWideToMultiByte(const std::wstring& utf16) const
    {
        if (utf16.empty()) {
            return EMPTY_STR;
        }

        const auto in_length = static_cast<int>(utf16.length());
        const auto out_length = WideCharToMultiByte(CP_ACP, 0, utf16.c_str(), in_length, 0, 0, 0, 0);

        if (out_length <= 0) {
            return EMPTY_STR;
        }

        std::vector<char> buffer(out_length);

        WideCharToMultiByte(CP_ACP, 0, utf16.c_str(), in_length, &(buffer[0]), out_length, 0, 0);

        std::string mbs(buffer.begin(), buffer.end());

        return std::move(mbs);
    }

    unsigned int ScriptsData::GetScriptNum()  const
    {
        if (scripts == nullptr) {
            return 0;
        }

        return static_cast<unsigned int>(scripts->size());
    }

    std::string ScriptsData::GetScriptLine(const unsigned int index) const
    {
        const auto size = GetScriptNum();

        if (size <= 0 || size <= index) {
            return EMPTY_STR;
        }

        return (*scripts)[index];
    }

    std::vector<std::string> ScriptsData::GetScript(const unsigned int index) const
    {
        const auto line = GetScriptLine(index);

        return string::Split(line, DELIMITER);
    }
}
