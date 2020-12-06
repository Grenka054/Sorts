#include <stdio.h>
#include <stdlib.h>

#define fget(f,x) fread(&x, sizeof(x), 1, f)
#define fput(f,x) fwrite(&x, sizeof(x), 1, f)

void merge(int a1[], int len1, int a2[], int len2, int ar[]) {
/* ������� ��������������� �������� a1 ����� len1 � �2
����� len2 � ������ �r */
	int i = 0, j = 0, k = 0;
	int x = 0;
	while ((i < len1) || (j < len2)) { /* ���� ���� �������� */
		if (i == len1) /* 1-� ���������: ����� �� 2-� */
			x = a2[j++];
		else
			if (j == len2) /* 2-� ���������: ����� �� 1-� */
				x = a1[i++];
			else
				if (a1[i] < a2[j]) /* �������� ���������� */
					x = a1[i++];
				else x = a2[j++];
		ar[k++] = x;
	}
}
const int N = 10;
static int aw[N]; /* ��������������� ���������� ������
��� ������� */

void sort_merging(int a[], int L, int R)
/* L,R - ������� ����������� ����� ������� � */
{
	int i, M;
	M = (L + R) / 2;
	if (L < M)
		sort_merging(a, L, M);
	if (M + 1 < R)
		sort_merging(a, M + 1, R);
	/* ������� ������ � aw */
	merge(&a[L], M - L + 1, &a[M + 1], R - M, &aw[L]);
	/* ����������� � �����.�������� */
	for (i = L; i <= R; i++)
		a[i] = aw[i];
}

int split(FILE* f, FILE* f1, FILE* f2) {
/* ���������� f: �������� ��������� �������� ������� �
f1, ������ - � f2 */
	int x;
	int n = 0; /* ������� ����� ����� */
	rewind(f); /* ������� � ������ ������������ �����*/
	fget(f, x);
	while (!feof(f)) {
		/* (feof ����������� ����� ������� ������!) */
		fput(f1, x);
		fget(f, x);
		if (!feof(f)) {
			fput(f2, x);
			fget(f, x);
		}
		n++;
	}
	return n > 1; /* 0 (����� 0 ��� 1) ������������� �
	����������� ���������� */
}

void merge(FILE* f1, FILE* f2, FILE* fr) {
/* ������� fl � f2 � fr */
	int x1, x2;
	rewind(f1); /* ��������� � ������ ���� ������ */
	rewind(f2);
	rewind(fr);
	fget(f1, x1); fget(f2, x2);
	while (!feof(f1) || !feof(f2)) {
		if (feof(f1)) {
			fput(fr, x2); fget(f2, x2);
		}
		else if (feof(f2)) {
			fput(fr, x1); fget(f1, x1);
		}
		else if (x1 < x2) {
			fput(fr, x1);
			fget(f1, x1);
		}
		else {
			fput(fr, x2);
			fget(f2, x2);
		}
	}
}

void sort_merge(FILE* f) {
/* ������� ��������� ����������, ������� ���� ������
���� ������ */
	/* �������� ��������� ������ ��� ������ */
	FILE* f1 = tmpfile(),
		* f2 = tmpfile();
	/* ���������� �� ���� ������ � �������� */
	if (split(f, f1, f2)) {
		sort_merge(f1);
		sort_merge(f2);
	}
	/* ������� �� ���� �������� �� �������� */
	merge(f1, f2, f);
	/* �������� � �������� ������� ������ */
	fclose(f1);
	fclose(f2);
}

int main() {
	FILE* fp;
	FILE* fopen(const char* name, const char* mode);
	if ((fp = fopen("C:\\my_prog.txt", "rt")) == NULL) {
		fprintf(stderr, "������� ���� �� �������\n");
		exit(1);
	}
	sort_merge(fp); /* ���������� ��������� ����� */
	fclose(fp);
	return 0;
}