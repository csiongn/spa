#pragma once

#include <list>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template<typename T, typename U>
class MapUtils {

 public:
  static std::vector<U> getValuesForKey(const std::unordered_map<T, std::unordered_set<U>> &map, const T &key) {
	if (map.find(key) == map.end()) {
	  return std::vector<U>();
	}
	return std::vector(map.at(key).begin(), map.at(key).end());
  };

  static std::vector<T> getAllKeys(const std::unordered_map<T, std::unordered_set<U>> &map) {
	std::vector<T> keys;
	for (const auto &[key, value] : map) {
	  keys.push_back(key);
	}
	return keys;
  };

  static std::vector<U> getAllValues(const std::unordered_map<T, std::unordered_set<U>> &map) {
	std::vector<U> values;
	for (const auto &[key, value] : map) {
	  for (const auto &val : value) {
		values.push_back(val);
	  }
	}
	return values;
  };

  static bool contains(const std::unordered_map<T, std::unordered_set<U>> &map, const T &key) {
	return map.find(key) != map.end();
  };
};