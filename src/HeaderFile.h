#pragma once

#include <filesystem>
#include <vector>
#include <string>

namespace OnlyOneHeader {

    namespace fs = std::filesystem;

    class HeaderFile
    {
        fs::path m_path{};
        std::vector<std::string> m_local_includes{};
        std::vector<std::string> m_global_includes{};
        std::string m_filename{};
        std::string m_contents{};
        inline static const std::string m_bom_string{ "\xEF\xBB\xBF" };
        bool m_remove_comments{ false };

    public:
        std::size_t position_value{ 9999 };

        HeaderFile(const fs::path& p, bool remove_comments = false) : m_path(p), m_filename(p.filename().string()), m_remove_comments(remove_comments)
        {
            extractData();
        }

        [[nodiscard]] const std::vector<std::string>& allLocalIncludes() const
        {
            return m_local_includes;
        }

        [[nodiscard]] const std::vector<std::string>& allGlobalIncludes() const
        {
            return m_global_includes;
        }

        [[nodiscard]] bool isInLocalIncludes(const HeaderFile& hf) const;

        [[nodiscard]] bool isInLocalIncludes(std::string_view file) const;

        [[nodiscard]] std::string filename() const
        {
            return m_filename;
        }

        void extractData();

        [[nodiscard]] std::string getContents() const
        {
            return m_contents;
        }

        [[nodiscard]] std::string path() const
        {
            return m_path.string();
        }

        [[nodiscard]] bool hasLocalIncludes() const
        {
            return !m_local_includes.empty();
        }

        [[nodiscard]] bool hasGlobalIncludes() const
        {
            return !m_global_includes.empty();
        }

    private:

        void extractIncludeFilename(const std::string& line);
        [[nodiscard]] bool startsWith(std::string_view str, std::string_view startsWithThis) const;
    };

}
