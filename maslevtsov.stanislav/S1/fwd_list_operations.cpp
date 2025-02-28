#include "fwd_list_operations.hpp"
#include <limits>
#include <ostream>

namespace {
  int safe_sum(unsigned long long a, unsigned long long b)
  {
    const unsigned long long max_ull = std::numeric_limits< unsigned long long >::max();
    if (max_ull - a > b) {
      return a + b;
    }
    throw std::overflow_error("overflow");
  }
}

std::size_t maslevtsov::get_max_pairs_list_size(pairs_list_t& list) noexcept
{
  std::size_t maximum = 0, size = 0;
  for (auto i = list.begin(); i != list.end(); ++i) {
    for (auto j = i->second.begin(); j != i->second.end(); ++j) {
      ++size;
    }
    maximum = maximum > size ? maximum : size;
  }
  return maximum;
}

std::size_t maslevtsov::get_sum_of_list_elements(list_t& list)
{
  std::size_t result = 0;
  for (auto i = list.begin(); i != list.end(); ++i) {
    result = safe_sum(result, *i);
  }
  return result;
}

void maslevtsov::print_lists_info(std::ostream& out, pairs_list_t pairs_list)
{
  std::size_t max_pairs_list_size = get_max_pairs_list_size(pairs_list);
  list_t sums;
  for (std::size_t i = 0; i != max_pairs_list_size; ++i) {
    list_t column_elements;
    for (auto j = pairs_list.begin(); j != pairs_list.end(); ++j) {
      if (!j->second.empty()) {
        column_elements.push_back(j->second.front());
        j->second.pop_front();
      }
    }
    if (!column_elements.empty()) {
      out << *column_elements.begin();
      for (auto j = ++column_elements.begin(); j != column_elements.end(); ++j) {
        out << ' ' << *j;
      }
      out << '\n';
      sums.push_back(get_sum_of_list_elements(column_elements));
    }
  }
  out << *sums.begin();
  for (auto j = ++sums.begin(); j != sums.end(); ++j) {
    out << ' ' << *j;
  }
}
