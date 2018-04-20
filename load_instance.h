/*
Loads an Steiner problem instance from a file into an structure composed of a vector of a vector of integers.
*/
#pragma once

#include <deque>

using TInstData = std::deque<std::deque<int>>;

TInstData LoadInstance(const std::string FileDirectory);
