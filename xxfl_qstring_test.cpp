#include "test_helper.h"

std::mt19937 rand_gen;

template<typename _string>
void string_test_compare_performance(uint32_t strings_count, uint32_t loops_count)
{
    std::vector<_string> aa;
    gen_random_string_vector(aa, strings_count);

    time_start();

    volatile int32_t tmp = 0;
    for (uint32_t i = 0; i < loops_count; ++i)
    {
        for (uint32_t j = 1; j < strings_count; ++j)
        {
            tmp += aa[j - 1].compare(aa[j]);
        }
    }

    std::printf("%f sec\n", get_elapsed_time());
}

void test_compare_performance()
{
    const uint32_t strings_count = 500000;
    const uint32_t loops_count = 50;

    std::printf("compare %u strings and loop %u times\n\n", strings_count, loops_count);

    std::printf("std::string: ");
    string_test_compare_performance<std::string>(strings_count, loops_count);

    std::printf("xxfl_qstring32: ");
    string_test_compare_performance<xxfl_qstring32>(strings_count, loops_count);

    std::printf("xxfl_qstring64: ");
    string_test_compare_performance<xxfl_qstring64>(strings_count, loops_count);

    std::printf("\n");
}

template<typename _string>
void string_test_sort_performance(uint32_t strings_count)
{
    std::vector<_string> aa;
    gen_random_string_vector(aa, strings_count);

    time_start();
    std::sort(aa.begin(), aa.end());
    std::printf("%f sec\n", get_elapsed_time());
}

void test_sort_performance()
{
    const uint32_t strings_count = 500000;

    std::printf("sort %u strings\n\n", strings_count);

    std::printf("std::string: ");
    string_test_sort_performance<std::string>(strings_count);

    std::printf("xxfl_qstring32: ");
    string_test_sort_performance<xxfl_qstring32>(strings_count);

    std::printf("xxfl_qstring64: ");
    string_test_sort_performance<xxfl_qstring64>(strings_count);

    std::printf("\n");
}

template<typename _string>
void string_test_set_insert_performance(uint32_t strings_count)
{
    std::vector<_string> aa;
    std::set<_string> bb;

    gen_random_string_vector(aa, strings_count);
    for (auto& str : aa)
    {
        bb.insert(str);
    }

    time_start();

    volatile uint64_t tmp = 0;
    for (auto& str : aa)
    {
        tmp += (uint64_t)&*bb.find(str);
    }

    std::printf("%f sec\n", get_elapsed_time());
}

void test_find_performance()
{
    const uint32_t strings_count = 500000;

    std::printf("find %u times in %u strings\n\n", strings_count, strings_count);

    std::printf("std::string: ");
    string_test_set_insert_performance<std::string>(strings_count);

    std::printf("xxfl_qstring32: ");
    string_test_set_insert_performance<xxfl_qstring32>(strings_count);

    std::printf("xxfl_qstring64: ");
    string_test_set_insert_performance<xxfl_qstring64>(strings_count);

    std::printf("\n");
}

void verification_test()
{
    const uint32_t strings_count = 500000;

    std::vector<std::string> aa;
    std::vector<xxfl_qstring32> bb;
    std::vector<xxfl_qstring64> cc;

    std::printf("testing...");

    gen_random_string_vector(aa, strings_count);

    for (uint32_t i = 0; i < strings_count; ++i)
    {
        std::string& str = aa[i];
        bb.push_back(xxfl_qstring32(str.c_str(), str.size()));
        cc.push_back(xxfl_qstring64(str.c_str(), str.size()));
    }

    std::sort(aa.begin(), aa.end());
    std::sort(bb.begin(), bb.end());
    std::sort(cc.begin(), cc.end());

    bool success = true;
    for (uint32_t i = 0; i < strings_count; ++i)
    {
        std::string& str = aa[i];
        success &= str == bb[i].c_str();
        success &= str == cc[i].c_str();
    }

    std::printf("%s\n", success? "passed" : "error");
}

int main()
{
    std::random_device rd;
    rand_gen.seed(rd());

    while (true)
    {
        std::printf("main menu:\n"
                    "  0. exit\n"
                    "  1. compare performance test\n"
                    "  2. sort performance test\n"
                    "  3. find performance test\n"
                    "  4. verification test\n"
                    "select: ");

        uint32_t select_idx = 0;
        std::scanf("%u", &select_idx);
        std::printf("\n");

        if (select_idx == 0)
        {
            break;
        }
        else if (select_idx == 1)
        {
            test_compare_performance();
        }
        else if (select_idx == 2)
        {
            test_sort_performance();
        }
        else if (select_idx == 3)
        {
            test_find_performance();
        }
        else if (select_idx == 4)
        {
            verification_test();
        }
        else
        {
            continue;
        }

        std::printf("\n");
    }

    return 0;
}
