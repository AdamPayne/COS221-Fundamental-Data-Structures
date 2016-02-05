#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

// Hierarchy 1
// Level 1:
class Spare_Part{
private:
	string name;
	double price;
	string description;
public:
	Spare_Part();
	string getName() { return name; }
	double getPrice() { return price; }
	virtual int getQuantity() { return -1; };
	virtual void addPart();						// polymorphic function to add new Spare Part
	virtual void addNewQuantity(){}				// polymorphic function to change quantity of an available Spare Part
	virtual void sellSparePart(){}				// polymorphic function to sell a Spare Part
};

Spare_Part::Spare_Part(){
	name = "";
	price = 0;
	description = "";
}

void Spare_Part::addPart(){
	cout << "\n\nEnter name of the part: ";
	cin >> name;
	cout << "Enter price for the part: ";
	cin >> price;
}

//Level 2:
class On_hand : public Spare_Part{
private:
	int quantity;
	string location;
public:
	On_hand();
	int getQuantity(){ return quantity; }
	void addPart();
	void addNewQuantity();
	void sellSparePart();
};

On_hand::On_hand(){
	quantity = 0;
	location = "";
}

void On_hand::addPart(){
	Spare_Part::addPart();						// call base function
	cout << "Enter quantity for the given part: ";
	cin >> quantity;
	cout << "Enter a location: ";
	cin >> location;
}

void On_hand::addNewQuantity(){
	cout << "Enter the new quantity: ";
	cin >> quantity;
}

void On_hand::sellSparePart(){
	quantity--;
	cout << "The part was sold successfully.\n";
}

class Contracted_suppliers : public Spare_Part{
private:
	string vendor, contact_person, phone;
	int delivery_time;
	string unit;
public:
	Contracted_suppliers();
	void addPart();
};

Contracted_suppliers::Contracted_suppliers(){
	vendor = "";
	contact_person = "";
	phone = "";
	delivery_time = 0;
	unit = "";
}

void Contracted_suppliers::addPart(){
	Spare_Part::addPart();					// call base function
	cout << "Enter the name of the vendor: ";
	cin >> vendor;
	cout << "Enter the name of the contact person: ";
	cin >> contact_person;
	cout << "Enther his/hers phone: ";
	cin >> phone;
	cout << "Enter time for delivery: ";
	cin >> delivery_time;
	cout << "Enter unit for the delivery time: ";
	cin >> unit;
}

//Level 3:
class New : public On_hand{
private:
	int warranty;
	string unit;
	string producer;
public:
	New();
	void addPart();
};

New::New(){
	warranty = 0;
	unit = "";
	producer = "";
}

void New::addPart(){
	On_hand::addPart();			// call base function
	cout << "Enter warranty: ";
	cin >> warranty;
	cout << "Enter unit for the warranty: ";
	cin >> unit;
	cout << "Enter producer of the part: ";
	cin >> producer;
}

class Recycled : public On_hand{
private:
	int Restored_resource;
	bool warrYN;
	int warranty;
	string unit;
public:
	Recycled();
	void addPart();
};

Recycled::Recycled(){
	Restored_resource = 0;
	warrYN = false;
	warranty = 0;
	unit = "";
}

void Recycled::addPart(){
	On_hand::addPart();			// call base function
	char boolVar;
	cout << "Enter restored percentage: ";
	cin >> Restored_resource;
	cout << "Warranty (Y/N)? ";
	cin >> boolVar;
	warrYN = ((boolVar == 'Y' || boolVar == 'y') ? true : false);
	if (warrYN){
		cout << "Enter warranty: ";
		cin >> warranty;
		cout << "Enter unit for the warranty: ";
		cin >> unit;
	}
}

// Hieararchy 2
// Level 1:
class Vehicle{
private:
	string producer;
public:
	Vehicle();
	string getProducer(){ return producer; }
	virtual string getName() = 0;
	virtual void addVehicle();			// polymorphic function to add a new Vehicle Model
	virtual void addToList(int) = 0;	// polymorphic function to add a new Spare Part to List (and add a copy vector in AvailableSP if applicable)
	virtual void changeQuantity() = 0;	// polymorphic function to change the quantity of specific spare part
	virtual void sellPart() = 0;		// polymorphic function to sell a specific spare part
	virtual void removePart() = 0;		// polymorphic function to remove a part from both List and AvailableSP
};

