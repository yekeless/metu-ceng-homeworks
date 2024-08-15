#include "the5.h"

// do not add extra libraries here

void dfs_part1(int file, std::vector<bool> &visited, const std::vector<std::vector<int>> &dependencyMatrix, std::vector<int> &st)
{
    visited[file] = true;
    for (int i = dependencyMatrix[file].size() - 1; i >= 0; i--)
    {
        if (dependencyMatrix[file][i] && !visited[i])
        {
            dfs_part1(i, visited, dependencyMatrix, st);
        }
    }
    st.push_back(file);
}
void dfs_part2(int file, std::vector<bool> &visited, const std::vector<std::vector<int>> &dependencyMatrix, std::vector<int> &st)
{
    visited[file] = true;
    for (int i = 0; i < dependencyMatrix[file].size(); i++)
    {
        if (dependencyMatrix[i][file] && !visited[i])
        {
            dfs_part2(i, visited, dependencyMatrix, st);
        }
    }
    st.push_back(file);
}
std::vector<int> helper_1(const std::vector<std::vector<int>> &dependencyMatrix)
{
    std::vector<bool> visited(dependencyMatrix.size(), false);
    std::vector<int> st;
    for (int i = dependencyMatrix.size() - 1; i >= 0; i--)
    {
        if (!visited[i])
        {
            dfs_part1(i, visited, dependencyMatrix, st);
        }
    }
    return st;
}
std::vector<int> helper_2(const std::vector<std::vector<int>> &dependencyMatrix, const std::vector<int> st, std::vector<std::vector<int>> &cyclicDependencies)
{
    std::vector<bool> visited(dependencyMatrix.size(), false);
    std::vector<int> order;
    for (int i = st.size() - 1; i >= 0; i--)
    {
        order.push_back(st[i]);
        if (!visited[st[i]])
        {
            std::vector<int> temp;
            dfs_part2(st[i], visited, dependencyMatrix, temp);
            if (temp.size() > 1 || (temp.size() == 1 && dependencyMatrix[temp[0]][temp[0]]))
            {
                cyclicDependencies.push_back(temp);
            }
        }
    }
    return order;
}
void run(const std::vector<std::vector<int>> &dependencyMatrix, bool &isCompilable, std::vector<int> &compileOrder, std::vector<std::vector<int>> &cyclicDependencies)
{
    std::vector<int> st;
    st = helper_1(dependencyMatrix);
    std::vector<int> result;
    result = helper_2(dependencyMatrix, st, cyclicDependencies);
    if (cyclicDependencies.size() > 0)
    {
        isCompilable = false;
    }
    else
    {
        isCompilable = true;
        compileOrder = result;
    }
}