#include <iostream>
#include <fstream>
#include <vector>

std::vector<std::string> read_input(std::string filepath)
{
    std::ifstream input_file(filepath);
    std::string line;
    std::vector<std::string> all_lines;

    if(!input_file.good())
    {
        std::cout << "could not open file " << filepath << std::endl;
        return all_lines;
    }

    while(std::getline(input_file, line))
    {
        all_lines.push_back(line);
    }
    return all_lines;
}


int main()
{
    std::cout << "Hello Advent" << std::endl;
    std::vector<std::string> commands = read_input("/home/sophie/Documents/AdventofCode/inputs/task_x.txt");
}
