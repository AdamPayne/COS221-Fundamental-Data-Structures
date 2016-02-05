#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <vector>
using namespace std;

// Level 1:
class Character{
private:
	string name;
	int attackPoints;
	int health;
	int defence;
	int luck;
	bool dead;
public:
	Character();
	// get and set functions:
	string getName() const { return name; }
	void setName(string cName){ name=cName;}
	int getAtPoints() { return attackPoints; }
	int getHealth()  { return health; }
	void setHealth(int cHealth){ health = cHealth; }
	int getDefence() { return defence; }
	void setDefence(int cDefence) { defence = cDefence; }
	int getLuck() const { return luck; }
	bool isDead() const { return dead; }
	void setDead(bool cDead){ dead = cDead; }
	// check for valid attacking points that aren't out of range
	bool checkAtPoints(int);
	// polymorphic functions
	// add custom character
	virtual void addCharacter();
	// add random character
	virtual void addCharacter(int rand){}
	void addCharacter(int, int, int, int);
	// attack and defend functions
	virtual void attack(Character*, int&);
	virtual void defend(Character*, int&);
};

Character::Character(){
	name = "";
	attackPoints = 0;
	health = 0;
	defence = 0;
	luck = 0;
	dead = false;
}

void Character::addCharacter(){
	string input = "";
	while (input == ""){
		cout << "\nEnter name for the character: ";
		getline(cin, input);
		if (input == ""){
			cout << "\nThe name can't be empty! Try again.\n";
		}
	}
	name += input;
	while (attackPoints < 1 || attackPoints > 100){
		cout << "\nEnter the attack points for " << getName() << ": ";
		getline(cin, input);
		stringstream(input) >> attackPoints;
		if (attackPoints < 1 || attackPoints>100){
			cout << "\nAttack points should be in the range 1 to 100 inclusive! Try again.\n";
		}
	}
	while (health < 1 || health > 100){
		cout << "\nEnter the health points for " << getName() << ": ";
		getline(cin, input);
		stringstream(input) >> health;
		if (health < 1 || health > 100){
			cout << "\nHealth points should be in the range 1 to 100 inclusive! Try again.\n";
		}
	}
	while (defence < 1 || defence > 100){
		cout << "\nEnter the defence points for " << getName() << ": ";
		getline(cin, input);
		stringstream(input) >> defence;
		if (defence < 1 || defence > 100){
			cout << "\nDefence points should be in the range 1 to 100 inclusive! Try again.\n";
		}
	}
}

void Character::addCharacter(int randName, int randAttack, int randHealth, int randDefence){
	switch (randName){
	case 0:
		name += "Adam"; break;
	case 1:
		name += "Google"; break;
	case 2:
		name += "Intel"; break;
	case 3:
		name += "Luke"; break;
	case 4:
		name += "Max"; break;
	case 5:
		name += "Microsoft"; break;
	case 6:
		name += "Vader"; break;
	case 7:
		name += "Yoda"; break;
	default:
		name += "John";
	}
	attackPoints = randAttack;
	health = randHealth;
	defence = randDefence;
}

bool Character::checkAtPoints(int atPoints){
	if (atPoints <= 0 || atPoints > attackPoints){
		if (atPoints!=0) 
			cout << "\nAttack points should be in the range 0-" << attackPoints << " inclusive! Try again.\n";
		return false;
	}
	return true;
}

void Character::attack(Character* enemy, int& atPoints){
	luck = attackPoints - atPoints;
}

void Character::defend(Character* opponent, int& atPoints){
	if (defence < atPoints) atPoints = (-atPoints);
}

// Level 2:
class Hero : public Character{
private:
	int specialAbility;
	int numUses;
	bool abilityOn;
public:
	Hero();
	bool isAbilityOn(){ return abilityOn; }
	void setAbilityOn(bool fl){ abilityOn = fl; }
	void addCharacter();
	void addCharacter(int, int);
	void attack(Character*, int&);
	void defend(Character*, int&);
};

Hero::Hero(){
	specialAbility = 0;
	numUses = 0;
	abilityOn = false;
}

