#include <iostream>
#include <thread>
#include <stdlib.h>
#include <unistd.h>

using std::cout;
using std::endl;
using std::thread;

int list[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
void binary_search(int id, long search_number) {
        int list_size = sizeof(list) / sizeof(list[0]);
	long tid = (long) std::this_thread::get_id;


        printf("[%d/%ld] buscando %ld...\n", id, tid, search_number);

	sleep(1);

        for (int i = 0; i < list_size; i++) {
                if (list[i] == search_number) {
                        printf("[%d/%ld] Numero %ld encontrado na posicao: %d\n",
                                            id, tid, search_number, i);
                        return;
                }
        }

       printf("[%d/%ld] Numero %ld nao encontrado\n",
                                id, tid, search_number);
}

int main(void) {

	thread t1(binary_search, 1, 12);
	thread t2(binary_search, 2, 28);

	sleep(1);

	t1.join();
	t2.join();

}
