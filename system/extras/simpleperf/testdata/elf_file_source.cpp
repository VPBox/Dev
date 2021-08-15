#include <pthread.h>

volatile int GlobalVar;

extern "C" void CalledFunc() {
  GlobalVar++;
}

extern "C" void GlobalFunc() {
  for (int i = 0; i < 1000000; ++i) {
    CalledFunc();
  }
}

int main() {
  while (true) {
    GlobalFunc();
  }
  return 0;
}
