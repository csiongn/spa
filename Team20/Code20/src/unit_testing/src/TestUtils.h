#include <algorithm>
#include <vector>

template <typename T>
bool checkVecValuesEqual(const std::vector<T>& vec1, const std::vector<T>& vec2) {
    if (vec1.size() != vec2.size()) {
        return false;
    }

    std::vector<T> sortedVec1(vec1);
    std::vector<T> sortedVec2(vec2);

    std::sort(sortedVec1.begin(), sortedVec1.end());
    std::sort(sortedVec2.begin(), sortedVec2.end());

    return sortedVec1 == sortedVec2;
}