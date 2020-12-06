#include <stdio.h>
#include <stdlib.h>

#define fget(f,x) fread(&x, sizeof(x), 1, f)
#define fput(f,x) fwrite(&x, sizeof(x), 1, f)

void merge(int a1[], int len1, int a2[], int len2, int ar[]) {
/* Слияние отсортированных массивов a1 длины len1 и а2
длины len2 в массив аr */
	int i = 0, j = 0, k = 0;
	int x = 0;
	while ((i < len1) || (j < len2)) { /* пока есть элементы */
		if (i == len1) /* 1-я кончилась: берем из 2-й */
			x = a2[j++];
		else
			if (j == len2) /* 2-я кончилась: берем из 1-й */
				x = a1[i++];
			else
				if (a1[i] < a2[j]) /* выбираем наименьший */
					x = a1[i++];
				else x = a2[j++];
		ar[k++] = x;
	}
}
const int N = 10;
static int aw[N]; /* вспомогательный глобальный массив
для слияния */

void sort_merging(int a[], int L, int R)
/* L,R - границы сортируемой части массива а */
{
	int i, M;
	M = (L + R) / 2;
	if (L < M)
		sort_merging(a, L, M);
	if (M + 1 < R)
		sort_merging(a, M + 1, R);
	/* слияние частей в aw */
	merge(&a[L], M - L + 1, &a[M + 1], R - M, &aw[L]);
	/* копирование в исход.фрагмент */
	for (i = L; i <= R; i++)
		a[i] = aw[i];
}

int split(FILE* f, FILE* f1, FILE* f2) {
/* Разделение f: перепись элементов нечетных позиций в
f1, четных - в f2 */
	int x;
	int n = 0; /* счетчик длины файла */
	rewind(f); /* возврат к началу разделяемого файла*/
	fget(f, x);
	while (!feof(f)) {
		/* (feof срабатывает ПОСЛЕ попытки чтения!) */
		fput(f1, x);
		fget(f, x);
		if (!feof(f)) {
			fput(f2, x);
			fget(f, x);
		}
		n++;
	}
	return n > 1; /* 0 (длина 0 или 1) сигнализирует о
	прекращении разделения */
}

void merge(FILE* f1, FILE* f2, FILE* fr) {
/* Слияние fl и f2 в fr */
	int x1, x2;
	rewind(f1); /* перемотка к началу всех файлов */
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
/* Главная процедура сортировки, входной файл должен
быть открыт */
	/* создание временных файлов для частей */
	FILE* f1 = tmpfile(),
		* f2 = tmpfile();
	/* разделение на фазе спуска в рекурсию */
	if (split(f, f1, f2)) {
		sort_merge(f1);
		sort_merge(f2);
	}
	/* слияние на фазе возврата из рекурсии */
	merge(f1, f2, f);
	/* закрытие и удаление рабочих файлов */
	fclose(f1);
	fclose(f2);
}

int main() {
	FILE* fp;
	FILE* fopen(const char* name, const char* mode);
	if ((fp = fopen("C:\\my_prog.txt", "rt")) == NULL) {
		fprintf(stderr, "Открыть файл не удалось\n");
		exit(1);
	}
	sort_merge(fp); /* сортировка открытого файла */
	fclose(fp);
	return 0;
}