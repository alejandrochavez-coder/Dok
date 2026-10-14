#include <stdio.h>
#include <dok/dok_window.h>

int main() {
    DokWindow window;
    printf("PE");
    dokCreateWindow("PEPE", 930, 200, &window);
    while (dokWindowExists(window)) {
        dokPollWindow(window);
    }
    return 0;
}