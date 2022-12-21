#include <iostream>
#include <vector>
#include <fstream>
#include <regex>
#include <algorithm>

class Stack
{
public:
    Stack(std::vector<char> input) : current_stack{input} {}

    void add_crates(std::vector<char> new_crates)
    {
        for (auto e : new_crates)
            current_stack.push_back(e);
    }

    void add_crates_reverse(std::vector<char> new_crates)
    {
        std::reverse(new_crates.begin(), new_crates.end());
        add_crates(new_crates);
    }

    // returns as they are removed: top first, bottom last
    std::vector<char> remove_crates(int amount)
    {
        std::vector<char> removed;
        for(size_t i = 0; i < amount; i++)
        {
            removed.push_back(current_stack.back());
            current_stack.pop_back();
        }
        return removed;
    }

    // first item on bottom, last on top
    std::vector<char> current_stack;
};

struct Order
{
    int amount;
    int from;
    int to;
};


/*
    [G] [R]                 [P]
    [H] [W]     [T] [P]     [H]
    [F] [T] [P] [B] [D]     [N]
[L] [T] [M] [Q] [L] [C]     [Z]
[C] [C] [N] [V] [S] [H]     [V] [G]
[G] [L] [F] [D] [M] [V] [T] [J] [H]
[M] [D] [J] [F] [F] [N] [C] [S] [F]
[Q] [R] [V] [J] [N] [R] [H] [G] [Z]
 1   2   3   4   5   6   7   8   9

    [D]
[N] [C]
[Z] [M] [P]
 1   2   3

*/
std::vector<Stack> hardcode_setup()
{
    std::vector<Stack> my_stack;
    my_stack.push_back(Stack{std::vector<char>{'Q', 'M', 'G', 'C', 'L'}});
    my_stack.push_back(Stack{std::vector<char>{'R', 'D', 'L', 'C', 'T', 'F', 'H', 'G'}});
    my_stack.push_back(Stack{std::vector<char>{'V', 'J', 'F', 'N', 'M', 'T', 'W', 'R'}});
    my_stack.push_back(Stack{std::vector<char>{'J', 'F', 'D', 'V', 'Q', 'P'}});
    my_stack.push_back(Stack{std::vector<char>{'N', 'F', 'M', 'S', 'L', 'B', 'T'}});
    my_stack.push_back(Stack{std::vector<char>{'R', 'N', 'V', 'H', 'C', 'D', 'P'}});
    my_stack.push_back(Stack{std::vector<char>{'H', 'C', 'T'}});
    my_stack.push_back(Stack{std::vector<char>{'G', 'S', 'J', 'V', 'Z', 'N', 'H', 'P'}});
    my_stack.push_back(Stack{std::vector<char>{'Z', 'F', 'H', 'G'}});

    // my_stack.push_back(Stack{std::vector<char>{'Z', 'N'}});
    // my_stack.push_back(Stack{std::vector<char>{'M', 'C', 'D'}});
    // my_stack.push_back(Stack{std::vector<char>{'P'}});

    return my_stack;
}

std::vector<Order> read_commands(std::string filepath)
{
    std::ifstream input_file(filepath);
    std::string line;
    std::vector<Order> commands;

    if(!input_file.good())
    {
        std::cout << "could not open file " << filepath << std::endl;
        return commands;
    }

    while(std::getline(input_file, line))
    {
        std::regex regex("move ([0-9]+) from ([0-9]) to ([0-9])");

        std::smatch all_matches;
        std::regex_match (line, all_matches, regex);
        std::vector<std::string> matches_as_str;

        for (auto m : all_matches)
        {
            matches_as_str.push_back(m.str());
        }

        Order current_order = {
            std::stoi(matches_as_str[1]),
            std::stoi(matches_as_str[2]),
            std::stoi(matches_as_str[3])};

        commands.push_back(current_order);
    }

    return commands;
}

int main()
{
    std::cout << "Hello Advent" << std::endl;
    std::vector<Order> commands = read_commands("/home/sophie/Documents/AdventofCode/inputs/task_5.txt");

    std::vector<Stack> ship = hardcode_setup();

    for (Order o : commands)
    {
        std::vector<char> moved_crates = ship[o.from - 1].remove_crates(o.amount);
        ship[o.to - 1].add_crates_reverse(moved_crates);
    }

    for (auto s : ship)
    {
        std::cout << s.current_stack.back() << " ";
    }
    std::cout << std::endl;
}
//GCFGLFNJZ
//GCFGLDNJZ
