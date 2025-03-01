#include <iostream>
#include <string>
#include <stdexcept>
#include "list.hpp"

int main()
{
  rychkov::List< std::pair< std::string, rychkov::List< size_t > > > entered;
  std::string name;
  while (std::cin >> name)
  {
    try
    {
      size_t processed = 0;
      size_t number = std::stoull(name, &processed);
      if (processed != name.size())
      {
        std::cerr << "int suffix is not supplied\n";
        return 1;
      }
      if (entered.empty())
      {
        std::cerr << "missing name in input\n";
        return 1;
      }
      entered.back().second.push_back(number);
    }
    catch (const std::invalid_argument&)
    {
      entered.push_back({name, {}});
    }
    catch (const std::out_of_range&)
    {
      std::cerr << "wrong input\n";
      return 1;
    }
  }

  rychkov::List< rychkov::List< size_t > > result;
  char space[2] = "\0";
  for (const decltype(entered)::value_type& i : entered)
  {
    std::cout << space << i.first;
    space[0] = ' ';
    decltype(result)::iterator wPoint = result.begin();
    for (size_t j : i.second)
    {
      if (wPoint != result.end())
      {
        wPoint->push_back(j);
        ++wPoint;
      }
      else
      {
        result.push_back({j});
      }
    }
  }
  std::cout << '\n';

  rychkov::List< size_t > sums(0, result.size());
  decltype(sums)::iterator wPoint = sums.begin();
  for (rychkov::List< size_t > i : result)
  {
    space[0] = '\0';
    for (int j : i)
    {
      *wPoint += j;
      std::cout << space << j;
      space[0] = ' ';
    }
    std::cout << '\n';
    ++wPoint;
  }
  if (sums.empty())
  {
    std::cout << 0 << '\n';
  }
  else
  {
    space[0] = '\0';
    for (decltype(sums)::value_type i : sums)
    {
      std::cout << space << i;
      space[0] = ' ';
    }
    std::cout << '\n';
  }
}
