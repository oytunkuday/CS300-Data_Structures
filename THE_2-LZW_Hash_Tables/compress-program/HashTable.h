#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#pragma once

#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct pairs {//creating a struct inside header that has code and key
    int code;
    string key;
    pairs(const int& codepara = 0, const string& keypara = "") :code(codepara), key(keypara) {}//creating a new constructor to assign values easier.

    bool  operator == (const pairs& rhs) const//overloading == operator such that it does the necessary checks inside the struct when we don't have an exact code but when we have the key
    {
        return (this->key==rhs.key || this->code == rhs.code);
    }

    bool operator != (const pairs& rhs) const//overloading != operator such that it does the necessary checks inside the struct when we don't have an exact code but when we have the key
    {
        return (this->key != rhs.key && this->code != rhs.code);
    }

};

/**
         *TAKEN FROM LECTURE SLIDES AND MODIFIED SLIGHTLY.
         *
         */

template <class HashedObj>
class HashTable
{
    
public:
    explicit HashTable(const HashedObj& notFound, int size = 4096);
    HashTable(const HashTable& rhs)
        : ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND),
        arrray(rhs.arrray), currentSize(rhs.currentSize) { }

    const HashedObj& find(const HashedObj& x) const;
    const bool& isInTable(const HashedObj& x) const;

   // void makeEmpty();
    void insert(const HashedObj& x);
    void remove(const HashedObj& x);
    

    //const HashTable& operator=(const HashTable& rhs);

    enum EntryType { ACTIVE, EMPTY, DELETED };
private:
    struct HashEntry
    {
        HashedObj element;//struct comes here
        EntryType info;
        int code;

        HashEntry(const HashedObj& e = HashedObj(), EntryType i = EMPTY)
            : element(e), info(i) { }
    };

    vector<HashEntry> arrray;
    int currentSize;
    const HashedObj ITEM_NOT_FOUND;
    bool isActive(int currentPos) const;
    int findPos(const HashedObj& x) const;
    void rehash();
    bool isPrime(int n);
    int nextPrime(int n);
    int hash(const string& key, int tableSize) const;//overloaded hash functions
    int hash(const HashedObj& key, int tableSize)const;
    int hash(const int& key, int tableSize)const;
    int hash(const char& key, int tableSize)const;

   
};

#include "HashTable.cpp"
#endif
