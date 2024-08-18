#include <vector>
#include <utility>
#include <iostream>
using namespace std;
#ifndef CAR_H_
#define CAR_H_
enum ConnectorType
{
    NARROW_BRIDGE,
    FERRY,
    CROSS_ROAD
};
struct Connector
{
    ConnectorType type;
    void *connector;
    pair<int, int> direction;
};

class Car
{
public:
    int id;
    int travel_time;
    std::vector<Connector> path;
    int path_length;
    Car(int id, int travel_time, std::vector<Connector> path, int path_length)
    {
        this->id = id;
        this->travel_time = travel_time;
        this->path = path;
        this->path_length = path_length;
    }
    void print()
    {
        cout << "Car " << id << " travel_time " << travel_time << " path_length " << path_length << endl;
        for (int i = 0; i < path_length; i++)
        {
            cout << "Connector " << i << " type " << path[i].type << " direction " << path[i].direction.first << " " << path[i].direction.second << endl;
        }
    }
};
#endif