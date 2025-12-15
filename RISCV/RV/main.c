#include "RV.h"
#include <windows.h>

int main(void)
{
    int res = app_init();
    if (res)
    {
        printf("[ERROR] EXIT\n");
        return res;
    }

    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq); // Взима честотата на таймера
    QueryPerformanceCounter(&start);  // Старт

    res = app_run();

    QueryPerformanceCounter(&end); // Край
    double elapsed_ms = (end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
    printf("\nELAPSED: %.3f ms\n", elapsed_ms);

    return res;
}