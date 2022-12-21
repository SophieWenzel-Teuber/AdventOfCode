#include <iostream>
#include <fstream>

#include <algorithm>
#include <numeric>
#include <vector>
#include <map>
#include <utility>

// Rock:        A, X, 1     > Paper
// Paper:       B, Y, 2     > Scissors
// Scissors:    C, Z, 3     > Rock

std::map<char, int> base_scores = {
    std::make_pair<char, int> ('A', 1), std::make_pair<char, int>('B', 2), std::make_pair<char, int>('C', 3),
    std::make_pair<char, int> ('X', 1), std::make_pair<char, int>('Y', 2), std::make_pair<char, int>('Z', 3)
    };

//scores you will get when loosing against input
std::map<char, int> looser_scores = {
    std::make_pair<char, int> ('A', 3), std::make_pair<char, int>('B', 1), std::make_pair<char, int>('C', 2),
    };

//scores you will get when winning against input
std::map<char, int> winner_scores = {
    std::make_pair<char, int> ('A', 2), std::make_pair<char, int>('B', 3), std::make_pair<char, int>('C', 1),
    };


int get_single_score(char opponent, char mine)
{
    // a draw
    if (base_scores[opponent] == base_scores[mine]) return 3 + base_scores[mine];

    if ((mine == 'X') && (opponent == 'C')) return 6 + base_scores[mine];
    if ((mine == 'Y') && (opponent == 'A')) return 6 + base_scores[mine];
    if ((mine == 'Z') && (opponent == 'B')) return 6 + base_scores[mine];

    return 0 + base_scores[mine];
}

int get_single_score_p2(char opponent, char outcome)
{
    // a draw
    if (outcome == 'Y') return 3 + base_scores[opponent];

    //loosing
    if (outcome == 'X') return looser_scores[opponent];

    if (outcome == 'Z') return 6 + winner_scores[opponent];

    return 0 ;
}

std::vector<int> read_scores(std::string filepath)
{
    std::ifstream input_file(filepath);
    std::string line;
    std::vector<int> scores;

    if(!input_file.good())
    {
        std::cout << "could not open file " << filepath << std::endl;
        return scores;
    }

    while(std::getline(input_file, line))
    {
        //scores.push_back(get_single_score(line[0], line[2]));
        scores.push_back(get_single_score_p2(line[0], line[2]));
    }

    for (auto it = scores.begin(); it != scores.end(); ++it) std::cout << *it << std::endl;

    return scores;
}

int main()
{
    std::cout << "Hello Advent" << std::endl;

    // std::cout << get_single_score_p2('A', 'Y') << std::endl;
    // std::cout << get_single_score_p2('B', 'X') << std::endl;
    // std::cout << get_single_score_p2('C', 'Z') << std::endl;

    std::vector<int> scores = read_scores("/home/sophie/Documents/AdventofCode/inputs/task_2.txt");

    std::cout << "Total score is " << std::accumulate(scores.begin(),scores.end(),0) << std::endl;

}
