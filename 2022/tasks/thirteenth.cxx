#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

enum results {wrong, equal, right};

class ListOrInt
{
public:
    ListOrInt()
    {}
    ListOrInt(ListOrInt * parent) : parent{parent}
    {}
    ListOrInt(ListOrInt * parent, int value) : parent{parent}, value{value}
    {}

    bool is_int()
    {
        return value != -1;
    }
    bool is_list()
    {
        return value == -1;
    }
    int value = -1;
    std::vector<ListOrInt> inner_things;
    ListOrInt * parent;
};


std::vector<ListOrInt> read_input(std::string filepath)
{
    std::ifstream input_file(filepath);
    std::string line;
    std::vector<ListOrInt> lists;

    if(!input_file.good())
    {
        std::cout << "could not open file " << filepath << std::endl;
        return lists;
    }

    while(std::getline(input_file, line))
    {
        if(line == "")
            continue;

        if(line[0] != '[')
        {
            ListOrInt number_only;
            number_only.value = std::stoi(line);
            lists.push_back(number_only);
            continue;
        }

        ListOrInt next_to_add;
        ListOrInt* current = &next_to_add;
        for(int i = 1; i < line.length(); i++)
        {
            if(line [i] == '[')
            {
                current->inner_things.push_back(ListOrInt(current));
                current = &(current->inner_things.back());
            }
            else if(line [i] == ']')
            {
                current = current->parent;
            }
            else if (line[i] == ',')
            {
                continue;
            }
            else
            {
                int number = std::stoi(std::string(1, line[i]));
                if(line[i+1] == '0')
                {
                    number = 10;
                    i++;
                }
                current->inner_things.push_back(ListOrInt(current, number));
            }
        }
        lists.push_back(next_to_add);
    }
    return lists;
}

results compare(ListOrInt& first, ListOrInt& second)
{
    // if(first == nullptr)
    //     return right;
    // if(second == nullptr)
    //     return wrong;

    if(first.is_int() && second.is_int())
    {
        if(first.value == second.value)
            return equal;
        if (first.value > second.value)
            return wrong;
        if (first.value < second.value)
            return right;
    }
    else if(!(first.is_int()) && !(second.is_int()))
    {

        if(first.inner_things.size() == 0)
        {
            if(second.inner_things.size() == 0)
                return equal;
            return right;
        }
        if(second.inner_things.size() == 0)
            return wrong;

        size_t min_items = std::min(first.inner_things.size(), second.inner_things.size());
        for(int i = 0; i < min_items; ++i )
        {
            results intermediate_res = compare(first.inner_things[i], second.inner_things[i]);
            if (intermediate_res != equal)
                return intermediate_res;
        }
        if( first.inner_things.size() == second.inner_things.size())
            return equal;
        if( first.inner_things.size() > second.inner_things.size())
            return wrong;
        if( first.inner_things.size() < second.inner_things.size())
            return right;
    }
    else
    {
        if(first.is_int())
        {
            ListOrInt new_list(&first, first.value);
            first.inner_things.push_back(new_list);
            first.value = -1;
        }
        if(second.is_int())
        {
            ListOrInt new_list(&second, second.value);
            second.inner_things.push_back(new_list);
            second.value = -1;
        }
        return compare(first, second);
    }
    return equal;
}

std::vector<bool> get_right_order(std::vector<ListOrInt> lists)
{
    std::vector<bool> all_res;
    for(int i=0; i < lists.size(); i += 2)
    {
        // ListOrInt * first = &lists[i];
        // ListOrInt * second = &lists[i + 1];

        results res = compare(lists[i], lists[i + 1]);
        if(res == right)
            all_res.push_back(true);
        else if(res == wrong)
            all_res.push_back(false);
        else
            std::cout << "Meh" << std::endl;
    }
    return all_res;
}

int main()
{
    std::cout << "Hello Advent" << std::endl;
    std::vector<ListOrInt> commands = read_input("/home/sophie/Documents/AdventofCode/inputs/task_13.txt");

    std::vector<bool> res = get_right_order(commands);
    int sum = 0;
    for(int i = 0; i<res.size(); ++i)
    {
        if(res[i])
            sum += i+1;
    }
    std::cout << "Sum of right order indices: " << sum << std::endl;

    // ListOrInt* first_div = &commands[commands.size() - 2];
    // ListOrInt* second_div = &commands[commands.size() - 1];

    // std::vector<ListOrInt *> pointers;
    // for(int i = 0; i < commands.size(); ++i)
    // {
    //     pointers.push_back(&(commands[i]));
    // }

    std::sort(commands.begin(), commands.end(), [](ListOrInt& a, ListOrInt& b) -> bool
    {
        results res = compare(a, b);
        if(res == right)
            return true;
        else
            return false;});

    std::cout << commands[0].value << std::endl;

    //Search for 2 and 6
    int dividers = -1;
    for(int i = 0; i < commands.size(); ++i)
    {
        ListOrInt& current = commands[i];
        bool break_out = false;
        while(!break_out && current.is_list())
        {
            if (current.inner_things.size() != 1)
                break_out = true;
            else
                current = current.inner_things[0];
        }
        if (current.value == 2 || current.value == 6)
        {
            if (dividers == -1)
                dividers = i + 1;
            else
            {
                dividers *= i + 1;
                break;
            }
        }
    }
    std::cout << "Div thingies: " << dividers << std::endl;

}
