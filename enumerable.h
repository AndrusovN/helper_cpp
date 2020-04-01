#include <iostream>
#include <sstream>
#include <windows.h>
#include "exceptions.h"

#ifndef ENUMERABLE
#define ENUMERABLE

#pragma warning(disable:4996)
namespace objects {
	//enumerable::Dictionary<enumerable::CharSet<128>, int> typeSizes = enumerable::Dictionary<enumerable::CharSet<128>, int>();

	class TypeException : public Exception {
	public:
		TypeException() {}
		TypeException(char* str) {
			message = str;
		}
	};

	class Object {
	public:
		bool operator == (Object a) {
			return(isEqual(a) == 0);
			//return(a.ref == this->ref && a.typeName == this->typeName);
		}

		bool operator > (Object a) {
			return(isEqual(a) == 1);
			//return(a.ref == this->ref && a.typeName == this->typeName);
		}

		bool operator < (Object a) {
			return(isEqual(a) == -1);
			//return(a.ref == this->ref && a.typeName == this->typeName);
		}

		bool operator != (Object a) {
			return(!(*this == a));
		}

		bool operator >>= (Object a){
			return(a.ref == this->ref && a.typeName == this->typeName);
		}

		template<class T>
		void set(T value) {
			initialized = true;
			//ref = (void*)malloc(sizeof(T));
			T* p = new T;
			*p = value;
			ref = (void*)p;
			//char* name = new char[128];
			typeName = (char*)(typeid(T).name());
			typeSize = sizeof(T);
			/*bool enough = false;
			for (int i = 0; i < 127; i++)
			{
				if (!enough) {
					name[i] = typeName[i];
					if (typeName[i] == '\0') {
						enough = true;
						name[i] = ' ';
					}
				}
				else {
					name[i] = ' ';
				}
				
			}
			name[127] = '\0';
			//delete[] typeName;
			typeName = name;
			typeSizes.set(enumerable::CharSet<128>(typeName), sizeof(T));*/
			//typeName = (char*)(typeid(T).name());
			//delete p;
		}

		char* getType() {
			return(typeName);
		}

		template <class T>
		T get() {
			try {
				if (typeid(T).name() == this->typeName) {
					//std::cout << typeid(T).name() << " " << this->typeName << std::endl;
					return(*(T*)this->ref);
				}
				else {
					//char* a = (char*)"Wrong type ";
					std::string p = "Wrong type ";
					p += typeid(T).name();
					p += " instead of ";
					p += typeName;
					//std::cout << p << std::endl;
					char* text = (char*)malloc(p.length()+1);
					strcpy(text, p.c_str());
					throw(&TypeException(text));
				}
			}
			catch (TypeException* ex) {
				ex->work();
				return(T());
				//throw(&TypeException((char*)"Wrong type!"));
			}
		}
		~Object() {
			if (initialized) {
				//std::cout << *(int*)ref << typeName << std::endl;
				//delete (char*)ref;
				//delete typeName;
			}
			
		}
	private:
		int typeSize;
		bool initialized = false;
		void* ref;
		char* typeName;
		int isEqual(Object a) {
			if (typeSize < a.typeSize) {
				return(-1);
			}
			if (typeSize > a.typeSize) {
				return(1);
			}
			int len = typeSize;
			char* meAsChar = (char*)ref;
			char* aAsChar = (char*)a.ref;

			for (int i = 0; i < len; i++)
			{
				if (meAsChar[i] > aAsChar[i]) {
					//std::cout << meAsChar[i] << " " << aAsChar[i] << " " << i << len << std::endl;
					return(1);
				}
				if (meAsChar[i] < aAsChar[i]) {
					return(-1);
				}
			}
			return(0);
		}
	};

}

namespace enumerable {
	objects::Object nullObject = objects::Object();

	__interface IItem
	{
	public:
		objects::Object getValue();
		void setValue(objects::Object val, void* sender);
		void* _init();
	};

	__interface IEnumerable
	{
	public:
		IItem* getNext(IItem * current, int index, int hash);
		int getNextIndex(IItem* current, int index, int hash);
		int getNextHash(IItem* current, int index, int hash);
		IItem* getFirst();
		int getFirstIndex();
		int getFirstHash();
	};
	
	class ItemEnumerator {
	public:
		ItemEnumerator(IEnumerable* en) {
			Enumerable = en;
			currentHash = en->getFirstHash();
			currentIdx = en->getFirstIndex();
			currentItem = en->getFirst();
		}

		IItem* next() {
			IItem* needed = currentItem;
			IItem* nI = Enumerable->getNext(currentItem, currentIdx, currentHash);
			int nIdx = Enumerable->getNextIndex(currentItem, currentIdx, currentHash);
			int nHash = Enumerable->getNextHash(currentItem, currentIdx, currentHash);

			currentItem = nI;
			currentHash = nHash;
			currentIdx = nIdx;

			//delete nI;
			return(needed);
		}

