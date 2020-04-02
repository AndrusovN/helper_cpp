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
	class KeyValuePair : public IItem {
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

		void operator = (KeyValuePair<keyType, valueType> a) {
			this->deleted = a.deleted;
			this->key = a.key;
			this->keyObj = a.keyObj;
			this->left = a.left;
			this->parent = a.parent;
			this->rightsender = a.rightsender;
			this->right = a.right;
			this->value = a.value;
			this->valueObj = a.valueObj;
		}

		KeyValuePair(keyType k, valueType v) {
			keyObj = objects::Object();
			keyObj.set<keyType>(k);
			valueObj = objects::Object();
			valueObj.set<valueType>(v);
			this->key = k;
			this->value = v;
		}

		KeyValuePair(objects::Object k, objects::Object v, bool isNull) {
			if (isNull) {
				key = keyType();
				value = valueType();
				keyObj = k;
				valueObj = v;
			}
			else {
				try {
					k.get<keyType>();
					v.get<valueType>();

					key = k.get<keyType>();
					value = v.get<valueType>();

					valueObj = v;
					keyObj = k;
				}
				catch (objects::Exception* ex) {
					throw ex;
				}
			}
		}

		keyType getKey() {
			return(key);
		}

		valueType Value() {
			return(value);
		}

		void _setValue(valueType val, void* sender) {
			try {
				if (sender == rightsender) {
					value = val;
					valueObj.set<valueType>(value);
				}
				else {
					throw& PrivacyException((char*)"This sender has no rights to change this item");
				}
			}
			catch (objects::Exception* ex) {
				std::cout << sender << std::endl << rightsender << std::endl;
				throw ex;
			}
			
		}

		void* _init() {
			rightsender = malloc(1);
			return(rightsender);
		}

		void _init(void* rightsender) {
			this->rightsender = rightsender;
		}

		objects::Object getValue() {
			return(valueObj);
		}

		int setValue(objects::Object value, void* sender) {
			try {
				if (sender == rightsender) {
					try {
						value.get<valueType>();
					}
					catch (objects::Exception* ex) {
						throw ex;
						return(2);
					}
					valueObj = value;
					this->value = value.get<valueType>();
					return(0);
				}
				else {
					throw& PrivacyException((char*)"This sender has no rights to change this item");
				}
			}
			catch (objects::Exception * ex) {
				throw ex;
				return(1);
			}
		}

		KeyValuePair(){}
		KeyValuePair* right;
		KeyValuePair* left;
		KeyValuePair* parent;
		bool deleted = false;
	protected:
		void* rightsender;
		objects::Object keyObj;
		keyType key;
		valueType value;
		objects::Object valueObj;
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
	class SpecialDictionaryItem : public SpecialItem {
	public:
		SpecialDictionaryItem(KeyValuePair<keyType, valueType>* item, void* key) {
			this->item = item;
			this->key = key;
		}

		valueType value() {
			return(item->getValue().get<valueType>());
		}

		operator valueType() {
			return(value());
		}

		void operator = (valueType value) {
			item->_setValue(value, key);
		}

		bool operator == (valueType a) {
			return(value() == a);
		}

		bool operator != (valueType a) {
			return(!(*this == a));
		}

		bool operator > (valueType a) {
			return(value() > a);
		}

		bool operator < (valueType a) {
			return(value() < a);
		}

		bool operator >= (valueType a) {
			return(value() >= a);
		}

		bool operator <= (valueType a) {
			return(value() <= a);
		}

	private:
		KeyValuePair<keyType, valueType>* item;
		void* key;
	};

	template <class keyType, class valueType>
	class Dictionary : public IEnumerable{
	public:
		void operator = (Dictionary<keyType, valueType> a) {
			this->size = a.size;
			this->head = a.head;
			this->myKey = a.myKey;
		}

		SpecialDictionaryItem<keyType, valueType> operator [] (keyType index) {
			try {
				return(SpecialDictionaryItem<keyType, valueType>(_get(index), myKey));
			}
			catch (objects::Exception* ex) {
				std::cout << index << std::endl;
				throw ex;
			}
			
		}

		Dictionary(keyType* keys, valueType* values, int len) {
			KeyValuePair<keyType, valueType>* kvpArray = new KeyValuePair<keyType, valueType>[len];
			for (int i = 0; i < len; i++)
			{
				kvpArray[i] = KeyValuePair<keyType, valueType>(keys[i], values[i]);
				kvpArray[i]._init((void*)this);
			}
			
			*this = Dictionary(kvpArray, len);
			myKey = (void*)this;
		}

		Dictionary(){
			head = new KeyValuePair<keyType, valueType>(keyType(), valueType());
			head->parent = (KeyValuePair<keyType, valueType>*)nullPtr;
			head->right = (KeyValuePair<keyType, valueType>*)nullPtr;
			head->left = (KeyValuePair<keyType, valueType>*)nullPtr;
			head->deleted = true;
			size = 0;
			myKey = (void*)this;
		}

		void* getKey() {
			return myKey;
		}

		int remove(keyType key) {
			KeyValuePair<keyType, valueType> cur = KeyValuePair<keyType, valueType>(key, valueType());
			KeyValuePair<keyType, valueType>* c = head;
			try {
				while (c != nullPtr) {
					if (cur == *c) {
						c->deleted = true;
						size--;
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
				throw ex;
				//ex->work();
				return(1);
			}
		}
		
		//returns 0 if success 1 if exception
		int set(KeyValuePair<keyType, valueType>* New) {
			KeyValuePair<keyType, valueType>* c = head;
			try {
				while (c != (KeyValuePair<keyType, valueType>*)nullPtr) {
					if (*New == *c) {
						c->deleted = false;
						size++;
						c->_setValue(New->Value(), myKey);
						return(0);
					}
					if (*New > * c) {
						if (c->right == (KeyValuePair<keyType, valueType>*)nullPtr) {
							c->right = New;
							New->parent = c;
							New->right = (KeyValuePair<keyType, valueType>*)nullPtr;
							New->left = (KeyValuePair<keyType, valueType>*)nullPtr;
							size++;
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
							size++;
							return(0);
						}
						c = c->left;
						continue;
					}
				}
				throw(&KeyException((char*)"Some key Exception caused"));
			}
			catch (objects::Exception* ex) {
				throw ex;
				//ex->work();
				return(1);
			}
		}

		int set(keyType key, valueType value) {
			KeyValuePair<keyType, valueType>* New = new KeyValuePair<keyType, valueType>(key, value);
			New->_init(myKey);
			return(set(New));
		}

		valueType get(keyType key) {
			try {
				return(_get(key)->getValue().get<valueType>());
			}
			catch (objects::Exception* ex) {
				throw ex;
			}
		}

		int getSize() {
			return(size);
		}

		int getFirstIndex() { return(0); }
		int getFirstHash() { return(0); }
		KeyValuePair<keyType, valueType>* getFirst() {
			KeyValuePair<keyType, valueType>* d = head;
			while (d->left != (KeyValuePair<keyType, valueType>*)nullPtr) {
				d = (KeyValuePair<keyType, valueType>*)d->left;
			}
			if (d->deleted) {
				return(getNext(d, 0, 0));
			}
			return(d);

		}

		int getNextIndex(IItem* current, int index, int hash) { return(index + 1); }
		int getNextHash(IItem* current, int index, int hash) { return(0); }
		KeyValuePair<keyType, valueType>* getNext(IItem* current, int index, int hash) {
			KeyValuePair<keyType, valueType>* cur = (KeyValuePair<keyType, valueType>*)current;
			if (cur->right != (KeyValuePair<keyType, valueType>*)nullPtr) {
				/*if (((KeyValuePair<keyType, valueType>*)(cur->right))->deleted) {
					return(getNext((KeyValuePair<keyType, valueType>*)cur->right, index, hash));
				}
				return((KeyValuePair<keyType, valueType>*)cur->right);*/
				KeyValuePair<keyType, valueType>* d = cur->right;
				while (d->left != (KeyValuePair<keyType, valueType>*)nullPtr) {
					d = (KeyValuePair<keyType, valueType>*)d->left;
				}
				if (d->deleted) {
					return(getNext(d, index, hash));
				}
				return(d);
			}

			if (cur == head) {
				KeyValuePair<keyType, valueType>* res = new KeyValuePair<keyType, valueType>(nullObject, nullObject, true);
				res->left = (KeyValuePair<keyType, valueType>*)nullPtr;
				res->right = (KeyValuePair<keyType, valueType>*)nullPtr;
				res->parent = (KeyValuePair<keyType, valueType>*)nullPtr;
				return(res);
			}
			if (cur->parent == (KeyValuePair<keyType, valueType>*)nullPtr) {
				KeyValuePair<keyType, valueType>* res = new KeyValuePair<keyType, valueType>(nullObject, nullObject, true);
				res->left = (KeyValuePair<keyType, valueType>*)nullPtr;
				res->right = (KeyValuePair<keyType, valueType>*)nullPtr;
				res->parent = (KeyValuePair<keyType, valueType>*)nullPtr;
				return(res);
			}

			if (cur->parent->left == cur) {
				if (cur->parent->deleted) {
					return(getNext((KeyValuePair<keyType, valueType>*)cur->parent, hash, index));
				}
				return((KeyValuePair<keyType, valueType>*)cur->parent);
			}

			KeyValuePair<keyType, valueType>* par = (KeyValuePair<keyType, valueType>*)cur->parent;
			bool flag = false;
			while (par != head) {
				if (par->parent->left == par) {
					flag = true;
					break;
				}
				par = (KeyValuePair<keyType, valueType>*)par->parent;
			}
			if (!flag) {
				KeyValuePair<keyType, valueType>* res = new KeyValuePair<keyType, valueType>(nullObject, nullObject, true);
				res->left = (KeyValuePair<keyType, valueType>*)nullPtr;
				res->right = (KeyValuePair<keyType, valueType>*)nullPtr;
				res->parent = (KeyValuePair<keyType, valueType>*)nullPtr;
				return(res);
			}
			if (par->deleted) {
				return(getNext(par, index, hash));
			}
			return(par);
		}

	protected:

		KeyValuePair<keyType, valueType>* _get(keyType key) {
			KeyValuePair<keyType, valueType> cur = KeyValuePair<keyType, valueType>(key, valueType());
			KeyValuePair<keyType, valueType>* c = head;
			try {
				while (c != nullPtr) {
					if (cur == *c) {
						if (!c->deleted) {
							return(c);
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
				throw ex;
				//ex->work();;
			}
		}

		Dictionary(KeyValuePair<keyType, valueType>* kvpArray, int len) {
			size = len;
			sortByKeys(kvpArray, len);
			head = makeTree(kvpArray, len);
		}

		int size;
		KeyValuePair<keyType, valueType>* head;
		void* myKey;
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

	class SuperKVPair : public KeyValuePair<objects::Object, objects::Object> {
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

		int setValue(objects::Object val, void* sender) {
			try {
				if (sender == superKey) {
					value = val;
					return(0);
				}
				else {
					//std::cout << "there" << std::endl;
					throw(&PrivacyException((char*)"Current sender has no rights to change this item!"));
				}
			}
			catch (objects::Exception* ex) {
				//std::cout << "there1" << std::endl;
				throw ex;
				//ex->work();
				return(1);
				//std::cout << "there3" << std::endl;
			}
		}

		void* _init() {
			superKey = malloc(1);
			return(superKey);
		}

		void _init(void* rightsender) {
			this->superKey = rightsender;
		}

		objects::Object getValue() {
			return(value);
		}
		SuperKVPair(){}
	private:
		void* superKey;
	};

	class SuperDictionary : public IEnumerable, public Dictionary<objects::Object, objects::Object> {
	public:
		void operator = (SuperDictionary a) {
			this->head = a.head;
			this->size = a.getSize();
			this->myKey = a.myKey;
		}

		SpecialItem operator [] (objects::Object key) {
			return(SpecialItem(_get(key), myKey));
		}

		SuperDictionary(objects::Object* keys, objects::Object* values, int len) {
			SuperKVPair* kvpArray = new SuperKVPair[len];
			for (int i = 0; i < len; i++)
			{
				kvpArray[i] = SuperKVPair(keys[i], values[i]);
				kvpArray[i]._init(myKey);
			}
			MakeSuperDictionary(kvpArray, len);
			myKey = (void*)this;
		}

		SuperDictionary() {
			head = new SuperKVPair(objects::Object(), objects::Object());
			head->parent = (SuperKVPair*)nullPtr;
			head->right = (SuperKVPair*)nullPtr;
			head->left = (SuperKVPair*)nullPtr;
			myKey = (void*)this;
		}

		objects::Object get(objects::Object key) {
			return(_get(key)->getValue());
		}

		int set(SuperKVPair* New) {
			SuperKVPair* c = head;
			try {
				while (c != nullPtr) {
					if (*New == *c) {
						c->deleted = false;
						c->setValue(New->Value(), myKey);
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
				throw ex;
				//ex->work();
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
				throw ex;
				//ex->work();
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

		SuperKVPair* getNext(IItem* current, int index, int hash){
			SuperKVPair* cur = (SuperKVPair*)current;
			if (cur->right != (SuperKVPair*)nullPtr) {
				/*if (((SuperKVPair*)(cur->right))->deleted) {
					return(getNext((SuperKVPair*)cur->right, index, hash));
				}
				return((SuperKVPair*)cur->right);*/
				SuperKVPair* d = (SuperKVPair*)cur->right;
				while (d->left != (SuperKVPair*)nullPtr) {
					d = (SuperKVPair*)d->left;
				}
				return(d);
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
			return(index + 1);
		}
		
		int getNextHash(IItem* current, int index, int hash) {
			return(0);
		}
		
		SuperKVPair* getFirst(){
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

		SuperKVPair* _get(objects::Object key) {
			SuperKVPair cur = SuperKVPair(key, objects::Object());
			SuperKVPair* c = head;
			try {
				while (c != nullPtr) {
					if (cur == *c) {
						if (!c->deleted) {
							return(c);
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
				throw ex;
				//ex->work();;
			}
		}


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
