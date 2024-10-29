#include <bateria.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/logging/log_ctrl.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>

// Esse código foi altamente inspirado no código de sample do zephyr

LOG_MODULE_REGISTER(Bateria);

const struct adc_dt_spec bateria_adc_channel =
    ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 0);
static uint16_t buf;
struct adc_sequence sequence = {
      .buffer = &buf,
      /* buffer size in bytes, not number of samples */
      .buffer_size = sizeof(buf),
  };

int bateria_init() {
  LOG_INF("Inicializando...");


  if (!adc_is_ready_dt(&bateria_adc_channel)) {
    printk("ADC controller device not ready\n");
    return 0;
  }
  return 0;
}

uint32_t bateria_read(){
  log_panic();
    int err;
    LOG_INF("Lendo...");

    int32_t val_mv;

    (void)adc_sequence_init_dt(&bateria_adc_channel, &sequence);

    err = adc_read_dt(&bateria_adc_channel, &sequence);
    if (err < 0) {
      LOG_ERR("Não foi possível ler (%d)", err);
      return err;
    }

    if (bateria_adc_channel.channel_cfg.differential) {
      val_mv = (int32_t)((int16_t)buf);
    } else {
      val_mv = (int32_t)buf;
    }

    printk("%" PRId32, val_mv);

    /* Converte o valor em milivolts*/
    err = adc_raw_to_millivolts_dt(&bateria_adc_channel, &val_mv);
    if (err < 0) {
      LOG_ERR("Não foi possível converter para mV (%d)", err);
      return err;
    }
    return val_mv;
}
