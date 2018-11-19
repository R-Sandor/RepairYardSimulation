#ifndef VEHICLE_H
#define VEHICLE_H

#include <vector>
#include <random>
#include <iostream>
#include <random>
#include <ctime>
#include <fstream>

using namespace std;

static vector<int> PART_TYPES [5] =
{
    {
        1, 2, 3, 5
    },
    {
        2, 4, 6, 8, 10
    },
    {
        3, 5, 7, 9, 11, 13
    },
    {
        10, 11, 12, 13, 14, 15, 16
    },
    {
        1, 2, 3, 5, 7, 11, 13, 17
    }
};

class Vehicle
{

public:
    Vehicle();
    Vehicle(int type);
	~Vehicle();

    default_random_engine * engine = new default_random_engine(time(NULL));

    int  getHoursReq();

    void setHoursReq(int hoursRequired);

    void minusTimeReq();

    int getIdNum();

    void assignPartsToType();
    Vehicle( const Vehicle &src);

    void display(ostream &outs);

    void displayToFile(ofstream& out);

    vector<int> getWorkingParts();

    vector<int> getBrokenParts();

    bool operator()(const Vehicle* lhs, const Vehicle* rhs) const
    {
        std::cout << "hoursNeeded lhs: " << lhs->m_hoursNeeded << "\n";
        std::cout << "hoursNeeded rhs: " << rhs->m_hoursNeeded << "\n";
        return lhs->m_hoursNeeded > rhs->m_hoursNeeded;
    }

    bool operator<( Vehicle *rhs);

    bool operator !=(Vehicle *rhs);

    bool operator ==(Vehicle *rhs);

    void setStatus(const char* status, ofstream& out);

    void breakPart();

private:
    int m_idNum;
    int m_type;
    vector<int> m_workingParts;
    vector<int> m_brokenParts;
    int m_hoursNeeded = 0;
    const char* m_status;
    static int m_idCount;
};

inline
std::ostream& operator<<(std::ostream &outs, Vehicle &veh)
{
    veh.display(outs);

    return outs;
}
inline
std::ofstream& operator<<(std::ofstream &out, Vehicle &veh)
{
    veh.displayToFile(out);

    return out;
}
#endif // VEHICLE_H
