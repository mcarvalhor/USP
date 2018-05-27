#include <iostream>
#include <vector>

using namespace std;


typedef struct {
    int v;
    bool a;
} number;




void array_insert(vector<int> el, vector< vector<int> > *to){
    unsigned int i, j, nto, nel;
    bool valid = true;
    nto = (*to).size();
    for(i = 0; i < nto; i++){
        nel = (*to)[i].size();
        if(nel != el.size())    continue;
        for(j = 0; j < nel; j++){
            if(el[j] != (*to)[i][j])   goto end_continue;
        }
        valid = false;
        break;
        end_continue: ;
    }
    if(valid){
        (*to).push_back(el);
    }
}

void combine(vector<number> *numbers, vector< vector<int> > *results, int t, int pos){
    unsigned int i, nn;
    int sum;
    nn = (*numbers).size();
    vector<int> aux = vector<int>();
    for(i = sum = 0; i < nn; i++){
        if((*numbers)[i].a){
            sum += (*numbers)[i].v;
            aux.push_back((*numbers)[i].v);
        }
    }
    if(sum == t && aux.size() > 0){
        array_insert(aux, results);
    }
    if(aux.size() >= nn)  return;
    for(i = pos; i < nn; i++){
        if((*numbers)[i].a == false){
            (*numbers)[i].a = true;
            combine(numbers, results, t, pos+1);
            (*numbers)[i].a = false;
        }
    }
}

void print_results(vector< vector<int> > results){
    unsigned int n, nr, i, j;
    n = results.size();
    for(i = 0; i < n; i++){
        nr = results[i].size();
        if(nr <= 0) continue;
        cout << results[i][0];
        for(j = 1; j < nr; j++){
            cout << "+" << results[i][j];
        }
        cout << "\n";
    }
    if(n <= 0)  cout << "NONE\n";
}

int main(int argc, char **argv){
    unsigned int n, t, i;
    cin >> t >> n;
    while(n > 0 && t > 0){
        vector<number> numbers = vector<number>();
        vector< vector<int> > results = vector< vector<int> >();
        for(i = 0; i < n; i++){
            number aux;
            aux.a = false;
            cin >> aux.v;
            numbers.push_back(aux);
        }
        cout << "Sums of " << t << ":\n";
        combine(&numbers, &results, t, 0);
        print_results(results);
        cin >> t >> n;
    }
    return 0;
}