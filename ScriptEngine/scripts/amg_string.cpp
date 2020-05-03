#include "amg_string.h"

namespace amg
{
    namespace string
    {
        bool ToInt(const std::string& str, int& integer)
        {
            auto result = true;

            try {
                integer = std::stoi(str);
            }
            catch (...) {
                result = false;
            }

            return result;
        }

        std::vector<std::string> Split(const std::string& str, const std::string& delimiter)
        {
            size_t first = 0;
            auto last = str.find_first_of(delimiter);
            std::vector<std::string> split;

            while (first < str.size()) {
                const std::string subStr(str, first, last - first);

                split.push_back(subStr);

                first = last + delimiter.length();
                last = str.find_first_of(delimiter, first);

                if (last == std::string::npos) {
                    last = str.size();
                }
            }

            return split;
        }
    }
}
