/*
 * Simple cross-correlation implementation
 *
 * iter 1:
 *  A:         -3  2 -1  1
 *  B:-1  0 -3  2
 *  R:          6
 *                      R[0] = 6
 * iter 2:
 *  A:         -3  2 -1  1
 *  B:   -1  0 -3  2
 *  R:          9  4
 *                     R[1]  = 9 + 4 =13
 * iter x:
 *     ...
 *
 * Where A is first arr, B - second and R is result.
 * Result: {-6, 13, -8, 8, -5, 1, -1}
 *
 * ref: https://www.allaboutcircuits.com/technical-articles/understanding-correlation/
 */
#include <stdio.h>
#include <stdlib.h>

#define ARR_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#define CROSS_RESULT_SIZE(arr) ((ARR_SIZE(arr) * 2) - 1)

void xcorr(const int A[], const int B[], int arr_size, int result[]) {
    int idx, sum;
    int max_lag = arr_size - 1;

    for (int i = -max_lag; i <= max_lag; i++) {
        sum = 0;

        for (int j = 0; j < arr_size; j++) {
            idx = i + j;
            if (idx >= 0 && idx < arr_size)
                sum  += A[idx] * B[j];
        }
        result[i + max_lag] = sum;
    }
}

int main() {
    int A[] = {-3, 2, -1, 1};
    int B[] = {-1, 0, -3, 2};
    int result[CROSS_RESULT_SIZE(A)] = {0};

    if (ARR_SIZE(A) != ARR_SIZE(B)) {
        printf("Array size must be equal");
        exit(1);
    }

    xcorr(A, B, ARR_SIZE(A), result);

    for (int i = 0; i < ARR_SIZE(result); i++)
        printf("result[%d] = %d\n", i, result[i]);

    return 0;
}