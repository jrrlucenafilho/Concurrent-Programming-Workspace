#include <iostream>
#include <thread>
#include <mutex>

using std::cout;
using std::endl;
using std::thread;
using std::mutex;

class conta_corrente {
	float saldo;
	mutex m;
public:
	conta_corrente(float f):saldo(f) { }

	float get_saldo() {
		return saldo;
	}

	void sacar(float valor) {
		m.lock();
		saldo -= valor;
		m.unlock();

	}

	void depositar(float valor) {
		m.lock();
		saldo += valor;
		m.unlock();
	}
};

int main(void) {
	conta_corrente c1(1000);

	thread t1(&conta_corrente::sacar, &c1, 100);
	thread t2(&conta_corrente::depositar, &c1, 200);
	thread t3(&conta_corrente::sacar, &c1, 250);
	thread t4(&conta_corrente::sacar, &c1, 500);

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	cout << "saldo final: " << c1.get_saldo() << endl;

	return 0;
}
