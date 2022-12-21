#include <iostream>
#include <fstream>

#include <algorithm>
#include <vector>

std::vector<int> read_calories(std::string filepath)
{
    std::ifstream input_file(filepath);
    std::string line;
    int current_counter = 0;
    std::vector<int> calories;

    if(!input_file.good())
    {
        std::cout << "could not open file " << filepath << std::endl;
        return calories;
    }

    while(std::getline(input_file, line))
    {
        if (line == "")
        {
            if (current_counter > 0)
            {
                calories.push_back(current_counter);
            }
            current_counter = 0;
        }
        else
        {
            current_counter += std::stoi(line);
        }
    }
    return calories;
}


int main()
{
    std::cout << "Hello first of Advent" << std::endl;

    std::vector<int> calories = read_calories("/home/sophie/Documents/AdventofCode/inputs/task_1.txt");

    std::sort(calories.rbegin(), calories.rend());

    std::cout << "Largest amount of calories are " << calories[0] << std::endl;

    int number_of_top_elves = 3;
    int top_calories = 0;
    for (size_t i = 0; i < number_of_top_elves; ++i)
    {
        top_calories += calories[i];
    }

    std::cout << "Largest amount of all " << number_of_top_elves << " top carrier elves is " << top_calories << std::endl;
}
