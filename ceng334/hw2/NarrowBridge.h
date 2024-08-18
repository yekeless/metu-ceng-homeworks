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
class NarrowBridge : public Monitor
{

public:
    int id;
    int travel_time;
    int maximum_waiting_time;
    queue<Car *> waiting_cars_lane0;
    queue<Car *> waiting_cars_lane1;
    Condition *waiting_lane0;
    Condition *waiting_lane1;
    int last_passed;
    int on_bridge;
    bool timed_out_lane0, timed_out_lane1;
    int turn;
    NarrowBridge(int id, int travel_time, int maximum_waiting_time)
    {
        waiting_lane0 = new Condition(this);
        waiting_lane1 = new Condition(this);
        this->id = id;
        this->travel_time = travel_time;
        this->maximum_waiting_time = maximum_waiting_time;
        this->on_bridge = 0;
        this->turn = 0;
        this->timed_out_lane0 = false;
        this->timed_out_lane1 = false;
        this->last_passed = -1;
    }
    void Pass(Car *car, int lane)
    {
        __synchronized__;
        WriteOutput(car->id, 'N', this->id, ARRIVE);
        if (lane == 0)
        {
            waiting_cars_lane0.push(car);
        }
        else if (lane == 1)
        {
            waiting_cars_lane1.push(car);
        }
        // cout << car->id << "numaralı araba" << lane << "queuya eklendi" << endl;
        bool can_pass = false;
        while (!can_pass)
        {
            if (lane == 0)
            {
                if (turn == 0)
                {
                    while (waiting_cars_lane0.front() != car)
                    {
                        waiting_lane0->wait();
                    }
                    if (turn == 0)
                    {
                        if (on_bridge > 0)
                        {
                            if (last_passed == 0)
                            {
                                timespec temp;
                                timespec now;
                                clock_gettime(CLOCK_REALTIME, &now);
                                temp.tv_sec = now.tv_sec + (PASS_DELAY / 1000);
                                temp.tv_nsec = now.tv_nsec + (1000UL * (PASS_DELAY % 1000)) * 1000UL;
                                if (temp.tv_nsec >= 1000000000UL)
                                {
                                    temp.tv_sec++;
                                    temp.tv_nsec -= 1000000000;
                                }
                                // cout << car->id << "numaralı araba pass delay için" << this->id << "köprüsünde bekliyor" << endl;
                                int pass_delay;
                                pass_delay = waiting_lane0->timedwait(&temp);
                                while (pass_delay != ETIMEDOUT && turn == 0)
                                { // may be we need to add condition for opposite lane time out
                                    pass_delay = waiting_lane0->timedwait(&temp);
                                }
                            }
                            else if (last_passed == 1)
                            {
                                // cout << car->id << "numaralı araba karşı şeritten gelen arabanın çıkmasını bekliyor" << this->id << " numaralı köprüde" << endl;
                                while (on_bridge > 0)
                                {
                                    waiting_lane0->wait();
                                }
                            }
                        }
                        if (turn == 1)
                        {
                            continue;
                        }
                        WriteOutput(car->id, 'N', this->id, START_PASSING);
                        // cout << car->id << "numaralı araba" << this->id << "köprüsünden geçmeye başlıyor" << lane << "->" << !lane << endl;
                        on_bridge++;
                        can_pass = true;
                        last_passed = 0;
                        waiting_cars_lane0.pop();
                        waiting_lane0->notifyAll();
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
                        int passs = waiting_lane0->timedwait(&passing_time);
                        while (passs != ETIMEDOUT)
                        {
                            passs = waiting_lane0->timedwait(&passing_time);
                        }
                        on_bridge--;
                        WriteOutput(car->id, 'N', this->id, FINISH_PASSING);
                        // cout << car->id << "numaralı araba" << this->id << "köprüsünden geçmeyi bitirdi" << lane <<"->" << !lane << endl;
                        if (turn == 1 || (waiting_cars_lane0.empty() && on_bridge == 0))
                        { // you  need to notify the other lane also when the queue is empty
                            if ((waiting_cars_lane0.empty() && on_bridge == 0) && waiting_cars_lane1.size() > 0)
                            {
                                turn = 1;
                            }
                            waiting_lane1->notifyAll();
                        }
                    }
                    else
                    {
                        continue;
                    }
                }
                else if (turn == 1)
                {
                    // here may be I need to check also queue front
                    while (car != waiting_cars_lane0.front())
                    {
                        waiting_lane0->wait();
                    }
                    if (turn == 0)
                    {
                        continue;
                    }
                    if (waiting_cars_lane1.empty() && on_bridge == 0)
                    {
                        // cout << car->id << "numaralı araba"<< this->id <<"numaralı köprüde" <<"karşı şerit boş olduğu için turn değiştiriliyor" << endl;
                        turn = 0;
                        waiting_lane0->notifyAll();
                        waiting_lane1->notifyAll();
                        continue;
                    }
                    else if (timed_out_lane0)
                    {
                        // cout << car->id << "numaralı araba"<< this->id <<"numaralı köprüde" <<"karşı şerit zaman aşımına uğradığı için turn değiştiriliyor" << endl;
                        turn = 0;
                        timed_out_lane0 = false;
                        waiting_lane0->notifyAll();
                        waiting_lane1->notifyAll();
                        continue;
                    }
                    else
                    {
                        timespec mwt;
                        timespec imdullah;
                        clock_gettime(CLOCK_REALTIME, &imdullah);
                        mwt.tv_sec = imdullah.tv_sec + (maximum_waiting_time / 1000);
                        mwt.tv_nsec = imdullah.tv_nsec + (1000UL * (maximum_waiting_time % 1000)) * 1000UL;
                        if (mwt.tv_nsec >= 1000000000UL)
                        {
                            mwt.tv_sec++;
                            mwt.tv_nsec -= 1000000000;
                        }
                        // cout << car->id << "numaralı araba"<< this->id <<"numaralı köprüde" <<"karşı şerit beklemeye başlıyor" << endl;
                        int ret = waiting_lane0->timedwait(&mwt);
                        if (ret == ETIMEDOUT)
                        {
                            timed_out_lane0 = true;
                            ret = 0;
                        }
                        else
                        {
                            // cout << car->id << "numaralı araba"<< this->id <<"numaralı köprüde" <<"karşı şerit beklemeyi yarıda kesiyor" << endl;
                            continue;
                        }
                    }
                }
            }
            else if (lane == 1)
            {
                if (turn == 1)
                {
                    while (waiting_cars_lane1.front() != car)
                    {
                        waiting_lane1->wait();
                    }
                    if (turn == 1)
                    {
                        if (on_bridge > 0)
                        {
                            if (last_passed == 1)
                            {
                                timespec temp;
                                timespec now;
                                clock_gettime(CLOCK_REALTIME, &now);
                                temp.tv_sec = now.tv_sec + (PASS_DELAY / 1000);
                                temp.tv_nsec = now.tv_nsec + (1000UL * (PASS_DELAY % 1000)) * 1000UL;
                                if (temp.tv_nsec >= 1000000000UL)
                                {
                                    temp.tv_sec++;
                                    temp.tv_nsec -= 1000000000;
                                }
                                int pass_delay;
                                // cout << car->id << "numaralı araba pass delay için" << this->id << "numaralı köprüde bekliyor" << endl;
                                pass_delay = waiting_lane1->timedwait(&temp);
                                while (pass_delay != ETIMEDOUT && turn == 1)
                                {
                                    pass_delay = waiting_lane1->timedwait(&temp);
                                }
                            }
                            else if (last_passed == 0)
                            {
                                // cout << car->id << "numaralı araba karşı şeritten gelen arabanın çıkmasını bekliyor" << this->id << " numaralı köprüde" << endl;
                                while (on_bridge > 0)
                                {
                                    waiting_lane1->wait();
                                }
                            }
                        }
                        if (turn == 0)
                        {
                            continue;
                        }
                        WriteOutput(car->id, 'N', this->id, START_PASSING);
                        // cout << car->id << "numaralı araba" << this->id << "köprüsünden geçmeye başlıyor" << lane << "->" << !lane << endl;
                        on_bridge++;
                        can_pass = true;
                        last_passed = 1;
                        waiting_cars_lane1.pop();
                        waiting_lane1->notifyAll();
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
                        int passs = waiting_lane1->timedwait(&passing_time);
                        while (passs != ETIMEDOUT)
                        {
                            passs = waiting_lane1->timedwait(&passing_time);
                        }
                        on_bridge--;
                        WriteOutput(car->id, 'N', this->id, FINISH_PASSING);
                        // cout << car->id << "numaralı araba" << this->id << "köprüsünden geçmeyi bitirdi" << lane <<"->" << !lane << endl;
                        if (turn == 0 || (waiting_cars_lane1.empty() && on_bridge == 0))
                        {
                            if ((waiting_cars_lane1.empty() && on_bridge == 0) && waiting_cars_lane0.size() > 0)
                            {
                                turn = 0;
                            }
                            waiting_lane0->notifyAll(); // notify opposite lane when the queue is empty
                        }
                    }
                    else
                    {
                        continue;
                    }
                }
                else if (turn == 0)
                {
                    while (car != waiting_cars_lane1.front())
                    {
                        waiting_lane1->wait();
                    }
                    if (turn == 1)
                    {
                        continue;
                    }
                    if (waiting_cars_lane0.empty() && on_bridge == 0)
                    {
                        // cout << car->id << "numaralı araba"<< this->id <<"numaralı köprüde" <<"karşı şerit boş olduğu için turn değiştiriliyor" << endl;
                        turn = 1;
                        waiting_lane0->notifyAll();
                        waiting_lane1->notifyAll();
                        continue;
                    }
                    else if (timed_out_lane1)
                    {
                        // cout << car->id << "numaralı araba"<< this->id <<"numaralı köprüde" <<"karşı şerit zaman aşımına uğradığı için turn değiştiriliyor" << endl;
                        turn = 1;
                        timed_out_lane1 = false;
                        waiting_lane0->notifyAll();
                        waiting_lane1->notifyAll();
                        continue;
                    }
                    else
                    {
                        timespec mwt;
                        timespec imdullah;
                        clock_gettime(CLOCK_REALTIME, &imdullah);
                        mwt.tv_sec = imdullah.tv_sec + (maximum_waiting_time / 1000);
                        mwt.tv_nsec = imdullah.tv_nsec + (1000UL * (maximum_waiting_time % 1000)) * 1000UL;
                        if (mwt.tv_nsec >= 1000000000UL)
                        {
                            mwt.tv_sec++;
                            mwt.tv_nsec -= 1000000000;
                        }
                        // cout << car->id << "numaralı araba"<< this->id <<"numaralı köprüde" <<"karşı şerit beklemeye başlıyor" << endl;
                        int ret = waiting_lane1->timedwait(&mwt);
                        if (ret == ETIMEDOUT)
                        {
                            timed_out_lane1 = true;
                            ret = 0;
                        }
                        else
                        {
                            // cout << car->id << "numaralı araba"<< this->id <<"numaralı köprüde" <<"karşı şerit beklemeyi yarıda kesiyor" << endl;
                            continue;
                        }
                    }
                }
            }
        }
    }
};