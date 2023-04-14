#include <stdio.h>
#include <stdlib.h>
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MAX_TERMS 101


struct {
	float coef;
	int expon;
} terms[MAX_TERMS] = { {8,3},{7,1},{1,0},{10,3},{3,2},{1,0} };	//0~2: A다항식, 3~5: B 다항식
int avail = 6;	//A,B 의 총 항의 갯수

char compare(int a, int b)
{
	if (a > b) return '>';
	else if (a == b)return '=';
	else return '<';
}

//새로운 항을 기존 다항식 배열에 추가한다.
void attach(float coef, int expon)
{
	if (avail > MAX_TERMS) {
		fprintf(stderr, "항의 개수가 너무 많음\n");
		exit(1);
	}
	terms[avail].coef = coef;
	terms[avail++].expon = expon;
}

void poly_add2(int As, int Ae, int Bs, int Be, int *Cs, int *Ce)
{
	float tempcoef;
	*Cs = avail;
	while (As <= Ae && Bs <= Be)
		switch (compare(terms[As].expon, terms[Bs].expon))
		{
		case '>':
			attach(terms[As].coef, terms[As].expon);
			As++;	break;
		case '=':
			tempcoef = terms[As].coef + terms[Bs].coef;
			if (tempcoef)
				attach(tempcoef, terms[As].expon);
			As++; Bs++; break;
		case '<':
			attach(terms[Bs].coef, terms[Bs].expon);
			Bs++;	break;
		default:
			break;
		}

	for (; As <= Ae; As++)
		attach(terms[As].coef, terms[As].expon);
	for (; Bs <= Be; Bs++)
		attach(terms[Bs].coef, terms[Bs].expon);
	*Ce= avail - 1;

}

void main()
{
	int Cs, Ce;
	poly_add2(0, 2, 3, 5, &Cs,&Ce);
	for (int i = 6; i <= Ce; i++)
		printf("%f   ", terms[i].coef);
}