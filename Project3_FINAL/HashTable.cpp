#include "HashTable.h"

HashTable::HashTable(Wine::Properties _hashBy)
{
	hashBy = _hashBy; //create hashtable with appropriate table size

	switch (hashBy) {
	case Wine::Properties::VARIETY:
		getHashedValue = &Wine::getVariety;
		tableSize = 1399;
		break;
	case Wine::Properties::COUNTRY:
		getHashedValue = &Wine::getCountry;
		tableSize = 89;
		break;
	case Wine::Properties::TITLE:
		getHashedValue = &Wine::getTitle;
		tableSize = 237619;
		break;
	case Wine::Properties::PROVINCE:
		getHashedValue = &Wine::getProvince;
		tableSize = 853;
		break;
	default:
		getHashedValue = &Wine::getTitle;
		tableSize = 237619;
	}
	hashTable.resize(tableSize, nullptr);
}

HashTable::HashTable(int _numData, Wine::Properties _hashBy)
{
	tableSize = _numData * 2;
	hashTable.resize(tableSize, nullptr);
	hashBy = _hashBy;
	switch (hashBy) {
	case Wine::Properties::VARIETY:
		getHashedValue = &Wine::getVariety;
		break;
	case Wine::Properties::COUNTRY:
		getHashedValue = &Wine::getCountry;
		break;
	case Wine::Properties::TITLE:
		getHashedValue = &Wine::getTitle;
		break;
	case Wine::Properties::PROVINCE:
		getHashedValue = &Wine::getProvince;
	default:
		getHashedValue = &Wine::getTitle;
	}
}

HashTable::~HashTable()
{
	for (HTNode* node : hashTable) {
		if (node != nullptr) {
			HTNode* chain = node->next;
			while (chain != nullptr) {
				HTNode* current = chain;
				chain = chain->next;
				delete current;
			}
			delete node;
		}
	}
}

int HashTable::hashFunction(string key)
{
	//Removes spaces from string to prevent overflow. 
	if (key.size() > 30)
		key.resize(30);

	char* str = &key[0];

	unsigned long long hash = 5381;
	int c;

	hash = ((hash << 5) + hash);
	while (c = *str++) {
		hash = ((hash << 5) + hash) + c;
	}
	return (hash % tableSize);
}

void HashTable::insert(Wine* data)
{
	//Converts key to index.
	string valueToBeHashed = (data->*getHashedValue)();
	unsigned int index = hashFunction(valueToBeHashed);

	//find open address for newNode
	for (; hashTable[index] != nullptr; index = (index + 1) % tableSize) {
		if ((hashTable[index]->data->*getHashedValue)() == valueToBeHashed)
			break;
	}
	HTNode* newNode = new HTNode(data, hashTable[index]);
	hashTable[index] = newNode;
}

void HashTable::search(string searchKey, vector<Wine*>& results)
{
	//Converts into the appropriate index it'll be located at.
	unsigned int index = hashFunction(searchKey);

	for (; hashTable[index] != nullptr; index = (index + 1) % tableSize) {
		if ((hashTable[index]->data->*getHashedValue)() == searchKey) {
			HTNode* temp = hashTable[index];
			while (temp != nullptr) {
				results.push_back(temp->data);
				temp = temp->next;
			}
			return;
		}
	}
}