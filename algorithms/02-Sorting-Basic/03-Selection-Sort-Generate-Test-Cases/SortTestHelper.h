//
// Created by dengxin05 on 2021/1/18.
//

#ifndef TEMPLATE_HACK_ALGORITHMS_02_SORTING_BASIC_03_SELECTION_SORT_GENERATE_TEST_CASES_SORTTESTHELPER_H_
#define TEMPLATE_HACK_ALGORITHMS_02_SORTING_BASIC_03_SELECTION_SORT_GENERATE_TEST_CASES_SORTTESTHELPER_H_

#include <iostream>
#include <ctime>
#include <cassert>

namespace SortTestHelper{
    // 生成有n个元素的随机数组，每个元素随机范围为[rangeL, rangeR]
    int* generateRandomArray(int n, int rangeL, int rangeR){
        assert(rangeL <= rangeR);

        int* arr = new int[n];

        srand(time(NULL));
        for(int i=0; i < n; i++)
            arr[i] = rand() % (rangeR - rangeL + 1) + rangeL;
        return arr;
    }

    // 打印arr数组的所有内容
    template <typename T>
    void printArray(T arr[], int n){
        for(int i=0; i<n; i++)
            std::cout << arr[i] << ' ';
        std::cout << std::endl;

        return;
    }
};

#endif //TEMPLATE_HACK_ALGORITHMS_02_SORTING_BASIC_03_SELECTION_SORT_GENERATE_TEST_CASES_SORTTESTHELPER_H_
