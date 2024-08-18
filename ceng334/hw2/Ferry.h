#include <vector>
#include "monitor.h"
#include "Car.h"
#include "errno.h"
#include <time.h>
#include "WriteOutput.h"
#include "helper.h"
#include <ctime>
using namespace std;

class Ferry : public Monitor {
    public:
        int id;
        int travel_time;
        int maximum_waiting_time;
        int capacity;  
        int side0_count;
        int side1_count;
        Condition * side0;
        Condition * side1;
        bool timedout_side0,timedout_side1;

        Ferry(int id, int travel_time, int maximum_waiting_time, int capacity){
            side0 = new Condition(this);
            side1 = new Condition(this);
            this->id = id;
            this->travel_time = travel_time;
            this->maximum_waiting_time = maximum_waiting_time;
            this->capacity = capacity;
            this->timedout_side0 = false;
            this->timedout_side1 = false;
            this->side0_count = 0;
            this->side1_count = 0;
        }
        void Pass(Car * car,int side){
            __synchronized__;
            WriteOutput(car->id, 'F', this->id, ARRIVE);
            if(side == 0){
                side0_count++;
            }
            else if(side == 1){
                side1_count++;
            }
            bool can_pass = false;
            while(!can_pass){
                if(side == 0){
                    if(timedout_side0){
                        timedout_side0 = false;
                        timespec imdi;
                        timespec passing_time;
                        clock_gettime(CLOCK_REALTIME, &imdi);
                        passing_time.tv_sec = imdi.tv_sec + (travel_time/1000);
                        passing_time.tv_nsec = imdi.tv_nsec + (1000UL * (travel_time % 1000)) * 1000UL;
                        if(passing_time.tv_nsec >= 1000000000UL){
                            passing_time.tv_sec++;
                            passing_time.tv_nsec -= 1000000000;
                        }
                        side0_count=0;
                        side0->notifyAll();
                        can_pass = true;
                        WriteOutput(car->id, 'F', this->id, START_PASSING);
                        int pass = side0->timedwait(&passing_time);
                        while(pass != ETIMEDOUT){
                            pass = side0->timedwait(&passing_time);
                        }
                        WriteOutput(car->id, 'F', this->id, FINISH_PASSING);

                    }
                    else if(side0_count == 1){
                        timespec now;
                        timespec temp;
                        clock_gettime(CLOCK_REALTIME, &now);
                        temp.tv_sec = now.tv_sec + (maximum_waiting_time/1000);
                        temp.tv_nsec = now.tv_nsec + (1000UL * (maximum_waiting_time % 1000)) * 1000UL;
                        if(temp.tv_nsec >= 1000000000UL){
                            temp.tv_sec++;
                            temp.tv_nsec -= 1000000000;
                        }
                        int ret = side0->timedwait(&temp);
                        if(ret == ETIMEDOUT){
                            timedout_side0 = true;
                            continue;
                        }
                        else{
                            //perform passing
                            timespec imdullah;
                            timespec pasullah;
                            clock_gettime(CLOCK_REALTIME, &imdullah);
                            pasullah.tv_sec = imdullah.tv_sec + (travel_time/1000);
                            pasullah.tv_nsec = imdullah.tv_nsec + (1000UL * (travel_time % 1000)) * 1000UL;
                            if(pasullah.tv_nsec >= 1000000000UL){
                                pasullah.tv_sec++;
                                pasullah.tv_nsec -= 1000000000;
                            }
                            can_pass = true;
                            WriteOutput(car->id, 'F', this->id, START_PASSING);
                            int returnullah = side0->timedwait(&pasullah);
                            while(returnullah != ETIMEDOUT){
                                returnullah = side0->timedwait(&pasullah);
                            }
                            WriteOutput(car->id, 'F', this->id, FINISH_PASSING);
                        }
                    }
                    else if( side0_count == capacity){
                        //perform passing
                        timespec gecemeyenullah;
                        timespec bekleyenullah;
                        clock_gettime(CLOCK_REALTIME, &gecemeyenullah);
                        bekleyenullah.tv_sec = gecemeyenullah.tv_sec + (travel_time/1000);
                        bekleyenullah.tv_nsec = gecemeyenullah.tv_nsec + (1000UL * (travel_time % 1000)) * 1000UL;
                        if(bekleyenullah.tv_nsec >= 1000000000UL){
                            bekleyenullah.tv_sec++;
                            bekleyenullah.tv_nsec -= 1000000000;
                        }
                        side0_count = 0;
                        side0->notifyAll();
                        can_pass = true;
                        WriteOutput(car->id, 'F', this->id, START_PASSING);
                        int denizedusenullah = side0->timedwait(&bekleyenullah);
                        while(denizedusenullah != ETIMEDOUT){
                            denizedusenullah = side0->timedwait(&bekleyenullah);
                        }
                        WriteOutput(car->id, 'F', this->id, FINISH_PASSING);
                    }
                    else if(side0_count < capacity && side0_count > 1){
                        side0->wait();
                        //perform passing
                        timespec gecenullah;
                        timespec cakullah;
                        clock_gettime(CLOCK_REALTIME, &gecenullah);
                        cakullah.tv_sec = gecenullah.tv_sec + (travel_time/1000);
                        cakullah.tv_nsec = gecenullah.tv_nsec + (1000UL * (travel_time % 1000)) * 1000UL;
                        if(cakullah.tv_nsec >= 1000000000UL){
                            cakullah.tv_sec++;
                            cakullah.tv_nsec -= 1000000000;
                        }
                        can_pass = true;
                        WriteOutput(car->id, 'F', this->id, START_PASSING);
                        int denizdebogulanullah = side0->timedwait(&cakullah);
                        while(denizdebogulanullah != ETIMEDOUT){
                            denizdebogulanullah = side0->timedwait(&cakullah);
                        }
                        WriteOutput(car->id, 'F', this->id, FINISH_PASSING);
                    }
                }
                else if(side == 1){
                    if(timedout_side1){
                        timedout_side1 = false;
                        timespec imdi;
                        timespec passing_time;
                        clock_gettime(CLOCK_REALTIME, &imdi);
                        passing_time.tv_sec = imdi.tv_sec + (travel_time/1000);
                        passing_time.tv_nsec = imdi.tv_nsec + (1000UL * (travel_time % 1000)) * 1000UL;
                        if(passing_time.tv_nsec >= 1000000000UL){
                            passing_time.tv_sec++;
                            passing_time.tv_nsec -= 1000000000;
                        }
                        side1_count=0;
                        side1->notifyAll();
                        can_pass = true;
                        WriteOutput(car->id, 'F', this->id, START_PASSING);
                        int pass = side1->timedwait(&passing_time);
                        while(pass != ETIMEDOUT){
                            pass = side1->timedwait(&passing_time);
                        }
                        WriteOutput(car->id, 'F', this->id, FINISH_PASSING);
                    }
                    else if(side1_count == 1){
                        timespec now;
                        timespec temp;
                        clock_gettime(CLOCK_REALTIME, &now);
                        temp.tv_sec = now.tv_sec + (maximum_waiting_time/1000);
                        temp.tv_nsec = now.tv_nsec + (1000UL * (maximum_waiting_time % 1000)) * 1000UL;
                        if(temp.tv_nsec >= 1000000000UL){
                            temp.tv_sec++;
                            temp.tv_nsec -= 1000000000;
                        }
                        int ret = side1->timedwait(&temp);
                        if(ret == ETIMEDOUT){
                            timedout_side1 = true;
                            continue;
                        }
                        else{
                            //perform passing
                            timespec imdullah;
                            timespec pasullah;
                            clock_gettime(CLOCK_REALTIME, &imdullah);
                            pasullah.tv_sec = imdullah.tv_sec + (travel_time/1000);
                            pasullah.tv_nsec = imdullah.tv_nsec + (1000UL * (travel_time % 1000)) * 1000UL;
                            if(pasullah.tv_nsec >= 1000000000UL){
                                pasullah.tv_sec++;
                                pasullah.tv_nsec -= 1000000000;
                            }
                            can_pass = true;
                            WriteOutput(car->id, 'F', this->id, START_PASSING);
                            int returnullah = side1->timedwait(&pasullah);
                            while(returnullah != ETIMEDOUT){
                                returnullah = side1->timedwait(&pasullah);
                            }
                            WriteOutput(car->id, 'F', this->id, FINISH_PASSING);
                        }
                    }
                    else if( side1_count == capacity){
                        //perform passing
                        timespec gecemeyenullah;
                        timespec bekleyenullah;
                        clock_gettime(CLOCK_REALTIME, &gecemeyenullah);
                        bekleyenullah.tv_sec = gecemeyenullah.tv_sec + (travel_time/1000);
                        bekleyenullah.tv_nsec = gecemeyenullah.tv_nsec + (1000UL * (travel_time % 1000)) * 1000UL;
                        if(bekleyenullah.tv_nsec >= 1000000000UL){
                            bekleyenullah.tv_sec++;
                            bekleyenullah.tv_nsec -= 1000000000;
                        }
                        side1_count = 0;
                        side1->notifyAll();
                        can_pass = true;
                        WriteOutput(car->id, 'F', this->id, START_PASSING);
                        int denizedusenullah = side1->timedwait(&bekleyenullah);
                        while(denizedusenullah != ETIMEDOUT){
                            denizedusenullah = side1->timedwait(&bekleyenullah);
                        }
                        WriteOutput(car->id, 'F', this->id, FINISH_PASSING);
                    }
                    else if(side1_count < capacity && side1_count > 1){
                        side1->wait();
                        //perform passing
                        timespec gecenullah;
                        timespec cakullah;
                        clock_gettime(CLOCK_REALTIME, &gecenullah);
                        cakullah.tv_sec = gecenullah.tv_sec + (travel_time/1000);
                        cakullah.tv_nsec = gecenullah.tv_nsec + (1000UL * (travel_time % 1000)) * 1000UL;
                        if(cakullah.tv_nsec >= 1000000000UL){
                            cakullah.tv_sec++;
                            cakullah.tv_nsec -= 1000000000;
                        }
                        can_pass = true;
                        WriteOutput(car->id, 'F', this->id, START_PASSING);
                        int denizdebogulanullah = side1->timedwait(&cakullah);
                        while(denizdebogulanullah != ETIMEDOUT){
                            denizdebogulanullah = side1->timedwait(&cakullah);
                        }
                        WriteOutput(car->id, 'F', this->id, FINISH_PASSING);
                    }
                    


                }
            }

        }
};