void Hero::addCharacter(){
	Character::addCharacter();
	string input = "";
	cout << "\nEnter special ability for " << getName() << ": \n";
	cout << "1. Deal double the attack points you chose.\n";
	cout << "2. Become immune to damage in your attack.\n";
	while (specialAbility != 1 && specialAbility != 2){
		getline(cin, input);
		stringstream(input) >> specialAbility;
		if (specialAbility != 1 && specialAbility != 2){
			cout << "\nCan't process that! Try again.\n";
		}
	}
	while (numUses < 1 || numUses > 100){
		cout << "\nEnter the number of uses for the special ability: ";
		getline(cin, input);
		stringstream(input) >> numUses;
		if (numUses < 1 || numUses > 100){
			cout << "\nNumber of uses should be in the range 1 to 100 inclusive! Try again.\n";
		}
	}
}

void Hero::addCharacter(int randSA, int randNU){
	Character::addCharacter(rand() % 8, rand() % 100 + 1, rand() % 100 + 1, rand() % 100 + 1);
	specialAbility = randSA;
	numUses = randNU;
}

void Hero::attack(Character* enemy, int& atPoints){
	Character::attack(enemy, atPoints);
	if (numUses){
		string input = "";
		while (input != "Y" && input != "y" && input != "N" && input != "n"){
			cout << "\nDo you want to use the special ability of " << getName() << " ( " << numUses << " uses left)(y/n): \n";
			getline(cin, input);
			if (input != "Y" && input != "y" && input != "N" && input != "n"){
				cout << "\nCan't process that! Try again.\n";
			}
		}
		if (input == "Y" || input == "y"){
			numUses--;
			abilityOn = true;
			if (specialAbility == 1){
				atPoints *= 2;
				cout << "\nYou'll now deal  twice the damage (" << atPoints << ").\n";
			}
			else{
				cout << "\nYou'll now be immune to damage for this turn.\n";
			}
		}
	}
}

void Hero::defend(Character* opponent, int& atPoints){
	Character::defend(opponent, atPoints);
	if (!abilityOn || specialAbility != 2){
		if (atPoints < 0){
			setHealth(getHealth() + atPoints);
			cout << "\nYour hero lost " << -atPoints << " health points.\n";
		}
		else if (getLuck() <= (rand() % 100)){
			setHealth(getHealth() - atPoints/2);
			cout << "\nYou resisted the attack but due to fatigue you lost " << atPoints / 2 << " health points.\n";
		}
		else {
			cout << "\nYou resisted the attack.\n";
		}
		if (getHealth() < 1){
			setDead(true);
			cout << "\nYou died!\n";
		}
	}
}

class Monster : public Character{
private:
	int evadeChance;
public:
	Monster();
	void addCharacter();
	void addCharacter(int);
	void attack(Character*, int&);
	void defend(Character*, int&);
};

Monster::Monster(){
	evadeChance = 0;
}

void Monster::addCharacter(){
	Character::addCharacter();
	string input = "";
	while (evadeChance < 1 || evadeChance > 100){
		cout << "\nEnter the chance of evasion for" << getName() << ": ";
		getline(cin, input);
		stringstream(input) >> evadeChance;
		if (evadeChance < 1 || evadeChance > 100){
			cout << "\nThe evade chance should be in the range 1 to 100 inclusive! Try again.\n";
		}
	}
}

void Monster::addCharacter(int randEC){
	Character::addCharacter(rand() % 8, rand() % 100 + 1, rand() % 100 + 1, rand() % 100 + 1);
	evadeChance = randEC;
}

void Monster::attack(Character* enemy, int& atPoints){
	Character::attack(enemy, atPoints);
}

