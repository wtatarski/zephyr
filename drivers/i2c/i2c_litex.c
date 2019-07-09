/*
 * Copyright (c) 2019 Antmicro <www.antmicro.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <device.h>
#include <drivers/i2c.h>
#include "i2c_bitbang.h"

#define SCL_BIT_POS                0
#define SDA_DIR_BIT_POS            1
#define SDA_BIT_W_POS              2
#define SDA_BIT_R_POS              0

#define SDA_DIR_OUTPUT             1
#define SDA_DIR_INPUT              0

#define HIGH_STATE_ON_I2C_LINES    0x7

struct i2c_litex_cfg {
	struct i2c_bitbang bitbang;
	volatile u32_t *w_reg;
	volatile u32_t *r_reg;
};

#define GET_I2C_CFG(dev)						     \
	((struct i2c_litex_cfg *) dev->config->config_info)

static inline void set_bit(volatile u32_t *reg, u32_t bit, u32_t val)
{
	u32_t mask = (1 << bit);
	*reg = ((*reg) & ~mask) | ((!!val) << bit);
}

static inline int get_bit(volatile u32_t *reg, u32_t bit)
{
	u32_t mask = (1 << bit);

	return !!((*reg) & mask);
}

void i2c_litex_bitbang_set_scl(void *context, int state)
{
	struct i2c_litex_cfg *config = (struct i2c_litex_cfg *) context;

	set_bit(config->w_reg, SCL_BIT_POS, state);
}

void i2c_litex_bitbang_set_sda(void *context, int state)
{
	struct i2c_litex_cfg *config = (struct i2c_litex_cfg *) context;

	set_bit(config->w_reg, SDA_DIR_BIT_POS, SDA_DIR_OUTPUT);
	set_bit(config->w_reg, SDA_BIT_W_POS, state);
}

int i2c_litex_bitbang_get_sda(void *context)
{
	struct i2c_litex_cfg *config = (struct i2c_litex_cfg *) context;

	set_bit(config->w_reg, SDA_DIR_BIT_POS, SDA_DIR_INPUT);
	return get_bit(config->r_reg, SDA_BIT_R_POS);
}

static const struct i2c_bitbang_io i2c_litex_bitbang_io = {
	.set_scl = i2c_litex_bitbang_set_scl,
	.set_sda = i2c_litex_bitbang_set_sda,
	.get_sda = i2c_litex_bitbang_get_sda,
};

int i2c_litex_init(struct device *dev)
{
	struct i2c_litex_cfg *config = GET_I2C_CFG(dev);

	*(config->w_reg) |= HIGH_STATE_ON_I2C_LINES;
	i2c_bitbang_init(&config->bitbang, &i2c_litex_bitbang_io, config);

	return 0;
}

int i2c_litex_configure(struct device *dev, u32_t dev_config)
{
	struct i2c_litex_cfg *config = GET_I2C_CFG(dev);

	return i2c_bitbang_configure(&config->bitbang, dev_config);
}

int i2c_litex_transfer(struct device *dev,  struct i2c_msg *msgs,
		       u8_t num_msgs, u16_t addr)
{
	struct i2c_litex_cfg *config = GET_I2C_CFG(dev);

	return i2c_bitbang_transfer(&config->bitbang, msgs, num_msgs, addr);
}

static const struct i2c_driver_api i2c_litex_driver_api = {
	.configure         = i2c_litex_configure,
	.transfer          = i2c_litex_transfer,
	.slave_register    = NULL,
	.slave_unregister  = NULL,
};

/* Device Instantiation */

#define I2C_LITEX_INIT(n)						       \
	static struct i2c_litex_cfg i2c_litex_cfg_##n = {		       \
		.w_reg =						       \
		 (volatile u32_t *) DT_INST_##n##_LITEX_I2C_WRITE_BASE_ADDRESS,\
		.r_reg =						       \
		 (volatile u32_t *) DT_INST_##n##_LITEX_I2C_READ_BASE_ADDRESS, \
	};								       \
									       \
	DEVICE_AND_API_INIT(litex_i2c_##n,				       \
			   DT_INST_##n##_LITEX_I2C_LABEL,		       \
			   i2c_litex_init,				       \
			   NULL,					       \
			   &i2c_litex_cfg_##n,				       \
			   POST_KERNEL,					       \
			   CONFIG_I2C_INIT_PRIORITY,			       \
			   &i2c_litex_driver_api			       \
			   )

#ifdef DT_INST_0_LITEX_I2C_LABEL
I2C_LITEX_INIT(0);
#endif

#ifdef DT_INST_1_LITEX_I2C_LABEL
I2C_LITEX_INIT(1);
#endif

#ifdef DT_INST_2_LITEX_I2C_LABEL
I2C_LITEX_INIT(2);
#endif

#ifdef DT_INST_3_LITEX_I2C_LABEL
I2C_LITEX_INIT(3);
#endif

#ifdef DT_INST_4_LITEX_I2C_LABEL
I2C_LITEX_INIT(4);
#endif

#ifdef DT_INST_5_LITEX_I2C_LABEL
I2C_LITEX_INIT(5);
#endif

#ifdef DT_INST_6_LITEX_I2C_LABEL
I2C_LITEX_INIT(6);
#endif

#ifdef DT_INST_7_LITEX_I2C_LABEL
I2C_LITEX_INIT(7);
#endif

#ifdef DT_INST_8_LITEX_I2C_LABEL
I2C_LITEX_INIT(8);
#endif
