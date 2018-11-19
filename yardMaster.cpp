#include "yardMaster.h"
#include <iostream>
#include <fstream>
#include <tuple>
#include <list>

// adds an hour to a 2400 military clock
// if the hours exceed 2400 hours
// then starts a new day.
// continues managing the yard.
 void YardMaster::addHour(ofstream& out)
 {
 	if (hours2400 >= 2400)
	{
 		hours2400 =0000;
		// generates new arrivals and increments days
		beginNewDay(out);
	}
	else
	{
		hours2400= hours2400+100;
	}
 }

// starts a new day.
// prints a report out to a file
// checks to ensure that all the vehicles in the
// waiting list are still waiting for a part or
// if they can be put in the priority queue.
// At the beginning of a new day the yard master
// should call the inventory(warehouse) and generate
// a new arrival of parts.
void YardMaster::beginNewDay(ofstream& out)
{
    numDays++;
	// calls the repair yards end of day to start a new day
	// and create a report to file and generate new vehicles
	repairYard->endOfDay(out);
	repairYard->generateNewArrivals();
	manageNewArrivals(repairYard->getArrived(), out);
	repairYard->displayArrived(cout, out);
	repairYard->getInventory()->generateArrivingParts();
	cout << "---------------------------------------------------\n";
	out << "---------------------------------------------------\n";
}

// establishes the yard master.
YardMaster::YardMaster(RepairYard *incomingRepairYard,  int desiredDays, string filename)
{
    this->desiredDays = desiredDays;
	numDays = 0;
	hours2400 = 0;
	repairYard = incomingRepairYard;
  	out.open(filename);
}

// close the output file.
YardMaster::~YardMaster()
{
	out.close();
}

// manage yard increases the hour by one and will go check the repair bays statuses
// if a repair bay is available it will pop the vehicle off the top of the queue and
// double check that there is a part available for the vehicle as precaution
// This function will also tell each repair to decrement it's bay repair times
// of all the vehicles in the bays.
void YardMaster::manageYard(ofstream& out)
{
	cout << "---------------------------------------------------\n";
	cout << "day: " << numDays<< endl;
	out << "---------------------------------------------------\n";
	out << "day: " << numDays<< endl;
	// get the repairBays from the repairYard
	// run for desired days
	while ( numDays < desiredDays)
	{
		list<RepairBay*> baysInYard = repairYard->getRepairBays();
		// go through each bayAnd subtract time
		for(auto bay: baysInYard)
		{
			bay->minusTimeReq();
		}

		// check if the vehicles are complete.
		for(auto bay: baysInYard)
		{
			// if the bay has a vehicle in it check if it is complete
			if (bay->returnComplete() != nullptr )
			{
				// returns a vehicle if a vehicle has 0 hours left
				Vehicle * vehicle = bay->returnComplete();

				// checks if the vehicle with 0 hours left was tested.
				if (bay->wasTested() == true)
				{
					vehicle->setStatus("Vehicle is fixed and being deployed", out);
					bay->setComplete(out);
					repairYard->addFixed();
				}
				else // otherwise test the vehicle
				{
					cout << "vehicle is off to be tested\n";
					out << "vehicle is off to be tested\n";
					bay->display(cout,out);
					bay->testVehicle(out);
				}
			}
		}
		/*
		*	Check if repair bays are empty
		*   if they are then place a vehicle in bay.
		*/
		//returns any empty repair bays
		list<RepairBay*> emptyBays = repairYard->checkBays();
		if (emptyBays.size() >0)
		{
			for (auto bay: emptyBays)
			{
				if (repairYard->getPQueue()->size() >0)
				{
					Vehicle *vehFromQueue = repairYard->getPQueue()->top();
					repairYard->getPQueue()->pop();
					if (checkForParts(vehFromQueue, out) == true)
					{
						bay->placeInBay(vehFromQueue, out);
						vehFromQueue = nullptr;
					}
				}
			}
		}

		if (numDays != 0 )
			manageWaitlist(out);
		addHour(out);
	}
}

