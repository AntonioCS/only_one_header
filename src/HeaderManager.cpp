#include <fstream>
#include "HeaderManager.h"
#include <chrono>
#include <iostream>

namespace OnlyOneHeader {


    double getMsCount(const std::chrono::steady_clock::time_point& begin)
    {        
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count() / 1000;
    }

    void HeaderManager::process()
    {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        grabAllHeaderFiles();
        removeUnIncludedFiles(); //this must come before createFileNameHeaderFileConnection()
        createFileNameHeaderFileConnection();
        order();

        m_elapsed_process = getMsCount(begin);
    }

    void HeaderManager::output(const std::string& output_file)
    {
        m_output_file = output_file;
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        std::fstream output{m_output_file, std::ios::out};

        if (output.is_open()) {

            output << "#pragma once\n\n";

            if (m_options.add_timestamp) {
                auto t = std::time(nullptr);
                auto tm = *std::localtime(&t);

                output << "////////////////////////////////////////\n";
                output << "//AUTO GENERATED FILE - DO NOT EDIT\n";
                output << "//Generated on " << std::put_time(&tm, "%Y-%m-%d at %H:%M:%S") << "\n";
                output << "////////////////////////////////////////\n\n";
            }

            for (const auto& [globalInc, v] : m_global_includes) {
                output << "#include <" << globalInc << ">\n";
            }

            output << '\n';

            for (const auto& hf : m_all_hf) {
                if (m_options.add_separators) output << "/*** " << hf.filename() << " START ***/\n";
                output << hf.getContents();
                if (m_options.add_separators) output << "/*** " << hf.filename() << " END ***/\n";
            }

            output.close();

            m_elapsed_ouput = getMsCount(begin);
            if (!m_options.silent) displayInfo();
        }
        else {
            throw std::runtime_error{"Unable to open output file"};
        }
    }

    void HeaderManager::grabAllHeaderFiles()
    {
        if (!fs::is_directory(m_dir_path)) {
            throw std::runtime_error{ "Invalid directory: " + m_dir_path };
        }

        for (const auto& p : fs::directory_iterator(m_dir_path)) {
            if (p.path().extension() == ".h" || p.path().extension() == ".hpp") {
                auto hf = m_all_hf.emplace_back(p.path(), m_options.remove_comments);

                if (hf.hasGlobalIncludes()) {
                    grabAllGlobalIncludesFromHeaderFile(hf);
                }
            }
        }
    }

    void HeaderManager::removeUnIncludedFiles()
    {
        std::vector<HeaderFile> included_files;
        for (const auto& hf : m_all_hf) {
            if (!hf.hasLocalIncludes()) {
                bool was_not_included{true};

                //since this file doesn't include any other local file
                //I'm looping all the other include files and checking them to see if they include this file
                for (const auto& hf2 : m_all_hf) {
                    if (hf2.isInLocalIncludes(hf.filename())) {
                        included_files.push_back(hf);
                        was_not_included = false;
                        break;
                    }
                }

                if (was_not_included) {
                    m_unused_hf.emplace_back(hf.filename().data());
                }
            }
            else {
                included_files.push_back(hf);
            }
        }

        m_all_hf = included_files;
    }

    void HeaderManager::grabAllGlobalIncludesFromHeaderFile(const HeaderFile& hf)
    {
        for (const auto& gi : hf.allGlobalIncludes()) {
            m_global_includes[gi] += 1;
        }
    }

    void HeaderManager::createFileNameHeaderFileConnection()
    {
        for (auto& hf : m_all_hf) {
            m_header_filename_connection[hf.filename()] = &hf;
        }
    }

    void HeaderManager::order()
    {
        for (auto& hf : m_all_hf) {
            if (hf.hasLocalIncludes()) {
                decreaseAllIncluded(hf);
            }
        }

        std::sort(
            m_all_hf.begin(),
            m_all_hf.end(),
            [](const HeaderFile& a, const HeaderFile& b)
            {
                return a.position_value < b.position_value;
            }
        );
    }

    void HeaderManager::decreaseAllIncluded(HeaderFile& hf)
    {
        constexpr std::size_t order_decrease_amount{100};

        for (const auto& infile : hf.allLocalIncludes()) {
            auto initv = m_header_filename_connection[infile]->position_value;

            while (m_header_filename_connection[infile]->position_value >= hf.position_value) {
                m_header_filename_connection[infile]->position_value -= order_decrease_amount;
            }

            if (
                m_header_filename_connection[infile]->position_value != initv &&
                m_header_filename_connection[infile]->hasLocalIncludes()
            ) {
                decreaseAllIncluded(*m_header_filename_connection[infile]);
            }
        }
    }

    void HeaderManager::displayInfo() const
    {
        std::cout << "Time for procesing: " << m_elapsed_process << "ms\n";
        std::cout << "Time for outputing: " << m_elapsed_ouput << "ms\n";
        std::cout << "Total time: " << (m_elapsed_ouput + m_elapsed_process) << "ms\n";
        std::cout << "Total global headers: " << m_global_includes.size() << '\n';
        std::cout << "File - Times it was included\n";
        for (const auto& [globalInc, v] : m_global_includes) {
            std::cout << globalInc << " - " << v << '\n';
        }
        std::cout << "Total local headers: " << m_header_filename_connection.size() + m_unused_hf.size() << '\n';
        std::cout << "Total headers not included: " << m_unused_hf.size() << '\n';
    }

}
