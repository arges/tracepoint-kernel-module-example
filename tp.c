/*
  Example code to hook into a tracepoint via kernel module.
*/

#include <linux/module.h>
#include <linux/tracepoint.h>
#include <linux/sched.h>
#define CREATE_TRACE_POINTS
#include <trace/events/task.h>

static struct tracepoint *tp_task_newtask = NULL;

static void probe_task_newtask(void *unused, struct task_struct *task, unsigned long clone_flags)
{
	printk(KERN_INFO "got task %s %lu\n", task->comm, clone_flags);
}

static void find_tracepoint(struct tracepoint *tp, void *priv)
{
	if (!strcmp(tp->name, "task_newtask"))
		tp_task_newtask = tp;
}

static int __init tp_hello(void)
{
	for_each_kernel_tracepoint(find_tracepoint, tp_task_newtask);

	WARN_ON(tracepoint_probe_register(tp_task_newtask, probe_task_newtask, NULL));
	return 0;
}

static void __exit tp_goodbye(void)
{
	tracepoint_probe_unregister(tp_task_newtask, probe_task_newtask, NULL);
	tracepoint_synchronize_unregister();
}

module_init(tp_hello);
module_exit(tp_goodbye);
MODULE_LICENSE("GPL");
