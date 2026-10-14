#include <stdio.h>
#include <dok/dok_window.h>

int main() {
    DokWindow window;
    printf("PE");
    dokCreateWindow("PEPE", 930, 200, &window);
    while (true) {
        dokPollWindow(window);
    }
    return 0;
}