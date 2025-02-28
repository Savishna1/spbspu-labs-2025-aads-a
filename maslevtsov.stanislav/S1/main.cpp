#include <iostream>
#include "fwd_list_operations.hpp"

int main()
{
  maslevtsov::pairs_list_t pairs_list;
  std::string list_name = "";

  while (std::cin >> list_name) {
    maslevtsov::list_t list;
    unsigned long long num = 0;
    while (std::cin >> num) {
      list.push_back(num);
    }
    pairs_list.push_back(std::make_pair(list_name, list));
    std::cin.clear();
  }

  if (pairs_list.empty()) {
    std::cout << 0;
  } else {
    std::cout << pairs_list.begin()->first;
    for (auto i = ++pairs_list.begin(); i != pairs_list.end(); ++i) {
      std::cout << ' ' << i->first;
    }
    std::cout << '\n';
    try {
      maslevtsov::print_lists_info(std::cout, pairs_list);
    } catch (const std::overflow_error&) {
      std::cerr << "Overflow error\n";
      return 1;
    }
  }
  std::cout << '\n';
}