void Monster::defend(Character* opponent, int& atPoints){
	bool fl = false;
	Character::defend(opponent, atPoints);
	if (atPoints < 0){
		if (evadeChance <= rand() % 100){
			setHealth(getHealth() + atPoints);
			cout << "\nYou dealt " << -atPoints << " damage to your opponent " << getName() << ".\n";
			if (getHealth() < 1){
				setDead(true);
				cout << "\nYou killed the " << getName() << "!\n";
			}
		}
		else{
			cout << "\nThe monster has evaded your attack.\n";
			fl = true;
		}
	}
	else
		cout << "\nThe monster has resisted your attack.\n";
	if (atPoints > 0 || fl){
		int attackP = rand() % getAtPoints() + 1;
		int preHealth = opponent->getHealth();
		cout << "\nThe monster is attacking you with " << attackP << " points!\n";
		attack(opponent, attackP);
		if (preHealth == opponent->getHealth() && getLuck() <= rand() % 100){
			setHealth(getHealth() - abs(atPoints)/2);
			cout << "\nDue to fatigue the monster lost " << abs(atPoints) / 2 << " health points.\n";
			if (getHealth() < 1){
				setDead(true);
				cout << "\nYou killed the " << getName() << "!\n";
			}
		}
	}
}

// Level 3:
class Warrior : public Hero{
private:
	int lastStandChance;
public:
	Warrior();
	void addCharacter();
	void addCharacter(int);
	void attack(Character*, int&);
	void defend(Character*, int&);
};

Warrior::Warrior(){
	lastStandChance = 0;
}

void Warrior::addCharacter(){
	Hero::addCharacter();
	string input = "";
	while (lastStandChance < 1 || lastStandChance > 100){
		cout << "\nEnter the chance for last stand for the warrior " << getName() << ": ";
		getline(cin, input);
		stringstream(input) >> lastStandChance;
		if (lastStandChance < 1 || lastStandChance > 100){
			cout << "\nThe chance for last stand should be in the range 1 to 100 inclusive! Try again.\n";
		}
	}
}

void Warrior::addCharacter(int randLSC){
	Hero::addCharacter(rand()%2+1, rand()%10+1);
	lastStandChance = randLSC;
}

void Warrior::attack(Character* enemy, int& atPoints){
	Hero::attack(enemy, atPoints);
	enemy->defend(this, atPoints);
	setAbilityOn(false);
}

void Warrior::defend(Character* opponent, int& atPoints){
	Hero::defend(opponent, atPoints);
	if (isDead() && (lastStandChance + getLuck()) > rand() % 100){
		setHealth(5);
		setDead(false);
		cout << "\nYour hero has returned from the dead is now in last stand!\n";
	}
}

class Mage : public Hero{
private:
	int evadeChance;
public:
	Mage();
	void addCharacter();
	void addCharacter(int);
	void attack(Character*, int&);
	void defend(Character*, int&);
};

Mage::Mage(){
	evadeChance = 0;
}

void Mage::addCharacter(){
	Hero::addCharacter();
	string input = "";
	while (evadeChance < 1 || evadeChance > 100){
		cout << "\nEnter the evasion chance for the mage " << getName() << ": ";
		getline(cin, input);
		stringstream(input) >> evadeChance;
		if (evadeChance < 1 || evadeChance > 100){
			cout << "\nThe evade chance should be in the range 1 to 100 inclusive! Try again.\n";
		}
	}
}

void Mage::addCharacter(int randEC){
	Hero::addCharacter(rand()%2+1, rand()%10+1);
	evadeChance = randEC;
}

void Mage::attack(Character* enemy, int& atPoints){
	Hero::attack(enemy, atPoints);
	enemy->defend(this, atPoints);
	setAbilityOn(false);
}

void Mage::defend(Character* opponent, int& atPoints){
	if ((evadeChance+getLuck()) <= rand() % 100){
		Hero::defend(opponent, atPoints);
	}
	else {
		cout << "\nYou have evaded the attack from the " << opponent->getName() << ".\n";
		if (getLuck() <= (rand() % 100)){
			setHealth(getHealth() - atPoints / 2);
			cout << "\nYou resisted the attack but due to fatigue you lost " << atPoints / 2 << " health points.\n";
		}
	}
}

class Droid : public Monster{
private:
	int increaseDefChance;
public:
	Droid();
	void addCharacter();
	void addCharacter(int);
	void attack(Character*, int&);
	void defend(Character*, int&);
};

