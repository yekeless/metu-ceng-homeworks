#include <string>
#include <iostream>
#include <pthread.h>
#include <vector>
#include <utility>
#include "WriteOutput.h"
#include "helper.h"
#include "Car.h"
#include "NarrowBridge.h"
#include "Ferry.h"
#include "CrossRoad.h"

using namespace std;

void *car_thread(void *car)
{
    Car *car1 = (Car *)car;
    int way;
    for (int i = 0; i < car1->path_length; i++)
    {
        Connector connector = car1->path[i];
        NarrowBridge *narrowBridge;
        Ferry *ferry;
        CrossRoad *crossRoad;
        switch (connector.type)
        {
        case NARROW_BRIDGE:
            way = connector.direction.first;
            narrowBridge = (NarrowBridge *)connector.connector;
            WriteOutput(car1->id, 'N', narrowBridge->id, TRAVEL);
            sleep_milli(car1->travel_time);
            // WriteOutput(car1->id, 'N', narrowBridge->id, ARRIVE);
            narrowBridge->Pass(car1, way);
            break;
        case FERRY:
            way = connector.direction.first;
            ferry = (Ferry *)connector.connector;
            WriteOutput(car1->id, 'F', ferry->id, TRAVEL);
            sleep_milli(car1->travel_time);
            // WriteOutput(car1->id, 'F', ferry->id, ARRIVE);
            ferry->Pass(car1, way);
            break;
        case CROSS_ROAD:
            way = connector.direction.first;
            crossRoad = (CrossRoad *)connector.connector;
            WriteOutput(car1->id, 'C', crossRoad->id, TRAVEL);
            sleep_milli(car1->travel_time);
            // WriteOutput(car1->id, 'C', crossRoad->id, ARRIVE);
            crossRoad->Pass(car1, way);
            break;
        }
    }
    return NULL;
}

int main()
{
    vector<pthread_t> threads;
    vector<Car *> cars;
    vector<NarrowBridge *> narrowBridges;
    vector<Ferry *> ferries;
    vector<CrossRoad *> crossRoads;
    int number_of_narrow_bridges, number_of_ferries, number_of_cross_roads;
    cin >> number_of_narrow_bridges;
    for (int i = 0; i < number_of_narrow_bridges; i++)
    {
        int travel_time, maximum_waiting_time;
        cin >> travel_time >> maximum_waiting_time;
        narrowBridges.push_back(new NarrowBridge(i, travel_time, maximum_waiting_time));
    }
    cin >> number_of_ferries;
    for (int i = 0; i < number_of_ferries; i++)
    {
        int travel_time, maximum_waiting_time, capacity;
        cin >> travel_time >> maximum_waiting_time >> capacity;
        ferries.push_back(new Ferry(i, travel_time, maximum_waiting_time, capacity));
    }
    cin >> number_of_cross_roads;
    for (int i = 0; i < number_of_cross_roads; i++)
    {
        int travel_time, maximum_waiting_time;
        cin >> travel_time >> maximum_waiting_time;
        crossRoads.push_back(new CrossRoad(i, travel_time, maximum_waiting_time));
    }
    int number_of_cars;
    cin >> number_of_cars;

    for (int i = 0; i < number_of_cars; i++)
    {
        int travel_time, path_length;
        cin >> travel_time >> path_length;
        vector<Connector> path;
        for (int j = 0; j < path_length; j++)
        {
            char connector_type;
            int connector_id;
            int from, to;
            cin >> connector_type >> connector_id;
            cin >> from >> to;
            Connector connector;
            connector.direction = make_pair(from, to);
            // connector.type= (ConnectorType)connector_type;
            switch (connector_type)
            {
            case 'N':
                connector.type = static_cast<ConnectorType>(NARROW_BRIDGE);
                // connector.connector = (narrowBridges[connector_id]); // here do I need to convert it to the void pointer ???
                connector.connector = static_cast<void *>(narrowBridges[connector_id]);
                break;
            case 'F':
                connector.type = static_cast<ConnectorType>(FERRY);
                // connector.connector = (ferries[connector_id]);
                connector.connector = static_cast<void *>(ferries[connector_id]);
                break;
            case 'C':
                connector.type = static_cast<ConnectorType>(CROSS_ROAD);
                // connector.connector = (crossRoads[connector_id]);
                connector.connector = static_cast<void *>(crossRoads[connector_id]);
                break;
            }
            path.push_back(connector);
        }
        cars.push_back(new Car(i, travel_time, path, path_length));
    }
    InitWriteOutput();
    for (int i = 0; i < number_of_cars; i++)
    {
        pthread_t thread;
        pthread_create(&thread, NULL, car_thread, cars[i]);
        threads.push_back(thread);
    }
    for (int i = 0; i < number_of_cars; i++)
    {
        pthread_join(threads[i], NULL);
    }
    return 0;
}