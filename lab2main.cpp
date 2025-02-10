#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "include/doctest.h"
#include "fisherYates.h"
#include "prefixSum.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

bool in_list(int list[], const int length, const int value) {
    for(int i = 0; i < length; i++) {
        if(value == list[i]) {
            return true;
        }
    }
    return false;
}

bool exactly_one_in_list(int list[], const int length, const int value) {
    bool found_once = false;
    for(int i = 0; i < length; i++) {
        if(value == list[i]) {
            //If already found, it's a duplicate
            if(found_once) {
                return false;
            }
            found_once = true;
        }
    }
    return found_once;
}

int nums_0[] = {1};
int nums_1[] = {1, 2, 3};
int nums_2[] = {1, 1, -1, -1};
int nums_3[] = {-1, -1, 1, 1};

TEST_CASE("In-list functions") {
    SUBCASE("in_list test") {
        int lst[] = {1,2,3};
        CHECK(in_list(lst, 3, 1));
        CHECK(in_list(lst, 3, 2));
        CHECK_FALSE(in_list(lst, 3, -4));
    }

    SUBCASE("exactly_one_in_list test") {
        int lst_0[] = {};
        CHECK_FALSE(exactly_one_in_list(lst_0, 0, 0));

        int lst_1[] = {1,2,3,1};
        CHECK_FALSE(exactly_one_in_list(lst_1, 4, 1)); //1 appears twice
        CHECK(exactly_one_in_list(lst_1, 4, 2)); //2 appears once
        CHECK(exactly_one_in_list(lst_1, 4, 3));
        CHECK_FALSE(exactly_one_in_list(lst_1, 4, 0)); //non-existent
        CHECK_FALSE(exactly_one_in_list(lst_1, 4, -55));
    }
}

int not_random_swaps0[] = {1, 0};
int not_random_swaps1[] = {3, 2, 1};
int init_list_0[] = {5, 40};
int init_list_1[] = {1, 2, 3, 4};

int r_index = 0; //index for "NOT_random" funcs

int NOT_random0() {
    return not_random_swaps0[r_index++];
}
int NOT_random1() {
    return not_random_swaps1[r_index++];
}

TEST_CASE("Fisher_yates function") {

    SUBCASE("swap test") {
        int a = 8, b = -5;
        swap(&a, &b);
        CHECK(a == -5);
        CHECK(b == 8);

        int nums[] = {1, 2};
        int old = nums[0];
        int new1 = nums[1];

        swap(&nums[0], &nums[1]);
        CHECK(nums[0] == new1);
        CHECK(nums[1] == old);
    }

    SUBCASE("fisher-yates basic tests with non-random funcs") {
        cout << "\n--- Not random length 2 ---\n";
        show_data(init_list_0, 2);
        cout << "  :  ";
        fisher_yates(init_list_0, 2, NOT_random0);
        show_data(init_list_0, 2); //test show_data as well

        //Check explicitly
        CHECK(init_list_0[0] == 40);
        CHECK(init_list_0[1] == 5);

        r_index = 0; //reset
        cout << "\n--- Not random length 4 ---\n";
        show_data(init_list_1, 4);
        cout << "  :  ";
        fisher_yates(init_list_1, 4, NOT_random1);
        show_data(init_list_1, 4);

        CHECK(init_list_1[0] == 4);
        CHECK(init_list_1[1] == 1);
        CHECK(init_list_1[2] == 2);
        CHECK(init_list_1[3] == 3);
    }

    SUBCASE("fisher-yates basic tests with std::rand") {
        //shuffle a 1-length array (no changes)
        fisher_yates(nums_0, 1, std::rand);
        CHECK(exactly_one_in_list(nums_0, 1, 1));

        fisher_yates(nums_1, 3, std::rand);
        cout << "\n--- Random length 3 ---\n";
        show_data(nums_1, 3);
        cout << "\n";

        //Check that there is still 1,2,3 each exactly once
        CHECK(exactly_one_in_list(nums_1, 3, 1));
        CHECK(exactly_one_in_list(nums_1, 3, 2));
        CHECK(exactly_one_in_list(nums_1, 3, 3));
    }
}

TEST_CASE("Prefix sum checks") {
    int arrA[4] = {1, -1, 1, -1};
    CHECK(non_negative_prefix_sum(arrA, 4) == true);
    CHECK(non_positive_prefix_sum(arrA, 4) == false);

    int arrB[4] = {-1, 1, -1, 1};
    CHECK(non_negative_prefix_sum(arrB, 4) == false);
    CHECK(non_positive_prefix_sum(arrB, 4) == true);
}

TEST_CASE("Ratio of well-balanced lists") {

    std::srand(std::time(nullptr)); //random seed
    

    //SET N HERE!---------------------
    const int n = 4;
    //--------------------------------

    const int length = 2 * n;
    const int trials = 10000;  //number of trials

    int count_valid = 0;
    int arr[length];

    for(int t = 0; t < trials; t++) {
        //build array of n +1s and n -1s
        for(int i = 0; i < n; i++) {
            arr[i]     = 1;
            arr[i + n] = -1;
        }
        //shuffle
        fisher_yates(arr, length, std::rand);

        //check the prefix sums
        if(non_negative_prefix_sum(arr, length) ||
           non_positive_prefix_sum(arr, length)) {
            count_valid++;
        }
    }

    double ratio = double(count_valid) / trials;

    //Print ratio and total for user
    cout << "----------\n"
         << "For n=" << n << ":\n"
         << "  Total trials = " << trials << "\n"
         << "  Well-balanced lists = " << count_valid << "\n"
         << "  Ratio = " << ratio << "\n"
         << "----------\n";

    //check that ratio is within a real range:
    CHECK(ratio >= 0.0);
    CHECK(ratio <= 1.0);
}