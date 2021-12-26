#include <iostream>
using namespace std;

int main(int argc, char **argv){
    int i, N, A, B, C, x, y, z;
    bool Ok = false;
    cin >> N;
    for(i = 0; i < N; i++){
        Ok = false;
        cin >> A >> B >> C;
        for(x = -100; x < 100; x++){
            for(y = -100; y < 100; y++){
                for(z = -100; z < 100; z++){
                    if(x*x+y*y+z*z == C && x*y*z == B && x+y+z==A){
                        Ok = true;
                        goto fim;
                    }
                }
            }
        }
        fim:
        if(Ok)
            printf("%d %d %d\n", x, y, z);
        else
            printf("No solution.\n");
    }
    return 0;
}