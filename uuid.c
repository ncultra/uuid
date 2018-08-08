/*******************************************************************
 * demonstration of in-kernel uuid APIs
 *
 * Copyright (C) 2018  Michael D. Day II
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 *
 *******************************************************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/uuid.h>

#define _MODULE_LICENSE "GPL v2"
#define _MODULE_AUTHOR "Michael D. Day II <ncultra@gmail.com>"
#define _MODULE_INFO "demo of the in-kernel uuid API"

/**
 * DEMONSTRATION
 * 1) generate a random uuid, 16 bytes
 * 2) convert the binary uuid to the proper string format
 * 3) parse the uuid string into a second 16 byte binary uuid
 * 4) compare the original binary uuid to the parsed uuid
 **/

/**
 * see <linux/uuid.h> and <uapi/linux/uuid.h>
 * UUID_SIZE 16
 * UUID_STRING_LEN 36
 * https://en.wikipedia.org/wiki/Universally_unique_identifier
 *
 * Kernel uuids are in big-endian format.
 *
 * see Documentation/printk-formats.txt
 **/


int
module_main(void)
{
	int ccode = 0;
	uuid_t uuid = {0}, parsed_uuid = {0};
	uint8_t uuid_string[UUID_STRING_LEN + 2] = {0};

	/* 1) create a random uuid */
	generate_random_uuid(uuid.b);

	/* 2) convert the binary uuid into a properly formatted string */
	snprintf(uuid_string, UUID_STRING_LEN + 1, "%pUb", &uuid);
	printk(KERN_INFO "UUID: %s\n", uuid_string);

	/* 3) parse the uuid string back into binary form */
    /* first, verify the string is properly formatted */
	if (! uuid_is_valid(uuid_string)) {
		printk(KERN_INFO "UUID string not valid\n");
		return -EINVAL;
	}

	/* now parse the uuid string into binary form */
	if (0 > uuid_parse(uuid_string, &parsed_uuid)) {
		printk(KERN_INFO "UUID parse failed\n");
		return -EINVAL;
	}

	printk(KERN_INFO "UUID parsed_uuid: %pUb\n", &parsed_uuid);

	/* 4) compare the original and parsed binary uuids */
	if (! uuid_equal(&uuid, &parsed_uuid)) {
		printk(KERN_INFO "UUID two uuids not equal\n");
		return -EINVAL;
	}

	return ccode;
}

static int
__init uuid_init(void)
{
	int ccode = module_main();
	if (ccode) {
		printk(KERN_INFO "UUID API demo failed\n");
	}

	return 0;
}

static void
__exit uuid_exit(void)
{
	return;
}

module_init(uuid_init);
module_exit(uuid_exit);

MODULE_LICENSE(_MODULE_LICENSE);
MODULE_AUTHOR(_MODULE_AUTHOR);
MODULE_DESCRIPTION(_MODULE_INFO);
