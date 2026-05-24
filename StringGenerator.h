#pragma once
#include <algorithm>
#include <random>
#include <string>
#include <vector>

class StringGenerator {
 private:
  const std::string alphabet =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#%:;^&*("
      ")-";
  std::mt19937 gen;

  std::string generateString() {
    std::uniform_int_distribution<> lenDist(10, 200);
    std::uniform_int_distribution<> charDist(0, alphabet.size() - 1);
    int len = lenDist(gen);
    std::string s;
    s.reserve(len);
    for (int i = 0; i < len; ++i) {
      s += alphabet[charDist(gen)];
    }
    return s;
  }

 public:
  StringGenerator(unsigned int seed = std::random_device{}()) : gen(seed) {}

  std::vector<std::string> generateRandomArray(int size) {
    std::vector<std::string> arr(size);
    for (int i = 0; i < size; ++i) {
      arr[i] = generateString();
    }
    return arr;
  }

  std::vector<std::string> generateReverseSortedArray(int size) {
    std::vector<std::string> arr = generateRandomArray(size);
    std::sort(arr.rbegin(), arr.rend());
    return arr;
  }

  std::vector<std::string> generateNearlySortedArray(int size, int swapsCount) {
    std::vector<std::string> arr = generateRandomArray(size);
    std::sort(arr.begin(), arr.end());
    std::uniform_int_distribution<> idxDist(0, size - 1);
    for (int i = 0; i < swapsCount; ++i) {
      int idx1 = idxDist(gen);
      int idx2 = idxDist(gen);
      std::swap(arr[idx1], arr[idx2]);
    }
    return arr;
  }

  std::vector<std::string> generateCommonPrefixArray(
      int size, const std::string& prefix) {
    std::vector<std::string> arr(size);
    std::uniform_int_distribution<> charDist(0, alphabet.size() - 1);
    for (int i = 0; i < size; ++i) {
      int minSuffixLen = std::max(0, 10 - static_cast<int>(prefix.length()));
      int maxSuffixLen = 200 - static_cast<int>(prefix.length());
      std::uniform_int_distribution<> suffDist(minSuffixLen, maxSuffixLen);
      int suffLen = suffDist(gen);
      std::string s = prefix;
      s.reserve(prefix.length() + suffLen);
      for (int j = 0; j < suffLen; ++j) {
        s += alphabet[charDist(gen)];
      }
      arr[i] = s;
    }
    std::shuffle(arr.begin(), arr.end(), gen);
    return arr;
  }

  std::vector<std::string> getSubArray(const std::vector<std::string>& source,
                                       int newSize) {
    if (newSize >= source.size()) {
      return source;
    }
    return std::vector<std::string>(source.begin(), source.begin() + newSize);
  }
};