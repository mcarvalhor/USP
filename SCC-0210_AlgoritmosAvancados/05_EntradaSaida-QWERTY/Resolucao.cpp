#include <map>
#include <iostream>
#include <algorithm>
using namespace std;

class Main {

    private: map<char, char> Decode = {
        {'w', 'q'},
        {'s', 'a'},
        {'x', 'z'},
        {'e', 'w'},
        {'d', 's'},
        {'c', 'x'},
        {'r', 'e'},
        {'f', 'd'},
        {'v', 'c'},
        {'t', 'r'},
        {'g', 'f'},
        {'b', 'v'},
        {'y', 't'},
        {'h', 'g'},
        {'n', 'b'},
        {'u', 'y'},
        {'j', 'h'},
        {'m', 'n'},
        {'i', 'u'},
        {'o', 'i'},
        {'k', 'j'},
        {'p', 'o'},
        {'W', 'Q'},
        {'S', 'A'},
        {'X', 'Z'},
        {'E', 'W'},
        {'D', 'S'},
        {'O', 'I'},
        {'C', 'X'},
        {'R', 'E'},
        {'F', 'D'},
        {'V', 'C'},
        {'T', 'R'},
        {'G', 'F'},
        {'B', 'V'},
        {'Y', 'T'},
        {'H', 'G'},
        {'N', 'B'},
        {'U', 'Y'},
        {'J', 'H'},
        {'M', 'N'},
        {'I', 'U'},
        {'K', 'J'},
        {',', 'M'},
        {'P', 'O'},
        {'[', 'P'},
        {';', 'L'},
        {']', '['},
        {'\\', ']'},
        {'\'', ';'},
        {'.', ','},
        {'/', '.'},
        {' ', ' '},
        {'\n', '\n'}
    };

    public: Main(){

        char Aux;

        while( (Aux=getchar()) != EOF )
            printf("%c", Decode[Aux]);

    }

};

int main(){

    Main a;

    return 0;
}