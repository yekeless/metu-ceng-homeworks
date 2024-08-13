#include "MeshGraph.h"
#include "BinaryHeap.h"

// For printing
#include <fstream>
#include <iostream>
#include <sstream>

MeshGraph::MeshGraph(const std::vector<Double3> &vertexPositions,
                     const std::vector<IdPair> &edges)
{
    int number_of_vertices = vertexPositions.size();
    int number_of_edges = edges.size();
    for (int i = 0; i < number_of_vertices; i++)
    {
        Vertex newnode;
        newnode.id = i;
        newnode.position3D = vertexPositions[i];
        vertices.push_back(newnode);
    }
    for (int j = 0; j < number_of_vertices; j++)
    {
        std::list<Vertex *> newlist;
        for (int k = 0; k < number_of_edges; k++)
        {
            if (j == edges[k].vertexId0)
            {
                Vertex *newnode = &vertices[edges[k].vertexId1];
                newlist.push_back(newnode);
            }
            else if (j == edges[k].vertexId1)
            {
                Vertex *newnode = &vertices[edges[k].vertexId0];
                newlist.push_back(newnode);
            }
        }
        adjList.push_back(newlist);
    }
}

double MeshGraph::AverageDistanceBetweenVertices() const
{
    int edge_count = 0;
    double sum = 0;

    for (int i = 0; i < adjList.size(); i++)
    {
        Vertex *curr;
        for (std::list<Vertex *>::const_iterator it = adjList[i].begin(); it != adjList[i].end(); ++it)
        {
            curr = *it;
            if (i < (curr->id))
            {
                edge_count++;
                sum += Double3::Distance(vertices[i].position3D, curr->position3D);
            }
        }
    }
    return sum / edge_count;
}

double MeshGraph::AverageEdgePerVertex() const
{
    double total_edge = 0;
    double total_vertices = vertices.size();
    for (int i = 0; i < adjList.size(); i++)
    {
        int size_of_list = adjList[i].size();
        total_edge += size_of_list;
    }
    return (total_edge / total_vertices) / 2;
}

int MeshGraph::TotalVertexCount() const
{
    return vertices.size();
}

int MeshGraph::TotalEdgeCount() const
{
    int edge_count = 0;
    for (int i = 0; i < adjList.size(); i++)
    {
        Vertex *curr;
        for (std::list<Vertex *>::const_iterator it = adjList[i].begin(); it != adjList[i].end(); ++it)
        {
            curr = *it;
            if (i < (curr->id))
            {
                edge_count++;
            }
        }
    }
    return edge_count;
}

int MeshGraph::VertexEdgeCount(int vertexId) const
{
    if (vertexId > vertices.size() - 1 || vertexId < 0)
    {
        return -1;
    }
    else
    {
        return adjList[vertexId].size();
    }
}

void MeshGraph::ImmediateNeighbours(std::vector<int> &outVertexIds,
                                    int vertexId) const
{
    if (vertexId > vertices.size() - 1 || vertexId < 0)
    {
        return;
    }
    else
    {
        Vertex *curr;
        for (std::list<Vertex *>::const_iterator it = adjList[vertexId].begin(); it != adjList[vertexId].end(); ++it)
        {
            curr = *it;
            outVertexIds.push_back(curr->id);
        }
    }
}

void MeshGraph::PaintInBetweenVertex(std::vector<Color> &outputColorAllVertex,
                                     int vertexIdFrom, int vertexIdTo,
                                     const Color &color) const
{
    int start = vertexIdFrom;
    int end = vertexIdTo;
    std::vector<int> shortway;
    int size = vertices.size();
    std::vector<double> distances(size, INFINITY);
    std::vector<int> previous(size, -1);
    std::vector<bool> visited(size, false);
    BinaryHeap min_heap;
    distances[start] = 0;
    min_heap.Add(start, 0);
    while (min_heap.HeapSize() > 0)
    {
        int curr_vertex;
        double curr_distance;
        min_heap.PopHeap(curr_vertex, curr_distance);
        if (visited[curr_vertex])
        {
            continue;
        }
        else
        {
            visited[curr_vertex] = true;
            Vertex *curr;
            for (std::list<Vertex *>::const_iterator it = adjList[curr_vertex].begin(); it != adjList[curr_vertex].end(); ++it)
            {
                curr = *it;

                double dist = distances[curr_vertex] + Double3::Distance(vertices[curr_vertex].position3D, curr->position3D);
                if (dist < distances[curr->id])
                {
                    distances[curr->id] = dist;
                    previous[curr->id] = curr_vertex;
                    if (min_heap.contains(curr->id))
                    {
                        min_heap.ChangePriority(curr->id, dist);
                    }
                    else
                    {
                        min_heap.Add(curr->id, dist);
                    }
                }
            }
        }
    }
    previous[start] = -1;
    std::vector<int> reverse_path;
    int path = end;
    reverse_path.push_back(path);
    while (previous[path] != -1)
    {
        reverse_path.push_back(previous[path]);
        path = previous[path];
    }
    while (!reverse_path.empty())
    {
        shortway.push_back(reverse_path.back());
        reverse_path.pop_back();
    }
    outputColorAllVertex.resize(0);
    for (int i = 0; i < vertices.size(); i++)
    {
        Color renk;
        renk.r = 0;
        renk.b = 0;
        renk.g = 0;
        outputColorAllVertex.push_back(renk);
    }
    for (int j = 0; j < shortway.size(); j++)
    {
        outputColorAllVertex[shortway[j]] = color;
    }
}

