#ifndef REPAIRBAY_H_INCLUDED
#define REPAIRBAY_H_INCLUDED
#include "inventory.h"
#include "vehicle.h"
#include <iostream>
#include <queue>
#include <vector>       // std::vector
#include <random>
class RepairBay
{
  public:
    static int bayNumber;

    Inventory *inventory;

    priority_queue<Vehicle,vector<Vehicle*>> *pQueue;

    default_random_engine * engine = new default_random_engine(time(NULL));

    ~RepairBay();

    RepairBay(Inventory* passedInventory);

    void repairAndTest();
    //returns whether a vehicle passed tests
    bool  testVehicle(ofstream& out);

    bool  isOccupied();

    // changes the status of vehicle to in repair
    void changeStatus();

    // subtracts one time unit from the vehicle
    void minusTimeReq();

    void setComplete(ofstream& out);

    Vehicle* returnComplete();

    // places a new vehicle in the bay
    void putVehicleInBay(Vehicle &addedVehicle);

    // display information about the bay
    void display(ostream &out, ofstream& outs);

    bool isPartAvailable();

    void placeInBay(Vehicle *vehicle, ofstream&);

    void getTimeReq();

    bool wasTested();

  private:
    //vehicle pointer
    // the vehicle in repair
    Vehicle* vPtr;
    int id;
    bool tested;

};
// sets the static variable.
#endif
