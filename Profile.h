#pragma once

#include "SaveState.h"

#include <fstream>
#include <string>
#include <vector>

template<size_t SaveStateSize>
struct GenericProfile final
{
  using SaveState = ::SaveState<SaveStateSize>;

  std::string name = "Untitled";

  std::vector<SaveState> saveStates;

  bool open(const std::string& path);

  bool save(const std::string& path) const;
};

using Profile = GenericProfile<4096>;

template<size_t SaveStateSize>
bool
GenericProfile<SaveStateSize>::open(const std::string& path)
{
  name.clear();

  saveStates.clear();

  std::ifstream file(path.c_str());

  if (!std::getline(file, name))
    return false;

  while (file) {

    SaveState state;

    if (!state.read(file))
      break;

    saveStates.emplace_back(std::move(state));
  }

  return true;
}

template<size_t SaveStateSize>
bool
GenericProfile<SaveStateSize>::save(const std::string& path) const
{
  std::ofstream file(path.c_str());

  if (!file.good())
    return false;

  file << name << std::endl;

  for (const SaveState& state : saveStates)
    file << state << std::endl;

  return true;
}
