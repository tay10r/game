#pragma once

#include <array>
#include <istream>
#include <ostream>
#include <string>

#include <cstddef>

template<std::size_t Size>
struct SaveState final
{
  using Byte = unsigned char;

  std::array<Byte, Size> data;

  bool read(std::istream& inStream);
};

template<std::size_t Size>
std::ostream&
operator<<(std::ostream& output, const SaveState<Size>& saveState)
{
  using Byte = unsigned char;

  const char hexTab[] = "0123456789abcdef";

  for (size_t i = 0; i < Size; i++) {
    const Byte byte = saveState.data[i];
    const Byte hi = byte >> 4;
    const Byte lo = byte & 0x0f;
    output << hexTab[hi] << hexTab[lo];
  }

  return output;
}

template<std::size_t Size>
bool
SaveState<Size>::read(std::istream& inStream)
{
  std::string line;

  std::getline(inStream, line);

  if (line.size() != (Size * 2))
    return false;

  auto inRange = [](char c) -> bool { return ((c >= '0') && (c <= '9')) || ((c >= 'a') && (c <= 'f')); };

  for (size_t i = 0; i < Size; i++) {

    const char hi = line[(i * 2) + 0];
    const char lo = line[(i * 2) + 1];

    if (!inRange(hi) || !inRange(lo))
      return false;

    using Byte = unsigned char;

    const Byte byte = (Byte(hi) << 4) | Byte(lo);

    data[i] = byte;
  }

  return true;
}
