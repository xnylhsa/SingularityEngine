#ifndef INCLUDED_CORE_COMMON_H
#define INCLUDED_CORE_COMMON_H

#define NOMINMAX

//Standard Headers
#include <cstdio>
#include <cstdint>
#include <cstring>
//STL Headers
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdint>
#include <functional>
#include <list>
#include <limits>
#include <sys/stat.h>
#include <locale>
#include <codecvt>
#include <string>
#include <map>
#include <memory>
#include <vector>
#include <initializer_list>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <tchar.h>
#include <type_traits>
#include <atlbase.h>
#include <random>
#include <atlconv.h>
#include <queue>
#include <bitset>
#include "MagicEnum.hpp"
#include "PlatformDetection.h"
#ifdef SINGULARITY_PLATFORM_WINDOWS
#include <Windows.h>
#endif
#define SINGULARITY_USE_GLFW
#define ToString(name) #name



#include <External/RapidXML/Inc/RapidXML.h>

#endif //INCLUDED_CORE_COMMON_H
