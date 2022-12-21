#include <iostream>
#include <vector>
#include <fstream>


std::string read_signals(std::string filepath)
{
    std::ifstream input_file(filepath);
    std::string line;

    if(!input_file.good())
    {
        std::cout << "could not open file " << filepath << std::endl;
        return "";
    }

    while(std::getline(input_file, line))
    {
        return line;
    }
    return "";
}

bool different_chars(char one, char two, char three, char four)
{
    if ( (one != two) && (one != three) && (one != four) &&
        (two != three) && (two != four) &&
        (three != four))
        return true;
    return false;
}

int char_in_string(std::string message, char test)
{
    for(int i = message.length() - 1; i >= 0; i--)
        if (message[i] == test)
            return i;
    return -1;
}

int test_for_fourteen(std::string signals_as_string)
{
    //test first 14 first
    std::string current_test = "";
    int last_duplicate_index = -1;
    for (int i=0; i < 14; ++i)
    {
        char test_char = signals_as_string[i];
        int current_duplicate_index  = char_in_string(current_test, test_char) ;
        if(current_duplicate_index != -1)
        {
            last_duplicate_index = std::max(last_duplicate_index, current_duplicate_index);
        }
        current_test = signals_as_string.substr(0, i);
    }
    if (last_duplicate_index == -1)
    {
        std::cout << "First 14 are different!" << std::endl;
        return 14;
    }

    current_test = signals_as_string.substr(1, 13);
    for (int i=14; i < signals_as_string.length(); ++i)
    {
        char test_char = signals_as_string[i];
        int current_duplicate_index  = char_in_string(current_test, test_char) ;
        if(current_duplicate_index != -1)
        {
            last_duplicate_index = std::max(last_duplicate_index, current_duplicate_index + (i - 13));
        }
        else
        {
            if (last_duplicate_index < (i - 13))
            {
                return i + 1;
            }
        }
        current_test = signals_as_string.substr(i - 12, 13);
    }
    return -1;
}

int main()
{
    std::cout << "Hello Advent" << std::endl;
    std::string signals_as_string = read_signals("/home/sophie/Documents/AdventofCode/inputs/task_6.txt");

    int counter = 4;
    for (auto it = signals_as_string.begin() + 3; it != signals_as_string.end(); it++, counter++)
    {
        if (different_chars(*it, *(it - 1), *(it - 2), *(it - 3)))
            break;
    }

    std::cout << "First match of 4 at " << counter << std::endl;

    std::cout << "And with 14 " << test_for_fourteen(signals_as_string) << std::endl;
}


