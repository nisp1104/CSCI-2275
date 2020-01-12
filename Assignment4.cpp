// Nick Spooner
// Assignment 3
// CSCI 2275
// 10/1/2019

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct node
	//Each city is represented by a node with a city name, the node before and after it, and a message that it can hold
{
	string name;
	node* next;
	node* prev;
	string message;
};

class CommunicationNetwork
{
	node *head, *tail; //The head and tail nodes are created to indicate where the doubly linked list starts and ends
	bool cityNotFound = false;
	string userChoice;

public:
	CommunicationNetwork() //List starts out as empty with null head and tail values when this constructor is called
	{
		head = tail = nullptr;
	}

	~CommunicationNetwork() //This destructor loops through the list, deletes all nodes, and resets head and tail nodes to null
	{
		node* temp = head;
		while (temp != nullptr)
		{
			node* next = temp->next;
			delete temp;
			temp = next;
		}

		head = tail = nullptr;
	}

	void insert(string city) //Insert function only used to build the network without using the prevCity parameter. This is only used when building the network since in that case the cities are just being added
							 //to the end of the list
	{
		node* temp = new node();
		temp->name = city;
		temp->next = temp->prev = nullptr;

		if (head == nullptr) //If the list is empty, add the city and set the head and tail of the list to that city
		{
			head = tail = temp;
		}
		else //If the list isn't empty, add the city to the end of the list and set the tail to that city
		{
			tail->next = temp;
			temp->prev = tail;
			tail = temp;
		}
	}

	void mainMenu(char* inputFile) //Takes an input as the same type as argv[]. This was the only way I could figure out how to get the command line argument inside the class function
	{
		while (true) //This while loop never breaks since the code exits itself once the user picks the 7th choice, and the program isn't needed after that point
		{
			cout << "======Main Menu======" << endl;
			cout << "1. Build Network" << endl;
			cout << "2. Print Network Path" << endl;
			cout << "3. Transmit Message Coast-To-Coast-To-Coast" << endl;
			cout << "4. Add City" << endl;
			cout << "5. Delete City" << endl;
			cout << "6. Clear Network" << endl;
			cout << "7. Quit" << endl;

			cin >> userChoice;

			switch (stoi(userChoice)) //Turns the user input into an integer so it can be used with the switch statement easily
			{
			case 1:
				{
					buildNetwork();
					break;
				}
			case 2:
				{
					printNetwork();
					break;
				}
			case 3:
				{
					ifstream inFile;
					inFile.open(inputFile);

					string userCityX, userCityY;
					string messageWord;

					cin.clear(); //Taking multiple inputs from getline with cin, so the next 2 lines are used to read multiple words
					cin.ignore();

					cout << "Enter City X: ";
					getline(cin, userCityX);
					cout << "Enter City Y: ";
					getline(cin, userCityY);

					while (inFile >> messageWord)
					{
						transmitMessage(userCityX, userCityY, messageWord);

						if (cityNotFound) //This exists so that "city not found" doesn't get printed multiple times
							break;
					}
					break;
				}
			case 4:
				{
					string userCity, userPrevCity;

					cin.clear(); //Same purpose as above
					cin.ignore();

					cout << "Enter a city name: ";
					getline(cin, userCity);
					cout << "Enter a previous city name: ";
					getline(cin, userPrevCity);

					addCity(userPrevCity, userCity);
					break;
				}
			case 5:
				{
					string userDeleteCity;

					cin.clear();
					cin.ignore();

					cout << "Enter a city name: ";
					getline(cin, userDeleteCity);

					deleteCity(userDeleteCity);
					break;
				}
			case 6:
				{
					clearNetwork();
					break;
				}
			case 7:
				{
					cout << "Goodbye!" << endl;
					exit(1); //Exits the code so that the while loop doesn't need to use any conditions
				}
			}

			cout << endl;
		}
	}

	void buildNetwork()
	{
		this->~CommunicationNetwork(); //Since the user might have added or deleted something they didn't want to, or if the user doesn't want duplicate cities, the destructor is ran before the base cities are put
									   //back into the list

		insert("Los Angeles"); //Like I said in the insert function, I used insert rather than addCity so that I didn't need to tell it a previous city and so that the cities would just be placed at the end of
								   //the list
		insert("Phoenix");
		insert("Denver");
		insert("Dallas");
		insert("St. Louis");
		insert("Chicago");
		insert("Atlanta");
		insert("Washington, D.C.");
		insert("New York");
		insert("Boston");

		printNetwork();
	}

	void printNetwork()
	{
		node* temp = head;

		cout << "===CURRENT PATH===" << endl;

		cout << "NULL <- ";

		while (temp != nullptr)
		{
			cout << temp->name; //Outputs the name of the city that temp equals, and temp goes through the list printing out all the cities

			if (temp->next == nullptr) //This makes it so that a city points to null, but null doesn't point back since it's not a pointer
			{
				cout << " -> ";
			}
			else
				cout << " <-> ";

			temp = temp->next;
		}

		cout << "NULL" << endl;

		cout << "==================" << endl;
	}

