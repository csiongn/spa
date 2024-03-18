# pragma once

# include <unordered_map>
# include <string>
# include <memory>
# include <unordered_set>
# include <vector>
#include "PKB/utils/MapUtils.h"

/*
The method is implemented in a template class in a .cpp file: If RelationshipManager is a template class and its implementation is in a .cpp file,
you will run into issues because templates in C++ need to have their implementation in the same file as their declaration (usually in the header file),
or in a file that is included in the header file.
This is because templates are compiled when they are instantiated,
so the compiler needs to have access to the full template definition, not just the declaration.
*/
template<typename T, typename U>
class RelationshipManager {

  // Only accessible by derived classes
 private:
  std::unordered_map<T, std::unordered_set<U> > data;
  std::unordered_map<U, std::unordered_set<T> > reverseData;

 public:
  // Insert a record into the table
  void insert(const T &key, const U &value) {
	data[key].insert(value);
	reverseData[value].insert(key);
  };

  // Get the value associated with a key
  std::vector<U> get(const T &key) {
	return MapUtils<T, U>::getValuesForKey(data, key);
  };

  // Get the key associated with a value
  std::vector<T> getReverse(const U &value) {
	return MapUtils<U, T>::getValuesForKey(reverseData, value);
  };

  std::vector<T> getKeys() {
	return MapUtils<T, U>::getAllKeys(data);
  }

  std::vector<U> getValues() {
	return MapUtils<U, T>::getAllKeys(reverseData);
  }

  // Check if a key exists in the table
  bool contains(const T &key) {
	return MapUtils<T, U>::contains(data, key);
  };

  // Check if a value exists in the table
  bool containsReverse(const U &value) {
	return MapUtils<U, T>::contains(reverseData, value);
  };

  bool containsRelationship(const T &key, const U &value) {
	if (contains(key)) {
	  const auto &valueSet = data[key];
	  return (valueSet.find(value) != valueSet.end());
	}
	return false; // key does not exist
  }

  bool hasRelationship() {
	return !data.empty();
  }
};