#include "bateria.h"
#include "zephyr/kernel.h"
#include "zephyr/logging/log.h"

LOG_MODULE_REGISTER();

int main(){
  bateria_init();
  int val;
  while (true) {
    val = bateria_read();
    LOG_INF("%d mV", val);
    k_msleep(100);
  }
}
