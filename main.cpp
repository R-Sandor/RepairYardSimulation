///CS 361 Vehicle repair project
///Collaborated with Joseph Rugh, Raphael Sandor, and Derek Tiller
#include <iostream>
#include "vehicle.h"
#include "inventory.h"
#include "repairYard.h"
#include "yardMaster.h"
#include <random>
#include <iostream>
#include <fstream>

int main()
{
	//inventory pointer that is passed to the repair yard.
	Inventory *inventory = new Inventory();

	// repair yard is the actual repair yard that is managed by the yard master.
	RepairYard *repairYard = new RepairYard(inventory);

	// user provided filename.
	string filename;

	cout << "Name the output file: " << endl;

	cin >> filename;

	cout << "The output report will have data about the simulation.\n";

	// used for user defined days i.e. the number of days the user chooses to run.
	int days;

	cout << "How many days would you like to run the simulation? ";

	cin >> days;

	// creates the yardmaster to manage the repair yard
	YardMaster *yardMaster = new YardMaster(repairYard, days, filename);

	// executes his job of managing the yard.
	yardMaster->manageYard(yardMaster->out);

	// the result of a user's input.
	bool answer;
	// holds user's choice to run another day.
	string userInput;

	cout << "do you want to run another day? Y/n ";
	cin >>userInput;
	// input validation
	if (userInput == "y" || userInput == "yes" || userInput== "Yes" || userInput == "y" )
			answer= true;
		else
			answer = false;

	while (answer== true)
	{
		yardMaster->anotherRun();
		yardMaster->manageYard(yardMaster->out);
		cout << "running another day..\n\n";
		cout << "do you want to run another day? Y/n ";
		cin >>userInput;
		if (userInput == "y" || userInput == "yes" || userInput== "Yes" || userInput == "y" )
			answer= true;
		else
			answer = false;
	}

	cout << endl;
	cout << "-------------------------------------------------" << endl << endl;
	// displays what is in the repair queue.
	yardMaster->getRepairYard()->displayQueue(cout);

    return 0;
}
