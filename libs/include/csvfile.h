#pragma once
#include <chrono>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <tuple>
#include <utility>
#include <vector>
#include <algorithm>

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
    std::vector<double> time_data;
    std::vector<double> input_data;
    std::vector<double> pid_data;

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

    static void read(const std::string& filename, std::vector<double>& time_data, std::vector<double>& input_data, std::vector<double>& pid_data) {
        std::ifstream file(filename);
        std::string line;

        // Check if the file is open
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return;
        } else {
            std::cout << "File opened successfully: " << filename << std::endl;
        }

        // Skip the header row
        if (std::getline(file, line)) {
            std::cout << "Header: " << line << std::endl;
        } else {
            std::cerr << "Failed to read header or file is empty." << std::endl;
            return;
        }

        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string time_str, input_str, pid_str;

            if (std::getline(ss, time_str, ',') &&
                std::getline(ss, input_str, ',') &&
                std::getline(ss, pid_str, ',')) {

                double input = std::stod(input_str);
                double pid = std::stod(pid_str);

                // Manually parse the time string
                int hours, minutes, seconds, milliseconds;
                char dot;

                time_str.erase(std::remove(time_str.begin(), time_str.end(), '\"'), time_str.end());

                std::sscanf(time_str.c_str(), "%d:%d:%d%c%d", &hours, &minutes, &seconds, &dot, &milliseconds);

                std::cout<<"Original time: "<< time_str <<std::endl;

                int time_in_milliseconds = (hours * 3600 + minutes * 60 + seconds) * 1000 + milliseconds;

                std::cout << "Parsed time: " << time_in_milliseconds << " ms, input: " << input << ", pid: " << pid << std::endl;

                time_data.push_back(time_in_milliseconds);
                input_data.push_back(input);
                pid_data.push_back(pid);
            } else {
                std::cerr << "Failed to parse line: " << line << std::endl;
            }
        }
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