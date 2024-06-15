#include "UI.h"
//Gets input,output and mode from user.
void UI::StartUp(string& input, string& output, char& mode)
{
	cout << "Enter input file name: ";
	getline(cin, input);
	cout << endl << "Enter output file name: ";
	getline(cin, output);
	cout << endl << "[I] for Interactive Mode" << endl << "[S] for Silent Mode" << endl << "[T] for Step-by-Step Mode" << endl;
	cin >> mode;
	cout << endl;
}
//Constructor
UI::UI(Company* myCompany)
{
	c = myCompany;
}
//Responsible for Printing all info.
void UI::Print(Time& currentTime, LinkedPQueue<Cargo*>& WaitingVIP, WaitingNormalList& WaitingNormal, LinkedQueue<Cargo*>& WaitingSpecial, LinkedPQueue<Truck*>& MovingT, LinkedQueue<Cargo*> DeliveredCargos, LinkedPQueue<Truck*>& LoadingT, LinkedQueue<Truck*>& EmptyVIPT, LinkedQueue<Truck*>& EmptyNormalT, LinkedQueue<Truck*>& EmptyspecialT, LinkedQueue<Truck*>& IncheckupNormalT, LinkedQueue<Truck*>& IncheckupSpecialT, LinkedQueue<Truck*>& IncheckupVIPT)
{
	cout << "Current Time (day:hour):" << currentTime << endl;
	int waitingCargoCount = WaitingNormal.getCount() + WaitingSpecial.getCount() + WaitingVIP.getPCount();
	cout << waitingCargoCount << " Waiting Cargos: [";
	WaitingNormal.PrintList();
	cout << "] (";
	WaitingSpecial.PrintQueue();
	cout << ") {";
	WaitingVIP.PrintQueue();
	cout << "}" << endl;
	for (int i = 0; i < 100; i++)
	{
		cout << "-";
	}
	cout << endl;
	int LoadingTrucksCount = LoadingT.getPCount();
	cout << LoadingTrucksCount << " Loading Trucks: ";
	LoadingT.PrintQueue();
	cout << endl;
	for (int i = 0; i < 100; i++)
	{
		cout << "-";
	}
	cout << endl;

	int emptyTruckCount = EmptyNormalT.getCount() + EmptyspecialT.getCount() + EmptyVIPT.getCount();
	cout << emptyTruckCount << " Empty Trucks: [";
	EmptyNormalT.PrintQueue();
	cout << "] (";
	EmptyspecialT.PrintQueue();
	cout << ") {";
	EmptyVIPT.PrintQueue();
	cout << "}" << endl;
	for (int i = 0; i < 100; i++)
	{
		cout << "-";
	}
	cout << endl;

	int MovingTrucksCount = MovingT.getPCount();
	cout << MovingTrucksCount << " Moving Trucks: ";
	MovingT.PrintQueue();
	cout << endl;
	for (int i = 0; i < 100; i++)
	{
		cout << "-";
	}
	cout << endl;
	int InCheckupTrucks = IncheckupNormalT.getCount() + IncheckupSpecialT.getCount() + IncheckupVIPT.getCount();
	cout << InCheckupTrucks << " In-Checkup Trucks: [";
	IncheckupNormalT.PrintQueue();
	cout << "] (";
	IncheckupSpecialT.PrintQueue();
	cout << ") {";
	IncheckupVIPT.PrintQueue();
	cout << "}" << endl;
	for (int i = 0; i < 100; i++)
	{
		cout << "-";
	}
	cout << endl;
	Cargo* C;
	int DeliveredCargoCount = DeliveredCargos.getCount();
	cout << DeliveredCargoCount << " Delivered Cargos: ";
	while (DeliveredCargos.dequeue(C))
	{
		if (C->getType() == 'N')
		{
			cout << "[" << C << "] ";
		}
		else if (C->getType() == 'S')
		{
			cout << "(" << C << ") ";
		}
		else
		{
			cout << "{" << C << "} ";
		}
	}
	cout << endl;
	for (int i = 0; i < 100; i++)
	{
		cout << "-";
	}
	cout << endl;
}
void UI::PrintEnd()
{
	cout << "simulation ends,output file created";
}