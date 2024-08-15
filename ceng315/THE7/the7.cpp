#include "the7.h"

// do not add extra libraries here

void get_infection_scores(const std::vector<std::vector<std::pair<int, int>>> &network, std::vector<float> &infection_scores)
{
    int N = network.size();
    std::vector<std::vector<int>> dist(N, std::vector<int>(N, std::numeric_limits<int>::max()));

    for (int i = 0; i < N; i++)
    {
        std::vector<int> distance(N, std::numeric_limits<int>::max());
        distance[i] = 0;
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
        pq.push({0, i});

        while (!pq.empty())
        {
            int u = pq.top().second;
            int d = pq.top().first;
            pq.pop();

            if (d > distance[u])
            {
                continue;
            }

            for (const auto &edge : network[u])
            {
                int v = edge.first;
                int weight = edge.second;
                if (distance[u] + weight < distance[v])
                {
                    distance[v] = distance[u] + weight;
                    pq.push({distance[v], v});
                }
            }
        }

        for (int j = 0; j < N; j++)
        {
            dist[i][j] = distance[j];
        }
    }

    infection_scores.resize(N);
    int max_distance = 0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (i != j && dist[i][j] != std::numeric_limits<int>::max())
            {
                max_distance = std::max(max_distance, dist[i][j]);
            }
        }
    }

    for (int i = 0; i < N; i++)
    {
        float sum = 0.0f;
        for (int j = 0; j < N; j++)
        {
            if (i != j)
            {
                if (dist[i][j] != std::numeric_limits<int>::max())
                {
                    sum += dist[i][j];
                }
                else
                {
                    sum += max_distance + 1;
                }
            }
        }
        float average_infection_speed = sum / (N - 1);
        infection_scores[i] = 1.0f / average_infection_speed;
    }
}
