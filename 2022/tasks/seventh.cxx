#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

class Directory;

class File
{
public:
    std::string name;
    int size = 0;
    Directory* parent;
};

class Directory
{
public:
    Directory* go_into_dir(std::string name)
    {
        for(std::map<std::string, Directory>::iterator iter = inner_dirs.begin();
            iter != inner_dirs.end();
            ++iter)
        {
            if (iter->first == name)
                return &(iter->second);
        }
        return nullptr;
    }

    void add_file(std::string name, int size)
    {
        File new_file;
        new_file.size = size;
        new_file.name = name;
        new_file.parent = this;
        inner_files.insert({name, new_file});

        update_size(size);
    }

    void add_dir(std::string name)
    {
        Directory new_dir;
        new_dir.name = name;
        new_dir.parent = this;
        inner_dirs.insert({name, new_dir});
    }

    void update_size(int additional_size)
    {
        accumulated_size += additional_size;
        if (parent != nullptr)
        {
            parent->update_size(additional_size);
        }
    }

    std::string name;
    int accumulated_size = 0;
    std::map<std::string, Directory> inner_dirs;
    std::map<std::string, File> inner_files;
    Directory* parent = nullptr;

};



std::vector<std::string> read_console_log(std::string filepath)
{
    std::ifstream input_file(filepath);
    std::string line;
    std::vector<std::string> lines;

    if(!input_file.good())
    {
        std::cout << "could not open file " << filepath << std::endl;
        return lines;
    }

    while(std::getline(input_file, line))
    {
        lines.push_back(line);
    }
    return lines;
}

std::vector<std::string> split_string(std::string str_input, std::string delimiter)
{
    size_t pos = 0;
    std::string token;
    std::vector<std::string> all_parts;
    std::string input_copy = str_input;

    while ((pos = input_copy.find(delimiter)) != std::string::npos) {
        token = input_copy.substr(0, pos);
        all_parts.push_back(token);
        input_copy.erase(0, pos + delimiter.length());
    }
    all_parts.push_back(input_copy);
    return all_parts;
}

Directory build_filesystem(std::vector<std::string> console_log)
{
    Directory base;
    Directory* current;
    bool inside_ls = false;
    if (console_log[0] == "$ cd /")
    {
        base.name = "/";
        current = &base;
    }
    for(size_t i = 1; i < console_log.size(); ++i)
    {
        std::vector<std::string> current_line = split_string(console_log[i], " ");

        if(current_line[0] == "$")
        {
            if (current_line[1] == "ls")
            {
                inside_ls = true;
            }
            else if (current_line[1] == "cd")
            {
                inside_ls = false;
                if (current_line[2] == "..")
                {
                    current = current->parent;
                }
                else
                {
                    Directory* new_dir = current->go_into_dir(current_line[2]);
                    if (new_dir == nullptr)
                    {
                        std::cout << "Bäääääh" << std::endl;
                        return base;
                    }
                    current = new_dir;
                }
            }
            else
            {
                std::cout << "Unknown command " << console_log[i] << std::endl;

            }
        }
        else
        {
            if (!inside_ls)
            {
                std::cout << "I should not be reading? " << console_log[i] << std::endl;
                return base;
            }
            if(current_line[0] == "dir")
                current->add_dir(current_line[1]);
            else
                current->add_file(current_line[1], std::stoi(current_line[0]));
        }
    }
    return base;
}

int sum_small_dirs(Directory base, int max_size)
{
    Directory& current = base;
    int sum = 0;
    for(std::map<std::string, Directory>::iterator iter = current.inner_dirs.begin();
        iter != current.inner_dirs.end();
        ++iter)
    {
        if (iter->second.accumulated_size <= max_size)
        {
            sum += iter->second.accumulated_size;
        }
        sum += sum_small_dirs(iter->second, max_size);
    }
    return sum;
}

bool enough(int size)
{
    int total_size = 70000000;
    int needed_size = 30000000;
    int currently_used = 43598596;
    bool result = (total_size - (currently_used - size)) >= needed_size;
    return result;
}

int directories_big_enough(Directory base, int best_candidate)
{
    Directory& current = base;
    int sum = 0;
    for(std::map<std::string, Directory>::iterator iter = current.inner_dirs.begin();
        iter != current.inner_dirs.end();
        ++iter)
    {
        int my_size = iter->second.accumulated_size;
        if (enough(my_size))
        {
            if(my_size < best_candidate)
                best_candidate = my_size;
            best_candidate = directories_big_enough(iter->second, best_candidate);
        }
    }
    return best_candidate;
}

// int get_deletion_size(Directory base)
// {
//     std::vector<int> candidate_sizes = directories_big_enough(base, std::vector<int>());
//     std::vector<int>::iterator result = std::min_element(candidate_sizes.begin(), candidate_sizes.end());
//     return *result;
// }

int main()
{
    std::cout << "Hello Advent" << std::endl;

    std::vector<std::string> console_log = read_console_log("/home/sophie/Documents/AdventofCode/inputs/task_7.txt");

    Directory filesystem = build_filesystem(console_log);

    std::cout << sum_small_dirs(filesystem, 100000) << std::endl;

    std::cout << directories_big_enough(filesystem, 70000000) << std::endl;

}