	void transmitMessage(string x, string y, string sendMessage)
	{
		node* cityX = head;
		node* cityY = head;

		while (cityX->name != x && cityX->next != nullptr) //Sets CityX equal to the city that has the same name as the user's named city (CityX is finding itself in the list)
			cityX = cityX->next;

		while (cityY->name != y && cityY->next != nullptr) //Same for CityY
			cityY = cityY->next;

		if (cityX->next == nullptr && cityX->name != x) //If CityX gets to the end of the list and doesn't find an existing city with the same name, the user must have given the wrong name for a city
		{
			cout << x << " not found" << endl;
			cityNotFound = true;
			return;
		}

		if (cityY->next == nullptr && cityY->name != y) //Same for CityY
		{
			cout << y << " not found" << endl;
			cityNotFound = true;
			return;
		}


		cityX->message = sendMessage; //Gives the message to CityX to start out

		node* currentCity = cityX;

		while (currentCity->name != y && currentCity->next != nullptr) //This while loop goes through the list until it reaches CityY or until it reaches the end of the list. It prints out which city got the
																	   //message, jumps to the next city, copies the message over, then deletes the message from the last city
		{
			cout << currentCity->name << " received " << currentCity->message << endl;
			currentCity = currentCity->next;
			currentCity->message = currentCity->prev->message;
			currentCity->prev->message = "";
		}

		while (currentCity->name != x && currentCity != nullptr) //Same loop as above but the other direction
		{
			cout << currentCity->name << " received " << currentCity->message << endl;
			currentCity = currentCity->prev;
			currentCity->message = currentCity->next->message;
			currentCity->next->message = "";
		}

		cout << currentCity->name << " received " << currentCity->message << endl; //I couldn't get the while loop above to print the last city's message because of the bounds (it would try to jump into null), so I
																				   //had to manually print it out a last time
	}

	void addCity(string prevCityName, string newCityName) //This function uses the previous city as a parameter for the user to say where they want the city inserted after in the list
	{
		node* temp = head;
		node* newCity = new node;
		newCity->name = newCityName;
		node* nextCity = new node;


		if (prevCityName == "first" || prevCityName == "First") //If the user wants their city at the beginning of the list...
		{
			if (head == nullptr) //If head is null (the list is empty), just use the insert function since head is null and we can't put a city in front of null (as I do with head below)
				insert(newCityName);
			else
			{
				newCity->next = head; //Otherwise put the new city before head and set head to the new city
				head->prev = newCity;
				newCity->prev = nullptr;
				head = newCity;
			}
		}

		else
		{
			while (temp->name != prevCityName) //Find where the user wants their city in the list
			{
				temp = temp->next;
			}

			if (temp->next == nullptr && temp->name != prevCityName) //If the previous city given by the user isn't found, they typed in a non-existing city
				cout << prevCityName << " not found" << endl;
			
			else if (temp->next == nullptr) //If the city is at the end of the list, place it after tail and set it to be the new tail of the list
			{
				temp->next = newCity;
				newCity->prev = temp;
				newCity->next = nullptr;
				tail = temp;
			}

			else
			{
				nextCity = temp->next; //If it's not before the head or after the tail, put it after the city given by the user and point it to the cities around it
				temp->next = newCity;
				newCity->prev = temp;
				newCity->next = nextCity;
				nextCity->prev = newCity;
			}
		}
	}

	void deleteCity(string cityNameDelete)
	{
		node* temp = head;
		node* prevCity = new node;
		node* nextCity = new node;

		while (temp->name != cityNameDelete && temp->next != nullptr) //Look for the city that the user wants deleted
			temp = temp->next;

		if (temp->next == nullptr && temp->name != cityNameDelete) //If the city isn't found, the user typed in a non-existing city
			cout << cityNameDelete << " not found" << endl;

		else if (temp->prev == nullptr) //If the city is at the beginning of the list, delete it and set the head to the next city
		{
			nextCity = temp->next;
			nextCity->prev = nullptr;
			delete temp;
			head = nextCity;
		}

		else if (temp->next == nullptr) //If it;s at the end of the list, delete it and set the tail to the city before
		{
			prevCity = temp->prev;
			prevCity->next = nullptr;
			delete temp;
			tail = prevCity;
		}

		else //Otherwise delete the city and set the surrounding cities to point to each other
		{
			nextCity = temp->next;
			prevCity = temp->prev;
			nextCity->prev = prevCity;
			prevCity->next = nextCity;
			delete temp;
		}
	}

	void clearNetwork()
	{
		node* temp = head;
		while (temp != nullptr) //This function loops through the list and deletes every city. As requested by the instructions, it outputs each city as it's deleted
		{
			node* next = temp->next;
			cout << "deleting " << temp->name << endl;
			delete temp;
			temp = next;
		}

		head = tail = nullptr; //The head and tail of the list are reset to null
	}
};

int main(int argc, char* argv[])
{
	CommunicationNetwork network;

	network.mainMenu(argv[1]); //Runs the main menu function and passes in the second command line argument, which should be the text file of the message to be passed. Since the main menu contains an "infinite"
							   //while loop, and since this function calls all the other functions internally, this is the only one that needs to be called.
}
