#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <numeric>


std::pair<int, int> pair_from_string(std::string input)
{
    auto dash = input.find('-');
    std::string first = input.substr(0, dash);
    std::string second = input.substr(dash + 1, input.size());

    std::pair<int, int> assignment = std::make_pair(std::stoi(first), std::stoi(second));

    return assignment;
}

class AssignmentGroup
{
public:
    void set_from_string(std::string input)
    {
        auto comma = input.find(',');

        first = pair_from_string(input.substr(0, comma));
        second = pair_from_string(input.substr(comma + 1, input.size()));
    }

    std::string print()
    {
        std::stringstream ss;
        ss << "First " << first.first << " to " << first.second << ", Second: " << second.first << " to " << second.second;
        return ss.str();
    }

    bool contained()
    {
        if (first.first <= second.first)
            if (first.second >= second.second)
                return true;

        if (second.first <= first.first)
            if (second.second >= first.second)
                return true;

        return false;
    }

    bool overlap()
    {
        // ie 3-5,1-4
        if(first.second >= second.first)
            if(second.second > first.first)
               return true;
        if(second.second >= first.first)
            if(first.second > second.first)
                return true;
        // ie 1-2,3-4
        return false;
    }

private:

    std::pair<int, int> first;
    std::pair<int, int> second;
};


std::vector<AssignmentGroup> read_group(std::string filepath)
{
    std::ifstream input_file(filepath);
    std::string line;
    std::vector<AssignmentGroup> assignments;

    if(!input_file.good())
    {
        std::cout << "could not open file " << filepath << std::endl;
        return assignments;
    }

    while(std::getline(input_file, line))
    {
        auto comma = line.find(',');
        // if not comma...

        AssignmentGroup new_group;
        new_group.set_from_string(line);
        assignments.push_back(new_group);
    }
    return assignments;
}

int main()
{
    std::cout << "Hello Advent" << std::endl;

    std::vector<AssignmentGroup> assignments = read_group("/home/sophie/Documents/AdventofCode/inputs/task_4.txt");

    auto get_contained = []( int counter, AssignmentGroup assignment)
    {
        if (assignment.contained()) return counter += 1;
        else return counter;
    };

    int contained = std::accumulate(
        assignments.begin(),
        assignments.end(),
        0,
        get_contained);

    std::cout << "Sum of contained groups: " << contained << std::endl;

    // task 2
    auto get_overlapped = []( int counter, AssignmentGroup assignment)
    {
        if (assignment.overlap()) return counter += 1;
        else return counter;
    };

    int overlapped = std::accumulate(
        assignments.begin(),
        assignments.end(),
        0,
        get_overlapped);

    std::cout << "Sum of contained groups: " << overlapped << std::endl;
}
