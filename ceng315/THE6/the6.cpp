#include "the6.h"

// do not add extra libraries here

int find_structure(std::vector<std::vector<std::pair<int, int>>> &bond_energies, std::vector<std::vector<std::pair<int, int>>> &lowest_energy_structure)
{
    int total_bond_energy = 0;
    int num_vertices = bond_energies.size();

    std::vector<bool> visited(num_vertices, false);
    std::vector<int> parent(num_vertices, -1);
    std::vector<int> min_weight(num_vertices, INT_MAX);

    min_weight[0] = 0;

    for (int count = 0; count < num_vertices - 1; ++count)
    {
        int min_index = -1;
        int min_value = INT_MAX;

        for (int v = 0; v < num_vertices; ++v)
        {
            if (!visited[v] && min_weight[v] < min_value)
            {
                min_index = v;
                min_value = min_weight[v];
            }
        }

        visited[min_index] = true;

        for (const auto &edge : bond_energies[min_index])
        {
            int u = edge.first;
            int weight = edge.second;

            if (!visited[u] && weight < min_weight[u])
            {
                parent[u] = min_index;
                min_weight[u] = weight;
            }
        }
    }

    for (int v = 1; v < num_vertices; ++v)
    {
        total_bond_energy += min_weight[v];
        lowest_energy_structure[parent[v]].emplace_back(v, min_weight[v]);
        lowest_energy_structure[v].emplace_back(parent[v], min_weight[v]);
    }

    return total_bond_energy;
}
void dfs(int start, std::vector<std::vector<std::pair<int, int>>> &molecule_structure, std::vector<int> &chain)
{
    std::stack<int> stack;
    std::vector<int> temp_chain;
    std::vector<bool> visited(molecule_structure.size(), false);
    stack.push(start);
    temp_chain.push_back(start);
    visited[start] = true;
    while (!stack.empty())
    {
        int vertex = stack.top();
        int visiting = -1;
        for (const auto &pair : molecule_structure[vertex])
        {
            int adj = pair.first;
            if (visited[adj] == false)
            {
                visiting = adj;
                break;
            }
        }
        if (visiting != -1)
        {
            stack.push(visiting);
            temp_chain.push_back(visiting);
            visited[visiting] = true;
            if (stack.size() > chain.size())
            {
                chain = temp_chain;
            }
        }
        else
        {
            stack.pop();
            temp_chain.pop_back();
        }
    }
}

int find_longest_chain(std::vector<std::vector<std::pair<int, int>>> &molecule_structure, std::vector<int> &chain)
{
    std::vector<int> longest;
    for (int i = 0; i < molecule_structure.size(); i++)
    {
        std::vector<int> current;
        dfs(i, molecule_structure, current);
        if (current.size() > longest.size())
        {
            longest = current;
        }
    }
    chain = longest;
    return longest.size();
}