	private:
		int currentIdx;
		int currentHash;
		IItem* currentItem;
		IEnumerable* Enumerable;
	};

	class Enumerator {
	public:
		Enumerator(IEnumerable* en) {
			Enumerable = en;
			currentHash = en->getFirstHash();
			currentIdx = en->getFirstIndex();
			currentItem = en->getFirst();
		}

		objects::Object next() {
			objects::Object needed = currentItem->getValue();
			IItem* nI = Enumerable->getNext(currentItem, currentIdx, currentHash);
			int nIdx = Enumerable->getNextIndex(currentItem, currentIdx, currentHash);
			int nHash = Enumerable->getNextHash(currentItem, currentIdx, currentHash);

			currentItem = nI;
			currentHash = nHash;
			currentIdx = nIdx;

			//delete nI;
			return(needed);
		}

	private:
		int currentIdx;
		int currentHash;
		IItem* currentItem;
		IEnumerable* Enumerable;
	};

	class PrivacyException : public objects::Exception {
	public:
		PrivacyException() {}
		PrivacyException(char* str) {
			message = str;
		}

	};

	class IndexException : public objects::Exception {
	public:
		IndexException() {}
		IndexException(char* str) {
			message = str;
		}
	};

	class ArrItem : public IItem {
	public:
		void operator = (ArrItem a) {
			this->set = a.set;
			this->value = a.value;
			this->rightsender = a.rightsender;
		}
		ArrItem() {

		}
		ArrItem(char* p) {

		}
		objects::Object getValue() {
			return(value);
		}
		void setValue(objects::Object val, void* sender) {
			try {
				if (sender == rightsender) {
					value = val;
				}
				else {
					//std::cout << "there" << std::endl;
					throw(&PrivacyException((char*)"Current sender has no rights to change this item!"));
				}
			}
			catch (objects::Exception* ex) {
				//std::cout << "there1" << std::endl;
				ex->work();
				//std::cout << "there3" << std::endl;
			}
			
		}

		void* _init() {
			
			rightsender = (void*)malloc(1);
			set = true;
			return(rightsender);
		}

		~ArrItem() {
			//delete rightsender;
		}
	private:
		objects::Object value;
		void* rightsender;
		bool set = false;
	};

	class Array : public IEnumerable {
	public:
		Array(int len) {
			arr = new ArrItem[len];
			keys = new void* [len];
			length = len;
			for (int i = 0; i < length; i++)
			{
				//ArrItem p = ArrItem();
				*(arr+i) = ArrItem();
				keys[i] = arr[i]._init();
			}
		}

		IItem* getNext(IItem* current, int index, int hash) {
			try {
				if (index < length - 1 && index > -2) {
					return(&arr[index+1]);
				}
				else {
					ArrItem* P = new ArrItem;
					*P = ArrItem();
					void* key = P->_init();
					P->setValue(nullObject, key);
					delete key;
					return(P);
				}
			}
			catch (objects::Exception* ex) {
				ex->work();
			}
			
		}

		int getNextIndex(IItem* current, int index, int hash) {
			return(index + 1);
		}

		int getNextHash(IItem* current, int index, int hash) {
			return(0);
		}

		ArrItem* getFirst() {
			return(&arr[0]);
		}

		int getFirstIndex() {
			return(0);
		}

		int getFirstHash() {
			return(0);
		}

		objects::Object get(int index) {
			try {
				if (index < length && index > -1) {
					return(arr[index].getValue());
				}
				else {
					std::string p = "Index out of range exception: array size ";
					std::stringstream ss;
					ss << length;
					std::string n;
					ss >> n;
					p += n;
					p += " when index is ";
					ss << index;
					ss >> n;
					p += n;
					char* text = (char*)malloc(p.length() + 1);
					strcpy(text, p.c_str());
					throw(&IndexException(text));
				}
			}
			catch (objects::Exception* ex) {
				ex->work();
			}
		}

		template <class T>
		T get(int index) {
			return(get(index).get<T>());
		}
		void setItem(int index, objects::Object value) {
			try {
				if (index < length && index > -1) {
					arr[index].setValue(value, keys[index]);
				}
				else {
					std::string p = "Index out of range exception: array size ";
					std::stringstream ss;
					ss << length;
					std::string n;
					ss >> n;
					p += n;
					p += " when index is ";
					ss << index;
					ss >> n;
					p += n;
					char* text = (char*)malloc(p.length() + 1);
					strcpy(text, p.c_str());
					throw(&IndexException(text));
				}
			}
			catch (objects::Exception* ex) {
				ex->work();
			}
		}

		template <class T>
		void setItem(int index, T value) {
			objects::Object obj = objects::Object();
			obj.set<T>(value);
			setItem(index, obj);
		}

		~Array() {
			delete[] arr;
			delete[] keys;
		}

	private :
		ArrItem* arr;
		void** keys;
		int length;
	};
}

#endif
