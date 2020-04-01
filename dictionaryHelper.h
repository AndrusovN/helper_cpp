#include "dictionary.h"

#ifndef DICTIONARY_HELPER
#define DICTIONARY_HELPER

namespace enumerable {
	void reconstructDictionary(SuperDictionary* dict) {
		int size = dict->getSize();
		objects::Object* keys = new objects::Object[size];
		objects::Object* values = new objects::Object[size];
		ItemEnumerator myEnumerator(dict);
		int n = 0;
		for (SuperKVPair* i = (SuperKVPair*)myEnumerator.next(); i->Value() != nullObject; i = (SuperKVPair*)myEnumerator.next()) {
			keys[n] = i->getKey();
			values[n] = i->getValue();
			n++;
			
		}

		*dict = SuperDictionary(keys, values, size);

	}
}

#endif
