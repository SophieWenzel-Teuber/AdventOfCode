#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

class Map
{

public:
    int get_height(std::pair<int, int> pos)
    {
        return grid[pos.first][pos.second];
    }

    int get_distance(std::pair<int, int> pos)
    {
        return distances[pos.first][pos.second];
    }
    void set_distance(std::pair<int, int> pos, int value)
    {
        distances[pos.first][pos.second] = value;
    }


    int get_visited(std::pair<int, int> pos)
    {
        return visited[pos.first][pos.second];
    }
    void set_visited(std::pair<int, int> pos)
    {
        visited[pos.first][pos.second] = true;
    }

    void reset_state()
    {
        distances = std::vector<std::vector<int>>(grid.size(), std::vector<int>(grid[0].size(), INT32_MAX));
        visited = std::vector<std::vector<bool>>(grid.size(), std::vector<bool>(grid[0].size(), false));

        set_visited(start_pos);
        set_distance(start_pos, 0);
    }

    std::pair<int, int> start_pos;
    std::pair<int, int> end_pos;
    std::vector<std::vector<int>> grid;
    std::vector<std::vector<int>> distances;
    std::vector<std::vector<bool>> visited;
};

Map read_input(std::string filepath)
{
    std::ifstream input_file(filepath);
    std::string line;
    Map map;

    if(!input_file.good())
    {
        std::cout << "could not open file " << filepath << std::endl;
        return map;
    }

    while(std::getline(input_file, line))
    {
        std::vector<int> vertical;
        for(int i = 0; i < line.length(); i++)
        {
            if(line[i] == 'S')
            {
                map.start_pos = std::make_pair(map.grid.size(), i);
                vertical.push_back(int('a') - 97);
            }
            else if (line[i] == 'E')
            {
                map.end_pos = std::make_pair(map.grid.size(), i);
                vertical.push_back(int('z') - 97);
            }
            else
                vertical.push_back(int(line[i]) - 97);
        }
        map.grid.push_back(vertical);
    }
    map.reset_state();
    return map;
}
/*
int count_steps_v1(Map& map, std::pair<int, int> current_pos, std::pair<int, int> previous_pos,  int current_steps)
{
    if(current_pos == map.end_pos)
        return current_steps;

    int current_height = map.grid[current_pos.first][current_pos.second];
    std::vector<int> udlf(4, 2147483647);
    //up
    if (current_pos.first > 0)
    {
        std::pair<int, int> new_pos(current_pos.first - 1, current_pos.second);
        if(previous_pos != new_pos)
            if(map.get_height(current_pos) <= map.get_height(new_pos))
                udlf[0] = count_steps(map, new_pos, current_pos, current_steps + 1);
    }
    //down
    if (current_pos.first < map.grid.size() - 1)
    {
        std::pair<int, int> new_pos(current_pos.first + 1, current_pos.second);
        if(previous_pos != new_pos)
            if(map.get_height(current_pos) <= map.get_height(new_pos))
                udlf[1] = count_steps(map, new_pos, current_pos, current_steps + 1);
    }
    //left
    if (current_pos.second > 0)
    {
        std::pair<int, int> new_pos(current_pos.first, current_pos.second - 1);
        if(previous_pos != new_pos)
            if(map.get_height(current_pos) <= map.get_height(new_pos))
                udlf[2] = count_steps(map, new_pos, current_pos, current_steps + 1);
    }
    //down
    if (current_pos.second < map.grid[0].size() - 1)
    {
        std::pair<int, int> new_pos(current_pos.first, current_pos.second + 1);
        if(previous_pos != new_pos)
            if(map.get_height(current_pos) <= map.get_height(new_pos))
                udlf[3] = count_steps(map, new_pos, current_pos, current_steps + 1);
    }
    std::sort(udlf.begin(), udlf.end());
    return udlf[0];
}
*/
/*
int count_steps(Map& map, std::pair<int, int> current_pos, std::vector<std::pair<int, int>> previous,  int current_steps)
{
    if(current_pos == map.end_pos)
        return current_steps;

    int current_height = map.grid[current_pos.first][current_pos.second];
    std::vector<std::pair<int, int>> higher_options;
    std::vector<std::pair<int, int>> leveled_options;
    std::vector<int> other_steps;
    //up
    if (current_pos.first > 0)
    {
        std::pair<int, int> new_pos(current_pos.first - 1, current_pos.second);
        if(std::find(previous.begin(), previous.end(), new_pos) ==  previous.end())
            if(map.get_height(new_pos) - map.get_height(current_pos) <= 1)
            {
                if (map.get_height(current_pos) < map.get_height(new_pos))
                    higher_options.push_back(new_pos);
                else if (map.get_height(current_pos) == map.get_height(new_pos))
                    leveled_options.push_back(new_pos);
            }
    }
    //down
    if (current_pos.first < map.grid.size() - 1)
    {
        std::pair<int, int> new_pos(current_pos.first + 1, current_pos.second);
        if(std::find(previous.begin(), previous.end(), new_pos) ==  previous.end())
            if(map.get_height(new_pos) - map.get_height(current_pos) <= 1)
            {
                if (map.get_height(current_pos) < map.get_height(new_pos))
                    higher_options.push_back(new_pos);
                else if (map.get_height(current_pos) == map.get_height(new_pos))
                    leveled_options.push_back(new_pos);
            }
    }
    //left
    if (current_pos.second > 0)
    {
        std::pair<int, int> new_pos(current_pos.first, current_pos.second - 1);
        if(std::find(previous.begin(), previous.end(), new_pos) ==  previous.end())
            if(map.get_height(new_pos) - map.get_height(current_pos) <= 1)
            {
                if (map.get_height(current_pos) < map.get_height(new_pos))
                    higher_options.push_back(new_pos);
                else if (map.get_height(current_pos) == map.get_height(new_pos))
                    leveled_options.push_back(new_pos);
            }
    }
    //down
    if (current_pos.second < map.grid[0].size() - 1)
    {
        std::pair<int, int> new_pos(current_pos.first, current_pos.second + 1);
        if(std::find(previous.begin(), previous.end(), new_pos) ==  previous.end())
            if(map.get_height(new_pos) - map.get_height(current_pos) <= 1)
            {
                if (map.get_height(current_pos) < map.get_height(new_pos))
                    higher_options.push_back(new_pos);
                else if (map.get_height(current_pos) == map.get_height(new_pos))
                    leveled_options.push_back(new_pos);
            }
    }

    // if (current_pos.second < map.grid[0].size() - 1)
    // {
    //     std::pair<int, int> new_pos(current_pos.first, current_pos.second + 1);
    //     if(std::find(previous.begin(), previous.end(), new_pos) ==  previous.end())
    //         if(map.get_height(current_pos) <= map.get_height(new_pos) &&
    //              map.get_height(new_pos) - map.get_height(current_pos) <= 1)
    //         {
    //             previous.push_back(current_pos);
    //             udlf[3] = count_steps(map, new_pos, previous, current_steps + 1);
    //         }
    // }

    if (higher_options.size() > 0)
    {
        previous.push_back(current_pos);
        for (auto it = higher_options.begin(); it != higher_options.end(); it++)
        {
            other_steps.push_back(count_steps(map, *it, previous, current_steps + 1));
        }
    }
    else if (leveled_options.size() > 0)
    {
        previous.push_back(current_pos);
        for (auto it = leveled_options.begin(); it != leveled_options.end(); it++)
        {
            other_steps.push_back(count_steps(map, *it, previous, current_steps + 1));
        }
    }
    else
        return INT32_MAX; // dead end


    std::sort(other_steps.begin(), other_steps.end());
    return other_steps[0];
}
*/

