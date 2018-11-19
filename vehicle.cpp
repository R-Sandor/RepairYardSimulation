#include "vehicle.h"
#include <iostream>
#include <list>
#include <ctime>
#include <algorithm>    // std::unique, std::distance
#include <vector>       // std::vector

int Vehicle::m_idCount = 0;

Vehicle::Vehicle(int type)
    : m_idNum(m_idCount), m_type(type)
{
    m_idCount++;
    m_status = "arrived";
    m_hoursNeeded = 0;
}

int Vehicle::getHoursReq()
{
	return  m_hoursNeeded;
}

int Vehicle::getIdNum()
{
  return m_idNum;
}

void Vehicle::setHoursReq(int hours)
{
	m_hoursNeeded = hours;
}

void Vehicle::minusTimeReq()
{
	 if (m_hoursNeeded >0)
    m_hoursNeeded=m_hoursNeeded-1;
}

/// Copy constructor
Vehicle::Vehicle( const Vehicle &src)
{
    this->m_idNum = src.m_idNum;
    this->m_type = src.m_type;
    this-> m_workingParts = m_workingParts;
    this->m_brokenParts = m_brokenParts;
    this->m_hoursNeeded = m_hoursNeeded;
}

Vehicle::Vehicle()
:m_idNum(m_idCount)
{
	    engine = new default_random_engine(time(NULL));
	    uniform_int_distribution<int> typeOfVehicle(0, 5);
	    if (m_idCount %2 == 0)
	    	srand(time(NULL)+m_idCount);
	    else if (m_idCount %2 == 1)
		    	srand(time(NULL)+rand());

	    m_type= (rand() % 5 );
	    m_idCount++;
	    m_status ="arrived";
      m_hoursNeeded = 0;
	   assignPartsToType();
}

Vehicle::~Vehicle()
{
}

void Vehicle::assignPartsToType()
{
	engine = new default_random_engine(rand());
	if (m_idCount %2 == 0)
	    	srand(time(NULL)+m_idCount);
	    else if (m_idCount %2 == 1)
		    	srand(time(NULL)+rand());
    int numberOfParts = m_type + 3;
    uniform_int_distribution<int> randomBrokenParts(1, numberOfParts);
    int numberOfBrokenParts = randomBrokenParts(*engine);
    vector<int> parts = PART_TYPES[m_type];

   for (int i=0; i < numberOfParts; i++)
   {
        m_workingParts.push_back(parts[i]);
   }

   for (int i=0; i < numberOfBrokenParts; i++)
   {
   		breakPart();
   }
}

vector<int> Vehicle::getWorkingParts()
{
    return m_workingParts;
}

vector<int> Vehicle::getBrokenParts()
{
    return m_brokenParts;
}

bool Vehicle::operator<( Vehicle *rhs)
{
   return this->m_hoursNeeded > rhs->getHoursReq();
}

bool Vehicle::operator ==(Vehicle *rhs)
{
  return this->m_idNum == rhs->getIdNum();
}

bool Vehicle::operator !=(Vehicle *rhs)
{
  return (!(this->m_idNum == rhs->getIdNum()));
}

/*
*	Whenever a vehicles status has been changed the vehicle will be displayed to screen.
*/
void Vehicle::setStatus(const char* status, ofstream& out)
{
    m_status = status;
    display(cout);
    displayToFile(out);
}

void Vehicle::display(ostream &outs)
{
    cout << "Vehicle: " << m_idNum << " has status: " << m_status << "\n";
    cout << "type: " << m_type << " vehicle" <<endl;
    cout << "Repair Time: " << m_hoursNeeded<< endl;
    cout << endl;
}

void Vehicle::displayToFile(ofstream &out)
{
    out << "Vehicle: " << m_idNum << " has status: " << m_status << "\n";
    out << "type: " << m_type << " vehicle" <<endl;
    out << "Repair Time: " << m_hoursNeeded<< endl;
    out << endl;
}

//default_random_engine* engine I want to see if there is a way around passing this thing around
void Vehicle::breakPart()
{
    int numberOfWorkingParts = m_workingParts.size();
    uniform_int_distribution<int> breakRandomPart(0, numberOfWorkingParts - 1);
    int brokenIndex = breakRandomPart(*engine);
    int part = m_workingParts[brokenIndex];
    m_brokenParts.push_back(part);
    m_workingParts.erase(m_workingParts.begin() + brokenIndex);
    m_hoursNeeded = 0;
    for (auto aBrokenPart: m_brokenParts)
    {
      m_hoursNeeded += aBrokenPart;
    }
}
