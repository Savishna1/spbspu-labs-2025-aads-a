#include <iostream>
#include <utility>
#include <string>
#include <limits>
#include "list.hpp"

using namespace averenkov;
using mainList = List< std::pair< std::string, List< unsigned long long > > >;
mainList readInput()
{
  mainList list;
  std::string name;
  while (std::cin >> name)
  {
    List< unsigned long long > numbersList;
    unsigned long long number = 0;
    while (std::cin >> number)
    {
      numbersList.push_back(number);
    }
    list.push_back(std::make_pair(name, numbersList));
    std::cin.clear();
  }
  return list;
}

void printNames(const mainList& list)
{
  if (list.empty())
  {
    std::cout << "0\n";
    return;
  }
  std::cout << list.begin()->first;
  for (auto it = ++(list.begin()); it != list.end(); it++)
  {
    std::cout << " " << it->first;
  }
  std::cout << "\n";
}

List< List< unsigned long long > > createResultList(const mainList& list)
{
  List< List< unsigned long long > > result;
  size_t maxLen = 0;
  for (auto it = list.begin(); it != list.end(); it++)
  {
    if (it->second.size() > maxLen)
    {
      maxLen = it->second.size();
    }
  }
  for (size_t i = 0; i < maxLen; i++)
  {
    List< unsigned long long > sublist;
    for (auto it = list.begin(); it != list.end(); it++)
    {
      if (i < it->second.size())
      {
        auto numIt = it->second.begin();
        for (size_t j = 0; j < i; j++)
        {
          numIt++;
        }
        sublist.push_back(*numIt);
      }
    }
    result.push_back(sublist);
  }
  return result;
}

void printResult(const List< List< unsigned long long > >& result)
{
  for (auto it = result.begin(); it != result.end(); it++)
  {
    std::cout << *(it->begin());
    for (auto numIt = ++(it->begin()); numIt != it->end(); numIt++)
    {
      std::cout << " " << *numIt;
    }
    std::cout << "\n";
  }
}

void calcPrintSum(const List< List< unsigned long long > >& result)
{
  List< unsigned long long > sums;
  for (auto it = result.begin(); it != result.end(); it++)
  {
    unsigned long long sum = 0;
    for (auto numIt = it->begin(); numIt != it->end(); numIt++)
    {
      if (sum > std::numeric_limits< unsigned long long >::max() - *numIt)
      {
        std::cerr << "Error\n";
        return;
      }
      sum += *numIt;
    }
    sums.push_back(sum);
  }
  std::cout << *(sums.begin());
  for (auto it = ++(sums.begin()); it != sums.end(); it++)
  {
    std::cout << " " << *it;
  }
  std::cout << "\n";
}

int main()
{
  auto list = readInput();
  printNames(list);
  auto result = createResult(list);
  printResultList(result);
  calcPrintSum(result);
  return 0;
}
