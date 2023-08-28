#include <iostream>
#include <thread>
#include <unistd.h>
//UNFINISHED

using std::cout;
using std::endl;

int list[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

void binary_search(int id, long search_number) //Can use non-voids
{
    int list_size = sizeof(list) / sizeof(list[0]); //Pointer arithmetic to get size of array
    long tid = (long) std::this_thread::get_id;

    printf("[%d/%ld] buscando %ld...", )
}