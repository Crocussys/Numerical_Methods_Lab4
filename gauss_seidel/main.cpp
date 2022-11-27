#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

const int N_DIGITS = 6;

void usage(){
    cout << "gauss_seidel [path_a] [path_b]" << endl << endl;
    cout << "path_a - Файл коэфициентов" << endl;
    cout << "path_b - Файл свободных членов" << endl;
}

int FileError(){
    cout << "Error 2: File opening error" << endl;
    return -2;
}

vector<string> split(string str, char sep){
    string buff = "";
    vector<string> ans = {};
    for (int i = 0; i < str.length(); i++){
        char symb = str[i];
        if (symb == sep){
            ans.push_back(buff);
            buff = "";
        }else{
            buff += symb;
        }
    }
    ans.push_back(buff);
    return ans;
}

long double* str_to_float(vector<string> elems){
    const string SYMBOLS = "-0123456789.";
    int size = elems.size();
    long double* ans = new long double[size];
    for (int k = 0; k < size; k++){
        string str = elems[k];
        for (int i = 0; i < str.length(); i++){
            bool flag = true;
            for (int j = 0; j < SYMBOLS.length(); j++){
                if (str[i] == SYMBOLS[j]){
                    flag = false;
                    break;
                }
            }
            if (flag){
                throw;
            }
        }
        ans[k] = stof(str);
    }
    return ans;
}

bool error(double* a, double* b, int size, double e){
    for (int i = 0; i < size; i++){
        if (abs(a[i] - b[i]) > e){
            return true;
        }
    }
    return false;
}

int main(int argc, char* argv[]){
    if (argc >= 2){
        if (argv[1] == "-h"){
            usage();
            return 0;
        }
    }
    if (argc < 3){
        usage();
        cout << endl << "Error 1: Not enough arguments" << endl;
        return -1;
    }

    int size = 0;
    long double** a;
    long double* b;
    try{
        fstream file(argv[1], ios::in);
        if (!file){
            return FileError();
        }
        string line;
        int count_lines = 0;
        bool flag = true;
        while (!file.eof()){
            getline(file, line);
            count_lines++;
            vector<string> elems_str = split(line, ' ');
            int count_elems = elems_str.size();
            if (size == 0){
                size = count_elems;
            }
            if (count_elems != size){
                throw;
            }
            if (flag){
                flag = false;
                a = new long double*[size];
                for (int i = 0; i < size; i++){
                    a[i] = new long double[size];
                }
            }
            long double* elems = str_to_float(elems_str);
            for (int i = 0; i < size; i++){
                a[count_lines - 1][i] = elems[i];
            }
            delete [] elems;
        }
        file.close();
        if (count_lines != size){
            throw;
        }
        file.open(argv[2], ios::in);
        if (!file){
            return FileError();
        }
        b = new long double[size];
        int count_elems = 0;
        int ind = 0;
        long double* elems;
        while (!file.eof()){
            getline(file, line);
            vector<string> elems_str = split(line, ' ');
            elems = str_to_float(elems_str);
            int size_array = elems_str.size();
            for (int i = 0; i < size_array; i++){
                b[ind + i] = elems[i];
            }
            count_elems += size_array;
            ind = count_elems;
        }
        delete [] elems;
        file.close();
        if (count_elems != size){
            throw;
        }
    }catch (string mess){
        cout << "Error 3: Invalid value" << endl;
        return -3;
    }

    double* x = new double [size];
    double* x_prev = new double [size];
    for (int i = 0; i < size; i++){
        x[i] = 0;
        x_prev[i] = 1;
    }
    double e = pow(10, -N_DIGITS - 1);
    int i = 0;
    int j = 0;
    while (j != size and error(x_prev, x, size, e)){
        j = i % size;
        for (int k = 0; k < size; k++){
            x_prev[k] = x[k];
        }
        x[j] = b[j];
        for (int k = 0; k < size; k++){
            if (k != j){
                x[j] -= a[j][k] * x[k];
            }
        }
        x[j] /= a[j][j];
        i++;
    }

    for (int i = 0; i < size; i++){
        cout << "x" << i + 1 << " = " << x[i] << endl;
    }
    
    delete [] a;
    delete [] b;
    delete [] x;
    delete [] x_prev;

    return 0;
}