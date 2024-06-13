#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/smp.h>
#include <asm/sysreg.h>
#include <asm/cpufeature.h>

static uint64_t cpuectlr_val;

static void read_cpuectlr_on_cpu(void *arg)
{
    // Read the CPUECTLR_EL1 register
    cpuectlr_val = read_sysreg_s(sys_reg(3, 0, 15, 1, 4));

    printk("CPUECTLR_EL1 value on CPU %d: 0x%llx\n", smp_processor_id(), cpuectlr_val);
}

static int __init read_cpuectlr_init(void)
{
    int cpu = 4; // CPU4 is A78 Core1
    preempt_disable();
    smp_call_function_single(cpu, read_cpuectlr_on_cpu, NULL, true);
    preempt_enable();
    return 0;
}
late_initcall(read_cpuectlr_init);

static void __exit read_cpuectlr_init_exit(void)
{
    printk(KERN_INFO "Module exiting.\n");
}
module_exit(read_cpuectlr_init_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Read CPUECTLR_EL1 register using smp_call_function_single()");
MODULE_AUTHOR("Const Coccinelle");