Vehicle::Vehicle(){
	producer = "";
}

void Vehicle::addVehicle(){
	cout << "\n\nEnter producer: ";
	cin >> producer;
}

// Level 2:
class Model : public Vehicle{
private:
	string Name;
	vector<Spare_Part*> List;			// List of all Spare Parts
	vector<Spare_Part*> AvailableSP;	// List of available Spare Parts. The pointers in this vector point only to members of List, which are of supertype On_hand
public:
	Model();
	string getName(){ return Name; }
	void printSP(vector<Spare_Part*>const);	// prints a table of Spare Parts (either those in List or in AvailableSP)
	void addVehicle();
	void addToList(int);
	void changeQuantity();
	void sellPart();
	void removePart();
};

Model::Model(){
	Name = "";
}

void Model::printSP(vector<Spare_Part*>const sParts){
	cout << "\n\tAvailable Parts for " << getProducer() << " " << getName() << "\n\n";
	cout << setw(3) << "ID" << setw(15) << "Name" << setw(8) << "Price" << setw(10) << "Quantity" << '\n';
	cout << setfill('-') << setw(36) << "" << '\n' << setfill(' ');
	for (int i = 0; i < (int)sParts.size(); i++){
		cout << setw(3) << i + 1 << setw(15) << sParts.at(i)->getName() << setw(8) << sParts.at(i)->getPrice() << setw(10);
		if (sParts.at(i)->getQuantity() == -1)
			cout << "ask vendor" << '\n';
		else
			cout << sParts.at(i)->getQuantity() << '\n';
	}
	cout << setfill('-') << setw(36) << "" << '\n' << setfill(' ');
	cout << '\n';
}

void Model::addVehicle(){
	Vehicle::addVehicle();					// call base function
	cout << "Enter a name for the model: ";
	cin >> Name;
}

void Model::addToList(int i){
	switch (i){
	case 1: List.push_back(new Contracted_suppliers);	// use of polymorphism
		break;
	case 2: List.push_back(new New);					// use of polymorphism
		break;
	case 3: List.push_back(new Recycled);				// use of polymorphism
		break;
	default:;
	}
	List.back()->addPart();
	if (List.back()->getQuantity()>0) 
		AvailableSP.push_back(List.back()); // add the part to AvailableSP if the quantity is greater than 0
}

void Model::changeQuantity(){
	int id;
	printSP(AvailableSP);
	cout << "For which part do you want to change the quantity?\nID: ";
	cin >> id;
	if (id <= (int)AvailableSP.size() && id>0){
		id -= 1;
		AvailableSP.at(id)->addNewQuantity();
		if (AvailableSP.at(id)->getQuantity()<1)
			AvailableSP.erase(AvailableSP.begin() + id);	// check if the new quantity is zero and if yes erase it from AvailableSP
	}
}

void Model::sellPart(){
	int id;
	printSP(AvailableSP);
	cout << "Which part do you want to sell?\nID: ";
	cin >> id;
	if (id <= (int)AvailableSP.size() && id > 0){
		id -= 1;
		AvailableSP.at(id)->sellSparePart();
		if (AvailableSP.at(id)->getQuantity()<1)
			AvailableSP.erase(AvailableSP.begin() + id);	// check if the new quantity is zero and if yes erase it from AvailableSP
	}
}

void Model::removePart(){
	int id;
	printSP(List);
	cout << "Which part do you want to remove from the database?\nID: ";
	cin >> id;
	if (id <= (int)List.size() && id > 0){
		id -= 1;
		AvailableSP.erase(remove(AvailableSP.begin(), AvailableSP.end(), List.at(id)), AvailableSP.end()); // first erase the pointer in AvailableSP
		delete List.at(id);																				   // then free dynamic memory
		List.erase(List.begin() + id);																	   // finally erase also from List
		cout << "The part was removed successfully";
	}
}

void search(vector<Vehicle*>&);
void askForPart(Vehicle*);

