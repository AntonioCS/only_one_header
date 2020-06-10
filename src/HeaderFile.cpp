
#include <fstream>
#include <algorithm> //std::sort

#include "HeaderFile.h"

namespace OnlyOneHeader {

    enum class ExtractState
    {
        BOM_CHECK,
        READ_CODE
    };

    bool HeaderFile::isInLocalIncludes(const HeaderFile& hf) const
    {
        return isInLocalIncludes(hf.filename());
    }

    bool HeaderFile::isInLocalIncludes(std::string_view file) const
    {
        return (std::find(m_local_includes.begin(), m_local_includes.end(), file) != m_local_includes.end());
    }

    void HeaderFile::extractData()
    {
        std::fstream f{path(), std::ios::in};

        if (!f.is_open()) {
            throw std::runtime_error{ "Unable to open file: " + path() };
        }

        ExtractState state{ ExtractState::BOM_CHECK };
        std::string line{};

        while (std::getline(f, line)) {
            switch (state) {
                case ExtractState::BOM_CHECK:
                {
                    //detect BOM
                    if (startsWith(line, m_bom_string)) {
                        line.erase(0, m_bom_string.size()); //simple way to get rid of this thanks to Gaurav from cpplang slack
                    }
                    state = ExtractState::READ_CODE;
                }
                [[fallthrough]];
                case ExtractState::READ_CODE:
                {
                    if (startsWith(line, "#include ")) {
                        extractIncludeFilename(line);
                    }
                    else if (!startsWith(line, "#pragma once") && !line.empty()) {
                        if (m_remove_comments) {
                            auto it = std::find_if(line.begin(), line.end(), [](int c) { return !std::isspace(c); });
                            if (it != line.end() && *it == '/' && *(++it) == '/')
                                continue;
                        }

                        m_contents += line + '\n';
                    }
                }
                break;
            }
        }
    }

    void HeaderFile::extractIncludeFilename(const std::string& line)
    {
        auto f = line.find('"');

        if (f != std::string::npos) {
            auto f2 = line.find('"', f + 1);
            if (f2 != std::string::npos) {
                m_local_includes.push_back(line.substr(f + 1, f2 - f - 1));
            }
        }

        else {
            f = line.find('<');
            if (f != std::string::npos) {
                auto f2 = line.find('>');
                if (f2 != std::string::npos) {
                    m_global_includes.push_back(line.substr(f + 1, f2 - f - 1));
                }
            }
        }
    }

    bool HeaderFile::startsWith(std::string_view str, std::string_view startsWithThis) const
    {
        auto v = str.rfind(startsWithThis, 0);
        return (v == 0);
    }
}
