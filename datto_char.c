/*
 * Copyright (C) 2017 Richard Yao
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <asm/uaccess.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/device.h>

#define	DATTO_DRIVER "datto_char"

int uninitialized = 1;
char byte;

static ssize_t
datto_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	int i;

	if (uninitialized)
		return (-EINVAL);

	/*
	 * We ought to do fewer copy_to_user() invocations, but this works for
	 * a toy module.
	 */
	for (i = 0; i < len; i++) {
		if (copy_to_user(buffer + i, &byte, 1))
			return (-EFAULT);
	}

	return (len);
}

static long
datto_ioctl(struct file *filp, unsigned cmd, unsigned long arg)
{
	uninitialized = 0;
	byte = (char) cmd;
	return (0);
}

#ifdef CONFIG_COMPAT
static long
datto_compat_ioctl(struct file *filp, unsigned cmd, unsigned long arg)
{
	return (datto_ioctl(filp, cmd, arg));
}
#else
#define	datto_compat_ioctl NULL
#endif


static struct file_operations datto_fops =
{
	.unlocked_ioctl	= datto_ioctl,
	.compat_ioctl	= datto_compat_ioctl,
	.read		= datto_read,
	.owner		= THIS_MODULE,
};

static struct miscdevice datto_misc = {
	.minor		= MISC_DYNAMIC_MINOR,
	.name		= DATTO_DRIVER,
	.fops		= &datto_fops,
	.mode		= S_IRWXU | S_IRWXG | S_IRWXO
};

int
init_module(void)
{
	int error;

	error = misc_register(&datto_misc);
	if (error)
		return (error);

	return (0);
}

void
cleanup_module(void)
{
	misc_deregister(&datto_misc);
}



MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard Yao");
MODULE_DESCRIPTION("Kernel module for Datto Interview.");
MODULE_VERSION("1.0");
