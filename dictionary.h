#include "exceptions.h"
#include "enumerable.h"

#ifndef DICTIONARY
#define DICTIONARY

namespace enumerable {
	void* nullPtr = new int[0];

	template <int size>
	class CharSet {
	public:
		char value[size];
		CharSet(char* source) {
			for (int i = 0; i < size; i++)
			{
				value[i] = source[i];
			}
		}

		CharSet(){}
	};

	class KeyException : public objects::Exception {
	public:
		KeyException() {}
		KeyException(char* str) {
			message = str;
		}
	};

	template <class keyType, class valueType>
	class KeyValuePair {
	public:
		bool operator > (KeyValuePair<keyType, valueType> a) {
			return(isEqual(a) == 1);
		}

		bool operator < (KeyValuePair<keyType, valueType> a) {
			return(isEqual(a) == -1);
		}

		bool operator == (KeyValuePair<keyType, valueType> a) {
			return(isEqual(a) == 0);
		}

		KeyValuePair(keyType k, valueType v) {
			this->key = k;
			this->value = v;
		}

		keyType getKey() {
			return(key);
		}

		valueType Value() {
			return(value);
		}

		void setValue(valueType val) {
			value = val;
		}

		KeyValuePair(){}

		KeyValuePair* right;
		KeyValuePair* left;
		KeyValuePair* parent;
		bool deleted = false;
	protected:
		keyType key;
		valueType value;
		int isEqual(KeyValuePair<keyType, valueType> a) {
			int len = sizeof(keyType);
			char* meAsChar = (char*)& key;
			char* aAsChar = (char*)& a.key;

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

	template <class keyType, class valueType>
	class Dictionary {
	public:
		Dictionary(keyType* keys, valueType* values, int len) {
			KeyValuePair<keyType, valueType>* kvpArray = new KeyValuePair<keyType, valueType>[len];
			for (int i = 0; i < len; i++)
			{
				kvpArray[i] = KeyValuePair<keyType, valueType>(keys[i], values[i]);
			}
			*this = Dictionary(kvpArray, len);
		}

		Dictionary(){
			head = new KeyValuePair<keyType, valueType>(keyType(), valueType());
			head->parent = (KeyValuePair<keyType, valueType>*)nullPtr;
			head->right = (KeyValuePair<keyType, valueType>*)nullPtr;
			head->left = (KeyValuePair<keyType, valueType>*)nullPtr;
		}

		int remove(keyType key) {
			KeyValuePair<keyType, valueType> cur = KeyValuePair<keyType, valueType>(key, valueType());
			KeyValuePair<keyType, valueType>* c = head;
			try {
				while (c != nullPtr) {
					if (cur == *c) {
						c->deleted = true;
						return(0);
					}
					if (cur > * c) {
						c = c->right;
						continue;
					}
					if (cur < *c) {
						c = c->left;
						continue;
					}
				}
				throw(&KeyException((char*)"There is no that key in the dictionary"));
			}
			catch (objects::Exception* ex) {
				ex->work();
				return(1);
			}
		}
		
		//returns 0 if success 1 if exception
		int set(KeyValuePair<keyType, valueType>* New) {
			KeyValuePair<keyType, valueType>* c = head;
			try {
				while (c != nullPtr) {
					if (*New == *c) {
						c->deleted = false;
						c->setValue(New->Value());
					}
					if (*New > * c) {
						if (c->right == (KeyValuePair<keyType, valueType>*)nullPtr) {
							c->right = New;
							New->parent = c;
							New->right = (KeyValuePair<keyType, valueType>*)nullPtr;
							New->left = (KeyValuePair<keyType, valueType>*)nullPtr;
							return(0);
						}
						c = c->right;
						continue;
					}
					if (*New < *c) {
						if (c->left == (KeyValuePair<keyType, valueType>*)nullPtr) {
							c->left = New;
							New->parent = c;
							New->right = (KeyValuePair<keyType, valueType>*)nullPtr;
							New->left = (KeyValuePair<keyType, valueType>*)nullPtr;
							return(0);
						}
						c = c->left;
						continue;
					}
				}
				throw(&KeyException((char*)"Some key Exception caused"));
			}
			catch (objects::Exception* ex) {
				ex->work();
				return(1);
			}
		}

		int set(keyType key, valueType value) {
			KeyValuePair<keyType, valueType>* New = new KeyValuePair<keyType, valueType>(key, value);
			set(New);
		}

		valueType get(keyType key) {
			KeyValuePair<keyType, valueType> cur = KeyValuePair<keyType, valueType>(key, valueType());
			KeyValuePair<keyType, valueType>* c = head;
			try {
				while (c != nullPtr) {
					if (cur == *c) {
						if (!c->deleted) {
							return(c->Value());
						}
						throw(&KeyException((char*)"This item was removed from dictionary"));
					}
					if (cur > * c) {
						c = c->right;
						continue;
					}
					if (cur < *c) {
						c = c->left;
						continue;
					}
				}
				throw(&KeyException((char*)"There is no that key in the dictionary"));
			}
			catch (objects::Exception* ex) {
				ex->work();;
			}
		}
	protected:
		Dictionary(KeyValuePair<keyType, valueType>* kvpArray, int len) {
			size = len;
			sortByKeys(kvpArray, len);
			head = makeTree(kvpArray, len);
		}

		int size;
		KeyValuePair<keyType, valueType>* head;

		void sortByKeys(KeyValuePair<keyType, valueType>* kvpArray, int len) {
			//KeyValuePair<keyType, valueType>* nArray = new KeyValuePair<keyType, valueType>[len];
			for (int i = 0; i < len; i++)
			{
				for (int j = 0; j < len-i-1; j++)
				{
					if (kvpArray[j] > kvpArray[j + 1]) {
						KeyValuePair<keyType, valueType> tmp = kvpArray[j];
						kvpArray[j] = kvpArray[j + 1];
						kvpArray[j + 1] = tmp;
					}
				}
			}
		}

		KeyValuePair<keyType, valueType>* makeTree(KeyValuePair<keyType, valueType>* kvpArray, int len) {
			int headIdx = len / 2;
			if (len == 0) {
				return((KeyValuePair<keyType, valueType>*)nullPtr);
			}
			if (len == 1) {
				kvpArray[0].right = (KeyValuePair<keyType, valueType>*)nullPtr;
				kvpArray[0].left = (KeyValuePair<keyType, valueType>*)nullPtr;
				return(kvpArray);
			}
			if (len == 2) {
				kvpArray[1].left = makeTree(kvpArray, 1);
				kvpArray[0].parent = &kvpArray[1];
				kvpArray[1].right = (KeyValuePair<keyType, valueType>*)nullPtr;
				return(&kvpArray[headIdx]);
			}

			KeyValuePair<keyType, valueType>* h = &kvpArray[headIdx];
			h->left = makeTree(kvpArray, headIdx);
			h->left->parent = h;
			h->right = makeTree(&kvpArray[headIdx + 1], len - headIdx - 1);
			h->right->parent = h;
			return(h);
		}
	};

	class SuperKVPair : public KeyValuePair<objects::Object, objects::Object>, public IItem {
	public:
		void operator = (SuperKVPair a) {
			this->deleted = a.deleted;
			this->key = a.key;
			this->left = a.left;
			this->parent = a.parent;
			this->right = a.right;
			this->superKey = a.superKey;
			this->value = a.value;
		}

		bool operator == (SuperKVPair a) {
			return(this->key == a.key);
		}
		bool operator > (SuperKVPair a) {
			return(this->key > a.key);
		}
		bool operator < (SuperKVPair a) {
			return(this->key < a.key);
		}

		SuperKVPair(objects::Object k, objects::Object v) {
			this->key = k;
			this->value = v;
		}

		objects::Object Value() {
			return(value);
		}

		void setValue(objects::Object val, void* sender) {
			try {
				if (sender == superKey) {
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
			superKey = malloc(1);
			return(superKey);
		}

		objects::Object getValue() {
			return(value);
		}
		void* superKey;
		SuperKVPair(){}
	private:
		
	};

	class SuperDictionary : public IEnumerable, public Dictionary<objects::Object, objects::Object> {
	public:
		void operator = (SuperDictionary a) {
			this->head = a.head;
			this->size = a.getSize();
		}

		SuperDictionary(objects::Object* keys, objects::Object* values, int len) {
			SuperKVPair* kvpArray = new SuperKVPair[len];
			for (int i = 0; i < len; i++)
			{
				kvpArray[i] = SuperKVPair(keys[i], values[i]);
				kvpArray[i]._init();
			}
			MakeSuperDictionary(kvpArray, len);
		}

		SuperDictionary() {
			head = new SuperKVPair(objects::Object(), objects::Object());
			head->parent = (SuperKVPair*)nullPtr;
			head->right = (SuperKVPair*)nullPtr;
			head->left = (SuperKVPair*)nullPtr;
		}

		objects::Object get(objects::Object key) {
			SuperKVPair cur = SuperKVPair(key, objects::Object());
			SuperKVPair* c = head;
			try {
				while (c != nullPtr) {
					if (cur == *c) {
						if (!c->deleted) {
							return(c->Value());
						}
						throw(&KeyException((char*)"This item was removed from dictionary"));
					}
					if (cur > * c) {
						c = (SuperKVPair*)c->right;
						continue;
					}
					if (cur < *c) {
						c = (SuperKVPair*)c->left;
						continue;
					}
				}
				throw(&KeyException((char*)"There is no that key in the dictionary"));
			}
			catch (objects::Exception* ex) {
				ex->work();;
			}
		}

		int set(SuperKVPair* New) {
			SuperKVPair* c = head;
			try {
				while (c != nullPtr) {
					if (*New == *c) {
						c->deleted = false;
						c->setValue(New->Value(), c->superKey);
					}
					if (*New > * c) {
						if (c->right == (SuperKVPair*)nullPtr) {
							c->right = New;
							New->parent = c;
							New->right = (SuperKVPair*)nullPtr;
							New->left = (SuperKVPair*)nullPtr;
							size++;
							return(0);

						}
						c = (SuperKVPair*)c->right;
						continue;
					}
					if (*New < *c) {
						if (c->left == (SuperKVPair*)nullPtr) {
							c->left = New;
							New->parent = c;
							New->right = (SuperKVPair*)nullPtr;
							New->left = (SuperKVPair*)nullPtr;
							size++;
							return(0);
						}
						c = (SuperKVPair*)c->left;
						continue;
					}
				}
				throw(&KeyException((char*)"Some key Exception caused"));
			}
			catch (objects::Exception* ex) {
				ex->work();
				return(1);
			}
		}

		int set(objects::Object key, objects::Object value) {
			SuperKVPair* New = new SuperKVPair(key, value);
			return(set(New));
		}

		int remove(objects::Object key) {
			SuperKVPair cur = SuperKVPair(key, objects::Object());
			SuperKVPair* c = head;
			try {
				while (c != nullPtr) {
					if (cur == *c) {
						c->deleted = true;
						size--;
						return(0);
					}
					if (cur > * c) {
						c = (SuperKVPair*)c->right;
						continue;
					}
					if (cur < *c) {
						c = (SuperKVPair*)c->left;
						continue;
					}
				}
				throw(&KeyException((char*)"There is no that key in the dictionary"));
			}
			catch (objects::Exception* ex) {
				ex->work();
				return(1);
			}
		}

		template <class T>
		objects::Object get(T key) {
			objects::Object keyObj = objects::Object();
			keyObj.set<T>(key);
			return(get(keyObj));
		}

		template <class keyT, class valT>
		valT get(keyT key) {
			objects::Object valObj = get<keyT>(key);
			return(valObj.get<valT>());
		}

		template <class keyT, class valT>
		int set(keyT key, valT value) {
			objects::Object keyObj = objects::Object();
			keyObj.set<keyT>(key);

			objects::Object valObj = objects::Object();
			valObj.set<valT>(value);

			return(set(keyObj, valObj));
		}

		template <class keyT>
		int remove(keyT key) {
			objects::Object keyObject = objects::Object();
			keyObject.set<keyT>(key);
			return(remove(keyObject));
		}

		IItem* getNext(IItem* current, int index, int hash){
			SuperKVPair* cur = (SuperKVPair*)current;
			if (cur->right != (SuperKVPair*)nullPtr) {
				if (((SuperKVPair*)(cur->right))->deleted) {
					return(getNext((SuperKVPair*)cur->right, index, hash));
				}
				return((SuperKVPair*)cur->right);
			}

			if (cur == head) {
				SuperKVPair* res = new SuperKVPair(nullObject, nullObject);
				res->left = (SuperKVPair*)nullPtr;
				res->right = (SuperKVPair*)nullPtr;
				res->parent = (SuperKVPair*)nullPtr;
				return(res);
			}
			if (cur->parent == (SuperKVPair*)nullPtr) {
				SuperKVPair* res = new SuperKVPair(nullObject, nullObject);
				res->left = (SuperKVPair*)nullPtr;
				res->right = (SuperKVPair*)nullPtr;
				res->parent = (SuperKVPair*)nullPtr;
				return(res);
			}

			if (cur->parent->left == cur) {
				if (cur->parent->deleted) {
					return(getNext((SuperKVPair*)cur->parent, hash, index));
				}
				return((SuperKVPair*)cur->parent);
			}

			SuperKVPair* par = (SuperKVPair*)cur->parent;
			bool flag = false;
			while (par != head) {
				if (par->parent->left == par) {
					flag = true;
					break;
				}
				par = (SuperKVPair*)par->parent;
			}
			if (!flag) {
				SuperKVPair* res = new SuperKVPair(nullObject, nullObject);
				res->left = (SuperKVPair*)nullPtr;
				res->right = (SuperKVPair*)nullPtr;
				res->parent = (SuperKVPair*)nullPtr;
				return(res);
			}
			if (par->deleted) {
				return(getNext(par, index, hash));
			}
			return(par);
		}
		
		int getNextIndex(IItem* current, int index, int hash) {
			return(0);
		}
		
		int getNextHash(IItem* current, int index, int hash) {
			return(0);
		}
		
		IItem* getFirst(){
			SuperKVPair* d = head;
			while (d->left != (SuperKVPair*)nullPtr) {
				d = (SuperKVPair*)d->left;
			}
			return(d);
		}

		int getFirstIndex(){
			return(0);
		}

		int getFirstHash(){
			return(0);
		}

		int getSize() {
			return(size);
		}
	private:
		SuperKVPair* head;

		void MakeSuperDictionary(SuperKVPair* skvpArr, int len) {
			size = len;
			sortByKeys(skvpArr, len);
			head = (SuperKVPair*)makeTree(skvpArr, len);
		}

		void sortByKeys(SuperKVPair* skvpArr, int len) {
			for (int i = 0; i < len; i++)
			{
				for (int j = 0; j < len - i - 1; j++)
				{
					if (skvpArr[j] > skvpArr[j + 1]) {
						SuperKVPair tmp = skvpArr[j];
						skvpArr[j] = skvpArr[j + 1];
						skvpArr[j + 1] = tmp;
					}
				}
			}
		}

		SuperKVPair* makeTree(SuperKVPair* skvpArr, int len) {
			int headIdx = len / 2;
			if (len == 0) {
				return((SuperKVPair*)nullPtr);
			}
			if (len == 1) {
				skvpArr[0].right = (SuperKVPair*)nullPtr;
				skvpArr[0].left = (SuperKVPair*)nullPtr;
				return(skvpArr);
			}
			if (len == 2) {
				skvpArr[1].left = makeTree(skvpArr, 1);
				skvpArr[0].parent = &skvpArr[1];
				skvpArr[1].right = (SuperKVPair*)nullPtr;
				return(&skvpArr[headIdx]);
			}

			SuperKVPair* h = &skvpArr[headIdx];
			h->left = makeTree(skvpArr, headIdx);
			h->left->parent = h;
			h->right = makeTree(&skvpArr[headIdx + 1], len - headIdx - 1);
			h->right->parent = h;
			return(h);
		}

	};

}

#endif
