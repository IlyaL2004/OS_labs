
#include <vector>
#include <iostream>
#include <string>
#include <pthread.h>
#include <ctime>
using namespace std;

using TMatrix = vector<vector<int>>;

struct thread_args{
	int hgh;
	int wth;
	TMatrix *f_mat;
	TMatrix *s_mat;
	int obl; 
	int l_row;
	int upr_row;
};

void median_filter(int &hgh, int &wth, TMatrix &a, TMatrix &b, int &obl, int &l_row, int &upr_row);
void Work(int &n, int &m, TMatrix &mat1, TMatrix &mat2, int &k, int &win_s, int &threadc, TMatrix &res);
 