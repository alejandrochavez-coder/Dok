#pragma once

#include <stdlib.h>

typedef struct DokVector {
    size_t capacity;
    size_t length;
    void* data;
} DokVector;

#define dokVectorCast(T, pVectorData) ((T*)(pVectorData))

#define dokCreateVector(T, pOut)\
    do {\
        (pOut)->data = calloc(0, sizeof(T));\
        (pOut)->capacity = 0;\
        (pOut)->length = 0;\
    } while(0)

#define dokVectorReserve(T, additional, vector)\
    do {\
        size_t neededCapacity = vector.length + additional;\
        size_t capacity = vector.capacity;\
        if (capacity < neededCapacity) {\
            break;\
        }\
        capacity = (capacity == 0) ? 1 : capacity;\
        while (neededCapacity > capacity) { \
            capacity *= 2;\
        }\
        void* realocated = realloc(vector.data, sizeof(T) * capacity);\
        if (realocated) {\
            vector.data = realocated;\
            vector.capacity = capacity;\
        }\
    } while(0)

#define dokVectorPush(T, vector, value) \
    do {\
        dokVectorReserve(T, 1, vector);\
        dokVectorCast(T, vector.data)[vector.length++] = value;\
    } while(0)

#define dokVectorPop(vector) \
    do {\
        if (vector.length > 0) {\
            vector.length--;\
        }\
    } while(0)

#define dokVectorRemove(T, vector, index) \
    do {\
        dokVectorCast(T, vector.data)[index] = dokVectorCast(T, vector.data)[vector.length - 1];\
        dokVectorPop(vector);\
    } while(0)

#define dokDestroyVector(vector) free(vector.data)