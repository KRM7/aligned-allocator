#include "aligned_alloc.hpp"
#include <vector>

int main()
{
    std::vector<int, aligned_allocator<int, 32>> vec = { 1, 2, 3, 4, 5 };

    return vec[3];
}