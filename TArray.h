#include "enumerable.h"
#include "exceptions.h"
#include "List.h"
#include "dictionary.h"

#ifndef TYPED_ARRAY
#define TYPED_ARRAY

namespace enumerable {
	template <class T>
	class TArrItem : public IItem {
	public:
		TArrItem() {}

		void operator = (TArrItem<T> a) {
			this->rightsender = a.rightsender;
			this->valObj = a.valObj;
		}

		TArrItem(objects::Object value, bool isNull) {
			if (isNull) {
				valObj = value;
			}
			else {
				try {
					value.get<T>();
				}
				catch (objects::Exception* ex) {
					throw ex;
				}
				valObj = value;
			}
		}

		TArrItem(T value) {
			valObj = objects::Object();
			valObj.set<T>(value);
		}

		void* _init() {
			rightsender = malloc(1);
			return(rightsender);
		}

		void _init(void* rightsender) {
			this->rightsender = rightsender;
		}

		objects::Object getValue() {
			return(valObj);
		}

		int setValue(objects::Object val, void* sender) {
			try {
				if (sender == rightsender) {
					try {
						val.get<T>();
					}
					catch (objects::Exception* ex) {
						throw ex;
						return(2);
					}
					valObj = val;
					return(0);
				}
				else {
					throw(&PrivacyException((char*)"Current sender has no rights to change this item!"));
				}
			}
			catch (objects::Exception* ex) {
				throw ex;
				return(1);
			}
		}
	private:
		objects::Object valObj;
		void* rightsender;

	};

	template <class T>
	class TArray : public IEnumerable {
	public:
		SpeciaListlItem<T> operator [] (int index) {
			try {
				if (index < size && index > -1) {
					return(SpeciaListlItem<T>((ListItem<T>*)(&arr[index]), (void*)this));
				}
				else {
					throw& IndexException((char*)"Index out of range exception!");
				}
			}
			catch (objects::Exception* ex) {
				throw ex;
			}

		}

		int length() {
			return(size);
		}

		TArray(int len) {
			size = len;
			arr = new TArrItem<T>[len];
			for (int i = 0; i < size; i++)
			{
				arr[i] = TArrItem<T>();
				arr[i]._init((void*)this);
			}
		}

		TArray(T* arr, int len) {
			size = len;
			this->arr = new TArrItem<T>[len];
			for (int i = 0; i < size; i++)
			{
				this->arr[i] = TArrItem<T>(arr[i]);
				this->arr[i]._init((void*)this);
			}
		}

		int getFirstIndex() { return(0); }
		int getFirstHash() { return(0); }
		TArrItem<T>* getFirst() {
			return(&arr[0]);
		}

		int getNextIndex(IItem* current, int index, int hash) { return(index + 1); }
		int getNextHash(IItem* current, int index, int hash) { return(0); }
		TArrItem<T>* getNext(IItem* current, int index, int hash) {
			if (index > -1 && index < size-1) {
				return(&arr[index + 1]);
			}
			else {
				
				return(new TArrItem<T>(nullObject, true));
			}
		}


	private:
		TArrItem<T>* arr;
		int size;
	};
}

#endif
