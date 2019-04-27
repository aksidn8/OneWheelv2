#include "esp_system.h"

#include <driver/dac.h>
#include <driver/ledc.h>

#include "motor_controller.h"

static void IRAM_ATTR gpio_isr_handler(void* arg) {
    ets_printf("Hallo");
}

void configure_brake() {
    ledc_timer_config_t timer_config = {
        .duty_resolution = LEDC_TIMER_10_BIT,
        .freq_hz = 500,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_num = LEDC_TIMER_0
    };
    ledc_timer_config(&timer_config);

    ledc_channel_config_t channel_config = {
        .channel = LEDC_CHANNEL_0,
        .duty = 0,
        .gpio_num = 16,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .hpoint = 0,
        .timer_sel = LEDC_TIMER_0
    };
    ledc_channel_config(&channel_config);

    ledc_fade_func_install(0);
}

void configure_hall_interrupts() {
    gpio_config_t hall_conf;
    hall_conf.intr_type = GPIO_PIN_INTR_POSEDGE;
    hall_conf.mode = GPIO_MODE_INPUT;
    hall_conf.pull_down_en = 0;
    hall_conf.pull_up_en = 0;
    hall_conf.pin_bit_mask = (
        (1ULL << PIN_HALL_BLUE) | 
        (1ULL << PIN_HALL_YELLOW) | 
        (1ULL << PIN_HALL_GREEN)
    );
    gpio_config(&hall_conf);

    gpio_install_isr_service(0);

    int hall_pins[3] = {PIN_HALL_BLUE, PIN_HALL_GREEN, PIN_HALL_YELLOW};
    for (int i = 0; i <= 2; i++) {
        gpio_isr_handler_add(hall_pins[i], gpio_isr_handler, (void*) hall_pins[i]);
    }
}

void motor_controller_init() {
    dac_output_enable(DAC_OUT);

    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << PIN_REVERSE);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    gpio_set_level(PIN_REVERSE, 0);
    configure_brake();
    configure_hall_interrupts();
}

void set_brake(uint32_t strength) {
    ledc_set_duty_and_update(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, strength, 0);
}