#ifndef _HASHTABLE_HPP_
#define _HASHTABLE_HPP_

#include <map>
#include <vector>
#include <cmath>
#include <TVector2.h>

class HashTable {
private:
  static HashTable *instance;
  std::map<unsigned int, std::vector<unsigned int>> hashTable;
  HashTable(){};
  unsigned int GetHash(TVector2);
  TVector2 Discretize(TVector2);
public:
  static HashTable* GetInstance();
  void Add(const unsigned int,TVector2);
  std::vector<unsigned int> FindNN(TVector2);
  void Clear();
};

#endif
