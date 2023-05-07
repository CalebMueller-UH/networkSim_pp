#pragma once

#include <iostream>
#include <memory>
#include <ostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "Color.hpp"
#include "Logger.hpp"
#include "Net.hpp"

class NetLink;

//////// MANAGER BEGIN ////////
class Manager {
 public:
  Manager() = default;

  void addLink(const std::shared_ptr<NetLink>& link) { _links.insert(link); }

  std::set<std::shared_ptr<NetLink>> getLinks() const { return _links; }

 private:
  std::set<std::shared_ptr<NetLink>> _links;
};  // End of Manager class