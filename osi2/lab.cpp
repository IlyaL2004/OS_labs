#include "lab.h"

#include <ctime>


void median_filter(int &hgh, int &wth, TMatrix &a, TMatrix &b, int &obl, int &l_row, int &upr_row){
	int rad = (obl - 1) / 2;
	for (int y = l_row; y < upr_row; y++){
		int top;
		if ((y-rad) > 0)
			top = y - rad;
		else
			top = 0;
		int bottom;
		if ((y+rad) < 0)
			bottom = y+rad;
		else
			bottom = hgh - 1;
		for (int x = 0; x < wth; x++){
			int left;
			if ((x- rad) > 0)
				left = x -rad;
			else
				left = 0;
			int right;
			if((x+rad) < (wth - 1))
				right = x + rad;
			else
				right = wth - 1;
			vector <int> m((bottom - top + 1) * (right - left + 1));
			int k = 0;
			for (int v = top; v <= bottom; v++){
				for (int u = left; u <= right; u++){
					m[k] = a[v][u];
					k++;
				}
			}
    		int n = m.size();
				for(int i = 0; i < n; i++){
        			for(int j = i; j < n; j++){
            			if(m[i] > m[j]){
               				int t = m[i];
               				m[i] = m[j];
               				m[j] = t;
            			}
        			}
    			}
			b[y][x] = m[n / 2];
		}
	}
}

void *liner(void *args){
	thread_args *arg = (thread_args*) args;
	median_filter(arg->hgh, arg->wth, *arg->f_mat, *arg->s_mat, arg->obl, arg->l_row, arg->upr_row);
	return NULL;
}


void Work(int &n, int &m, TMatrix &mat1, TMatrix &mat2, int &k, int &win_s, int &threadc, TMatrix &res){
	int imed = n / threadc;
    pthread_t threads[threadc];
	thread_args p_args[threadc];


	for (int i  = 0; i < k; i++){
		for(int j = 0; j < threadc; j++){
			p_args[j].hgh = n;
			p_args[j].wth = m;

			if (i % 2 == 0){
				p_args[j].f_mat = &mat1;
				p_args[j].s_mat = &mat2;
			}
			else{
				p_args[j].f_mat = &mat2;
				p_args[j].s_mat = &mat1;
			}
			p_args[j].obl = win_s;
			p_args[j].l_row = j * imed;
			p_args[j].upr_row = (j + 1) * imed;
			if (j == threadc - 1)
				p_args[j].upr_row = n;
		}
		for (int i = 0; i < threadc; i++){
			int res = pthread_create(&threads[i], NULL, liner, &p_args[i]);
			if (res != 0){
				cout << "Ошибка";
				exit(-2);
			}
		}
		for (int i = 0; i < threadc; i++){
			int res = pthread_join(threads[i], NULL);
			if (res != 0){
				cout << "Ошибка";
				exit(-3);
			}
		}
		printf("\n");
	}
	if (k % 2 == 1){
		res = mat2;}
	else {
		res = mat1;}

}
