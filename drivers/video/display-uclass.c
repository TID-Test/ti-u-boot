// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2014 Google Inc.
 */

#define LOG_CATEGORY UCLASS_DISPLAY

#include <common.h>
#include <dm.h>
#include <display.h>
#include <edid.h>
#include <errno.h>

int display_read_edid(struct udevice *dev, u8 *buf, int buf_size)
{
	struct dm_display_ops *ops = display_get_ops(dev);

	if (!ops || !ops->read_edid)
		return -ENOSYS;
	return ops->read_edid(dev, buf, buf_size);
}

int display_enable(struct udevice *dev, int panel_bpp,
				   const struct display_timing *timing)
{
	struct dm_display_ops *ops = display_get_ops(dev);
	struct display_plat *disp_uc_plat;
	int ret;

	printf("display-uclass: display_enable\n");

	if (!ops || !ops->enable)
	{
		printf("display-uclass: err !ops || !ops->enable\n");
		return -ENOSYS;
	}
	ret = ops->enable(dev, panel_bpp, timing);
	if (ret)
	{
		printf("display-uclass: err ops->enable(dev, panel_bpp, timing)\n");
		return ret;
	}

	disp_uc_plat = dev_get_uclass_plat(dev);
	disp_uc_plat->in_use = true;

	return 0;
}

static bool display_mode_valid(void *priv, const struct display_timing *timing)
{
	struct udevice *dev = priv;
	struct dm_display_ops *ops = display_get_ops(dev);

	printf("display-uclass: display mode valid?\n");

	if (ops && ops->mode_valid)
		return ops->mode_valid(dev, timing);

	printf("display-uclass: display mode valid!!\n");

	return true;
}

int display_read_timing(struct udevice *dev, struct display_timing *timing)
{
	struct dm_display_ops *ops = display_get_ops(dev);
	int panel_bits_per_colour;
	u8 buf[EDID_EXT_SIZE];
	int ret;

	printf("display-uclass: read timing\n");

	if (ops && ops->read_timing)
		return ops->read_timing(dev, timing);

	printf("display-uclass: read timing 1\n");

	if (!ops || !ops->read_edid)
		return -ENOSYS;

	printf("display-uclass: read timing 2\n");

	ret = ops->read_edid(dev, buf, sizeof(buf));
	if (ret < 0)
		return ret;

	printf("display-uclass: read timing OK\n");

	return edid_get_timing_validate(buf, ret, timing,
									&panel_bits_per_colour,
									display_mode_valid, dev);
}

bool display_in_use(struct udevice *dev)
{
	struct display_plat *disp_uc_plat = dev_get_uclass_plat(dev);

	printf("display-uclass: display in use\n");

	return disp_uc_plat->in_use;
}

UCLASS_DRIVER(display) = {
	.id = UCLASS_DISPLAY,
	.name = "display",
	.per_device_plat_auto = sizeof(struct display_plat),
};