void MeshGraph::PaintInRangeGeodesic(std::vector<Color> &outputColorAllVertex,
                                     int vertexId, const Color &color,
                                     int maxDepth, FilterType type,
                                     double alpha) const
{

    if (vertexId < 0 || vertexId >= vertices.size())
    {
        outputColorAllVertex.resize(0);
    }
    else
    {
        outputColorAllVertex.resize(0);
        std::vector<int> depths(vertices.size(), -1);
        depths[vertexId] = 0;
        int weight = 0;
        std::vector<bool> visited(vertices.size(), false);
        std::vector<double> distances(vertices.size(), INFINITY);
        distances[vertexId] = 0;
        BinaryHeap queue;
        queue.Add(vertexId, weight);
        weight++;
        while (queue.HeapSize() > 0)
        {
            int currVertex;
            double unne;
            queue.PopHeap(currVertex, unne);
            if (depths[currVertex] > maxDepth)
            {
                break;
            }
            if (visited[currVertex])
            {
                continue;
            }
            else
            {
                visited[currVertex] = true;
                BinaryHeap helper;
                Vertex *curr;
                for (std::list<Vertex *>::const_iterator it = adjList[currVertex].begin(); it != adjList[currVertex].end(); ++it)
                {
                    curr = *it;
                    helper.Add(curr->id, curr->id);
                    if (distances[curr->id] == INFINITY)
                    {
                        double distance = Double3::Distance(vertices[currVertex].position3D, vertices[curr->id].position3D);
                        distances[curr->id] = distances[currVertex] + distance;
                    }
                    if (depths[curr->id] == -1)
                    {
                        depths[curr->id] = depths[currVertex] + 1;
                    }
                }
                while (helper.HeapSize() > 0)
                {
                    int curr_node;
                    double unnecessary;
                    helper.PopHeap(curr_node, unnecessary);
                    queue.Add(curr_node, weight);
                    weight++;
                }
            }
        }
        for (int i = 0; i < vertices.size(); i++)
        {
            Color renk;
            renk.r = 0;
            renk.g = 0;
            renk.b = 0;
            outputColorAllVertex.push_back(renk);
        }
        for (int i = 0; i < vertices.size(); i++)
        {
            if (type == FILTER_GAUSSIAN)
            {
                double filter_value;
                if (visited[i])
                {
                    Color new_color;
                    double x = distances[i];
                    double ust = -pow(x, 2) / pow(alpha, 2);
                    filter_value = exp(ust);
                    double red, green, blue;
                    red = double(color.r);
                    green = double(color.g);
                    blue = double(color.b);
                    red = red * filter_value;
                    blue = blue * filter_value;
                    green = green * filter_value;

                    new_color.r = (unsigned char)(red);
                    new_color.g = (unsigned char)(green);
                    new_color.b = (unsigned char)(blue);
                    outputColorAllVertex[i] = new_color;
                }
            }
            else if (type == FILTER_BOX)
            {
                double filter_value;
                if (visited[i])
                {
                    Color new_color;
                    double x = distances[i];
                    if (x <= alpha && (-1) * alpha <= x)
                    {
                        filter_value = 1;
                    }
                    else
                    {
                        filter_value = 0;
                    }

                    double red, green, blue;
                    red = double(color.r);
                    green = double(color.g);
                    blue = double(color.b);
                    red = red * filter_value;
                    blue = blue * filter_value;
                    green = green * filter_value;

                    new_color.r = (unsigned char)(red);
                    new_color.g = (unsigned char)(green);
                    new_color.b = (unsigned char)(blue);
                    outputColorAllVertex[i] = new_color;
                }
            }
        }
    }
}

