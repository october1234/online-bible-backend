#include <string>
#include <iostream>

void abcd(void* a) {
    std::string s((char*) a);
    std::cout << s << std::endl;
}

int main() {
    char a[] = "hello";
    abcd(a);
}
