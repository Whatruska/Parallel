
#include <iostream>
#include <omp.h>

using namespace std;

void print_vars(int a, int b, string prefix)
{
    cout << prefix << endl;
    printf("a=%d, b=%d\n", a, b);
}

int main()
{
    int a = 0;
    int b = 0;

    print_vars(a, b, "before");

#pragma omp parallel num_threads(2) private(a) firstprivate(b)
    {
        cout << "Hello world!\n";
        int thread_num = omp_get_thread_num();
        int a = thread_num;
        int b = thread_num;
        print_vars(a, b, "in first section");
    }
#pragma omp parallel num_threads(4) private(b)
    {
        cout << "Hello world!\n";
        int thread_num = omp_get_thread_num();
        a += thread_num;
        int b = thread_num;
        print_vars(a, b, "in second section");
    }
    print_vars(a, b, "after second section");
}
