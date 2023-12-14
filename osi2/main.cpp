#include "lab.h"
#include <ctime>

int main(int argc, char* argv[]){
    
    int n, m, k, threadc, win_s;
    TMatrix res;
    cout << "Потоки или поток (количество): ";
    cin >> threadc;
    cout << "Размеры матрицы: ";
    cin >> n >> m;
    TMatrix mat1(n, vector<int>(m));
    TMatrix mat2(n, vector<int>(m));
    if (threadc > n) 
        threadc = n;
	printf("Размер окна: ");
	cin >> win_s;
    printf("Медианные фильтры(количество): ");
	cin >> k;
	printf("Матрица:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> mat1[i][j];
        }
    }
    //unsigned int start_time =  clock(); // начальное время
    Work(n, m, mat1, mat2, k, win_s, threadc, res);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << res[i][j] << " ";
        }
        cout << "\n";
    }
    //unsigned int end_time = clock(); // конечное время
    //unsigned int search_time = end_time - start_time; // искомо
    //cout << search_time << "endl";
    return 0;
}