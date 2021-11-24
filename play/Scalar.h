#pragma once

template<typename Scalar>
constexpr Scalar
min(Scalar a, Scalar b)
{
  return (a < b) ? a : b;
}

template<typename Scalar>
constexpr Scalar
max(Scalar a, Scalar b)
{
  return (a > b) ? a : b;
}

template<typename Scalar>
constexpr Scalar
clamp(Scalar x, Scalar lowerBound, Scalar upperBound)
{
  return min(max(x, lowerBound), upperBound);
}