int main(){
	char choice = '0';
	vector<Vehicle*> vctrVehicle;					// vector containing all the vehicle models
	cout << "\tAUBG Vehicles' Spare Parts Supermarket Database\n";
	cout << "\t\t\tWELCOME\n";
	do{
		cout << "\n\n\tMain Menu\n";
		cout << "1. Add a new vehicle model.\n";
		cout << "2. Search for a model and update information regarding it and its spare parts.\n";
		cout << "0. Exit.\n";
		cout << "Input: ";
		cin >> choice;
		switch (choice){
		case '1': vctrVehicle.push_back(new Model); vctrVehicle.back()->addVehicle(); break;
		case '2': if (vctrVehicle.size())
					  search(vctrVehicle);
				  else
					  cout << "\nNo Vehicle Models in the database. Try adding a new vehicle.\n";
				  break;
		case '0': break;
		default: cout << "Option doesn't exist. Try again.\n";
		}
	} while (choice != '0');
	// Free the memory
	for (int i = 0; i < (int)vctrVehicle.size(); i++){
		delete vctrVehicle.at(i);
	}
	vctrVehicle.erase(vctrVehicle.begin(), vctrVehicle.end());
	return 0;
}

void search(vector<Vehicle*>& vctrVehicle){
	char choice = '0';
	int id;
	cout << "\n\tAvailable Vehicle Models\n\n";
	cout << setw(3) << left << "ID" << setw(12) << "Producer" << setw(15) << "Model" << '\n';
	cout << setfill('-') << setw(35) << "" << '\n' << setfill(' ');
	for (int i = 0; i < (int)vctrVehicle.size(); i++){
		cout << setw(3) << i + 1 << setw(12) << vctrVehicle.at(i)->getProducer() << setw(15) << vctrVehicle.at(i)->getName() << '\n';
	}
	cout << setfill('-') << setw(35) << "" << '\n' << setfill(' ');
	cout << "For which vehicle do you want to edit information?\nID: ";
	cin >> id;
	if (id <= (int)vctrVehicle.size() && id>0)
	{
		id -= 1;
		do{
			cout << "\n\n\tControl Menu for " << vctrVehicle.at(id)->getProducer() << ' ' << vctrVehicle.at(id)->getName() << "\n\n";
			cout << "1. Add a spare part.\n";
			cout << "2. Change the quantity of available parts.\n";
			cout << "3. Sell a spare part.\n";
			cout << "4. Remove a spare part.\n";
			cout << "5. Remove this model (also goes to Main Menu).\n";
			cout << "0. Go to Main Menu.\n";
			cout << "Input: ";
			cin >> choice;
			switch (choice){
			case '1': askForPart(vctrVehicle.at(id)); break;
			case '2': vctrVehicle.at(id)->changeQuantity(); break; // if there are no parts, the user will be viewed an empty list and
			case '3': vctrVehicle.at(id)->sellPart(); break;	   // regardless of the input he will be redirected back
			case '4': vctrVehicle.at(id)->removePart(); break;
			case '5': delete vctrVehicle.at(id);				   // free the dynamic memory
					  vctrVehicle.erase(vctrVehicle.begin());	   // erase the member of the vector
					  cout << "\nThe vehicle model was removed successfully.\n";
					  choice = '0';
				      break;
			case '0': break;
			default: cout << "Option doesn't exist. Try again.\n";
			}
		} while (choice != '0');
	}

}

void askForPart(Vehicle* vehicle){
	char choice = '0';
	do{
		cout << "\n\n\tAdd Menu for " << vehicle->getProducer() << ' ' << vehicle->getName() << "\n\n";
		cout << "1. Add a spare part from a vendor.\n";
		cout << "2. Add a new spare part.\n";
		cout << "3. Add a recycled spare part.\n";
		cout << "0. Go to Control Menu.\n";
		cout << "Input: ";
		cin >> choice;
		switch (choice){
		case '1': vehicle->addToList(1); break;
		case '2': vehicle->addToList(2); break;
		case '3': vehicle->addToList(3); break;
		case '0': break;
		default: cout << "Option doesn't exist. Try again.\n";
		}
	} while (choice != '0');
}