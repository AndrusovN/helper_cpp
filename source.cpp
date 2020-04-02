
#include "helper.h"

int main() {
	/*Object* keys = new Object[1];
	keys[0] = Object();
	keys[0].set<CharSet<13>>(CharSet<13>((char*)"Hello world!"));

	Object* values = new Object[1];
	values[0] = Object();
	values[0].set<std::string>("Some string");

	SuperDictionary mySuperDictionary = SuperDictionary(keys, values, 1);
	
	mySuperDictionary.set<int, std::string>(17, "qwerty");

	mySuperDictionary.set<char, std::string>('q', "179.73");

	mySuperDictionary.set<std::string, std::string>("zxcv", "poiuy");

	mySuperDictionary.remove<CharSet<13>>(CharSet<13>((char*)"Hello world!"));

	Enumerator myEnumerator(&mySuperDictionary);

	for (Object i = myEnumerator.next(); i != nullObject; i = myEnumerator.next()) {
		std::cout << i.get<std::string>() << std::endl;
	}

	std::cout << std::endl;
	std::string n;
	std::cin >> n;

	reconstructDictionary(&mySuperDictionary);

	Enumerator myNewEnumerator(&mySuperDictionary);

	for (Object i = myNewEnumerator.next(); i != nullObject; i = myNewEnumerator.next()) {
		std::cout << i.get<std::string>() << std::endl;
	}

	std::cout << std::endl;
	std::cin >> n;

	mySuperDictionary.remove<std::string>("zxcv");

	Enumerator myFinEnumerator(&mySuperDictionary);

	for (Object i = myFinEnumerator.next(); i != nullObject; i = myFinEnumerator.next()) {
		std::cout << i.get<std::string>() << std::endl;
	}

	std::cout << std::endl;
	std::cin >> n;

	std::string A = mySuperDictionary.get<int, std::string>(17);

	std::cout << std::endl << A << std::endl << mySuperDictionary.get<char, std::string>('q') << std::endl;*/

	List<int> myList = List<int>();

	myList.append(17);
	myList.append(19);
	myList.append(-179);

	Enumerator myEnumerator = Enumerator(&myList);

	for (Object i = myEnumerator.next(); i != nullObject; i = myEnumerator.next())
	{
		std::cout << i.get<int>() << std::endl;
	}
	std::cout << std::endl;
	myList.set(1, 1111);

	myList.pop(0);
	
	myList.append(123);
	myEnumerator = Enumerator(&myList);

	for (Object i = myEnumerator.next(); i != nullObject; i = myEnumerator.next())
	{
		std::cout << i.get<int>() << std::endl;
	}

	std::cout << std::endl << myList[0].value() << std::endl;
	myList[0] = 1234;
	myList[1] = 1234;

	if (myList[0] == myList[1] && myList[0] == 1234) {
		std::cout << myList[0].value() << std::endl;
	}


	return(0);
}