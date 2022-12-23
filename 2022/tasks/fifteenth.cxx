#include <iostream>
#include <fstream>
#include <vector>
#include <regex>

class SignalBeacon
{
public:
    std::pair<long, long> signal;
    std::pair<long, long> beacon;
    long distance = 0;

    SignalBeacon(std::pair<long, long> signal, std::pair<long, long> beacon):
    signal{signal}, beacon{beacon}
    {
        distance = std::abs(signal.first - beacon.first) + std::abs(signal.second - beacon.second);
    }

    bool in_area(std::pair<long, long> pos)
    {
        long distance_to_signal = std::abs(signal.first - pos.first) + std::abs(signal.second - pos.second);
        return distance_to_signal <= distance;
    }

    void print()
    {
        std::cout << "Signal: " << signal.first << ", " << signal.second;
        std::cout << " - Beacon: " << beacon.first << ", " << beacon.second;
        std::cout << " which makes a distance of " << distance << std::endl;
    }
};

std::vector<SignalBeacon> read_signals(std::string filepath)
{
    std::ifstream input_file(filepath);
    std::string line;
    std::vector<SignalBeacon> signals;

    if(!input_file.good())
    {
        std::cout << "could not open file " << filepath << std::endl;
        return signals;
    }

    while(std::getline(input_file, line))
    {
        std::regex regex("Sensor at x=(-?[0-9]+), y=(-?[0-9]+): closest beacon is at x=(-?[0-9]+), y=(-?[0-9]+)");

        std::smatch all_matches;
        std::regex_search (line, all_matches, regex);
        std::vector<std::string> matches_as_str;

        for (auto m : all_matches)
        {
            matches_as_str.push_back(m.str());
        }


        SignalBeacon current_signal = {
            std::make_pair(
                std::stol(matches_as_str[1]),
                std::stol(matches_as_str[2])),
            std::make_pair(
                std::stol(matches_as_str[3]),
                std::stol(matches_as_str[4]))};

        signals.push_back(current_signal);
    }

    return signals;
}

std::vector<long> get_dimensions(std::vector<SignalBeacon>& signals)
{
    long x_min = INT64_MAX;
    long y_min = INT64_MAX;
    long x_max = 0;
    long y_max = 0;
    long max_distance = 0;
    for(auto it = signals.begin(); it != signals.end(); it++)
    {
        if(it->beacon.first < x_min)
            x_min = it->beacon.first;
        if(it->signal.first < x_min)
            x_min = it->signal.first;

        if(it->beacon.first > x_max)
            x_max = it->beacon.first;
        if(it->signal.first > x_max)
            x_max = it->signal.first;

        if(it->beacon.second < y_min)
            y_min = it->beacon.second;
        if(it->signal.second < y_min)
            y_min = it->signal.second;

        if(it->beacon.second > y_max)
            y_max = it->beacon.second;
        if(it->signal.second > y_max)
            y_max = it->signal.second;

        if(it->distance > max_distance)
            max_distance = it->distance;
    }
    return std::vector<long>{x_min, x_max, y_min, y_max, max_distance};
}

int get_covered(std::vector<SignalBeacon>& signals, long y_pos, long x_min, long x_max)
{
    int count_covered = 0;
    for(long i = x_min; i < x_max; i++)
    {
        bool covered_by_something = false;
        bool is_beacon = false;
        std::pair<long, long> current_pos = std::make_pair(i, y_pos);
        for(auto it = signals.begin(); it != signals.end(); it++)
        {
            if(it->in_area(current_pos))
            {
                if(current_pos == it->beacon)
                    is_beacon = true;
                covered_by_something = true;

            }
        }
        if (covered_by_something && !is_beacon)
            count_covered ++;
    }
    return count_covered;
}

