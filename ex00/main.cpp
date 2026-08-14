#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

// O(log n) time, O(1) aux space. precondition: v is sorted ascending.
// Returns the index of target, or -1 if absent.
int binarySearch(const std::vector<int>& v, int target) {
    int lo = 0, hi = static_cast<int>(v.size());
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (v[mid] == target) { return mid; }
        else if (v[mid] > target) { hi = mid + 1; }
        else { lo = mid - 1; }
    }
    return -1;
}

// O(n) time, O(1) aux space. Prints the label, then " <n>" for each element.
static void printRow(const std::string& label, const std::vector<int>& v) {
    std::cout << label;
    for (int n : v) {
        std::cout << ' ' << n;
    }
    std::cout << '\n';
}

// O(n log n) overall: dominated by the three sorts. The search itself is O(log n).
int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "usage: ./search <target> <numbers...>\n";
        return 1;
    }

    const int target = std::stoi(argv[1]);

    std::vector<int> numbers;
    numbers.reserve(static_cast<size_t>(argc - 2));
    for (int i = 2; i < argc; ++i) {
        numbers.push_back(std::stoi(argv[i]));
    }

    std::sort(numbers.begin(), numbers.end());
    printRow("sorted asc: ", numbers);

    std::sort(numbers.begin(), numbers.end(),
              [](int a, int b) { return a > b; });
    printRow("sorted desc:", numbers);

    std::sort(numbers.begin(), numbers.end());

    const int index = binarySearch(numbers, target);
    std::cout << "find " << target << " -> ";
    if (index >= 0) {
        std::cout << "index " << index << '\n';
    } else {
        std::cout << "-1\n";
    }

    return 0;
}
