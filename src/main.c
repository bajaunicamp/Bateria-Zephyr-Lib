#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/logging/log.h>

#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
	!DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "No suitable devicetree overlay specified"
#endif

const struct adc_dt_spec adc_channel = ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 0);

LOG_MODULE_REGISTER();
int main(){
  LOG_INF("Inicializando");

  int err;
	uint16_t buf;
	struct adc_sequence sequence = {
        .buffer = &buf,
        /* buffer size in bytes, not number of samples */
        .buffer_size = sizeof(buf),
      };

    if (!adc_is_ready_dt(&adc_channel)) {
			printk("ADC controller device not ready\n");
			return 0;
    }
  while (true) {

    printk("ADC reading");

    int32_t val_mv;

    (void)adc_sequence_init_dt(&adc_channel, &sequence);

    err = adc_read_dt(&adc_channel, &sequence);
			if (err < 0) {
				printk("Could not read (%d)\n", err);
				continue;
    }

    if (adc_channel.channel_cfg.differential) {
            val_mv = (int32_t)((int16_t)buf);
        } else {
            val_mv = (int32_t)buf;
        }

    printk("%"PRId32, val_mv);

    /* Converte o valor em milivolts*/
        err = adc_raw_to_millivolts_dt(&adc_channel, &val_mv);
        if (err < 0) {
            printk(" (value in mV not available)\n");
        } else {
            printk(" = %"PRId32" mV\n", val_mv);
        }

        /* Espera 1 segundo antes da próxima leitura */
        k_sleep(K_MSEC(1000));
    }
    return 0;
}
