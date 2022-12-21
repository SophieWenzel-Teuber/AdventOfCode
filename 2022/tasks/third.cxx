#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <functional>
#include <fstream>
#include <algorithm>


int item_to_priority(char item)
{
    if (isupper(item))
    {
        return int(item) - (int('A') - 27);
    }
    else
    {
        return int(item) - int('a') + 1;

    }
}

std::vector<std::pair<std::string, std::string>> read_calories(std::string filepath)
{
    std::ifstream input_file(filepath);
    std::string line;
    std::vector<std::pair<std::string, std::string>> items_per_rucksack;

    if(!input_file.good())
    {
        std::cout << "could not open file " << filepath << std::endl;
        return items_per_rucksack;
    }

    while(std::getline(input_file, line))
    {
        size_t number_of_items = line.length();
        if (number_of_items % 2 != 0)
        {
            std::cout << "Meh!" << std::endl;
        }

        items_per_rucksack.push_back(
            std::make_pair(
                line.substr(0, number_of_items / 2),
                line.substr(number_of_items / 2, number_of_items)
                )
            );
    }
    return items_per_rucksack;
}

std::vector<std::vector<std::string>> read_group(std::string filepath)
{
    std::ifstream input_file(filepath);
    std::string line;
    std::vector<std::string> items_per_rucksack;
    std::vector<std::vector<std::string>> all_groups;

    if(!input_file.good())
    {
        std::cout << "could not open file " << filepath << std::endl;
        return all_groups;
    }

    while(std::getline(input_file, line))
    {
        size_t number_of_items = line.length();
        if (number_of_items % 2 != 0)
        {
            std::cout << "Meh!" << std::endl;
        }

        items_per_rucksack.push_back(line);

        if (items_per_rucksack.size() == 3)
        {
            all_groups.push_back(items_per_rucksack);
            items_per_rucksack = std::vector<std::string>();
        }
    }
    return all_groups;

}

char find_doubled_item(std::pair<std::string, std::string> rucksack_content)
{
    for (size_t i = 0; i < rucksack_content.first.length(); ++i)
    {
        if (rucksack_content.second.find(rucksack_content.first[i]) != std::string::npos)
            return rucksack_content.first[i];
    }

    std::cout << "More meh!" << std::endl;
    return '0';
}

char find_badges(std::vector<std::string> rucksack_content)
{
    for (size_t i = 0; i < rucksack_content[0].length(); ++i)
    {
        if (rucksack_content[1].find(rucksack_content[0][i]) != std::string::npos)
        {
            if (rucksack_content[2].find(rucksack_content[0][i]) != std::string::npos)
                return rucksack_content[0][i];
        }
    }

    std::cout << "More meh!" << std::endl;
    return '0';
}

int main()
{
    std::cout << "Hello Advent" << std::endl;

    std::vector<std::pair<std::string, std::string>> items_per_rucksack = read_calories("/home/sophie/Documents/AdventofCode/inputs/task_3.txt");
    std::vector<std::vector<std::string>> all_groups = read_group("/home/sophie/Documents/AdventofCode/inputs/task_3.txt");

    std::cout << find_doubled_item(items_per_rucksack[0]) << std::endl;
    std::cout << find_badges(all_groups[0]) << std::endl;

    auto get_rucksack_priority = []( int counter, std::pair<std::string, std::string> input)
    {
        return counter + item_to_priority(find_doubled_item(input));
    };

    auto get_group_priority = []( int counter, std::vector<std::string> input)
    {
        return counter + item_to_priority(find_badges(input));
    };


    int priorities = std::accumulate(
        items_per_rucksack.begin(),
        items_per_rucksack.end(),
        0,
        get_rucksack_priority);

    int priorities_badges = std::accumulate(
        all_groups.begin(),
        all_groups.end(),
        0,
        get_group_priority);

    std::cout << "Total priorities: " << priorities << std::endl;
    std::cout << "Total badge priorities: " << priorities_badges << std::endl;

}
