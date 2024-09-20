#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>
#include <utility>
#include <vector>

class csvfile;

inline static csvfile& endrow(csvfile& file);
inline static csvfile& flush(csvfile& file);

class csvfile
{
    std::ofstream fs_;
    bool is_first_;
    const std::string separator_;
    const std::string escape_seq_;
    const std::string special_chars_;
public:
    explicit csvfile(const std::string& filename, std::string  separator = ";")
        : fs_()
        , is_first_(true)
        , separator_(std::move(separator))
        , escape_seq_("\"")
        , special_chars_("\"")
    {
        fs_.exceptions(std::ios::failbit | std::ios::badbit);
        fs_.open(filename);
    }

    ~csvfile()
    {
        flush();
        fs_.close();
    }

    void flush()
    {
        fs_.flush();
    }

    void endrow()
    {
        fs_ << std::endl;
        is_first_ = true;
    }

    csvfile& operator << ( csvfile& (* val)(csvfile&))
    {
        return val(*this);
    }

    csvfile& operator << (const char * val)
    {
        return write(escape(val));
    }

    csvfile& operator << (const std::string & val)
    {
        return write(escape(val));
    }

    static std::tuple<std::vector<double>, std::vector<double>, std::vector<double>> ReadCSV(const std::string& file_path) {
        std::vector<double> times;
        std::vector<double> input_data;
        std::vector<double> pid_output;

        std::ifstream file(file_path);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << file_path << std::endl;
            return {times, input_data, pid_output};
        }

        std::string line;
        while (std::getline(file, line)) {
            std::stringstream line_stream(line);
            std::string cell;
            std::vector<double> row_data;

            while (std::getline(line_stream, cell, ',')) {
                row_data.push_back(std::stod(cell));
            }

            if (row_data.size() >= 3) { // Ensure there are at least 3 columns
                times.push_back(row_data[0]);
                input_data.push_back(row_data[1]);
                pid_output.push_back(row_data[2]);
            }
        }
        file.close();
        return {times, input_data, pid_output};
    }

    template<typename T>
    csvfile& operator << (const T& val)
    {
        return write(val);
    }

private:
    template<typename T>
    csvfile& write (const T& val)
    {
        if (!is_first_)
        {
            fs_ << separator_;
        }
        else
        {
            is_first_ = false;
        }
        fs_ << val;
        return *this;
    }

    std::string escape(const std::string & val) const {
        std::ostringstream result;
        result << '"';
        std::string::size_type to, from = 0u;
        const std::string::size_type len = val.length();
        while (from < len &&
               std::string::npos != (to = val.find_first_of(special_chars_, from)))
        {
            result << val.substr(from, to - from) << escape_seq_ << val[to];
            from = to + 1;
        }
        result << val.substr(from) << '"';
        return result.str();
    }
};


static csvfile& endrow(csvfile& file)
{
    file.endrow();
    return file;
}

static csvfile& flush(csvfile& file)
{
    file.flush();
    return file;
}