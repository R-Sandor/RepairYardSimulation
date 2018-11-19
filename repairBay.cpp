#include  "repairBay.h"
#include  "inventory.h"
#include <algorithm>
#include <vector>

RepairBay::RepairBay(Inventory *passedInventory)
{
	// sets the vehicle pointer to null
	vPtr = nullptr;

	// set this equal to the number of objects created
	// from the bay class using a static variable
	id= ++bayNumber;
	inventory = passedInventory;
    tested = false;
}

RepairBay::~RepairBay()
{
}

bool RepairBay::isOccupied()
{
    if (vPtr== nullptr)
    {
        return false;
    }
    return true;
}

bool RepairBay::testVehicle(ofstream& out)
{
    if(vPtr->getHoursReq() != 0)
        return false;
    vPtr->setStatus("testing", out);
    uniform_real_distribution<double> percent(0,1);
    double chance = percent(*engine);
    if(chance > .90)
    {
        vPtr->breakPart();
        return false;
    }
    tested=true;
    return true;
}

bool RepairBay::wasTested()
{
    return tested;
}

bool RepairBay::isPartAvailable()
{
	std::vector<int> myvector (inventory->getUsedParts().begin(), inventory->getUsedParts().end());
   // vector<int>::iterator it;
    auto it = inventory->getUsedParts().begin();
    //it = inventory->getUsedParts().begin();
    it = find(inventory->getUsedParts().begin(), inventory->getUsedParts().end(), vPtr->getBrokenParts()[0]);
    if (it!= inventory->getUsedParts().end())
        return true;
    else
        it = find(inventory->getNewParts().begin(), inventory->getNewParts().end(), vPtr->getBrokenParts()[0]);
    if(it != inventory->getNewParts().end())
        return true;
    else
        return false;
}

void RepairBay::placeInBay(Vehicle *vehicle, ofstream& out)
{
    vPtr = vehicle;
    vPtr->setStatus("in repair - repair bay", out);
}

void RepairBay::minusTimeReq()
{
    if (vPtr != nullptr)
    vPtr->minusTimeReq();
}

void RepairBay::setComplete(ofstream& out)
{
    if(vPtr->getHoursReq() == 0 && tested == true)
    {
        vPtr->setStatus("vehicle complete.. Being deployed ", out);
        delete vPtr;
        vPtr = nullptr;
    }
}

Vehicle* RepairBay::returnComplete()
{
    if (vPtr != nullptr)
    {
        if(vPtr->getHoursReq() <= 0)
        {
            return vPtr;
        }
        else
            return nullptr;
    }
    else nullptr;
}

void RepairBay::display(ostream &out, ofstream& outs)
{
	if (this->isOccupied())
    {
		out <<"This is the vehicle in the bay: \n" << *vPtr << endl;
		outs <<"This is the vehicle in the bay: \n" << *vPtr << endl;
    }
	else
    {
		out<< "bay is empty \n";
        outs<< "bay is empty \n";
    }
	out << "id of bay "<< id << endl;
	outs << "id of bay "<< id << endl;
}
int RepairBay::bayNumber = 0;
