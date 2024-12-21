#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("Andrii Pozdniakov <pozniakov12334@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training!");
MODULE_LICENSE("Dual BSD/GPL");

static uint print_count = 1;
module_param(print_count, uint, 0444);
MODULE_PARM_DESC(print_count, "Number of times to print 'Hello, world!'");

struct hello_event {
    struct list_head list;
    ktime_t time;
};

static LIST_HEAD(hello_list);

static int __init hello_init(void)
{
    int i;
    struct hello_event *event;

    if (print_count == 0 || (print_count >= 5 && print_count <= 10)) {
        printk(KERN_WARNING "Warning: print_count is 0 or between 5 and 10\n");
    }

    if (print_count > 10) {
    BUG_ON(1); // Аварійне завершення, якщо print_count > 10
    }


    for (i = 0; i < print_count; i++) {
    if (i == 4) { // Примусова помилка для 5-го повідомлення
        event = NULL;
    } else {
        event = kmalloc(sizeof(*event), GFP_KERNEL);
    }

    if (!event) {
        printk(KERN_ERR "Failed to allocate memory for hello_event at iteration %d\n", i);
        continue; // Продовжуємо без додавання до списку
    }

    event->time = ktime_get();
    list_add_tail(&event->list, &hello_list);

    printk(KERN_EMERG "Hello, world!\n");
    }


    return 0;
}

static void __exit hello_exit(void)
{
    struct hello_event *event, *tmp;

    list_for_each_entry_safe(event, tmp, &hello_list, list) {
        printk(KERN_INFO "Event time: %llu ns\n", ktime_to_ns(event->time));
        list_del(&event->list);
        kfree(event);
    }
}

module_init(hello_init);
module_exit(hello_exit);

