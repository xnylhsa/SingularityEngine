#ifndef INCLUDED_CORE_COMMON_H
#define INCLUDED_CORE_COMMON_H

#define NOMINMAX

//Standard Headers
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <Windows.h>

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
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

#include <External/RapidXML/Inc/RapidXML.h>

#endif //INCLUDED_CORE_COMMON_H
