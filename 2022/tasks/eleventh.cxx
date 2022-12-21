#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <algorithm>

#define really_long_number unsigned long long

//really_long_number dividing_modifier = 23 * 19* 13 * 17;
really_long_number dividing_modifier = 5*17*7*13*19*3*11*2;

class Monkey
{
public:
    Monkey(
        std::vector<really_long_number> items,
        std::function< really_long_number(really_long_number) > operation,
        int diviser,
        int target_true,
        int target_false) :
        items{items},
        operation{operation},
        diviser{diviser},
        target_true{target_true},
        target_false{target_false}
        {}

    void play(std::vector<Monkey>& monkeys)
    {
        for(int i = 0; i < items.size(); ++i)
        {
            //int new_item = operation(items[i]) / 3;
            really_long_number new_item = operation(items[i]) % dividing_modifier;
            if(new_item % diviser == 0)
                monkeys[target_true].catch_item(new_item);
            else
                monkeys[target_false].catch_item(new_item);
        }
        total_inspections += items.size();
        items = std::vector<really_long_number>();
    }

    void catch_item(int item)
    {
        items.push_back(item);
    }

    really_long_number total_inspections = 0;

private:
    std::vector<really_long_number> items;
    std::function< really_long_number(really_long_number) > operation = [](really_long_number old) { return old * 19; };
    int diviser;
    int target_true;
    int target_false;
};

std::vector<really_long_number> game(std::vector<Monkey> monkeys, int number_of_turns)
{
    for(int i = 0; i < number_of_turns; ++i)
    {
        for(auto it = monkeys.begin(); it != monkeys.end(); ++it)
        {
            it->play(monkeys);
        }
    }

    std::vector<really_long_number> inspections;
    for(auto it = monkeys.begin(); it != monkeys.end(); ++it)
    {
        inspections.push_back(it->total_inspections);
    }
    return inspections;

}

int main()
{
    std::cout << "Hello Advent" << std::endl;

    // std::vector<Monkey> monkeys =
    // {
    //     Monkey(std::vector<int>{79, 98}, [](really_long_number old) { return old * 19; }, 23, 2, 3),
    //     Monkey(std::vector<int>{54, 65, 75, 74}, [](really_long_number old) { return old + 6; }, 19, 2, 0),
    //     Monkey(std::vector<int>{79, 60, 97}, [](really_long_number old) { return old * old; }, 13, 1, 3),
    //     Monkey(std::vector<int>{74}, [](really_long_number old) { return old + 3; }, 17, 0, 1),
    // };

    std::vector<Monkey> monkeys =
    {
        Monkey(std::vector<really_long_number>{74, 64, 74, 63, 53}, [](really_long_number old) { return old * 7; }, 5, 1, 6),
        Monkey(std::vector<really_long_number>{69, 99, 95, 62}, [](really_long_number old) { return old * old; }, 17, 2, 5),
        Monkey(std::vector<really_long_number>{59, 81}, [](really_long_number old) { return old  + 8; }, 7, 4, 3),
        Monkey(std::vector<really_long_number>{50, 67, 63, 57, 63, 83, 97}, [](really_long_number old) { return old + 4; }, 13, 0, 7),
        Monkey(std::vector<really_long_number>{61, 94, 85, 52, 81, 90, 94, 70}, [](really_long_number old) { return old + 3; }, 19, 7, 3),
        Monkey(std::vector<really_long_number>{69}, [](really_long_number old) { return old + 5; }, 3, 4, 2),
        Monkey(std::vector<really_long_number>{54, 55, 58}, [](really_long_number old) { return old + 7; }, 11, 1, 5),
        Monkey(std::vector<really_long_number>{79, 51, 83, 88, 93, 76}, [](really_long_number old) { return old * 3; }, 2, 0, 6),
    };


    std::vector<really_long_number> inspections = game(monkeys, 10000);
    std::cout << inspections[0]  << " " << inspections[1]  << " " << inspections[2]  << " " << inspections[3]  << " " << std::endl;

    std::sort(inspections.rbegin(), inspections.rend() );
    std::cout << "Total Monkey business is " << inspections [0] * inspections[1] << std::endl;
}
