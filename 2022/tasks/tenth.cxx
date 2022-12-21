#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>

#include "../common_utils/thingies.hxx"

enum CommandType {noop, addx};

struct Command
{
    int duration;
    CommandType type;
    int value = 0;
};

std::vector<Command> read_input(std::string filepath)
{
    std::ifstream input_file(filepath);
    std::string line;
    std::vector<Command> commands;

    if(!input_file.good())
    {
        std::cout << "could not open file " << filepath << std::endl;
        return commands;
    }

    while(std::getline(input_file, line))
    {
        std::vector<std::string> inputs = split_string(line, " ");
        if(inputs[0] == "noop")
            commands.push_back(Command() = {1, noop, 0});
        else if (inputs[0] == "addx")
            commands.push_back(Command() = {2, addx, std::stoi(inputs[1])});
        else
            std::cout << "Unknown Command" << std::endl;

    }
    return commands;
}

std::vector<size_t> relevant_signal_cylces()
{
    std::vector<size_t> cycles;
    for(size_t i = 20; i < 221; i += 40)
    {
        cycles.push_back(i);
    }
    return cycles;
}

// int get_signal_strength(std::vector<Command> commands, std::vector<size_t> important_cycle_indices)
// {
//     int X = 1;
//     int strength = 0;
//     size_t current_execution = 0;
//     auto command_iterator = commands.begin();
//     for(size_t c = 0; c <= important_cycle_indices.back(); ++c)
//     {
//         if (std::find(important_cycle_indices.begin(), important_cycle_indices.end(), c) != std::npos)
//         {
//             strength += X;
//         }
//         if(current_execution == 0)
//         {
//             //next command

//         }

//     }
// }

int get_signal_strength(std::vector<Command> commands, std::vector<size_t> important_cycle_indices)
{
    int X = 1;
    size_t current_cycle = 0;
    int strength = 0;
    for(auto command_it = commands.begin(); command_it != commands.end(); command_it++)
    {
        current_cycle += command_it->duration;
        if(current_cycle >= important_cycle_indices[0])
        {
            //need to log!
            int new_value = X * (important_cycle_indices[0]);
            strength += new_value;
            if(important_cycle_indices.size() == 1)
                break;
            important_cycle_indices.erase(important_cycle_indices.begin());
        }
        if(command_it->type == addx)
        {
            X += command_it->value;
        }
    }
    return strength;
}

std::vector<char> draw(std::vector<Command> commands)
{
    int X = 1;
    int current_cycle = 0;
    std::vector<char> screen;
    for(auto command_it = commands.begin(); command_it != commands.end(); command_it++)
    {
        for(int c = 0; c < command_it->duration; c++)
        {
            if(((current_cycle % 40) > (X-2)) && ((current_cycle% 40) < (X+2)))
                screen.push_back('#');
            else
                screen.push_back('.');
            current_cycle ++;
        }
        if(command_it->type == addx)
        {
            X += command_it->value;
        }
    }
    return screen;
}



int main()
{
    std::cout << "Hello Advent" << std::endl;
    std::vector<Command> commands = read_input("/home/sophie/Documents/AdventofCode/inputs/task_10.txt");

    int strength = get_signal_strength(commands, relevant_signal_cylces());

    std::cout << "Accumulated strength is " << strength << std::endl;

    std::vector<char> screen = draw(commands);

    int counter = 0;
    for(auto it = screen.begin(); it != screen.end(); it++, counter++)
    {
        if (counter % 40 == 0)
        {
            std::cout << std::endl;
        }
        std::cout << *it;
    }
}
