#include <stdio.h>
#include<stdlib.h>
#define ROWS 3
#define COLS 3
#define MAX_TERMS 25

typedef struct {
	int row;
	int col;
	int value;
}element;

typedef struct SparseMatrix {
	element data[MAX_TERMS];

	int rows;	//행의 갯수
	int cols;	//열의 갯수
	int terms;	//항의 갯수
}SparseMatrix;

//행렬 출력함수
void print(SparseMatrix a)
{
	//배열의 인덱스를 가리키는 변수
	int ac = 0;

	//행렬을 출력하는 반복문
	for (int i = 0; i < a.rows*a.cols; i++)
	{
		//행렬에 값이 있는 term은 값을 출력하고 없으면 0을 출력한다.
		if (a.data[ac].row*a.cols + a.data[ac].col == i)
		{
			printf("%4d ", a.data[ac++].value);
			if (ac > a.terms)
				ac = 0;
		}
		else
			printf("   0 ");
		//한 행을 전부 출력하면 줄바꿈 처리하여 행렬을 보기 쉽게 한다.
		if (i%a.cols == a.cols - 1)
			printf("\n\n");
	}
}

//전치행렬을 반환하는 함수
SparseMatrix transpose(SparseMatrix a)
{
	//a 의 전치행렬을 반환할 구조체 at 선언
	SparseMatrix at;

	at.cols = a.rows;
	at.rows = a.cols;
	at.terms = a.terms;

	//배열접근을 위한 index 변수
	int ac = 0;

	for (int i = 0; i < a.cols; i++)
		for (int j = 0; j < at.terms; j++)
		{
			//행렬의 i열에 해당하는 term을 찾아 전치시킨다.
			if (a.data[j].col == i) {
				at.data[ac].row = a.data[j].col;
				at.data[ac].col = a.data[j].row;
				at.data[ac].value = a.data[j].value;
				ac++;
			}
		}

	return at;
}

//행렬곱을 게산하는 함수
SparseMatrix sparse_matrix_product(SparseMatrix a, SparseMatrix b)
{
	//행렬곱 계산 결과를 받고 반환하는 행렬c 선언
	SparseMatrix c;
	int ca = 0, cb = 0, cc = -1;	//각 배열의 항목을 가리키는 인덱스


	if (a.cols != b.rows) {
		fprintf(stderr, "희소행렬 크기에러--> 1번째 행렬의 열과 2번째 행렬의 행의 크기가 다릅니다.\n");
		exit(1);
	}

	//행렬 n*m 크기의 행렬 a와 m*k 크기의 행렬 b의 계산결과는 n*k 크기의 행렬이다.
	c.rows = a.rows;	//n
	c.cols = b.cols;	//k
	c.terms = 0;


	int ctemp;

	//c 행렬의 계산결과 저장을 위한 배열 d 생성 각 항목의 값은 a행렬과 b 행렬의 
	//계산결과가 누적합되어 저장된다.
	int *d;
	d = (int*)calloc(c.rows*c.cols, sizeof(int));

	//행렬곱 계산
	for (int i = 0; i < a.terms; i++)
		for (int j = 0; j < b.terms; j++)
		{
			//행렬곱의 계산 조건 1의 열과 2의 행 번호가 같을때
			if (a.data[i].col == b.data[j].row)
			{
				//곱한 값을 C 배열에서의 행과 열의 요소를 순차적인 번호로 계산해 누적합 저장
				ctemp = a.data[i].row*b.cols + b.data[j].col;
				d[ctemp] += a.data[i].value*b.data[j].value;
			}
		}

	//d의 계산결과를 c 행렬에 옮긴다.
	for (int i = 0; i < c.rows*c.cols; i++)
	{
		if (d[i] != 0)
		{
			//순차적으로 번호를 매긴걸 행과 열의 요소로 분리하여 저장
			c.data[++cc].row = i / c.cols;
			c.data[cc].col = i % c.cols;
			c.data[cc].value = d[i];
		}
	}
	//다 사용한 d는 메모리해제를 해준다.
	free(d);

	//cc의 값은 c 행렬에 값이 존재하는 항목의 갯수이다.
	c.terms = cc;

	return c;
}

int main()
{
	//A 행렬 입력받기
	int Arows, Acols;
	printf("A행렬의 행과 열의 크기를 입력하세요: ");
	scanf("%d %d", &Arows, &Acols);

	int Aterms;
	printf("A 행렬의 희소행렬의 개수를 입력하세요: ");
	scanf("%d", &Aterms);

	SparseMatrix *sparse;
	sparse = (SparseMatrix*)malloc(Aterms * sizeof(SparseMatrix));
	sparse->cols = Acols;
	sparse->rows = Arows;
	sparse->terms = Aterms;
	printf("희소행렬의 요소를 입력하세요 (행, 열, 값):\n");
	for (int i = 0; i < Aterms; i++) {
		printf("희소행렬 요소 %d: ", i + 1);
		scanf("%d %d %d", &sparse->data[i].row, &sparse->data[i].col, &sparse->data[i].value);
	}

	//A의 전치행렬과 계산될 B 행렬 입력받기
	printf("A의 전치행렬과 계산될 행렬 B 행렬을 입력 받겠습니다.\n");
	int Brows, Bcols;
	printf("B행렬의 행과 열의 크기를 입력하세요: ");
	scanf("%d %d", &Brows, &Bcols);

	int Bterms;
	printf("B 행렬의 희소행렬의 개수를 입력하세요: ");
	scanf("%d", &Bterms);

	SparseMatrix *B;
	B = (SparseMatrix*)malloc(Bterms * sizeof(SparseMatrix));
	B->cols = Bcols;
	B->rows = Brows;
	B->terms = Bterms;
	printf("희소행렬의 요소를 입력하세요 (행, 열, 값):\n");
	for (int i = 0; i < Bterms; i++) {
		printf("희소행렬 요소 %d: ", i + 1);
		scanf("%d %d %d", &B->data[i].row, &B->data[i].col, &B->data[i].value);
	}

	SparseMatrix At;		//A의 전치행렬을 저장
	SparseMatrix m3;		//A의 전치행렬과 B행렬의 곱 결과를 저장
	At = transpose(*sparse);

	printf("A 행렬 출력\n");
	print(*sparse);
	printf("\nA의 전치행렬 출력\n");
	print(At);
	printf("\nB 행렬 출력\n");
	print(*B);

	//사용이 끝난 sparse 메모리를 해제
	free(sparse);

	m3 = sparse_matrix_product(At, *B);
	printf("\n\nA의 전치행렬과 행렬 B의 곱 연산결과\n");
	print(m3);




}
