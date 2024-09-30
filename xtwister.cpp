#include <functional>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <random>

#ifdef _WIN32
    #include <windows.h>
    static inline
    #if _WIN32_WINNT >= 0x0600
    uint64_t
    #else
    uint32_t
    #endif
    get_tick_count(void)
    {
        #if _WIN32_WINNT >= 0x0600
            return GetTickCount64();
        #else
            return GetTickCount();
        #endif
    }
#else
    #include <unistd.h>
    #include <cstdint>
    static inline uint64_t get_tick_count(void)
    {
        using namespace std;
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return (uint64_t)(ts.tv_nsec / 1000000) + ((uint64_t)ts.tv_sec * 1000ull);
    }
#endif

// Calculate build-specific value
constexpr size_t AntiRE_GetBuildSpecificValue(void)
{
    size_t ich = 0, value = 0xB00B00;
    // ex) "Sep 29 2024"
    if (__DATE__[ich]) { value += __DATE__[ich++]; value ^= 0xDEADFACE; value *= 0xBADBAD; }
    if (__DATE__[ich]) { value += __DATE__[ich++]; value ^= 0xDEADFACE; value *= 0xBADBAD; }
    if (__DATE__[ich]) { value += __DATE__[ich++]; value ^= 0xDEADFACE; value *= 0xBADBAD; }
    if (__DATE__[ich]) { value += __DATE__[ich++]; value ^= 0xDEADFACE; value *= 0xBADBAD; }
    if (__DATE__[ich]) { value += __DATE__[ich++]; value ^= 0xDEADFACE; value *= 0xBADBAD; }
    if (__DATE__[ich]) { value += __DATE__[ich++]; value ^= 0xDEADFACE; value *= 0xBADBAD; }
    if (__DATE__[ich]) { value += __DATE__[ich++]; value ^= 0xDEADFACE; value *= 0xBADBAD; }
    if (__DATE__[ich]) { value += __DATE__[ich++]; value ^= 0xDEADFACE; value *= 0xBADBAD; }
    if (__DATE__[ich]) { value += __DATE__[ich++]; value ^= 0xDEADFACE; value *= 0xBADBAD; }
    if (__DATE__[ich]) { value += __DATE__[ich++]; value ^= 0xDEADFACE; value *= 0xBADBAD; }
    if (__DATE__[ich]) { value += __DATE__[ich++]; value ^= 0xDEADFACE; value *= 0xBADBAD; }
    ich = 0;
    // ex) "19:30:06"
    if (__TIME__[ich]) { value += __TIME__[ich++]; value ^= 0x0BADFACE; value *= 0xDEADDEAD; }
    if (__TIME__[ich]) { value += __TIME__[ich++]; value ^= 0x0BADFACE; value *= 0xDEADDEAD; }
    if (__TIME__[ich]) { value += __TIME__[ich++]; value ^= 0x0BADFACE; value *= 0xDEADDEAD; }
    if (__TIME__[ich]) { value += __TIME__[ich++]; value ^= 0x0BADFACE; value *= 0xDEADDEAD; }
    if (__TIME__[ich]) { value += __TIME__[ich++]; value ^= 0x0BADFACE; value *= 0xDEADDEAD; }
    if (__TIME__[ich]) { value += __TIME__[ich++]; value ^= 0x0BADFACE; value *= 0xDEADDEAD; }
    if (__TIME__[ich]) { value += __TIME__[ich++]; value ^= 0x0BADFACE; value *= 0xDEADDEAD; }
    if (__TIME__[ich]) { value += __TIME__[ich++]; value ^= 0x0BADFACE; value *= 0xDEADDEAD; }
    value += __LINE__;
    return value;
}

enum COMMAND
{
#undef DEFINE_COMMAND
#define DEFINE_COMMAND(id, fn, index) id,
#include "commands.h"
    MAX_COMMAND
};

typedef int (*FN_COMMAND)(void);
static size_t s_fn_table[MAX_COMMAND];
static int s_jam_table[MAX_COMMAND];

template <int t_max_value = MAX_COMMAND>
constexpr int do_twist_0(int value)
{
    value += 82347;
    value %= t_max_value;
    auto n = (unsigned)AntiRE_GetBuildSpecificValue();
    value = (value + n) % t_max_value;
    value = t_max_value - value - 1;
    return value;
}

#define DO_TWIST(value) s_jam_table[do_twist_0(value)]

int command_zero(void)
{
    std::puts("zero");
    return 0;
}
int command_one(void)
{
    std::puts("one");
    return 1;
}
int command_two(void)
{
    std::puts("two");
    return 2;
}
int command_three(void)
{
    std::puts("three");
    return 3;
}
int command_four(void)
{
    std::puts("four");
    return 4;
}

void init_xtwister(void)
{
#if defined(_WIN32) && defined(NDEBUG)
    if (IsDebuggerPresent())
    {
        std::exit(1);
        return;
    }
#endif

    for (int i = 0; i < MAX_COMMAND * sizeof(float) / sizeof(int); ++i)
        ((float*)s_jam_table)[i] = (float)std::log(std::round(std::exp(3.14159 * i)));
    for (int i = 0; i < MAX_COMMAND * sizeof(double) / sizeof(int); ++i)
        ((double*)s_jam_table)[i] /= std::log10(3.14159) * 3;

    for (int i = 0; i < MAX_COMMAND; ++i)
        s_jam_table[i] = i;

    std::random_device seed_gen;
    std::mt19937 random_engine(seed_gen() + AntiRE_GetBuildSpecificValue() + get_tick_count());
    std::shuffle(&s_jam_table[0], &s_jam_table[MAX_COMMAND], random_engine);

    for (int i = 0; i < MAX_COMMAND; ++i)
        std::printf("%d --> %d\n", i, s_jam_table[i]);

    for (int id = 0; id < MAX_COMMAND; ++id)
    {
        for (int i = MAX_COMMAND - 1; i >= 0; --i)
        {
#undef DEFINE_COMMAND
#define DEFINE_COMMAND(id, fn, index) do { \
    if (DO_TWIST(id) == s_jam_table[i]) { s_fn_table[s_jam_table[i]] = ~(size_t)fn; } \
} while (0);
#include "commands.h"
        }
    }
}

int main(void)
{
    auto time0 = get_tick_count();
    init_xtwister();
    auto time1 = get_tick_count();

#ifdef NDEBUG
    if (time1 - time0 >= 3 * 1000)
    {
        std::exit(1);
        return 1;
    }
#endif

    std::printf("__DATE__: %s\n", __DATE__);
    std::printf("__TIME__: %s\n", __TIME__);

#define xtwister(command) ((FN_COMMAND)~s_fn_table[DO_TWIST(command)])()
    xtwister(COMMAND_ZERO);
    xtwister(COMMAND_ZERO);
    xtwister(COMMAND_THREE);

    return 0;
}
