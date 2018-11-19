#ifndef INVENTORY_H
#define INVENTORY_H

#include "vehicle.h"
#include <list>
#include <random>

using namespace std;

class Inventory
{
    public:
        Inventory();

        list<int> getNewParts();

        list<int> getUsedParts();

        void addVehicleToStrip(Vehicle* veh);

        int numberOfNewParts();

        int numberOfUsedParts();

        void generateArrivingParts();

    private:
        list<int> m_newParts;
        list<int> m_usedParts;
        default_random_engine *engine = new default_random_engine(rand());

};

#endif // INVENTORY_h
