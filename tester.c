#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "allocator.h"

void fill_pattern(void* ptr, size_t size) {
    unsigned char* byte_ptr = (unsigned char*)ptr;
    for (size_t i = 0; i < size; i++) {
        byte_ptr[i] = (unsigned char)(i % 256);
    }
}

void test_malloc_free() {
    printf("Running test_malloc_free...\n");
    int* agr = (int*)malloc(10 * sizeof(int));
    if(!agr) {
        printf("malloc failed. \n");
        return;
    }
    fill_pattern(agr, 10 * sizeof(int));

    for (int i = 0; i < 10; i++) {
        if (agr[i] != i) {
            printf("Failed at %d\n", i);
            free(agr);
            return;
        }
    }
    free(agr);
    printf("Test pass.\n");
}

void test_allocations() {
    printf("Running test_allocations...\n");
    int* agr[10];
    for (int i = 0; i < 10; i++) {
        if (!agr[i]) {
            printf("Failed at %d\n", i);
            return;
        }
        fill_pattern(agr, 10 * sizeof(int));
    }

    for (int i = 0; i < 10; i++) {
        free(agr);
    }
    free(agr);
    printf("Test pass.\n");
}

void test_interleaved_and_free() {
printf("Running test_interleaved_and_free...\n");
    int* agr[10];
    for (int i = 0; i < 5; i++) {
        agr[i] = (int*)malloc(10 * sizeof(int));
        fill_pattern(agr[i], 10 * sizeof(int));
    }
    for (int i = 0; i < 5; i++) {
        free(agr[i]);
    }

    for (int i = 0; i < 5; i++) {
        agr[i] = (int*)malloc(10 * sizeof(int));
        fill_pattern(agr[i], 10 * sizeof(int));
    }
    for (int i = 0; i < 5; i++) {
        free(agr[i]);
    }
    free(agr);
    printf("Test passed.\n");
}

void test_realloc() {
    printf("Running test_realloc...\n");
    int* agr = (int*)malloc(5 * sizeof(int));
    if(!agr) {
        printf("malloc failed. \n");
        return;
    }

    fill_pattern(agr, 5 * sizeof(int));
    agr = (int*)realloc(agr, 10 * sizeof(int));
    if(!agr) {
        printf("realloc failed. \n");
        return;
    }

    fill_pattern(agr + 5, 5 * sizeof(int));
    for (int i = 0; i < 10; i++) {
        if (!agr[i]) {
            printf("Failed at %d\n", i);
            free(agr[i]);
            return;
        }
    }
    free(agr);
    printf("Test passed.\n");
}

int main(int argc, char* argv[]) {
    if (argc == 3 && strcmp(argv[1], "-t") == 0) {
        int test_number = atoi(argv[2]);
        switch (test_number) {
            case 1: test_malloc_free(); break;
            case 2: test_allocations(); break;
            case 3: test_interleaved_and_free(); break;
            case 4: test_realloc(); break;
            default:
                printf("Unknown test number.\n");
                break;
        }
    } else {
        test_malloc_free();
        test_allocations();
        test_interleaved_and_free();
        test_realloc();
    }
    return 0;
}