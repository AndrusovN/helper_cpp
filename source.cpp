
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

	std::cout << std::endl << myList[0] << std::endl;
	myList[0] = 1234;
	myList[1] = 1234;

	if (myList[0] == myList[1] && myList[0] == 1234) {
		std::cout << myList[0] << std::endl;
	}
	

	Dictionary<char, int> myDict = Dictionary<char, int>();

	myDict.set('q', 123);
	myDict.set('w', -179);
	myDict.set('e', 98);
	myDict.set('r', 567);

	myDict.remove('q');

	reconstructDictionary(&myDict);

	myEnumerator = Enumerator(&myDict);

	for (Object i = myEnumerator.next(); i != nullObject; i = myEnumerator.next())
	{
		std::cout << i.get<int>() << std::endl;
	}
	std::cout << std::endl;

	//std::cout << myDict['w'] << std::endl;

	myDict['e'] = 1234;

	if (myDict['r'] == 567) {
		std::cout << myDict['e'] << std::endl;
		std::cout << myDict['w'] << std::endl;
		std::cout << myDict['r'] << std::endl;
	}
	

	Array myArray = Array(3);

	myArray.setItem<int>(0, 11);
	objects::Object obj1 = objects::Object();
	obj1.set<int>(179);
	myArray[1] = obj1;
	objects::Object obj2 = objects::Object();
	obj2.set<int>(-234);
	myArray[2] = obj2;

	myEnumerator = Enumerator(&myArray);

	for (objects::Object i = myEnumerator.next(); i != nullObject; i = myEnumerator.next())
	{
		std::cout << i.get<int>() << std::endl;
	}

	std::cout << "------------------------" << std::endl;

	TArray<double> myTArray = TArray<double>(4);
	myTArray[0] = 179.0;
	myTArray[1] = 0.73;
	myTArray[2] = 3.1415926535897932384626433;
	myTArray[3] = 2.718281828459045;

	myEnumerator = Enumerator(&myTArray);

	for (Object i = myEnumerator.next(); i != nullObject; i = myEnumerator.next())
	{
		std::cout << i.get<double>() << std::endl;
	}

	std::cout << std::endl;
	std::cout << std::endl;
	myTArray[1] = 3.1415926535897932384626433;

	std::cout << myTArray[2] << std::endl << (bool)(myTArray[2] < 2.1415926535897932384626433) << std::endl << (bool)(myTArray[2] == myTArray[1]) << std::endl;;

	return(0);
}