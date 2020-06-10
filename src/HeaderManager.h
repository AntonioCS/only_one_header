#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "HeaderFile.h"

namespace OnlyOneHeader {

    class HeaderManager
    {
        std::vector<HeaderFile> m_all_hf{};
        std::string m_dir_path{};
        std::unordered_map<std::string, HeaderFile*> m_header_filename_connection;
        std::unordered_map<std::string, uint16_t> m_global_includes;
        std::string m_output_file{ "all_in_one.h" };
        std::vector<std::string> m_unused_hf{};
        double m_elapsed_process{};
        double m_elapsed_ouput{};

        struct {
            bool add_separators{ true };
            bool add_timestamp{ true };
            bool remove_comments{ false };
            bool silent{ false };
        } m_options;
    public:
        HeaderManager(const std::string& path) : m_dir_path(path) {}

        void process();
        void output(const std::string& output_file);

        void setOptionAddSeparatorState(bool state) { m_options.add_separators = state; }
        void setOptionAddTimestampState(bool state) { m_options.add_timestamp = state; }
        void setOptionRemoveCommentsState(bool state) { m_options.remove_comments = state; }
        void setOptionSilentState(bool state) { m_options.silent = state; }
    private:
        void displayInfo() const;
        void grabAllHeaderFiles();
        void removeUnIncludedFiles();
        void grabAllGlobalIncludesFromHeaderFile(const HeaderFile& hf);
        void createFileNameHeaderFileConnection();
        void order();
        void decreaseAllIncluded(HeaderFile& hf);
    };
}
