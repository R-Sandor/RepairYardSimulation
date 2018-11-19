#include "repairYard.h"
#include "repairBay.h"
using namespace std;

#include <random>
#include <iostream>
#include <fstream>

/*
* Basic constructor sets every field.
*/
RepairYard::RepairYard(Inventory *passedInInventory)
{
	// number of vehicles fixed in one day.
	fixedToday = 0;
	// number of vehicles fixed since the start of the simulation
	totalFixed = 0;

	inventory = passedInInventory;

	numberOfDays = 0;

	for (int i = 0; i < 3; i++)
			repairBays.push_back(new RepairBay(inventory));
	generateNewArrivals();
}

/*
* Generates a list of new arrivals
* Creates between 1 and 8 vehicles to be fixed in a day.
*/
void RepairYard::generateNewArrivals()
{
	if (rand()%8== 0)
	    srand(time(NULL)+5);
	else if (rand()%3 == 1)
		srand(time(NULL)+rand());
	int numVeh = (rand() % 8 )+1;
	numberOfArrivals= numVeh;
	for (int i=0; i < numVeh; i++)
	{
		arrived.push_back(new Vehicle());
	}
}

/*
*  Makes sure we don't get any of those
*  dreaded memory leaks when to program is finished
*/
RepairYard::~RepairYard()
{
	// iterator to go throught the stl list of repair bays
	bayItr =  repairBays.begin();
	while(bayItr!= repairBays.end())
		// deletes the itr
		delete *bayItr;
		bayItr++;
}

/*
*  Resets the number of hours and number of
*  vehicle completed in a day.
*/
void RepairYard::endOfDay(ofstream &out)
{
	// adds one to days
	numberOfDays++;

	fixedToday = 0;

	generateNewArrivals();
}

/*
 * This adds one to fixed today and total fixed vehicles.
 */
void RepairYard::addFixed()
{
	fixedToday++;
	totalFixed++;
}

// returns total of vehicles that have
// been fixed
int RepairYard::getTotalFixed()
{
	return totalFixed;
}

int RepairYard::getFixedToday()
{
	return fixedToday;
}

vector<Vehicle*> RepairYard::getArrived()
{
	return arrived;
}

vector<tuple<Vehicle*,int>>* RepairYard::getWaitingVehicles()
{
    return &m_waitingVehicles;
}

list<RepairBay*> RepairYard::getRepairBays()
{
	return repairBays;
}

list<RepairBay*> RepairYard::checkBays()
{
	list<RepairBay*> emptyBays;
	// iterator to go through the stl list of repair bays
	bayItr =  repairBays.begin();
	while(bayItr!= repairBays.end())
	{
	 	if ((**bayItr).isOccupied() == false )
			emptyBays.push_back(*bayItr);
		bayItr++;
	}
	return emptyBays;
}

void RepairYard::displayBay(ostream &out, ofstream& outs)
{
	bayItr =  repairBays.begin();
	while(bayItr!= repairBays.end())
	{
	 	(**bayItr).display(out, outs);
		bayItr++;
	}
}

void  RepairYard::displayQueue(ostream &out)
{
	vector<Vehicle*>tempStorage;

	while(!pQueue.empty())
{
    Vehicle* temp = pQueue.top();
    pQueue.pop();
    std::cout << *temp << endl;
    tempStorage.push_back(temp);
}
	for(unsigned int i=0; i < tempStorage.size(); i++)
    {
        pQueue.push(tempStorage[i]);
	}
}

void RepairYard::displayArrived(ostream &out, ofstream& outs)
{

	for (Vehicle *vehicle : arrived )
    {
			cout << *vehicle;
			outs << *vehicle;
    }
}

priority_queue<Vehicle*,vector<Vehicle*>,LessThanByP> *RepairYard::getPQueue()
{
	return &pQueue;
}

Inventory* RepairYard::getInventory()
{
	return inventory;
}

void RepairYard::removeFromArrived()
{
	arrived.erase(arrived.begin());

}
