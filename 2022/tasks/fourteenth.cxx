#include <iostream>
#include <fstream>
#include <vector>
#include <exception>

#include "../common_utils/thingies.hxx"

enum element {air, rock, sand};

class GridHandler
{
public:
    GridHandler(std::vector<std::vector<element>> grid): grid{grid}
    {}

    bool check_bounds(std::pair<int, int> current)
    {
        if ((current.first >= max_x) || (current.second >= max_y))
        {
            return false;
        }
        return true;
    }

    std::pair<int, int> try_pouring_down( std::pair<int, int> current)
    {
        if (!check_bounds(std::make_pair(current.first, current.second + 1)))
        {
            throw std::exception();
        }
        if(grid[current.first][current.second + 1] == air)
        {
            return std::make_pair(current.first, current.second + 1);
        }
        return current;
    }

    std::pair<int, int> try_pouring_left( std::pair<int, int> current)
    {
        if (!check_bounds(std::make_pair(current.first - 1, current.second + 1)))
        {
            throw std::exception();
        }
        if(grid[current.first - 1][current.second + 1] == air)
        {
            return std::make_pair(current.first - 1, current.second + 1);
        }
        return current;
    }

    std::pair<int, int> try_pouring_right( std::pair<int, int> current)
    {
        if (!check_bounds(std::make_pair(current.first + 1, current.second + 1)))
        {
            throw std::exception();
        }
        if(grid[current.first + 1][current.second + 1] == air)
        {
            return std::make_pair(current.first + 1, current.second + 1);
        }
        return current;
    }

    void set_sand(std::pair<int, int> pos)
    {
        grid[pos.first][pos.second] = sand;
    }

    void print()
    {
        for(int j = 0; j != 150; j++)
        {
            for(int i = 470; i != 520; i++)
            {
                if(grid[i][j] == air)
                {
                    std::cout << ".";
                }
                else if(grid[i][j] == sand)
                {
                    std::cout << "o";
                }
                else
                {
                    std::cout << "#";
                }
            }
            std::cout << std::endl;
        }
    }
    std::vector<std::vector<element>> grid;
    int max_x = 900;
    int max_y = 200;
};

std::vector<std::vector<std::pair<int, int>>> read_input(std::string filepath)
{
    std::ifstream input_file(filepath);
    std::string line;
    std::vector<std::vector<std::pair<int, int>>> all_positions;


    if(!input_file.good())
    {
        std::cout << "could not open file " << filepath << std::endl;
        return all_positions;
    }

    while(std::getline(input_file, line))
    {
        std::vector<std::string> positions = split_string(line, " -> ");

        std::vector<std::pair<int, int>> current_positions;
        for (auto pos = positions.begin(); pos != positions.end(); pos++)
        {
            std::vector<std::string> x_and_y = split_string(*pos, ",");

            current_positions.push_back(std::make_pair(std::stoi(x_and_y[0]), std::stoi(x_and_y[1])));
        }
        all_positions.push_back(current_positions);

    }
    return all_positions;
}

std::vector<std::vector<element>> create_grid(std::vector<std::vector<std::pair<int, int>>> all_positions)
{
    std::vector<std::vector<element>> grid = std::vector<std::vector<element>>(900, std::vector<element>(200, air));
    int max_y = 0;
    for(auto line = all_positions.begin(); line != all_positions.end(); line++)
    {
        std::pair<int, int> current_pos = line->at(0);
        grid[current_pos.first][current_pos.second] = rock;
        if(current_pos.second > max_y)
            max_y = current_pos.second;
        for(int i = 1; i < line->size(); ++i)
        {
            std::pair<int, int> next_pos = line->at(i);

            if(next_pos.second > max_y)
                max_y = next_pos.second;

            if(next_pos.first != current_pos.first)
            {
                int update = 1;
                if (next_pos.first > current_pos.first)
                    update = -1;
                for(int j = next_pos.first; j != current_pos.first; j += update)
                {
                    grid[j][current_pos.second] = rock;
                }
            }
            if(next_pos.second != current_pos.second)
            {
                int update = 1;
                if (next_pos.second > current_pos.second)
                    update = -1;
                for(int j = next_pos.second; j != current_pos.second; j += update)
                {
                    grid[current_pos.first][j] = rock;
                }
            }
            current_pos = next_pos;
        }
    }

    for(int i = 0; i < grid.size(); i++)
    {
        grid[i][max_y + 2] = rock;
    }
    return grid;
}

std::pair<int, int> new_unit(GridHandler& grid)
{
    std::pair<int, int> sand_source = std::make_pair(500, 0);
    std::pair<int, int> current_sand = sand_source;
    std::pair<int, int> next_sand = current_sand;
    while(true)
    {
        next_sand = grid.try_pouring_down(current_sand);
        if(next_sand == current_sand)
        {
            //could not pour down, try left

            next_sand = grid.try_pouring_left(current_sand);
            if(next_sand == current_sand)
            {
                //could not pour left, try right

                next_sand = grid.try_pouring_right(current_sand);
                if(next_sand == current_sand)
                {
                    //could not pour anywhere, sand rests
                    grid.set_sand(current_sand);
                    return current_sand;
                }
            }
        }
        current_sand = next_sand;
    }
}

int time_loop_2(GridHandler& grid)
{
    int sand_counter = 0;
    try
    {
        while(true)
        {
            std::pair<int, int> resting_pos = new_unit(grid);
            sand_counter ++;
            if(resting_pos == std::make_pair(500, 0))
            {
                return sand_counter;

            }
//            if(sand_counter == 24)
//                std::cout << "Interesting" << std::endl;
        }
    }
    catch(std::exception & e)
    {
        return sand_counter;
    }

}

int time_loop_1(GridHandler& grid)
{
    int sand_counter = 0;
    try
    {
        while(true)
        {
            new_unit(grid);
            sand_counter ++;
//            if(sand_counter == 24)
//                std::cout << "Interesting" << std::endl;
        }
    }
    catch(std::exception & e)
    {
        return sand_counter;
    }

}

int main()
{
    std::cout << "Hello Advent" << std::endl;
    std::vector<std::vector<std::pair<int, int>>> all_positions = read_input("/home/sophie/Documents/AdventofCode/AdventOfCode/2022/inputs/task_14.txt");

    GridHandler grid(create_grid(all_positions));

    int sand_counter = time_loop_2(grid);
    grid.print();

    std::cout << "Poured " << sand_counter << " units of sand." << std::endl;
}
