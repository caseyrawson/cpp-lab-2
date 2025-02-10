#include "prefixSum.h"

bool non_negative_prefix_sum(int lst[], int len) {
    int sum = 0;
    for(int i = 0; i < len; i++) {
        sum += lst[i];
        if(sum < 0) return false;
    }
    return true;
}

bool non_positive_prefix_sum(int lst[], int len) {
    int sum = 0;
    for(int i = 0; i < len; i++) {
        sum += lst[i];
        if(sum > 0) return false;
    }
    return true;
}
