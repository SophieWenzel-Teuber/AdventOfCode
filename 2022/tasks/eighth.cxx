#include <iostream>
#include <fstream>
#include <vector>

std::vector<std::vector<int>> read_woods(std::string filepath)
{
    std::ifstream input_file(filepath);
    std::string line;
    std::vector<std::vector<int>> woods;

    if(!input_file.good())
    {
        std::cout << "could not open file " << filepath << std::endl;
        return woods;
    }

    while(std::getline(input_file, line))
    {
        std::vector<int> treeline;
        for (int i = 0; i < line.length(); ++i)
        {
            treeline.push_back(std::stoi(std::string(1, line[i])));
        }
        woods.push_back(treeline);
    }
    return woods;
}

int visible_trees(std::vector<std::vector<int>> woods)
{
    // start off with outer trees
    int visible_count = (2 * woods.size()) + (2 * (woods[0].size() - 2));

    for(int i = 1; i < woods.size() - 1; i++)
    {
        for(int j = 1; j < woods[i].size() - 1; j++)
        {
            bool blocked_left = false;
            bool blocked_right = false;
            bool blocked_up = false;
            bool blocked_down = false;
            //left
            for(int k = 0; k < j; k++)
            {
                if (woods[i][k] >= woods[i][j])
                {
                    blocked_left = true;
                    break;
                }
            }
            //right
            for(int k = j + 1; k < woods[i].size(); k++)
            {
                if (woods[i][k] >= woods[i][j])
                {
                    blocked_right = true;
                    break;
                }
            }

            //up
            for(int k = 0; k < i; k++)
            {
                if (woods[k][j] >= woods[i][j])
                {
                    blocked_up = true;
                    break;
                }
            }
            //down
            for(int k = i + 1; k < woods.size(); k++)
            {
                if (woods[k][j] >= woods[i][j])
                {
                    blocked_down = true;
                    break;
                }
            }
            // if not blocked from all sides it's visible
            if (!( blocked_down && blocked_left && blocked_right && blocked_up ))
                visible_count++;
        }
    }
    return visible_count;
}


int scenic_trees(std::vector<std::vector<int>> woods)
{
    int best_score = 0;
    for(int i = 1; i < woods.size() - 1; i++)
    {
        for(int j = 1; j < woods[i].size() - 1; j++)
        {
            int score_left = -1;
            int score_right = -1;
            int score_up = -1;
            int score_down = -1;
            //left
            for(int k = j - 1; k >= 0; k--)
            {
                if (woods[i][k] >= woods[i][j])
                {
                    score_left = std::abs(j-k);
                    break;
                }
            }
            if (score_left == -1)
                score_left = j;

            //right
            for(int k = j + 1; k < woods[i].size(); k++)
            {
                if (woods[i][k] >= woods[i][j])
                {
                    score_right = std::abs(j-k);
                    break;
                }
            }
            if (score_right == -1)
                score_right = woods[i].size() - 1 - j;

            //up
            for(int k = i - 1; k >= 0; k--)
            {
                if (woods[k][j] >= woods[i][j])
                {
                    score_up = std::abs(i-k);
                    break;
                }
            }
            if (score_up == -1)
                score_up = i;
            //down
            for(int k = i + 1; k < woods.size(); k++)
            {
                if (woods[k][j] >= woods[i][j])
                {
                    score_down = std::abs(i-k);
                    break;
                }
            }
            if (score_down == -1)
                score_down = woods.size() - 1 - i;

            int total_score = score_down * score_left * score_right * score_up;
            if (best_score < total_score)
            {
                best_score = total_score;
            }
        }
    }
    return best_score;
}

int main()
{
    std::cout << "Hello Advent" << std::endl;
    std::vector<std::vector<int>> woods = read_woods("/home/sophie/Documents/AdventofCode/inputs/task_8.txt");
    std::cout << woods[0][0] << " " << woods[0][1] << " " << woods[0][2] << " " << woods[0][3] << " " << woods[0][4] << std::endl;
    std::cout << woods[1][0] << " " << woods[1][1] << " " << woods[1][2] << " " << woods[1][3] << " " << woods[1][4] << std::endl;

    std::cout << "Visible trees: " << visible_trees(woods) << std::endl;
    std::cout << "Best scenic score: " << scenic_trees(woods) << std::endl;
}
