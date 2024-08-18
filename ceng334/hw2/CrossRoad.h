#include <queue>
#include "monitor.h"
#include "Car.h"
#include "errno.h"
#include <time.h>
#include "WriteOutput.h"
#include "helper.h"
#include <ctime>
#include <iostream>
using namespace std;
class CrossRoad : public Monitor
{
public:
    int id;
    int travel_time;
    int maximum_waiting_time;
    Condition *lane0;
    Condition *lane1;
    Condition *lane2;
    Condition *lane3;
    queue<Car *> lane0_queue;
    queue<Car *> lane1_queue;
    queue<Car *> lane2_queue;
    queue<Car *> lane3_queue;
    int turn;
    int on_road;
    int last_passed;
    bool to0, to1, to2, to3;
    CrossRoad(int id, int travel_time, int maximum_waiting_time)
    {
        this->id = id;
        this->travel_time = travel_time;
        this->maximum_waiting_time = maximum_waiting_time;
        lane0 = new Condition(this);
        lane1 = new Condition(this);
        lane2 = new Condition(this);
        lane3 = new Condition(this);
        turn = 0;
        on_road = 0;
        last_passed = -1;
        to0 = false;
        to1 = false;
        to2 = false;
        to3 = false;
    }
    void Pass(Car *car, int way)
    {
        __synchronized__;
        WriteOutput(car->id, 'C', this->id, ARRIVE);
        if (way == 0)
        {
            lane0_queue.push(car);
        }
        if (way == 1)
        {
            lane1_queue.push(car);
        }
        if (way == 2)
        {
            lane2_queue.push(car);
        }
        if (way == 3)
        {
            lane3_queue.push(car);
        }
        // cout << "Car " << car->id << " added to lane " << way << " of CrossRoad " << id << endl;
        bool can_pass = false;
        while (!can_pass)
        {
            if (way == 0)
            {
                if (turn == 0)
                {
                    while (lane0_queue.front() != car)
                    {
                        lane0->wait();
                    }
                    if (turn != 0)
                    {
                        continue;
                    }
                    if (on_road > 0)
                    {
                        if (last_passed == 0)
                        {
                            // cout << car->id << " is waiting for " << PASS_DELAY << " ms" << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                            timespec now;
                            timespec pass_delay_amount;
                            clock_gettime(CLOCK_REALTIME, &now);
                            pass_delay_amount.tv_sec = now.tv_sec + (PASS_DELAY / 1000);
                            pass_delay_amount.tv_nsec = now.tv_nsec + (1000UL * (PASS_DELAY % 1000)) * 1000UL;
                            if (pass_delay_amount.tv_nsec >= 1000000000UL)
                            {
                                pass_delay_amount.tv_sec++;
                                pass_delay_amount.tv_nsec -= 1000000000;
                            }
                            int result = lane0->timedwait(&pass_delay_amount);
                            while (result != ETIMEDOUT && turn == 0)
                            {
                                result = lane0->timedwait(&pass_delay_amount); // may be we need to break this while if any lane timedout
                            }
                        }
                        else
                        {
                            // cout << car->id << " is waiting for " << on_road << " cars to pass" << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                            while (on_road > 0)
                            {
                                lane0->wait();
                            }
                        }
                    }
                    if (turn != 0)
                    {
                        continue;
                    }
                    WriteOutput(car->id, 'C', this->id, START_PASSING);
                    // cout << car->id << " is passing" << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                    on_road++;
                    can_pass = true;
                    last_passed = 0;
                    lane0_queue.pop();
                    lane0->notifyAll(); // turn cannot change here so we do not need to notify other lanes
                    timespec passing_time;
                    timespec imdi;
                    clock_gettime(CLOCK_REALTIME, &imdi);
                    passing_time.tv_sec = imdi.tv_sec + (travel_time / 1000);
                    passing_time.tv_nsec = imdi.tv_nsec + (1000UL * (travel_time % 1000)) * 1000UL;
                    if (passing_time.tv_nsec >= 1000000000UL)
                    {
                        passing_time.tv_sec++;
                        passing_time.tv_nsec -= 1000000000;
                    }
                    int passs = lane0->timedwait(&passing_time);
                    while (passs != ETIMEDOUT)
                    {
                        passs = lane0->timedwait(&passing_time);
                    }
                    on_road--;
                    WriteOutput(car->id, 'C', this->id, FINISH_PASSING);
                    // cout << car->id << " finished passing" << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                    if ((lane0_queue.empty() && on_road == 0) || turn != 0)
                    { // you  need to notify the other lane also when the queue is empty
                        if (turn == 1)
                        {
                            lane1->notifyAll();
                        }
                        else if (turn == 2)
                        {
                            lane2->notifyAll();
                        }
                        else if (turn == 3)
                        {
                            lane3->notifyAll();
                        }
                        else
                        {
                            // change lane(0)
                            if (lane1_queue.size() > 0)
                            {
                                turn = 1;
                            }
                            else if (lane2_queue.size() > 0)
                            {
                                turn = 2;
                            }
                            else if (lane3_queue.size() > 0)
                            {
                                turn = 3;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();
                        }
                    }
                }
                else
                {
                    bool perform_timeout = false;
                    while (lane0_queue.front() != car)
                    {
                        lane0->wait();
                    }
                    if (turn == 0)
                    {
                        continue;
                    }
                    if (turn == 1)
                    {
                        if (to0 || to2 || to3)
                        {
                            // change_turn(1);
                            if (lane2_queue.size() > 0)
                            {
                                turn = 2;
                            }
                            else if (lane3_queue.size() > 0)
                            {
                                turn = 3;
                            }
                            else if (lane0_queue.size() > 0)
                            {
                                turn = 0;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();

                            to0 = false;
                            to2 = false;
                            to3 = false;
                            if (turn == 0)
                            {
                                continue;
                            }
                            else
                            {
                                perform_timeout = true;
                            }
                        }
                        else if (lane1_queue.empty() && on_road == 0)
                        {
                            // change_turn(1);
                            if (lane2_queue.size() > 0)
                            {
                                turn = 2;
                            }
                            else if (lane3_queue.size() > 0)
                            {
                                turn = 3;
                            }
                            else if (lane0_queue.size() > 0)
                            {
                                turn = 0;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();
                            if (turn == 0)
                            {
                                continue;
                            }
                            else
                            {
                                perform_timeout = true;
                            }
                        }
                        else
                        {
                            perform_timeout = true;
                        }
                    }
                    else if (turn == 2)
                    {
                        if (to0 || to1 || to3)
                        {
                            // change_turn(2);
                            if (lane3_queue.size() > 0)
                            {
                                turn = 3;
                            }
                            else if (lane0_queue.size() > 0)
                            {
                                turn = 0;
                            }
                            else if (lane1_queue.size() > 0)
                            {
                                turn = 1;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();
                            to0 = false;
                            to1 = false;
                            to3 = false;
                            if (turn == 0)
                            {
                                continue;
                            }
                            else
                            {
                                perform_timeout = true;
                            }
                        }
                        else if (lane2_queue.empty() && on_road == 0)
                        {
                            // change_turn(2);
                            if (lane3_queue.size() > 0)
                            {
                                turn = 3;
                            }
                            else if (lane0_queue.size() > 0)
                            {
                                turn = 0;
                            }
                            else if (lane1_queue.size() > 0)
                            {
                                turn = 1;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();
                            if (turn == 0)
                            {
                                continue;
                            }
                            else
                            {
                                perform_timeout = true;
                            }
                        }
                        else
                        {
                            perform_timeout = true;
                        }
                    }
                    else if (turn == 3)
                    {
                        if (to0 || to1 || to2)
                        {
                            // change_turn(3);
                            if (lane0_queue.size() > 0)
                            {
                                turn = 0;
                            }
                            else if (lane1_queue.size() > 0)
                            {
                                turn = 1;
                            }
                            else if (lane2_queue.size() > 0)
                            {
                                turn = 2;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();
                            to0 = false;
                            to1 = false;
                            to2 = false;
                            if (turn == 0)
                            {
                                continue;
                            }
                            else
                            {
                                perform_timeout = true;
                            }
                        }
                        else if (lane3_queue.empty() && on_road == 0)
                        {
                            // change_turn(3);
                            if (lane0_queue.size() > 0)
                            {
                                turn = 0;
                            }
                            else if (lane1_queue.size() > 0)
                            {
                                turn = 1;
                            }
                            else if (lane2_queue.size() > 0)
                            {
                                turn = 2;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();
                            if (turn == 0)
                            {
                                continue;
                            }
                            else
                            {
                                perform_timeout = true;
                            }
                        }
                        else
                        {
                            perform_timeout = true;
                        }
                    }
                    if (perform_timeout)
                    {
                        // cout << car->id << " is waiting for " << maximum_waiting_time << " ms" << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                        perform_timeout = false;
                        timespec imdullah;
                        timespec timeoutatanullah;
                        clock_gettime(CLOCK_REALTIME, &imdullah);
                        timeoutatanullah.tv_sec = imdullah.tv_sec + (maximum_waiting_time / 1000);
                        timeoutatanullah.tv_nsec = imdullah.tv_nsec + (1000UL * (maximum_waiting_time % 1000)) * 1000UL;
                        if (timeoutatanullah.tv_nsec >= 1000000000UL)
                        {
                            timeoutatanullah.tv_sec++;
                            timeoutatanullah.tv_nsec -= 1000000000;
                        }
                        int result = lane0->timedwait(&timeoutatanullah);
                        if (result == ETIMEDOUT)
                        {
                            to0 = true;
                            // cout << car->id << " timed out" << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                            continue;
                        }
                        else
                        {
                            // cout << car->id << " is breaking max wait time" << on_road << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                            continue;
                        }
                    }
                }
            }
            if (way == 1)
            {
                if (turn == 1)
                {
                    while (lane1_queue.front() != car)
                    {
                        lane1->wait();
                    }
                    if (turn != 1)
                    {
                        continue;
                    }
                    if (on_road > 0)
                    {
                        if (last_passed == 1)
                        {
                            // cout << car->id << " is waiting for " << PASS_DELAY << " ms" << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                            timespec now;
                            timespec pass_delay_amount;
                            clock_gettime(CLOCK_REALTIME, &now);
                            pass_delay_amount.tv_sec = now.tv_sec + (PASS_DELAY / 1000);
                            pass_delay_amount.tv_nsec = now.tv_nsec + (1000UL * (PASS_DELAY % 1000)) * 1000UL;
                            if (pass_delay_amount.tv_nsec >= 1000000000UL)
                            {
                                pass_delay_amount.tv_sec++;
                                pass_delay_amount.tv_nsec -= 1000000000;
                            }
                            int result = lane1->timedwait(&pass_delay_amount);
                            while (result != ETIMEDOUT && turn == 1)
                            {
                                result = lane1->timedwait(&pass_delay_amount); // may be we need to break this while if any lane timedout
                            }
                        }
                        else
                        {
                            // cout << car->id << " is waiting for " << on_road << " cars to pass" << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                            while (on_road > 0)
                            {
                                lane1->wait();
                            }
                        }
                    }
                    if (turn != 1)
                    {
                        continue;
                    }
                    WriteOutput(car->id, 'C', this->id, START_PASSING);
                    // cout << car->id << " is passing" << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                    on_road++;
                    can_pass = true;
                    last_passed = 1;
                    lane1_queue.pop();
                    lane1->notifyAll(); // turn cannot change here so we do not need to notify other lanes
                    timespec passing_time;
                    timespec imdi;
                    clock_gettime(CLOCK_REALTIME, &imdi);
                    passing_time.tv_sec = imdi.tv_sec + (travel_time / 1000);
                    passing_time.tv_nsec = imdi.tv_nsec + (1000UL * (travel_time % 1000)) * 1000UL;
                    if (passing_time.tv_nsec >= 1000000000UL)
                    {
                        passing_time.tv_sec++;
                        passing_time.tv_nsec -= 1000000000;
                    }
                    int passs = lane1->timedwait(&passing_time);
                    while (passs != ETIMEDOUT)
                    {
                        passs = lane1->timedwait(&passing_time);
                    }
                    on_road--;
                    WriteOutput(car->id, 'C', this->id, FINISH_PASSING);
                    // cout << car->id << " finished passing" << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                    if ((lane1_queue.empty() && on_road == 0) || turn != 1)
                    { // you  need to notify the other lane also when the queue is empty
                        if (turn == 0)
                        {
                            lane0->notifyAll();
                        }
                        else if (turn == 2)
                        {
                            lane2->notifyAll();
                        }
                        else if (turn == 3)
                        {
                            lane3->notifyAll();
                        }
                        else
                        {
                            // change lane(1)
                            if (lane2_queue.size() > 0)
                            {
                                turn = 2;
                            }
                            else if (lane3_queue.size() > 0)
                            {
                                turn = 3;
                            }
                            else if (lane0_queue.size() > 0)
                            {
                                turn = 0;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();
                        }
                    }
                }
                else
                {
                    bool perform_timeout = false;
                    while (lane1_queue.front() != car)
                    {
                        lane1->wait();
                    }
                    if (turn == 1)
                    {
                        continue;
                    }
                    if (turn == 0)
                    {
                        if (to1 || to2 || to3)
                        {
                            // change_turn(0);
                            if (lane1_queue.size() > 0)
                            {
                                turn = 1;
                            }
                            else if (lane2_queue.size() > 0)
                            {
                                turn = 2;
                            }
                            else if (lane3_queue.size() > 0)
                            {
                                turn = 3;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();
                            to1 = false;
                            to2 = false;
                            to3 = false;
                            if (turn == 1)
                            {
                                continue;
                            }
                            else
                            {
                                perform_timeout = true;
                            }
                        }
                        else if (lane0_queue.empty() && on_road == 0)
                        {
                            // change_turn(0);
                            if (lane1_queue.size() > 0)
                            {
                                turn = 1;
                            }
                            else if (lane2_queue.size() > 0)
                            {
                                turn = 2;
                            }
                            else if (lane3_queue.size() > 0)
                            {
                                turn = 3;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();
                            if (turn == 1)
                            {
                                continue;
                            }
                            else
                            {
                                perform_timeout = true;
                            }
                        }
                        else
                        {
                            perform_timeout = true;
                        }
                    }
                    else if (turn == 2)
                    {
                        if (to0 || to1 || to3)
                        {
                            // change_turn(2);
                            if (lane3_queue.size() > 0)
                            {
                                turn = 3;
                            }
                            else if (lane0_queue.size() > 0)
                            {
                                turn = 0;
                            }
                            else if (lane1_queue.size() > 0)
                            {
                                turn = 1;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();
                            to0 = false;
                            to1 = false;
                            to3 = false;
                            if (turn == 1)
                            {
                                continue;
                            }
                            else
                            {
                                perform_timeout = true;
                            }
                        }
                        else if (lane2_queue.empty() && on_road == 0)
                        {
                            // change_turn(2);
                            if (lane3_queue.size() > 0)
                            {
                                turn = 3;
                            }
                            else if (lane0_queue.size() > 0)
                            {
                                turn = 0;
                            }
                            else if (lane1_queue.size() > 0)
                            {
                                turn = 1;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();
                            if (turn == 1)
                            {
                                continue;
                            }
                            else
                            {
                                perform_timeout = true;
                            }
                        }
                        else
                        {
                            perform_timeout = true;
                        }
                    }
                    else if (turn == 3)
                    {
                        if (to0 || to1 || to2)
                        {
                            // change_turn(3);
                            if (lane0_queue.size() > 0)
                            {
                                turn = 0;
                            }
                            else if (lane1_queue.size() > 0)
                            {
                                turn = 1;
                            }
                            else if (lane2_queue.size() > 0)
                            {
                                turn = 2;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();
                            to0 = false;
                            to1 = false;
                            to2 = false;
                            if (turn == 1)
                            {
                                continue;
                            }
                            else
                            {
                                perform_timeout = true;
                            }
                        }
                        else if (lane3_queue.empty() && on_road == 0)
                        {
                            // change_turn(3);
                            if (lane0_queue.size() > 0)
                            {
                                turn = 0;
                            }
                            else if (lane1_queue.size() > 0)
                            {
                                turn = 1;
                            }
                            else if (lane2_queue.size() > 0)
                            {
                                turn = 2;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();
                            if (turn == 1)
                            {
                                continue;
                            }
                            else
                            {
                                perform_timeout = true;
                            }
                        }
                        else
                        {
                            perform_timeout = true;
                        }
                    }
                    if (perform_timeout)
                    {
                        // cout << car->id << " is waiting for " << maximum_waiting_time << " ms" << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                        perform_timeout = false;
                        timespec imdullah;
                        timespec timeoutatanullah;
                        clock_gettime(CLOCK_REALTIME, &imdullah);
                        timeoutatanullah.tv_sec = imdullah.tv_sec + (maximum_waiting_time / 1000);
                        timeoutatanullah.tv_nsec = imdullah.tv_nsec + (1000UL * (maximum_waiting_time % 1000)) * 1000UL;
                        if (timeoutatanullah.tv_nsec >= 1000000000UL)
                        {
                            timeoutatanullah.tv_sec++;
                            timeoutatanullah.tv_nsec -= 1000000000;
                        }
                        int result = lane1->timedwait(&timeoutatanullah);
                        if (result == ETIMEDOUT)
                        {
                            to1 = true;
                            // cout << car->id << " timed out" << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                            continue;
                        }
                        else
                        {
                            // cout << car->id << " is breaking max wait time" << on_road << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                            continue;
                        }
                    }
                }
            }
            if (way == 2)
            {
                if (turn == 2)
                {
                    while (lane2_queue.front() != car)
                    {
                        lane2->wait();
                    }
                    if (turn != 2)
                    {
                        continue;
                    }
                    if (on_road > 0)
                    {
                        if (last_passed == 2)
                        {
                            // cout << car->id << " is waiting for " << PASS_DELAY << " ms" << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                            timespec now;
                            timespec pass_delay_amount;
                            clock_gettime(CLOCK_REALTIME, &now);
                            pass_delay_amount.tv_sec = now.tv_sec + (PASS_DELAY / 1000);
                            pass_delay_amount.tv_nsec = now.tv_nsec + (1000UL * (PASS_DELAY % 1000)) * 1000UL;
                            if (pass_delay_amount.tv_nsec >= 1000000000UL)
                            {
                                pass_delay_amount.tv_sec++;
                                pass_delay_amount.tv_nsec -= 1000000000;
                            }
                            int result = lane2->timedwait(&pass_delay_amount);
                            while (result != ETIMEDOUT && turn == 2)
                            {
                                result = lane2->timedwait(&pass_delay_amount); // may be we need to break this while if any lane timedout
                            }
                        }
                        else
                        {
                            // cout << car->id << " is waiting for " << on_road << " cars to pass" << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                            while (on_road > 0)
                            {
                                lane2->wait();
                            }
                        }
                    }
                    if (turn != 2)
                    {
                        continue;
                    }
                    WriteOutput(car->id, 'C', this->id, START_PASSING);
                    // cout << car->id << " is passing" << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                    on_road++;
                    can_pass = true;
                    last_passed = 2;
                    lane2_queue.pop();
                    lane2->notifyAll(); // turn cannot change here so we do not need to notify other lanes
                    timespec passing_time;
                    timespec imdi;
                    clock_gettime(CLOCK_REALTIME, &imdi);
                    passing_time.tv_sec = imdi.tv_sec + (travel_time / 1000);
                    passing_time.tv_nsec = imdi.tv_nsec + (1000UL * (travel_time % 1000)) * 1000UL;
                    if (passing_time.tv_nsec >= 1000000000UL)
                    {
                        passing_time.tv_sec++;
                        passing_time.tv_nsec -= 1000000000;
                    }
                    int passs = lane2->timedwait(&passing_time);
                    while (passs != ETIMEDOUT)
                    {
                        passs = lane2->timedwait(&passing_time);
                    }
                    on_road--;
                    WriteOutput(car->id, 'C', this->id, FINISH_PASSING);
                    // cout << car->id << " finished passing" << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                    if ((lane2_queue.empty() && on_road == 0) || turn != 2)
                    { // you  need to notify the other lane also when the queue is empty
                        if (turn == 0)
                        {
                            lane0->notifyAll();
                        }
                        else if (turn == 1)
                        {
                            lane1->notifyAll();
                        }
                        else if (turn == 3)
                        {
                            lane3->notifyAll();
                        }
                        else
                        {
                            // change lane 2
                            if (lane3_queue.size() > 0)
                            {
                                turn = 3;
                            }
                            else if (lane0_queue.size() > 0)
                            {
                                turn = 0;
                            }
                            else if (lane1_queue.size() > 0)
                            {
                                turn = 1;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();
                        }
                    }
                }
                else
                {
                    bool perform_timeout = false;
                    while (lane2_queue.front() != car)
                    {
                        lane2->wait();
                    }
                    if (turn == 2)
                    {
                        continue;
                    }
                    if (turn == 0)
                    {
                        if (to1 || to2 || to3)
                        {
                            // change_turn(0);
                            if (lane1_queue.size() > 0)
                            {
                                turn = 1;
                            }
                            else if (lane2_queue.size() > 0)
                            {
                                turn = 2;
                            }
                            else if (lane3_queue.size() > 0)
                            {
                                turn = 3;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();
                            to1 = false;
                            to2 = false;
                            to3 = false;
                            if (turn == 2)
                            {
                                continue;
                            }
                            else
                            {
                                perform_timeout = true;
                            }
                        }
                        else if (lane0_queue.empty() && on_road == 0)
                        {
                            // change_turn(0);
                            if (lane1_queue.size() > 0)
                            {
                                turn = 1;
                            }
                            else if (lane2_queue.size() > 0)
                            {
                                turn = 2;
                            }
                            else if (lane3_queue.size() > 0)
                            {
                                turn = 3;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();
                            if (turn == 2)
                            {
                                continue;
                            }
                            else
                            {
                                perform_timeout = true;
                            }
                        }
                        else
                        {
                            perform_timeout = true;
                        }
                    }
                    else if (turn == 1)
                    {
                        if (to0 || to2 || to3)
                        {
                            // change_turn(1);
                            if (lane2_queue.size() > 0)
                            {
                                turn = 2;
                            }
                            else if (lane3_queue.size() > 0)
                            {
                                turn = 3;
                            }
                            else if (lane0_queue.size() > 0)
                            {
                                turn = 0;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();
                            to0 = false;
                            to2 = false;
                            to3 = false;
                            if (turn == 2)
                            {
                                continue;
                            }
                            else
                            {
                                perform_timeout = true;
                            }
                        }
                        else if (lane1_queue.empty() && on_road == 0)
                        {
                            // change_turn(1);
                            if (lane2_queue.size() > 0)
                            {
                                turn = 2;
                            }
                            else if (lane3_queue.size() > 0)
                            {
                                turn = 3;
                            }
                            else if (lane0_queue.size() > 0)
                            {
                                turn = 0;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();
                            if (turn == 2)
                            {
                                continue;
                            }
                            else
                            {
                                perform_timeout = true;
                            }
                        }
                        else
                        {
                            perform_timeout = true;
                        }
                    }
                    else if (turn == 3)
                    {
                        if (to0 || to1 || to2)
                        {
                            // change_turn(3);
                            if (lane0_queue.size() > 0)
                            {
                                turn = 0;
                            }
                            else if (lane1_queue.size() > 0)
                            {
                                turn = 1;
                            }
                            else if (lane2_queue.size() > 0)
                            {
                                turn = 2;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();
                            to0 = false;
                            to1 = false;
                            to2 = false;
                            if (turn == 2)
                            {
                                continue;
                            }
                            else
                            {
                                perform_timeout = true;
                            }
                        }
                        else if (lane3_queue.empty() && on_road == 0)
                        {
                            // change_turn(3);
                            if (lane0_queue.size() > 0)
                            {
                                turn = 0;
                            }
                            else if (lane1_queue.size() > 0)
                            {
                                turn = 1;
                            }
                            else if (lane2_queue.size() > 0)
                            {
                                turn = 2;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();
                            if (turn == 2)
                            {
                                continue;
                            }
                            else
                            {
                                perform_timeout = true;
                            }
                        }
                        else
                        {
                            perform_timeout = true;
                        }
                    }
                    if (perform_timeout)
                    {
                        // cout << car->id << " is waiting for " << maximum_waiting_time << " ms" << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                        perform_timeout = false;
                        timespec imdullah;
                        timespec timeoutatanullah;
                        clock_gettime(CLOCK_REALTIME, &imdullah);
                        timeoutatanullah.tv_sec = imdullah.tv_sec + (maximum_waiting_time / 1000);
                        timeoutatanullah.tv_nsec = imdullah.tv_nsec + (1000UL * (maximum_waiting_time % 1000)) * 1000UL;
                        if (timeoutatanullah.tv_nsec >= 1000000000UL)
                        {
                            timeoutatanullah.tv_sec++;
                            timeoutatanullah.tv_nsec -= 1000000000;
                        }
                        int result = lane2->timedwait(&timeoutatanullah);
                        if (result == ETIMEDOUT)
                        {
                            // cout << car->id << " timed out" << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                            to2 = true;
                            continue;
                        }
                        else
                        {
                            // cout << car->id << " is breaking max wait time" << on_road << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                            continue;
                        }
                    }
                }
            }
            if (way == 3)
            {
                if (turn == 3)
                {
                    while (lane3_queue.front() != car)
                    {
                        lane3->wait();
                    }
                    if (turn != 3)
                    {
                        continue;
                    }
                    if (on_road > 0)
                    {
                        if (last_passed == 3)
                        {
                            // cout << car->id << " is waiting for " << PASS_DELAY << " ms" << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                            timespec now;
                            timespec pass_delay_amount;
                            clock_gettime(CLOCK_REALTIME, &now);
                            pass_delay_amount.tv_sec = now.tv_sec + (PASS_DELAY / 1000);
                            pass_delay_amount.tv_nsec = now.tv_nsec + (1000UL * (PASS_DELAY % 1000)) * 1000UL;
                            if (pass_delay_amount.tv_nsec >= 1000000000UL)
                            {
                                pass_delay_amount.tv_sec++;
                                pass_delay_amount.tv_nsec -= 1000000000;
                            }
                            int result = lane3->timedwait(&pass_delay_amount);
                            while (result != ETIMEDOUT && turn == 3)
                            {
                                result = lane3->timedwait(&pass_delay_amount); // may be we need to break this while if any lane timedout
                            }
                        }
                        else
                        {
                            // cout << car->id << " is waiting for " << on_road << " cars to pass" << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                            while (on_road > 0)
                            {
                                lane3->wait();
                            }
                        }
                    }
                    if (turn != 3)
                    {
                        continue;
                    }
                    WriteOutput(car->id, 'C', this->id, START_PASSING);
                    // cout << car->id << " is passing" << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                    on_road++;
                    can_pass = true;
                    last_passed = 3;
                    lane3_queue.pop();
                    lane3->notifyAll(); // turn cannot change here so we do not need to notify other lanes
                    timespec passing_time;
                    timespec imdi;
                    clock_gettime(CLOCK_REALTIME, &imdi);
                    passing_time.tv_sec = imdi.tv_sec + (travel_time / 1000);
                    passing_time.tv_nsec = imdi.tv_nsec + (1000UL * (travel_time % 1000)) * 1000UL;
                    if (passing_time.tv_nsec >= 1000000000UL)
                    {
                        passing_time.tv_sec++;
                        passing_time.tv_nsec -= 1000000000;
                    }
                    int passs = lane3->timedwait(&passing_time);
                    while (passs != ETIMEDOUT)
                    {
                        passs = lane3->timedwait(&passing_time);
                    }
                    on_road--;
                    WriteOutput(car->id, 'C', this->id, FINISH_PASSING);
                    // cout << car->id << " finished passing" << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                    if ((lane3_queue.empty() && on_road == 0) || turn != 3)
                    {
                        if (turn == 0)
                        {
                            lane0->notifyAll();
                        }
                        else if (turn == 1)
                        {
                            lane1->notifyAll();
                        }
                        else if (turn == 2)
                        {
                            lane2->notifyAll();
                        }
                        else
                        {
                            // change lane 3
                            if (lane0_queue.size() > 0)
                            {
                                turn = 0;
                            }
                            else if (lane1_queue.size() > 0)
                            {
                                turn = 1;
                            }
                            else if (lane2_queue.size() > 0)
                            {
                                turn = 2;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();
                        }
                    }
                }
                else
                {
                    bool perform_timeout = false;
                    while (lane3_queue.front() != car)
                    {
                        lane3->wait();
                    }
                    if (turn == 3)
                    {
                        continue;
                    }
                    if (turn == 0)
                    {
                        if (to1 || to2 || to3)
                        {
                            // change_turn(0);
                            if (lane1_queue.size() > 0)
                            {
                                turn = 1;
                            }
                            else if (lane2_queue.size() > 0)
                            {
                                turn = 2;
                            }
                            else if (lane3_queue.size() > 0)
                            {
                                turn = 3;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();
                            to1 = false;
                            to2 = false;
                            to3 = false;
                            if (turn == 3)
                            {
                                continue;
                            }
                            else
                            {
                                perform_timeout = true;
                            }
                        }
                        else if (lane0_queue.empty() && on_road == 0)
                        {
                            // change_turn(0);
                            if (lane1_queue.size() > 0)
                            {
                                turn = 1;
                            }
                            else if (lane2_queue.size() > 0)
                            {
                                turn = 2;
                            }
                            else if (lane3_queue.size() > 0)
                            {
                                turn = 3;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();
                            if (turn == 3)
                            {
                                continue;
                            }
                            else
                            {
                                perform_timeout = true;
                            }
                        }
                        else
                        {
                            perform_timeout = true;
                        }
                    }
                    else if (turn == 1)
                    {
                        if (to0 || to2 || to3)
                        {
                            // change_turn(1);
                            if (lane2_queue.size() > 0)
                            {
                                turn = 2;
                            }
                            else if (lane3_queue.size() > 0)
                            {
                                turn = 3;
                            }
                            else if (lane0_queue.size() > 0)
                            {
                                turn = 0;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();
                            to0 = false;
                            to2 = false;
                            to3 = false;
                            if (turn == 3)
                            {
                                continue;
                            }
                            else
                            {
                                perform_timeout = true;
                            }
                        }
                        else if (lane1_queue.empty() && on_road == 0)
                        {
                            // change_turn(1);
                            if (lane2_queue.size() > 0)
                            {
                                turn = 2;
                            }
                            else if (lane3_queue.size() > 0)
                            {
                                turn = 3;
                            }
                            else if (lane0_queue.size() > 0)
                            {
                                turn = 0;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();
                            if (turn == 3)
                            {
                                continue;
                            }
                            else
                            {
                                perform_timeout = true;
                            }
                        }
                        else
                        {
                            perform_timeout = true;
                        }
                    }
                    else if (turn == 2)
                    {
                        if (to0 || to1 || to3)
                        {
                            // change_turn(2);
                            if (lane3_queue.size() > 0)
                            {
                                turn = 3;
                            }
                            else if (lane0_queue.size() > 0)
                            {
                                turn = 0;
                            }
                            else if (lane1_queue.size() > 0)
                            {
                                turn = 1;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();
                            to0 = false;
                            to1 = false;
                            to3 = false;
                            if (turn == 3)
                            {
                                continue;
                            }
                            else
                            {
                                perform_timeout = true;
                            }
                        }
                        else if (lane2_queue.empty() && on_road == 0)
                        {
                            // change_turn(2);
                            if (lane3_queue.size() > 0)
                            {
                                turn = 3;
                            }
                            else if (lane0_queue.size() > 0)
                            {
                                turn = 0;
                            }
                            else if (lane1_queue.size() > 0)
                            {
                                turn = 1;
                            }
                            lane0->notifyAll();
                            lane1->notifyAll();
                            lane2->notifyAll();
                            lane3->notifyAll();
                            if (turn == 3)
                            {
                                continue;
                            }
                            else
                            {
                                perform_timeout = true;
                            }
                        }
                        else
                        {
                            perform_timeout = true;
                        }
                    }
                    if (perform_timeout)
                    {
                        // cout << car->id << " is waiting for " << maximum_waiting_time << " ms" << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                        perform_timeout = false;
                        timespec imdullah;
                        timespec timeoutatanullah;
                        clock_gettime(CLOCK_REALTIME, &imdullah);
                        timeoutatanullah.tv_sec = imdullah.tv_sec + (maximum_waiting_time / 1000);
                        timeoutatanullah.tv_nsec = imdullah.tv_nsec + (1000UL * (maximum_waiting_time % 1000)) * 1000UL;
                        if (timeoutatanullah.tv_nsec >= 1000000000UL)
                        {
                            timeoutatanullah.tv_sec++;
                            timeoutatanullah.tv_nsec -= 1000000000;
                        }
                        int result = lane3->timedwait(&timeoutatanullah);
                        if (result == ETIMEDOUT)
                        {
                            // cout << car->id << " timed out" << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                            to3 = true;
                            continue;
                        }
                        else
                        {
                            // cout << car->id << " is breaking max wait time" << on_road << "at" << this->id << "Crossroad" << "at lane" << way <<endl;
                            continue;
                        }
                    }
                }
            }
        }
    }
};