#pragma once

#include <vector>
#include <string>

namespace amg
{
    namespace string
    {
        bool ToInt(const std::string& str, int& integer);
        std::vector<std::string> Split(const std::string& str, const std::string& delimiter);
    }
}
