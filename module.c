#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>
#include "sys_hook.h"
#include "hooks.h"

struct sys_hook *fpc_sys_hook;

/* Module parameter macros */
static char *fpc_pathname = "";
module_param(fpc_pathname, charp, 0);
MODULE_PARM_DESC(fpc_pathname, "the path to be protected");

/* it doesnt' work on some platform because of the page fault */
unsigned long *
get_sys_call_table(unsigned int syscall_nr, unsigned long syscall_addr)
{
    unsigned long *base = (unsigned long *)(syscall_addr - 64*0x100000);
    unsigned long *end = (unsigned long *)(syscall_addr + 64*0x100000);
    unsigned long *entry;

    for (entry = base;entry < end ; entry += 1) {

        if (entry[syscall_nr] == syscall_addr) {
            printk("entry at %lx, %pK, syscall addr %lx, mem addr %lx\n",
                    (unsigned long)entry, entry, syscall_addr, entry[syscall_nr]);
            return entry;
        }
    }

    return NULL;
}

static int __init
module_entry(void)
{
    unsigned long syscall_close_addr = 0;
    unsigned long syscall_table_addr = 0;

    printk(KERN_INFO "fpc initializing...\n");

    syscall_close_addr = kallsyms_lookup_name("sys_close");

    if (0 == syscall_close_addr) {
        printk(KERN_INFO "failed to get syscall close addr\n");
        return -1;
    }

    //printk(KERN_INFO"sys_close at %lx\n", syscall_close_addr);

    //syscall_table_addr = get_sys_call_table(__NR_close, syscall_close_addr);
    /* it will fail on some platform which doesn't export the sct */
    syscall_table_addr = kallsyms_lookup_name("sys_call_table");

    if (0 == syscall_table_addr) {
        printk(KERN_INFO "failed to get x86 syscall table\n");
        return -1;
    }
    //printk(KERN_INFO "sys_close at %lx, table at %lx\n",
    //        syscall_close_addr, syscall_table_addr);

    if ((fpc_sys_hook = sys_hook_init(0, (uintptr_t)syscall_table_addr)) == NULL) {
        printk(KERN_INFO "failed to initialize sys_hook\n");
        return -1;
    }

    sys_hook_add64(fpc_sys_hook, __NR_mkdir, (void *)mkdir_hook);
    sys_hook_add64(fpc_sys_hook, __NR_unlink, (void *)unlink_hook);
    sys_hook_add64(fpc_sys_hook, __NR_unlinkat, (void *)unlinkat_hook);

    printk(KERN_INFO "fpc loaded\n");
    return 0;
}

static void __exit
module_cleanup(void)
{
    sys_hook_free(fpc_sys_hook);
    printk(KERN_INFO "fpc has finished\n");
}

/* Declare the entry and exit points of our module */
module_init(module_entry);
module_exit(module_cleanup);

/* Shut up kernel warnings about tainted kernels due to non-free software */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Just");
MODULE_DESCRIPTION("File Permission Control Hook");
