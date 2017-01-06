/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright (c) 2017 Richard Yao. All rights reserved.
 */

/*
 * This is a sample program intended to work with the datto_dev Linux kernel
 * module for my programming interview with Datto. It takes a string and a path
 * to a device. It then attempts to open the device and execute an ioctl
 * passing the first character of the input to the device.
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

int
main(int argc, char *argv[])
{
	int fd;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s character path\n", argv[0]);
		return (0);
	}

	fd = open(argv[2], O_RDWR);
	if (fd == -1) {
		char *error;
		(void) asprintf(&error, "failed to open %s", argv[2]);

		perror(error);
		free(error);

		return (errno);
	}

	ioctl(fd, argv[1][0]);

	close(fd);

	return (EXIT_SUCCESS);
}
