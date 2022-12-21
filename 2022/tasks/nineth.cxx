#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>

struct Step
{
    char direction;
    int number;
};

std::vector<Step> read_input(std::string filepath)
{
    std::ifstream input_file(filepath);
    std::string line;
    std::vector<Step> all_steps;

    if(!input_file.good())
    {
        std::cout << "could not open file " << filepath << std::endl;
        return all_steps;
    }

    while(std::getline(input_file, line))
    {
        all_steps.push_back(Step{line[0], std::stoi(line.substr(2, line.length()))});
    }
    return all_steps;
}



void update_tail(
    std::pair<int, int>& head_pos,
    std::pair<int, int>& tail_pos)
{
    // same row!
    if (head_pos.first == tail_pos.first)
    {
        // close enough
        if (std::abs(head_pos.second - tail_pos.second) < 2)
            return;

        //move
        if(head_pos.second - tail_pos.second > 1)
            tail_pos.second ++;
        if(head_pos.second - tail_pos.second < 1)
            tail_pos.second --;
    }
    // same column!
    if (head_pos.second == tail_pos.second)
    {
        // close enough
        if (std::abs(head_pos.first - tail_pos.first) < 2)
            return;

        //move
        if(head_pos.first - tail_pos.first > 1)
            tail_pos.first ++;
        if(head_pos.first - tail_pos.first < 1)
            tail_pos.first --;
    }

    // diagonal
    if (std::abs(head_pos.first - tail_pos.first) * std::abs(head_pos.second - tail_pos.second) > 1)
    {
        //move
        if(head_pos.first - tail_pos.first > 0)
            tail_pos.first ++;
        else if(head_pos.first - tail_pos.first < 0)
            tail_pos.first --;

        if(head_pos.second - tail_pos.second > 0)
            tail_pos.second ++;
        else if(head_pos.second - tail_pos.second < 0)
            tail_pos.second --;
    }

}

void make_step(
    std::vector<std::vector<int>>& rope,
    std::pair<int, int>& head_pos,
    std::pair<int, int>& tail_pos,
    Step step)
{
    for(int i = 0; i < step.number; ++i)
    {
        if (step.direction == 'R')
            head_pos.second ++;
        if (step.direction == 'L')
            head_pos.second --;
        if (step.direction == 'D')
            head_pos.first ++;
        if (step.direction == 'U')
            head_pos.first --;

        update_tail(head_pos, tail_pos);
        rope[tail_pos.first][tail_pos.second] = 1;
    }
}

int walk(std::vector<std::vector<int>> rope, std::vector<Step> steps)
{
    //set initial state
    int initial_i = rope.size() / 2;
    int initial_j = rope[0].size() / 2;
    std::pair<int, int> head_pos = std::make_pair(initial_i,initial_j);
    std::pair<int, int> tail_pos = std::make_pair(initial_i,initial_j);
    rope[initial_i][initial_j] = 1;

    for (Step s : steps)
    {
        make_step(rope, head_pos, tail_pos, s);
    }

    int sum = std::accumulate(rope.cbegin(), rope.cend(), 0, [](auto lhs, const auto& rhs) {
        return std::accumulate(rhs.cbegin(), rhs.cend(), lhs);
    });

    return sum;
}


void make_longer_step(
    std::vector<std::vector<int>>& rope,
    std::vector<std::pair<int, int>>& head_pos,
    std::pair<int, int>& tail_pos,
    Step step)
{
    for(int i = 0; i < step.number; ++i)
    {
//        head_pos.erase(head_pos.end() - 1);
        std::pair<int, int> last_position = head_pos.back();
        head_pos.pop_back();
        if (step.direction == 'R')
        {
            last_position.second ++;
            head_pos.push_back(last_position);
        }
        if (step.direction == 'L')
        {
            last_position.second --;
            head_pos.push_back(last_position);
        }
        if (step.direction == 'D')
        {
            last_position.first ++;
            head_pos.push_back(last_position);
        }
        if (step.direction == 'U')
        {
            last_position.first --;
            head_pos.push_back(last_position);
        }

        //update heads
        for(int h = head_pos.size() - 1; h > 0; h--)
        {
            update_tail(head_pos[h], head_pos[h-1]);
        }

        update_tail(head_pos[0], tail_pos);
        rope[tail_pos.first][tail_pos.second] = 1;
    }
}

int walk_longer(std::vector<std::vector<int>>& rope, std::vector<Step> steps)
{
    //set initial state
    int initial_i = rope.size() / 2;
    int initial_j = rope[0].size() / 2;
    std::vector<std::pair<int, int>> head_pos = std::vector<std::pair<int, int>>(9, std::make_pair(initial_i,initial_j));
    std::pair<int, int> tail_pos = std::make_pair(initial_i,initial_j);
    rope[initial_i][initial_j] = 1;

    for (Step s : steps)
    {
        make_longer_step(rope, head_pos, tail_pos, s);
    }

    int sum = std::accumulate(rope.cbegin(), rope.cend(), 0, [](auto lhs, const auto& rhs) {
        return std::accumulate(rhs.cbegin(), rhs.cend(), lhs);
    });

    return sum;
}

int main()
{
    std::cout << "Hello Advent" << std::endl;
    std::vector<Step> steps = read_input("/home/sophie/Documents/AdventofCode/inputs/task_9.txt");

    int dimension = 700;
    std::vector<std::vector<int>> rope(dimension, std::vector<int>(dimension, 0));

    int end = walk(rope, steps);
    std::cout << "Walk! " << end << std::endl;

    int end_longer = walk_longer(rope, steps);
    std::cout << "Walk you stupid long rope! " << end_longer << std::endl;

    // for(int i=0; i < dimension; i++)
    // {
    //     for(int j=0; j<dimension; j++)
    //     {
    //         int res = rope[i][j];
    //         if (res == 0)
    //             std::cout << ".";
    //         if (res == 1)
    //             std::cout << "#";
    //     }
    //     std::cout << std::endl;
    // }
}
