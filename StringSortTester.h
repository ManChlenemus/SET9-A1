#pragma once
#include <algorithm>
#include <chrono>
#include <string>
#include <utility>
#include <vector>

struct SortResult {
  double timeMs;
  long long comparisons;
};

class StringSortTester {
 private:
  long long compCount;
  const int R = 256;
  const int CUTOFF = 74;

  bool isLessStandard(const std::string& a, const std::string& b) {
    int lenA = a.length();
    int lenB = b.length();
    int minLen = lenA < lenB ? lenA : lenB;
    for (int i = 0; i < minLen; ++i) {
      compCount++;
      if (a[i] != b[i]) {
        return a[i] < b[i];
      }
    }
    return lenA < lenB;
  }

  void stdMerge(std::vector<std::string>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    std::vector<std::string> L(n1), R(n2);
    for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
      if (isLessStandard(L[i], R[j]) ||
          (!isLessStandard(L[i], R[j]) && !isLessStandard(R[j], L[i]))) {
        arr[k] = L[i++];
      } else {
        arr[k] = R[j++];
      }
      k++;
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
  }

  void stdMergeSort(std::vector<std::string>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    stdMergeSort(arr, left, mid);
    stdMergeSort(arr, mid + 1, right);
    stdMerge(arr, left, mid, right);
  }

  int stdPartition(std::vector<std::string>& arr, int low, int high) {
    std::string pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
      if (isLessStandard(arr[j], pivot)) {
        i++;
        std::swap(arr[i], arr[j]);
      }
    }
    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
  }

  void stdQuickSort(std::vector<std::string>& arr, int low, int high) {
    if (low < high) {
      int pi = stdPartition(arr, low, high);
      stdQuickSort(arr, low, pi - 1);
      stdQuickSort(arr, pi + 1, high);
    }
  }

  int charAt(const std::string& s, int d) {
    compCount++;
    if (d < s.length()) {
      return static_cast<unsigned char>(s[d]);
    }
    return -1;
  }

  int lcpCompare(const std::string& a, const std::string& b) {
    int i = 0;
    int min_len = std::min(a.length(), b.length());
    while (i < min_len) {
      compCount++;
      if (a[i] == b[i]) {
        ++i;
      } else {
        break;
      }
    }
    return i;
  }

  bool isLessStr(const std::string& a, const std::string& b) {
    int lcp = lcpCompare(a, b);
    if (lcp == a.length() && lcp == b.length()) return false;
    if (lcp == a.length()) return true;
    if (lcp == b.length()) return false;
    return a[lcp] < b[lcp];
  }

  void stringMerge(std::vector<std::string>& arr, int left, int mid,
                   int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    std::vector<std::string> L(n1), R(n2);
    for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
      if (!isLessStr(R[j], L[i])) {
        arr[k++] = L[i++];
      } else {
        arr[k++] = R[j++];
      }
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
  }

  void stringMergeSortWorker(std::vector<std::string>& arr, int left,
                             int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    stringMergeSortWorker(arr, left, mid);
    stringMergeSortWorker(arr, mid + 1, right);
    stringMerge(arr, left, mid, right);
  }

  void stringQuickSortWorker(std::vector<std::string>& a, int lo, int hi,
                             int d) {
    if (hi <= lo) return;
    int lt = lo, gt = hi;
    int v = charAt(a[lo], d);
    int i = lo + 1;
    while (i <= gt) {
      int t = charAt(a[i], d);
      if (t < v)
        std::swap(a[lt++], a[i++]);
      else if (t > v)
        std::swap(a[i], a[gt--]);
      else
        i++;
    }
    stringQuickSortWorker(a, lo, lt - 1, d);
    if (v >= 0) stringQuickSortWorker(a, lt, gt, d + 1);
    stringQuickSortWorker(a, gt + 1, hi, d);
  }

  void msdWorker(std::vector<std::string>& a, std::vector<std::string>& aux,
                 int lo, int hi, int d) {
    if (hi <= lo) return;
    std::vector<int> count(R + 2, 0);
    for (int i = lo; i <= hi; i++) count[charAt(a[i], d) + 2]++;
    for (int r = 0; r < R + 1; r++) count[r + 1] += count[r];
    for (int i = lo; i <= hi; i++) aux[count[charAt(a[i], d) + 1]++] = a[i];
    for (int i = lo; i <= hi; i++) a[i] = aux[i - lo];
    for (int r = 0; r < R; r++)
      msdWorker(a, aux, lo + count[r], lo + count[r + 1] - 1, d + 1);
  }

  void msdQSWorker(std::vector<std::string>& a, std::vector<std::string>& aux,
                   int lo, int hi, int d) {
    if (hi <= lo) return;
    if (hi - lo + 1 < CUTOFF) {
      stringQuickSortWorker(a, lo, hi, d);
      return;
    }
    std::vector<int> count(R + 2, 0);
    for (int i = lo; i <= hi; i++) count[charAt(a[i], d) + 2]++;
    for (int r = 0; r < R + 1; r++) count[r + 1] += count[r];
    for (int i = lo; i <= hi; i++) aux[count[charAt(a[i], d) + 1]++] = a[i];
    for (int i = lo; i <= hi; i++) a[i] = aux[i - lo];
    for (int r = 0; r < R; r++)
      msdQSWorker(a, aux, lo + count[r], lo + count[r + 1] - 1, d + 1);
  }

 public:
  SortResult testMergeSort(std::vector<std::string> arr) {
    compCount = 0;
    auto start = std::chrono::high_resolution_clock::now();
    stdMergeSort(arr, 0, arr.size() - 1);
    auto end = std::chrono::high_resolution_clock::now();
    return { std::chrono::duration<double, std::milli>(end - start).count(),
             compCount };
  }

  SortResult testQuickSort(std::vector<std::string> arr) {
    compCount = 0;
    auto start = std::chrono::high_resolution_clock::now();
    stdQuickSort(arr, 0, arr.size() - 1);
    auto end = std::chrono::high_resolution_clock::now();
    return { std::chrono::duration<double, std::milli>(end - start).count(),
             compCount };
  }

  SortResult testStringMergeSort(std::vector<std::string> arr) {
    compCount = 0;
    auto start = std::chrono::high_resolution_clock::now();
    stringMergeSortWorker(arr, 0, arr.size() - 1);
    auto end = std::chrono::high_resolution_clock::now();
    return { std::chrono::duration<double, std::milli>(end - start).count(),
             compCount };
  }

  SortResult testStringQuickSort(std::vector<std::string> arr) {
    compCount = 0;
    auto start = std::chrono::high_resolution_clock::now();
    stringQuickSortWorker(arr, 0, arr.size() - 1, 0);
    auto end = std::chrono::high_resolution_clock::now();
    return { std::chrono::duration<double, std::milli>(end - start).count(),
             compCount };
  }

  SortResult testMSD(std::vector<std::string> arr) {
    compCount = 0;
    std::vector<std::string> aux(arr.size());
    auto start = std::chrono::high_resolution_clock::now();
    msdWorker(arr, aux, 0, arr.size() - 1, 0);
    auto end = std::chrono::high_resolution_clock::now();
    return { std::chrono::duration<double, std::milli>(end - start).count(),
             compCount };
  }

  SortResult testMSD_QS(std::vector<std::string> arr) {
    compCount = 0;
    std::vector<std::string> aux(arr.size());
    auto start = std::chrono::high_resolution_clock::now();
    msdQSWorker(arr, aux, 0, arr.size() - 1, 0);
    auto end = std::chrono::high_resolution_clock::now();
    return { std::chrono::duration<double, std::milli>(end - start).count(),
             compCount };
  }
};