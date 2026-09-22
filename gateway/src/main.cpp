#include "iostream"
#include "version.cpp"

int main() {
    std::cout << "Embedded Gateway " << gateway::get_version() << " starting up...\n";

    return 0;
}