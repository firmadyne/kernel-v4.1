#include <linux/export.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>

#include "stubs.h"
#include "firmadyne.h"

#define STUB_ENTRIES \
	/* Used by "Firmware_TEW-435BRM_0121e.zip" (12973) */ \
	STUB("push_button", read, write, push_dev) \
	STUB("gpio", read, write, gpio_dev)

static ssize_t read(struct file *file, char __user *buf, size_t size, loff_t *ppos) {
	return 0;
}

static ssize_t write(struct file *file, const char __user *data, size_t len, loff_t *ppos) {
	return len;
}

#define STUB(a, b, c, d) \
	static const struct file_operations d = { \
		.owner = THIS_MODULE, \
		.read = b, \
		.write = c, \
	};

	STUB_ENTRIES
#undef STUB

int register_stubs(void) {
	int ret = 0;

#define STUB(a, b, c, d) \
	if (!proc_create_data(a, 0, NULL, &d, NULL)) { \
		remove_proc_entry(a, NULL); \
		printk(KERN_WARNING MODULE_NAME": Cannot register procfs entry: %s!\n", a); \
		ret = -ENODEV; \
	}


	STUB_ENTRIES
#undef STUB

	return ret;
}

void unregister_stubs(void) {
#define STUB(a, b, c, d) \
	remove_proc_entry(a, NULL);

	STUB_ENTRIES
#undef STUB
}
