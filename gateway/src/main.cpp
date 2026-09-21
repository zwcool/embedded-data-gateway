#include "version.cpp"
#include "iostream"

int main() {
    std::cout << "Embedded Gateway " << gateway::get_version() << " starting up...\n";

    return 0;
}