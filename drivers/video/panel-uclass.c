// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 2016 Google, Inc
 * Written by Simon Glass <sjg@chromium.org>
 */

#define LOG_CATEGORY UCLASS_PANEL

#include <common.h>
#include <dm.h>
#include <panel.h>

int panel_enable_backlight(struct udevice *dev)
{
	struct panel_ops *ops = panel_get_ops(dev);

	printf("panel-uclass: ops->enable_backlight: %d\n", ops->enable_backlight);

	if (!ops->enable_backlight)
		return -ENOSYS;

	printf("panel-uclass: ops->enable_backlight: %d\n", ops->enable_backlight(dev));

	return ops->enable_backlight(dev);
}

/**
 * panel_set_backlight - Set brightness for the panel backlight
 *
 * @dev:	Panel device containing the backlight to update
 * @percent:	Brightness value (0=off, 1=min brightness,
 *		100=full brightness)
 * Return: 0 if OK, -ve on error
 */
int panel_set_backlight(struct udevice *dev, int percent)
{
	struct panel_ops *ops = panel_get_ops(dev);

	printf("panel-uclass: ops->set_backlight: %d\n", ops->set_backlight);

	if (!ops->set_backlight)
		return -ENOSYS;

	printf("panel-uclass: ops->set_backlight(dev, percent): %d\n", ops->set_backlight(dev, percent));

	return ops->set_backlight(dev, percent);
}

int panel_get_display_timing(struct udevice *dev, struct display_timing *timings)
{
	struct panel_ops *ops = panel_get_ops(dev);

	printf("panel-uclass: ops->get_display_timing: %d\n", ops->get_display_timing);

	// if (!ops->get_display_timing)
	// 	return -ENOSYS;
	if (ops->get_display_timing != 0)
		return -ENOSYS;

	printf("panel-uclass: ops->get_display_timing(dev, timings): %d\n", ops->get_display_timing(dev, timings));

	return ops->get_display_timing(dev, timings);
}

UCLASS_DRIVER(panel) = {
	.id = UCLASS_PANEL,
	.name = "panel",
};