bool update_distance(Map& map, std::pair<int, int>& current_pos, std::pair<int, int>& new_pos)
{
    if(!map.get_visited(new_pos))
    {
        // only visit if height close enough together or equal
        if(map.get_height(new_pos) - map.get_height(current_pos) <= 1)
        {
            int tentative_distance = map.get_distance(current_pos) + 1;
            if (tentative_distance < map.get_distance(new_pos))
            {
                map.set_distance(new_pos, tentative_distance);
            }
            return true;
        }
    }
    return false;
}

std::pair<int, int> go_to_neighbour(Map& map, std::pair<int, int>& current_pos)
{
    int current_min = INT32_MAX;
    std::pair<int, int> next_pos = current_pos;
    //up
    if (current_pos.first > 0)
    {
        std::pair<int, int> new_pos(current_pos.first - 1, current_pos.second);
        bool possible_path = update_distance(map, current_pos, new_pos);
        if(possible_path && (map.get_distance(new_pos) < current_min))
        {
            next_pos = new_pos;
        }
    }
    //down
    if (current_pos.first < map.grid.size() - 1)
    {
        std::pair<int, int> new_pos(current_pos.first + 1, current_pos.second);
        bool possible_path = update_distance(map, current_pos, new_pos);
        if(possible_path && (map.get_distance(new_pos) < current_min))
        {
            next_pos = new_pos;
        }
    }
    //left
    if (current_pos.second > 0)
    {
        std::pair<int, int> new_pos(current_pos.first, current_pos.second - 1);
        bool possible_path = update_distance(map, current_pos, new_pos);
        if(possible_path && (map.get_distance(new_pos) < current_min))
        {
            next_pos = new_pos;
        }
    }
    //right
    if (current_pos.second < map.grid[0].size() - 1)
    {
        std::pair<int, int> new_pos(current_pos.first, current_pos.second + 1);
        bool possible_path = update_distance(map, current_pos, new_pos);
        if(possible_path && (map.get_distance(new_pos) < current_min))
        {
            next_pos = new_pos;
        }
    }

    return next_pos;
}

int Dijkstra(Map& map)
{
    std::vector<std::pair<int, int>> previous;
    std::pair<int, int> current_pos = map.start_pos;
    while(current_pos != map.end_pos)
    {
        std::pair<int, int> new_pos = go_to_neighbour(map, current_pos);
        if(new_pos == current_pos)
        {
            //dead end - go back
            current_pos = previous.back();
            previous.pop_back();
        }
        else
        {
            previous.push_back(current_pos);
            current_pos = new_pos;
            map.set_visited(current_pos);
        }
    }

    return map.get_distance(current_pos);
}

int main()
{
    std::cout << "Hello Advent" << std::endl;
    Map map = read_input("/home/sophie/Documents/AdventofCode/inputs/task_12.txt");

    int shortest = Dijkstra(map);

    std::cout << shortest << std::endl;

    for(int i = 0; i < map.grid.size(); ++i)
    {
        for(int j = 0; j < map.grid[i].size(); ++j)
        {
            std::cout << map.get_distance(std::make_pair(i,j)) << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
    for(int i = 0; i < map.grid.size(); ++i)
    {
        for(int j = 0; j < map.grid[i].size(); ++j)
        {
            if(map.get_visited(std::make_pair(i,j)))
                std::cout << "+" << " ";
            else
                std::cout << "." << " ";

        }
        std::cout << std::endl;
    }
}