// the name of this function needs to be changed to manage arrivals
// yes this function does calculate the number of hours of work
// but it now calls scrap or repair with that information
// and send the vehicle and the hours to another function
// that will place the vehicle in the priority queue or to the
// scrapyard i.e. the inventory object.
// if a part is not available and the vehicle is worth fixing
// gets added to a waiting list
void YardMaster::manageNewArrivals(vector<Vehicle*> newArrivals, ofstream& out)
{
	for(auto aVeh: newArrivals)
	{
		vector<int>brokenParts = aVeh->getBrokenParts();
		vector<int>goodParts = aVeh->getWorkingParts();

		int totalPartHours=0;
		int totalBrokenParts=0;
		int totalWorkingParts=0;
		int i = 0;

		for (auto workingPart: aVeh->getWorkingParts())
		{
			totalPartHours += workingPart;
			totalWorkingParts += workingPart;
			i++;
		}

		for (auto aBrokenPart: aVeh->getBrokenParts())
		{
			totalPartHours += aBrokenPart;
			totalBrokenParts += aBrokenPart;
			i++;
		}
			aVeh->setHoursReq(totalBrokenParts);
			double partsRatio = totalBrokenParts/totalPartHours;
			repairYard->removeFromArrived();

			repairOrScrap(aVeh, partsRatio, out);
	}
}

void YardMaster::manageWaitlist(ofstream& out)
{
	vector<tuple<Vehicle*,int>> *vechileTupleVector = repairYard->getWaitingVehicles();
	for (auto vehTuple: *vechileTupleVector)
	{
		Vehicle *vehicle = std::get<0>(vehTuple);
		int daysWaiting = get<1>(vehTuple);
		if (checkForParts(vehicle, out)== false)
		{
			get<1>(vehTuple)+= 1;
		}
	}
}

// increase the number of desired days allowing the program to run another day.
void YardMaster::anotherRun()
{
	desiredDays++;
}

// returns the repair yard.
RepairYard* YardMaster::getRepairYard()
{
	return	repairYard;
}

//Determines if a vehicle is worth fixing or not.
void YardMaster::repairOrScrap(Vehicle * aVeh, int partsRatio, ofstream& out)
{
	if (partsRatio <= .7)
	{
		bool partsAvailable = checkForParts(aVeh, out);
		if (partsAvailable== true)
		{
			repairYard->getPQueue()->push(aVeh);
			aVeh->setStatus("In RepairQueue", out);
		}
	}
	else
	{
		// should put this vehicle in the inventory to be stripped for its good parts.
		aVeh->setStatus("being stripped for parts and thrown away.", out);
		repairYard->getInventory()->addVehicleToStrip(aVeh);
	}
}

bool YardMaster::isVehInWaitinglist(Vehicle * vehicle)
{
	int i=0;
	vector<tuple<Vehicle*,int>> *vechileTupleVector = repairYard->getWaitingVehicles();
	for (auto vehTuple: *vechileTupleVector)
	{
		Vehicle *vehInWaitlist = std::get<0>(vehTuple);
		if (vehInWaitlist == vehicle)
			return true;
		if (get<1>(vehTuple) >7)
			   vechileTupleVector->erase(vechileTupleVector->begin() +i);
			i++;
	}
	return false;
}

/*
*	Checks if all the parts for a vehicle are available or not.
*	add the vehicle to the priority queue if it is available and return a boolean
*	to tell the programmer which case occurred.
*/
bool YardMaster::checkForParts(Vehicle * aVeh, ofstream& out)
{
	vector<int> brokenParts = aVeh->getBrokenParts();
	Inventory* inventory = repairYard->getInventory();
	list<int> newParts = inventory->getNewParts();
	list<int> usedParts = inventory->getUsedParts();
	list<int>::iterator partsItr;
	bool foundEveryPart = true;
	// Used to check if the part is in the list
	auto checkForPart = [](list<int>::iterator& itr, list<int>& parts, bool& pf, int bp) {
        itr = parts.begin();
        while (itr != parts.end())
        {
            if (*itr == bp)
            {
                pf = true;
                break;
            }
            itr++;
        }
	};
	// Checking if every broken part has an associated used/new part
	for (int brokenPart : brokenParts)
    {
        bool partFound = false;
        checkForPart(partsItr, usedParts, partFound, brokenPart);
        if (partFound) continue; // No need to check new parts if it was found in the used parts
        checkForPart(partsItr, newParts, partFound, brokenPart);
        // If the part is still not found it needs to go to the waiting list
        if (!partFound)
        {
            foundEveryPart = false;
            break;
        }
    }

    if (foundEveryPart)
    {
        return true;
    }
    else
    {
    	if (isVehInWaitinglist(aVeh)== false)
    	{
    	auto aVehicleTuple = make_tuple(aVeh,0);
    	aVeh->setStatus("in waiting list for part", out);
        repairYard->getWaitingVehicles()->push_back(aVehicleTuple);
    	}
        return false;
    }
}
