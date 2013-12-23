#include "HashTable.hpp"
#include "Config.hpp"

HashTable* HashTable::instance = 0;

HashTable* HashTable::GetInstance() {
  if(instance == 0)
    instance = new HashTable();
  return instance;
}

void HashTable::Add(unsigned int i, TVector2 position) {
  position = Discretize(position);
  hashTable[GetHash(position)].push_back(i);
}

void HashTable::Clear() {
  hashTable.clear();
}

std::vector<unsigned int> HashTable::FindNN(TVector2 position) {
  std::vector<unsigned int> results;
  std::vector<unsigned int> cell;
  TVector2 min = Discretize(position) - TVector2(_NG_H_,_NG_H_);
  TVector2 max = Discretize(position) + TVector2(_NG_H_,_NG_H_);

  for(TVector2 it(min); it.X() <= max.X() ; it.Set(it.X()+1,it.Y())) {
    for(it.Set(it.X(),min.Y());it.Y() <= max.Y(); it.Set(it.X(),it.Y()+1)) {
      cell.clear();
      cell = hashTable[GetHash(it)];
      results.insert(results.end(), cell.begin(), cell.end());
    }
  }

  return results;
}

unsigned int HashTable::GetHash(TVector2 position) {
  unsigned long int x = (unsigned long int) position.X();
  unsigned long int y = (unsigned long int) position.Y();
  return ((x*_HT_P1_)^(y*_HT_P2_))%_HT_NH_;
}

TVector2 HashTable::Discretize(TVector2 position) {
  position /= _NG_H_;
  position.Set(
    floor(position.X()),
    floor(position.Y())
  );
  return position;
}