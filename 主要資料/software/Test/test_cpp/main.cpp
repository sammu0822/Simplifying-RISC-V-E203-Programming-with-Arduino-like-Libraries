#include <cstdio>

class LED {
public:
    LED() {
        printf("LED Constructor\n");
    }
};

LED led_global;

int main() {
    printf("Main Start\n");
    return 0;
}