Droid::Droid(){
	increaseDefChance=0;
	setName("droid ");
}

void Droid::addCharacter(){
	Monster::addCharacter();
	string input = "";
	while (increaseDefChance < 1 || increaseDefChance > 100){
		cout << "\nEnter the chance for increase in defence of the " << getName() << ": ";
		getline(cin, input);
		stringstream(input) >> increaseDefChance;
		if (increaseDefChance < 1 || increaseDefChance > 100){
			cout << "\nThe chance for increase in defence should be in the range 1 to 100 inclusive! Try again.\n";
		}
	}
}

void Droid::addCharacter(int randIDC){
	Monster::addCharacter(rand() % 100 + 1);
	increaseDefChance = randIDC;
}

void Droid::attack(Character* enemy, int& atPoints){
	Monster::attack(enemy, atPoints);
	enemy->defend(this, atPoints);
}

void Droid::defend(Character* opponent, int& atPoints){
	int increase = 0;
	if (increaseDefChance > (rand() % 100)){
		increase = rand() % 20 + 1;
		setDefence(getDefence() + increase);
		cout << "\nThe " << getName() << " has increased his defence points by " << increase << ".\n";
	}
	Monster::defend(opponent, atPoints);
	setDefence(getDefence() - increase);
}

class Vampire : public Monster{
private:
	int suckChance;
public:
	Vampire();
	void addCharacter();
	void addCharacter(int);
	void attack(Character*, int&);
	void defend(Character*, int&);
};

Vampire::Vampire(){
	suckChance = 0;
	setName("vampire ");
}

void Vampire::addCharacter(){
	Monster::addCharacter();
	string input = "";
	while (suckChance < 1 || suckChance > 100){
		cout << "\nEnter the chance for the " << getName() << " to suck the blood of his opponent: ";
		getline(cin, input);
		stringstream(input) >> suckChance;
		if (suckChance < 1 || suckChance > 100){
			cout << "\nThe chance should be in the range 1 to 100 inclusive! Try again.\n";
		}
	}
}

void Vampire::addCharacter(int randSC){
	Monster::addCharacter(rand() % 100 + 1);
	suckChance = randSC;
}

void Vampire::attack(Character* enemy, int& atPoints){
	Monster::attack(enemy, atPoints);
	enemy->defend(this, atPoints);
}

void Vampire::defend(Character* opponent, int& atPoints){
	int preHealth = opponent->getHealth();
	Monster::defend(opponent, atPoints);
	if(preHealth != opponent->getHealth() && (suckChance+getLuck())>rand()%100){
			setHealth(getHealth() + (preHealth - opponent->getHealth()));
			cout << "\nThe " << getName() << " has sucked your blood and is now stronger!\n";
		}
}

class Werewolf : public Monster{
private:
	int attackTwiceChance;
public:
	Werewolf();
	void addCharacter();
	void addCharacter(int);
	void attack(Character*, int&);
	void defend(Character*, int&);
};

Werewolf::Werewolf(){
	attackTwiceChance = 0;
	setName("werewolf ");
}

void Werewolf::addCharacter(){
	Monster::addCharacter();
	string input = "";
	while (attackTwiceChance < 1 || attackTwiceChance > 100){
		cout << "\nEnter the chance for the " << getName() << " to attack twice: ";
		getline(cin, input);
		stringstream(input) >> attackTwiceChance;
		if (attackTwiceChance < 1 || attackTwiceChance > 100){
			cout << "\nThe chance should be in the range 1 to 100 inclusive! Try again.\n";
		}
	}
}

void Werewolf::addCharacter(int randATC){
	Monster::addCharacter(rand() % 100 + 1);
	attackTwiceChance = randATC;
}

void Werewolf::attack(Character* enemy, int& atPoints){
	Monster::attack(enemy, atPoints);
	enemy->defend(this, atPoints);
	if (!enemy->isDead() && (attackTwiceChance + getLuck()) > rand() % 100){
		cout << "\nThe " << getName() << " is attacking you again!\n";
		enemy->defend(this, atPoints);
	}
}

void Werewolf::defend(Character* opponent, int& atPoints){
	Monster::defend(opponent, atPoints);
}

