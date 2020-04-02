#include "enumerable.h"
#include "exceptions.h"
#include "dictionary.h"
#include <iostream>

#ifndef LIST
#define LIST

namespace enumerable {

	template <class T>
	class ListItem : public IItem {
	public:
		ListItem(T val) {
			next = (ListItem<T>*)nullPtr;
			prev = (ListItem<T>*)nullPtr;
			valueObj.set<T>(val);
		}



		ListItem(T val, ListItem<T>* next, ListItem<T>* prev) {
			this->next = next;
			this->prev = prev;
			valueObj.set<T>(val);
		}

		ListItem(objects::Object valObj) {
			next = (ListItem<T>*)nullPtr;
			prev = (ListItem<T>*)nullPtr;
			valueObj = valObj;
		}

		ListItem(objects::Object valObj, ListItem<T>* next, ListItem<T>* prev) {
			this->next = next;
			this->prev = prev;
			valueObj = valObj;
		}

		ListItem<T>* getNext() {
			return(next);
		}

		ListItem<T>* getPrev() {
			return(prev);
		}

		T Value() {
			return(valueObj.get<T>());
		}

		objects::Object getValue() {
			return(valueObj);
		}

		void* _init() {
			rightsender = malloc(1);
			return(rightsender);
		}

		void _init(void* rightsender) {
			this->rightsender = rightsender;
		}


		int setValue(objects::Object val, void* sender) {
			try {
				if (sender == rightsender) {
					this->valueObj = val;
					return(0);
				}
				else {
					//std::cout << "there" << std::endl;
					throw(&PrivacyException((char*)"Current sender has no rights to change this item!"));
				}
			}
			catch (objects::Exception* ex) {
				//std::cout << "there1" << std::endl;
				ex->work();
				return(1);
				//std::cout << "there3" << std::endl;
			}
		}


		int setValue(T val, void* sender) {
			objects::Object valObj = objects::Object();
			valObj.set<T>(val);
			return(setValue(valObj, sender));
		}

		int setPrev(ListItem<T>* prev, void* sender) {
			try {
				if (sender == rightsender) {
					this->prev = prev;
					return(0);
				}
				else {
					//std::cout << "there" << std::endl;
					throw(&PrivacyException((char*)"Current sender has no rights to change this item!"));
				}
			}
			catch (objects::Exception* ex) {
				//std::cout << "there1" << std::endl;
				ex->work();
				return(1);
				//std::cout << "there3" << std::endl;
			}
		}

		int setNext(ListItem<T>* next, void* sender) {
			try {
				if (sender == rightsender) {
					this->next = next;
					return(0);
				}
				else {
					//std::cout << "there" << std::endl;
					throw(&PrivacyException((char*)"Current sender has no rights to change this item!"));
				}
			}
			catch (objects::Exception* ex) {
				//std::cout << "there1" << std::endl;
				ex->work();
				return(1);
				//std::cout << "there3" << std::endl;
			}
		}

	protected:
		void* rightsender;
		ListItem<T>* next;
		ListItem<T>* prev;
		objects::Object valueObj;
	};

	template <class T>
	class SpeciaListlItem : public SpecialItem {
	public:
		
		SpeciaListlItem(ListItem<T>* item, void* key) {
			this->item = item;
			this->key = key;
		}

		T value() {
			return(item->getValue().get<T>());
		}

		operator T(){
			return(value());
		}

		void operator = (T value) {
			((ListItem<T>*)(item))->setValue(value, key);
		}

		bool operator == (T a) {
			return(value() == a);
		}

		bool operator != (T a) {
			return(!(*this == a));
		}

		bool operator > (T a) {
			return(value() > a);
		}

		bool operator < (T a) {
			return(value() < a);
		}

		bool operator >= (T a) {
			return(value() >= a);
		}

		bool operator <= (T a) {
			return(value() <= a);
		}
	};

	template <class T>
	class List : public IEnumerable {
	public:
		SpeciaListlItem<T> operator [] (int index) {
			try {
				ListItem<T>* current = _get(index);
				if (current != (ListItem<T>*)nullPtr) {
					return(SpeciaListlItem<T>(current, (void*)this));
				}
				else {
					throw new IndexException((char*)"Index out of range exception!");
				}
			}
			catch (objects::Exception * ex) {
				throw ex;
			}
		}

		List() {
			first = new ListItem<T>(T(), (ListItem<T>*)nullPtr, (ListItem<T>*)nullPtr);
			first->_init((void*)this);
			last = first;
			length = 0;
		}

		void append(T value) {
			ListItem<T>* NewItem = new ListItem<T>(value, (ListItem<T>*)nullPtr, last);
			length++;
			last->setNext(NewItem, (void*)this);
			NewItem->_init((void*)this);
			last = NewItem;
		}

		ListItem<T>* _get(int index) {
			//std::cout << "index " << index << std::endl << "size : " << length << std::endl;
			try {
				if (index < length && index > -1) {
					ListItem<T>* current = first->getNext();
					for (int i = 0; i < index; i++)
					{
						//std::cout << "i " << i << std::endl;
						current = current->getNext();
					}
					return(current);
				}
				else {
					throw new IndexException((char*)"Index out of range exception");
				}
			}
			catch (objects::Exception* ex) {
				throw ex;
				return((ListItem<T>*)nullPtr);
			}
		}

		void set(int index, T value) {
			try {
				ListItem<T>* current = _get(index);
				if (current != (ListItem<T>*)nullPtr) {
					current->setValue(value, (void*)this);
				}
				else {
					throw new IndexException((char*)"Index out of range exception!");
				}
			}
			catch (objects::Exception* ex) {
				throw ex;
			}
		}

		T get(int index) {
			try {
				ListItem<T>* current = _get(index);
				if (current != (ListItem<T>*)nullPtr) {
					return(current->getValue().get<T>());
				}
				else {
					throw new IndexException((char*)"Index out of range exception!");
				}
			}
			catch (objects::Exception * ex) {
				throw ex;
			}
		}

		void pop(int index) {
			try {
				ListItem<T>* current = _get(index);
				if (current != (ListItem<T>*)nullPtr) {
					current->getNext()->setPrev(current->getPrev(), (void*)this);
					current->getPrev()->setNext(current->getNext(), (void*)this);
					delete current;
					length--;
				}
				else {
					throw new IndexException((char*)"Index out of range exception!");
				}
			}
			catch (objects::Exception* ex) {
				throw ex;
			}
		}

		int getFirstIndex() {
			return(0);
		}

		int getFirstHash() {
			return(0);
		}

		ListItem<T>* getFirst() {
			if (first->getNext() != (ListItem<T>*)nullPtr) {
				return(first->getNext());
			}
			return(new ListItem<T>(nullObject, (ListItem<T>*)nullPtr, (ListItem<T>*)nullPtr));
		}

		ListItem<T>* getNext(IItem* current, int index, int hash) {
			ListItem<T>* cur = (ListItem<T>*)(current);
			if (cur->getNext() != (ListItem<T>*)nullPtr) {
				return(cur->getNext());
			}
			return(new ListItem<T>(nullObject, (ListItem<T>*)nullPtr, (ListItem<T>*)nullPtr));
		}

		int getNextIndex(IItem* current, int index, int hash) {
			return(index + 1);
		}

		int getNextHash(IItem* current, int index, int hash) {
			return(0);
		}

		int size() {
			return(length);
		}

	protected:
		ListItem<T>* first;
		ListItem<T>* last;
		int length;
	};
}

#endif
