/*
 * Copyright (c) 2023 Cypress Semiconductor Corporation (an Infineon company) or
 * an affiliate of Cypress Semiconductor Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>

#include <zephyr/device.h>
#include <zephyr/drivers/counter.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

struct counter_alarm_cfg alarm_cfg;

#define COUNTER_1  DEVICE_DT_GET(DT_NODELABEL(counter1_0))
#define PULSE_PIN  1
#define PULSE_NODE DEVICE_DT_GET(DT_NODELABEL(gpio_prt9))

static struct counter_top_cfg counter_top;
static int counter_top_userdata;

static void cb_counter_top(const struct device *dev, void *user_data)
{
	int *p_data = (int *)user_data;

	int err;

	printk("Counter top reached. Resetting Count\n");
	printk("Top Count = %d\n", *p_data);
	*p_data = *p_data + 1;

	counter_top.ticks = counter_top.ticks + 20;

	err = counter_set_top_value(dev, &counter_top);
	if (err != 0) {
		printk("Counter settings failed with error code %d\n", err);
	}
}

static void cb_alarm(const struct device *counter_dev, uint8_t chan_id, uint32_t ticks,
		     void *user_data)
{
	struct counter_alarm_cfg *p_alarm = (struct counter_alarm_cfg *)user_data;

	int err;

	printk("!! Alarm !!\n");
	printk("!! Count = %u\n", ticks);

	p_alarm->ticks = ticks + 10;

	err = counter_set_channel_alarm(counter_dev, 0, p_alarm);
	if (err != 0) {
		printk("Alarm settings failed with error code %d\n", err);
	}
}

static const struct gpio_dt_spec pulse = {PULSE_NODE, PULSE_PIN, GPIO_ACTIVE_HIGH};

void main(void)
{
	const struct device *p_dev_counter1 = COUNTER_1;

	int err;

	if (!device_is_ready(p_dev_counter1)) {
		printk("Counter 1 is not ready.\n");
		return;
	}

	if (!gpio_is_ready_dt(&pulse)) {
		printk("Pulse Pin is not ready.\n");
		return;
	}

	err = gpio_pin_configure_dt(&pulse, GPIO_OUTPUT_ACTIVE);
	if (err < 0) {
		return;
	}

	printk("Pulse Counter Sample\n\n");

	uint32_t ticks = 0;

	counter_start(p_dev_counter1);

	counter_top.ticks = 100;
	counter_top.callback = cb_counter_top;
	counter_top.user_data = (void *)&counter_top_userdata;
	counter_top.flags = COUNTER_TOP_CFG_RESET_WHEN_LATE;

	err = counter_set_top_value(p_dev_counter1, &counter_top);
	if (err != 0) {
		printk("Counter settings failed with error code %d\n", err);
	}

	alarm_cfg.flags = 0;
	alarm_cfg.ticks = 10;
	alarm_cfg.callback = cb_alarm;
	alarm_cfg.user_data = &alarm_cfg;

	err = counter_set_channel_alarm(p_dev_counter1, 0, &alarm_cfg);
	printk("Set alarm in %u count\n", alarm_cfg.ticks);
	if (err != 0) {
		printk("Alarm settings failed with error code %d\n", err);
	}

	while (1) {

		if (counter_get_value(p_dev_counter1, &ticks) != 0) {
			printk("Error in reading values\n");
		} else {
			printk("Count = %u\n", ticks);
		}

		gpio_pin_toggle_dt(&pulse);

		k_msleep(500);
	}
}