void MeshGraph::PaintInRangeEuclidian(std::vector<Color> &outputColorAllVertex,
                                      int vertexId, const Color &color,
                                      int maxDepth, FilterType type,
                                      double alpha) const
{
    if (vertexId < 0 || vertexId >= vertices.size())
    {
        outputColorAllVertex.resize(0);
    }
    else
    {
        outputColorAllVertex.resize(0);
        std::vector<int> depths(vertices.size(), -1);
        depths[vertexId] = 0;
        int weight = 0;
        std::vector<bool> visited(vertices.size(), false);
        std::vector<double> distances(vertices.size(), INFINITY);
        distances[vertexId] = 0;
        BinaryHeap queue;
        queue.Add(vertexId, weight);
        weight++;
        while (queue.HeapSize() > 0)
        {
            int currVertex;
            double unne;
            queue.PopHeap(currVertex, unne);
            if (depths[currVertex] > maxDepth)
            {
                break;
            }
            if (visited[currVertex])
            {
                continue;
            }
            else
            {
                visited[currVertex] = true;
                BinaryHeap helper;
                Vertex *curr;
                for (std::list<Vertex *>::const_iterator it = adjList[currVertex].begin(); it != adjList[currVertex].end(); ++it)
                {
                    curr = *it;
                    helper.Add(curr->id, curr->id);
                    if (distances[curr->id] == INFINITY)
                    {
                        double distance = Double3::Distance(vertices[currVertex].position3D, vertices[curr->id].position3D);
                        distances[curr->id] = distances[currVertex] + distance;
                    }
                    if (depths[curr->id] == -1)
                    {
                        depths[curr->id] = depths[currVertex] + 1;
                    }
                }
                while (helper.HeapSize() > 0)
                {
                    int curr_node;
                    double unnecessary;
                    helper.PopHeap(curr_node, unnecessary);
                    queue.Add(curr_node, weight);
                    weight++;
                }
            }
        }
        for (int i = 0; i < vertices.size(); i++)
        {
            Color renk;
            renk.r = 0;
            renk.g = 0;
            renk.b = 0;
            outputColorAllVertex.push_back(renk);
        }
        for (int i = 0; i < vertices.size(); i++)
        {
            if (type == FILTER_GAUSSIAN)
            {
                double filter_value;
                if (visited[i])
                {
                    Color new_color;
                    double x = Double3::Distance(vertices[vertexId].position3D, vertices[i].position3D);
                    double ust = -pow(x, 2) / pow(alpha, 2);
                    filter_value = exp(ust);
                    double red, green, blue;
                    red = double(color.r);
                    green = double(color.g);
                    blue = double(color.b);
                    red = red * filter_value;
                    blue = blue * filter_value;
                    green = green * filter_value;

                    new_color.r = (unsigned char)(red);
                    new_color.g = (unsigned char)(green);
                    new_color.b = (unsigned char)(blue);
                    outputColorAllVertex[i] = new_color;
                }
            }
            else if (type == FILTER_BOX)
            {
                double filter_value;
                if (visited[i])
                {
                    Color new_color;
                    double x = Double3::Distance(vertices[vertexId].position3D, vertices[i].position3D);
                    if (x <= alpha && (-1) * alpha <= x)
                    {
                        filter_value = 1;
                    }
                    else
                    {
                        filter_value = 0;
                    }

                    double red, green, blue;
                    red = double(color.r);
                    green = double(color.g);
                    blue = double(color.b);
                    red = red * filter_value;
                    blue = blue * filter_value;
                    green = green * filter_value;

                    new_color.r = (unsigned char)(red);
                    new_color.g = (unsigned char)(green);
                    new_color.b = (unsigned char)(blue);
                    outputColorAllVertex[i] = new_color;
                }
            }
        }
    }
}

void MeshGraph::WriteColorToFile(const std::vector<Color> &colors,
                                 const std::string &fileName)
{
    std::stringstream s;
    for (int i = 0; i < static_cast<int>(colors.size()); i++)
    {
        int r = static_cast<int>(colors[i].r);
        int g = static_cast<int>(colors[i].g);
        int b = static_cast<int>(colors[i].b);

        s << r << ", " << g << ", " << b << "\n";
    }
    std::ofstream f(fileName.c_str());
    f << s.str();
}

void MeshGraph::PrintColorToStdOut(const std::vector<Color> &colors)
{
    // IMPLEMENTED
    for (int i = 0; i < static_cast<int>(colors.size()); i++)
    {
        std::cout << static_cast<int>(colors[i].r) << ", "
                  << static_cast<int>(colors[i].g) << ", "
                  << static_cast<int>(colors[i].b) << "\n";
    }
}