// control flow functions
void chooseHero(Character*&);
void chooseWave(vector<Character*>&);
void sortWave(vector<Character*>&);
// sorting functions
// use pointer to function (get) to determine by which parameter to sort
void insertionSort(vector<Character*>&, int, int, int (Character::*)());
void quickSort(vector<Character*>&, int, int, int (Character::*)());
void swap(Character*&, Character*&);

int main(){
	srand(unsigned(time(0)));
	Character* hero;
	chooseHero(hero);
	vector<Character*> monsters;
	chooseWave(monsters);
	string input="";
	while (input != "Y" && input != "y" && input != "N" && input != "n"){
		cout << "\nDo you want to sort the wave by the stats of the monsters(y/n)?";
		getline(cin, input);
		if (input == "Y" || input == "y"){
			sortWave(monsters);
		}
		else if (input != "N" && input != "n"){
			cout << "\nCan't process that! Try again!\n";
		}
	}
	int aPoints = -1;
	cout << '\n' << left;
	do{
		cout << '\n';
		cout << setw(10) << "Hero: "    << setw(10) << left << hero->getName() << setw(10) << "Monster: " << setw(10) << monsters.front()->getName() << '\n';
		cout << setw(10) << "Attack: "  << setw(10) << hero->getAtPoints()     << setw(10) << "Attack: "  << setw(10) << "-" << '\n';
		cout << setw(10) << "Defence: " << setw(10) << hero->getDefence()      << setw(10) << "Defence: "  << setw(10) << "-" << '\n';
		cout << setw(10) << "Health: "  << setw(10) << hero->getHealth()       << setw(10) << "Health: "   << setw(10) << monsters.front()->getHealth() << '\n';
		cout << "Enter the points with which to attack the " << monsters.front()->getName() << "\n(0 to surrender): ";
		getline(cin, input);
		stringstream(input) >> aPoints;
		system("cls");
		if (aPoints == -1 && input != "-1"){
			cout << "Can't process that! Try again.\n";
		}
		else if (hero->checkAtPoints(aPoints)){
			hero->attack(monsters.front(), aPoints);
		}
		if (monsters.front()->isDead()){
			monsters.erase(monsters.begin());
		}
	} while (aPoints!=0 && !hero->isDead() && !monsters.empty());
	if (hero->isDead()){
		cout << "\n\nYou lost the game.\n\n";
	}
	else if (aPoints){
		cout << "\n\nCongratulations you are the winner!\n\n";
	}
	else  {
		cout << "\n\nYou surrendered. Try with more courage next time!\n\n";
	}
	return 0;
}

void chooseHero(Character*& hero){
	string input="";
	while (input != "W" && input != "w" && input != "M" && input != "m"){
		cout << "Do you want to play as a (W)arrior or a (M)age?\n";
		getline(cin, input);
		if (input == "W" || input == "w") hero = new Warrior;
		else if (input == "M" || input == "m") hero = new Mage;
		else cout << "\nCan't process that! Try again.\n";
	}
	input = "";
	while (input!= "O" && input!="o" && input!="R" && input!="r"){
		cout << "\nEnter your (o)wn statistics for the hero or generate them (r)andomly?\n";
		getline(cin, input);
		if (input == "O" || input == "o") hero->addCharacter();
		else if (input == "R" || input == "r") hero->addCharacter(rand() % 100 + 1);
		else cout << "\nCan't process that! Try again.";
	}
}

