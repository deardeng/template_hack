//
// Created by dengxin05 on 2021/1/18.
//

#ifndef TEMPLATE_HACK_ALGORITHMS_02_SORTING_BASIC_02_SELECTION_SORT_USING_TEMPLATE_STUDENT_H_
#define TEMPLATE_HACK_ALGORITHMS_02_SORTING_BASIC_02_SELECTION_SORT_USING_TEMPLATE_STUDENT_H_

#include <iostream>
#include <string>

using namespace std;

struct Student{
  string name;
  int score;

  bool operator<(const Student& other){
      return score != other.score ? score > other.score : name < other.name;
  }

  friend ostream& operator<<(ostream &os, const Student &student){
      os << "Student: " << student.name << " " << student.score << endl;
      return os;
  }
};

#endif //TEMPLATE_HACK_ALGORITHMS_02_SORTING_BASIC_02_SELECTION_SORT_USING_TEMPLATE_STUDENT_H_