long find_tuning_frequency_step(std::vector<SignalBeacon>& signals, int x_min, int x_max, int y_min, int y_max)
{
    std::vector<std::vector<bool>> possibilities((x_max - x_min) +1, std::vector<bool>((y_max - y_min) + 1, true));

    for(auto it = signals.begin(); it != signals.end(); it++)
    {
        it->print();
        for(long i = 0; i <= std::min(it->distance, long(x_max - x_min)); i++)
        {
            for(long j = 0; j <= std::min(it->distance - i, long(y_max - y_min)); ++j)
            {
                long x_down = it->signal.first - i - x_min;
                long x_up = it->signal.first + i - x_min;
                long y_down = it->signal.second - j - y_min;
                long y_up = it->signal.second + j - y_min;

                if (x_down >= 0 && y_down >= 0 && x_down <= (x_max - x_min) && y_down <= (y_max - y_min))
                {
                    possibilities[x_down][y_down] = false;
                }
                if (x_up >= 0 && y_down >= 0 && x_up <= (x_max - x_min) && y_down <= (y_max - y_min))
                {
                    possibilities[x_up][y_down] = false;
                }
                if (x_down >= 0 && y_up >= 0 && x_down <= (x_max - x_min) && y_up <= (y_max - y_min))
                {
                    possibilities[x_down][y_up] = false;
                }
                if (x_up >= 0 && y_up >= 0 && x_up <= (x_max - x_min) && y_up <= (y_max - y_min))
                {
                    possibilities[x_up][y_up] = false;
                }
            }
        }
    }

    // for(size_t i = 0; i < possibilities.size(); i++)
    // {
    //     for(size_t j = 0; j < possibilities[i].size(); ++j)
    //     {
    //         if(possibilities[i][j])
    //         {
    //             std::cout << "+";
    //         }
    //         else
    //         {
    //             std::cout << "-";
    //         }
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout << std::endl;

    for(int i = 0; i < (x_max - x_min); i++)
    {
        for(int j = 0; j < (y_max - y_min); ++j)
        {
            if (possibilities[i][j])
            {
                std::cout << (i+ x_min) << " " << (j + y_min) << " " << ((i+ x_min) * 4000000) + (j + y_min) << std::endl;
                return ((i+ x_min) * 4000000) + (j + y_min);
            }
        }
    }
    return -1;
}

long find_tuning_frequency(std::vector<SignalBeacon>& signals)
{
    int total_min = 0;
    int total_max = 4000000;
    int step = 500000;

    for(int i = total_min; i < total_max; i+= step)
    {
        for(int j = total_min; j < total_max; j+= step)
        {
            std::cout << i << " and " << j << " to " << i + step << " and " << j + step << std::endl;
            long result = find_tuning_frequency_step(signals, i, i + step, j, j + step);
            if (result != -1)
                return result;
        }
    }
    return -1;
}

long find_tuning_freq(std::vector<SignalBeacon>& signals)
{
    int min = 0;
    int max = 20;

    long last_result = 0;

    #pragma omp parallel for
    for(int i = 0; i <= max; i++)
    {
        // if((i % 10) == 0)
        //     std::cout << "Testing i = " << i << std::endl;

        for(int j = min; j <= max; j++)
        {
            std::pair<long, long> current_pos = std::make_pair(i, j);

            bool covered_by_something = false;
            for(auto it = signals.begin(); it != signals.end(); it++)
            {
                if(it->in_area(current_pos))
                {
                    covered_by_something = true;
                    break;
                }
            }
            if(!covered_by_something)
            {
                std::cout << "Found uncovered! X = " << i << ", Y = " << j << std::endl;
                last_result = (i * 4000000) +  j;
                //return last_result;
            }
        }
    }
    return last_result;
}

int main()
{
    std::cout << "Hello Advent" << std::endl;
    std::vector<SignalBeacon> signals = read_signals("/home/sophie/Documents/AdventofCode/AdventOfCode/2022/inputs/task_15.txt");

    //std::vector<long> dimensions = get_dimensions(signals);

    //int count_covered = get_covered(signals, 2000000, dimensions[0]-dimensions[4], dimensions[1] + dimensions[4]);

//    std::cout << count_covered << " positions cannot contain a beacon as they are covered by some signal." << std::endl;

    //long freq = find_tuning_freq(signals);
    long freq = find_tuning_frequency(signals);

    std::cout << "Tuning Frequency is " << freq << std::endl;
}