void chooseWave(vector<Character*>& monsters){
	string input = "";
	while (input != "O" && input != "o" && input != "R" && input != "r"){
		cout << "\nDo you want to enter your (o)wn wave of monsters or a generate a (r)andom wave?\n";
		getline(cin, input);
		if (input != "O" && input != "o" && input != "R" && input != "r")
			cout << "\nCan't process that! Try again.";
	}
	if (input == "O" || input == "o"){
		int choice = -1;
		while (choice != 0){
			choice = -1;
			cout << "\n1. Droid";
			cout << "\n2. Vampire";
			cout << "\n3. Werewolf";
			cout << "\n0. Exit";
			cout << "\nInput: ";
			getline(cin, input);
			stringstream(input) >> choice;
			switch (choice){
			case 1:
				monsters.push_back(new Droid);
				break;
			case 2:
				monsters.push_back(new Vampire);
				break;
			case 3:
				monsters.push_back(new Werewolf);
				break;
			case 0:
				break;
			default:
				cout << "\nCan't process that! Try again.";
			}
			if (choice == 1 || choice == 2 || choice == 3) 
				monsters.back()->addCharacter();
		}
	}
	else{
		int num = -1;
		while (num < 1){
			num = -1;
			cout << "\nHow many monsters do you want to fight?\n";
			getline(cin, input);
			stringstream(input) >> num;
			if (num < 1)
				cout << "\nCan't process that! Try again.";
		}
		for (int i = 0; i < num; i++){
			int choice = rand() % 3 + 1;
			switch (choice){
			case 1:
				monsters.push_back(new Droid);
				break;
			case 2:
				monsters.push_back(new Vampire);
				break;
			case 3:
				monsters.push_back(new Werewolf);
				break;
			}
			monsters.back()->addCharacter(rand() % 100 + 1);
		}
	}
}

void sortWave(vector<Character*>& monsters){
	int choice = 0;
	string input = "";
	while (choice != 1 && choice != 2 && choice != 3){
		cout << "\nSort by:";
		cout << "\n1. Attack Points.";
		cout << "\n2. Health Points.";
		cout << "\n3. Defence Points.";
		cout << "\nInput: ";
		getline(cin, input);
		stringstream(input) >> choice;
		if (choice != 1 && choice != 2 && choice != 3){
			cout << "\nCan't process that! Try again.";
		}
	}
	switch (choice){
	case 1:
		quickSort(monsters, 0, monsters.size() - 1, &Character::getAtPoints); break;
	case 2:
		quickSort(monsters, 0, monsters.size() - 1, &Character::getHealth); break;
	case 3:
		quickSort(monsters, 0, monsters.size() - 1, &Character::getDefence); break;
	default:
		quickSort(monsters, 0, monsters.size() - 1, &Character::getAtPoints); break;
	}
}

void insertionSort(vector<Character*>& monsters, int low, int high, int (Character::*get)()){
	int j;
	for (int p = low; p < high + 1; p++){
		Character* tmp = monsters.at(p);
		for (j = p; j > 0 && (tmp->*get)() < (monsters.at(j - 1)->*get)(); j--)
			monsters.at(j) = monsters.at(j - 1);
		monsters.at(j) = tmp;
	}
}

void quickSort(vector<Character*>& monsters, int low, int high, int (Character::*get)()){
	if (low + 10 > high) insertionSort(monsters, low, high, get);	// call insertion sort if there are less than 10 monsters
	else{
		int middle = (low + high) / 2;
		if ((monsters.at(middle)->*get)() < (monsters.at(low)->*get)()) swap(monsters.at(low), monsters.at(middle));
		if ((monsters.at(high)->*get)() < (monsters.at(low)->*get)())   swap(monsters.at(low), monsters.at(high));
		if ((monsters.at(high)->*get)() < (monsters.at(middle)->*get)())swap(monsters.at(middle), monsters.at(high));
		int pivot = (monsters.at(middle)->*get)();
		swap(monsters.at(middle), monsters.at(high - 1));
		int i, j;
		for (i = low, j = high - 1;;){	//distribute the elements with regard to the pivot
			while ((monsters.at(++i)->*get)() < pivot){}
			while (pivot < (monsters.at(--j)->*get)()){}
			if (i < j) swap(monsters.at(i), monsters.at(j));
			else break;
		}
		swap(monsters.at(i), monsters.at(high - 1)); // put the pivot in its final place
		quickSort(monsters, low, i - 1, get);			 // sort the part left from the pivot
		quickSort(monsters, i + 1, high, get);			 // sort the part right from the pivot	
	}
}

void swap(Character*& a, Character*& b){
	Character* tmp = a;
	a = b;
	b = tmp;
}