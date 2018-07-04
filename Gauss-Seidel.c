#include<stdio.h>
#include<math.h>
#include<string.h>
//#include<stlib.h>
#include<stdlib.h>

#define EPS 1e-15			//最大許容誤差
#define MAX_size 10			//行列の最大サイズ
#define K_MAX 300			//誤差が一定値以下にならなかった場合の最大反復回数

void input_matrix(double *p_ans);
void input_solution(void);
void check_input(void);
void output(double *p_ans);

int size = 0;					//利用者が求める係数行列のサイズ格納

double ans[MAX_size];			//解析結果を格納
double n[MAX_size][MAX_size];	//係数行列格納
double m[MAX_size];				//解を格納
double *p_ans = ans;
double x_tmp, eps_tmp;			//過去数値(x)　相対誤差算出用, 算出した誤差を格納
double sum = 0;

int i,j;						//i=列,j=行
int k = 0;						//処理の繰り返し回数

int main(void)
{
	input_matrix(p_ans);

/********************演算********************/

	do {
		x_tmp = ans[0];	 			  //誤差算出用に前回のxを格納

		for(i = 0; i < size; i++, sum = 0){

			for(j = 0; j < size; j++){

				if(i != j){
					sum += n[i][j]*ans[j];
				}
			}

			ans[i] = (m[i]-sum)/n[i][i];
			//printf("ans_%d = %lf\n", i+1, ans[i]);	//デバッグ用
		}

		eps_tmp = fabs( (x_tmp-ans[0])/ans[0] );		//これ以降の処理で一々再計算させたくないので値を保存
		k++;							//反復回数

	} while(eps_tmp >= EPS && k < K_MAX );

	output(p_ans);

	return 0;
}

/********************入力********************/
/**********係数行列入力*********/
void input_matrix(double *p_ans)
{
	int i,j;
	printf("start Gauss-Seidel program.\n\n");
	printf("係数行列（正方行列）のサイズはいくつですか.\n->");
	scanf("%d", &size);

	if(size > MAX_size){
		printf("\nerror! 大きすぎます。やり直してください。\n");
		exit(1);
	}

	for(i = 0; i < size; i++){	//初期化
		p_ans[i] = 0;
	}

	printf("\n例に示す番号の順に要素を入力してください.\n\n");
	printf("\t\t例\n\t\t|1 2|\n\t\t|3 4|\n");

	for(i = 0, j = 0; i < size; j++){

		if(j == size){		//折り返し

			j = 0;
			i++;
		}

		if(i != size){

			printf("%d行%d列目 = ", i+1, j+1);
			scanf("%lf", &n[i][j]);

			if(i == j && n[i][j] == 0){		//無限大に発散することが明らかであるのでプログラムを終了
				printf("\nerror! 対角成分に0が含まれています。収束しません。終了します。\n\n");
				exit(1);
			}
		}

	}
	input_solution(); 
}

/**********解入力*********/
void input_solution(void)
{
	int i;
	printf("\n解を入力してください.\n"    );

	for(i = 0; i < size; i++){

		scanf("%lf",&m[i]);
	}
	check_input();
}

/********************入力チェック********************/
void check_input(void)
{
	int fg = 0;
	int i, j;

	printf("\n入力された行列を確認します。\n");
	for(i = 0, j = 0, sum = 0; i < size; j++){

		if(j == size){				//折り返し＆後ろ整形

			if(fabs(n[i][i]) <= sum){	//対角優位判定
				fg = 1;
			}

			j = 0;
			sum = 0;
			i++;
			printf("| %g|",m[i-1]);
		}

		if(j == 0 && i != size)	printf("\n |");	//前整形

		else printf(" ");						//字間

		if(i != size)	printf("%g", n[i][j]);

		if(i != j)	sum += fabs(n[i][j]);	//対角成分以外の和
	}

	printf("\n\n");

	if(fg){
		printf("対角優位ではありません。収束しない可能性があります。続行しますか？\n");
		printf("Yes : 0, No : 1を入力してください\n");
		scanf("%d", &fg);

		switch(fg){

			case 0:
				printf("\n続行します。\n");
				break;
			case 1:
				printf("\n終了します。\n");	//不正な入力もこちらに流れる。はず。
				exit(1);
		}

	}

}

/********************出力********************/
void output(double *p_ans)
{
	int i;
	printf("解析結果を少数以下6桁に丸めて表示します。\n\n");

	for(i = 0; i < size; i++){	
		printf("ans_%d = %.6lf\n", i+1, p_ans[i]);			//演算結果の表示
	}

	printf("\n%d回演算を行いました。", k);					//演算回数の表示

	if(eps_tmp < EPS)	printf("誤差はおよそ%.6lfです。\n", eps_tmp);	//収束した場合誤差の表示

	else	printf("収束しませんでした。\n");				//収束しなかった旨を表示
}
