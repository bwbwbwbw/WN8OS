#include <kernel.h>

Terminal terminal;

extern "C"
void kernel_main() {
  terminal.init();
  terminal.printf("WN8OS kernel\n");
}