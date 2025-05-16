#include <iostream>
#include <utility>

using namespace std;

void print() {
    cout << '\n';
}

template<typename... Ts>
void print(Ts&&... args) {
    ((cout << forward<Ts>(args) << ' '), ...);
    cout << '\n';
}


int main() {

    print();

    print(9);

    print("Rosario", "Diogo", "Francisco");

    print(5, 1.25, string("Hello world"));

    int x = 10;
    print(x, move(x), x + 5);

    return 0;
}
