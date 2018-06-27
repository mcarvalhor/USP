#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

class Main {

    private: vector<int> Data;

    void print(bool reverse){
        if(!reverse){
            vector<int>::iterator data_search;
            for(data_search = Data.begin(); data_search != Data.end(); data_search++){
                printf("%d\n", *data_search);
            }
        }else{
            vector<int>::reverse_iterator data_search;
            for(data_search = Data.rbegin(); data_search != Data.rend(); data_search++){
                printf("%d\n", *data_search);
            }
        }
    }

    void print(){
        print(false);
    }

    public: Main(){

        int Aux;

        while(scanf("%d", &Aux) > 0){
            Data.push_back(Aux);
        }

        //vector<int>::iterator data_search;

        sort(Data.begin(), Data.end());

        print();

        printf("\n");

        print(true);

    }

};

int main(){

    Main a;

    return 0;
}