#include <iostream>
#include <exception>  // logic_error
#include <cstring>    // memcpy
#include <ctime>      // clock
#include <cstdlib>    // rand
#include <cmath>      // pow

using namespace std;

inline void swap( int& a, int& b )
{
    int tmp = a; a = b; b = tmp;
}


void sort_bubble( int* arr, size_t size )
{
    for (int i{ 0 }; i < size - 1; ++i) {
        for (int j{ 0 }; j < size - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void sort_shaker( int* arr, size_t size )
{
    int right{ int(size - 1) }, left{ 0 };
    for (int i{ 0 }; i < size - 1; ++i) {
        for (int j{ left }; j < right; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
        right--;
        for (int j{ right }; j >= left; --j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
        left++;
    }
}

void sort_select( int* arr, size_t size )
{
    int min{ 0 }; //ћинимальный элемент несортированной части
    for (int i{ 0 }; i < size; ++i) {
        for (int j{ i }; j < size; ++j) {
            if (arr[j] < arr[min])
                min = j;
        }
        swap(arr[min], arr[i]);
        min = size - 1;
    }
}

void sort_insert_lin( int* arr, size_t size )
{
    int k { 0 }; //¬ременннна€ переменна€, хран€ща€ индексы
    for (int i{ 1 }; i < size; ++i) {
        k = i;
        for (int j{ k - 1 }; j >= 0; --j) {
            if (arr[k] < arr[j]) {
                swap(arr[j], arr[k]);
                --k;
            }
        }
    }
}

void sort_insert_bin( int* arr, size_t size )
{
    int left{ 0 }, right{ 0 }, //Ћева€ и права€ границы
        k{ 0 }; //¬ременннна€ переменна€, хран€ща€ индексы
    for (int i{ 1 }; i < size; ++i) {
        right = i;
        left = 0;
        while (left < right) {
            if (arr[(left + right) / 2] <= arr[i]) {
                left = (left + right) / 2 + 1;
            }
            else {
                right = (left + right) / 2 - 1;
            }
        }
        k = i;
        for (int j{ k - 1 }; j >= left; --j) {
            if (arr[k] < arr[j]) {
                swap(arr[j], arr[k]);
                --k;
            }
        }
    }
}

void sort_shell_div2( int* arr, size_t size )
{
    //ѕозици€ дл€ temp
    int j{ 0 };
    //Ќачинаем с большого промежутка, затем его уменьшаем
    //gap - шаг расчЄски
    for (int gap{ int(size) / 2 }; gap > 0; gap /= 2) {
        //»дем по элементам от a[gap] и правее
        for (int i{ gap }; i < size; ++i) {
            // сохран€ем [i] в temp и делаем отверстие в позиции i
            int temp = arr[i];
            //сдвигаем ранее отсортированные элементы до тех пор,
            //пока не будет найдено правильное местоположение дл€ a[i]
            j = i;
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap; //Ўаг gap влево
            }
            //помещаем temp в правильное место
            arr[j] = temp;
        }
    }
}

void sort_shell_pow3( int* arr, size_t size )
{
    //ѕозици€ дл€ temp
    int j{ 0 };
    //Ќачинаем с большого промежутка, затем его уменьшаем
    int gap = pow(size, (float)2 / 3); //шаг расчЄски
    while ( true ) {
        //»дем по элементам от a[gap] и правее
        for (int i{ gap }; i < size; ++i) {
            // сохран€ем [i] в temp и делаем отверстие в позиции i
            int temp = arr[i];
            //сдвигаем ранее отсортированные элементы до тех пор,
            //пока не будет найдено правильное местоположение дл€ a[i]
            j = i;
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap; //Ўаг gap влево
            }
            //помещаем temp в правильное место
            arr[j] = temp;
        }
        if (gap == 1) break;
        gap = pow(gap, (float)2 / 3);
    }
}


void siftDown(int* arr, int root, size_t size) {
    int maxChild{ 0 }; //индекс максимального потомка
    bool done{ false }; //флаг, построено ли дерево
    while (root * 2 <= size - 1 && !done) { //пока не вышли за пределы массива и дерево не построено
        if (root * 2 == size - 1) //если мы в последнем р€ду
            maxChild = size - 1; //запоминаем один единственный потомок
        else if (arr[root * 2] > arr[root * 2 + 1]) //иначе запоминаем больший потомок из двух
            maxChild = root * 2;
        else
            maxChild = root * 2 + 1;
        if (arr[root] < arr[maxChild]) { //если элемент вершины меньше максимального потомка, мен€ем местами
            swap(arr[root], arr[maxChild]);
            root = maxChild;
        }
        else
            done = 1; // пирамида сформирована
    }
}

void sort_heap( int* arr, size_t size )
{
    for (int i{ (int)size / 2 - 1 }; i >= 0; i--) {
        siftDown(arr, i, size - 1);
    }
    for (int i{ (int)size - 1 }; i >= 1; i--) {
        swap(arr[0], arr[i]);
        siftDown(arr, 0, i);
    }
}

void sort_quick_rec(int* arr, size_t size)
{
    if (size > 1) {
        int pivot = arr[size - 1];
        int i = -1;
        //ѕроходим по массиву, сдвигаем маленькие влево и считаем их кол-во
        for (int j{ 0 }; j < size - 1; ++j) {
            if (arr[j] < pivot) {
                swap(arr[++i], arr[j]);
            }
        }
        //ѕоставим пилотный элемент сразу же после маленьких
        swap(arr[i + 1], arr[size - 1]);
        
        //Ћева€ часть
        sort_quick_rec(arr, i + 1);
        //ѕрава€ часть
        sort_quick_rec(arr + i + 2, size - i - 2);
    }
}

void sort_quick_nonrec( int* arr, size_t size )
{
    
}

void sort_merge( int* arr, size_t size )
{

}

///////////////////////////////////////////////////////////////////////////////

static clock_t timer_start_time{ 0 }, timer_stop_time{ 0 };
void start_timer()
{
    timer_start_time = clock();
}
void stop_timer()
{
    timer_stop_time = clock();
    cout << double( timer_stop_time - timer_start_time )/CLOCKS_PER_SEC << " sec" << endl;
}

bool array_is_sorted( const int* arr, size_t size )
{
    for ( size_t i{ 1 }; i < size; ++i ) if ( arr[i-1] > arr[i] ) return false;
    return true;
}

void exec_sort( const int* arr_unsorted, size_t size, void (*sort_function)( int*, size_t ), const char* comment )
{
    cout << endl << comment << endl;
    int* arr = new int[size];
    memcpy( arr, arr_unsorted, size*sizeof( int ) );
    start_timer();
    sort_function( arr, size );
    stop_timer();
    if ( !array_is_sorted( arr, size ) ) throw logic_error( "FAILURE" );
    delete[] arr;
}

int main()
{
    try
    {
        size_t size{ 0 }; // size{ 5 };
        int *arr_unsorted{ nullptr };

        if ( !size )
        {
            cout << "Array size?" << endl;
            if ( !( cin >> size ) || size <= 0 || size > 10000000 ) throw logic_error( "WRONG ARRAY SIZE" );

            arr_unsorted = new int[size];
            srand( unsigned( time( nullptr ) ) );
            for ( size_t i = 0; i < size; arr_unsorted[i++] = rand() % size );
        }
        else
        {
            cout << "Array size:" << endl << size << endl;
            arr_unsorted = new int[5]{ 5, 4, 3, 2, 1 };
        }

//        exec_sort( "Quick nonrecursive:"arr_unsorted, size,  sort_quick_nonrec );
        exec_sort(arr_unsorted, size, sort_quick_rec, "Quick:" );
        exec_sort( arr_unsorted, size,   sort_heap, "Heap:" );
        exec_sort(arr_unsorted, size,  sort_shell_div2, "Shell:");
        exec_sort( arr_unsorted, size,  sort_shell_pow3, "Shell:" );
//        exec_sort( arr_unsorted, size,  sort_merge, "Merge:" );
        exec_sort( arr_unsorted, size, sort_insert_bin, "InsertBin:" );
        exec_sort( arr_unsorted, size, sort_insert_lin, "InsertLin:" );
        exec_sort( arr_unsorted, size, sort_select, "Select:" );
        exec_sort( arr_unsorted, size, sort_shaker, "Shaker:" );
        exec_sort( arr_unsorted, size, sort_bubble, "Bubble:" );

        delete[] arr_unsorted;
    }
    catch ( const logic_error& e )
    {
        cerr << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
