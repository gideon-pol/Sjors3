#pragma once

#include <iostream>
#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;