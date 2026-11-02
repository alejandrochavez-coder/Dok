#define DOK_VECTOR_IMPLEMENTATION
#include <dok/dok_vector.h>

#include <stdio.h>

int main() {

    DokVector vector;
    dokCreateVector(int, &vector);
    dokVectorPush(int, vector, 5);
    dokVectorPush(int, vector, 3);

    for (int i = 0; i < vector.length; i++) {
        printf("%d\n", dokVectorCast(int, vector.data)[i]);
    }

    dokDestroyVector(vector);
    return 0;
}