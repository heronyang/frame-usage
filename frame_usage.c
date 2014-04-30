/*
 * OSDI - Lab 7
 *
   Three different states:
1.  The frame usage of process before malloc
2.  The frame usage of process before access memory 
3.  The frame usage of process when access the memory
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/pagemap.h>

static int frame_usage_init(void) {

    struct task_struct *task;
    struct mm_struct *mm;
    struct vm_area_struct *vma;
    unsigned long address;
    unsigned long sum;
    unsigned long vmas;
    unsigned long rss;

    pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;
	spinlock_t *ptl;

    printk("[frame_usage] frame_usage start\n");
    for_each_process(task) {

        if( task == NULL )  break;
        if( task->mm == NULL )  continue;

        if( strcmp(task->comm, "reclim-me")==0 ){
            printk("[frame_usage] get process (%d) : %s\n", task->pid, task->comm);

            mm = task->mm;
            rss = get_mm_rss(mm);
            printk("rss = %lu\n", rss);

            sum = 0;
            vmas = 0;
            for( vma=mm->mmap ; vma!=NULL ; vma=vma->vm_next) {
                for( address=vma->vm_start ; address<vma->vm_end ; address += PAGE_SIZE ){

                    pgd = pgd_offset(mm, address);
                    if (!pgd_present(*pgd)) continue;

                    pud = pud_offset(pgd, address);
                    if (!pud_present(*pud)) continue;

                    pmd = pmd_offset(pud, address);
                    if (!pmd_present(*pmd)) continue;

                    pte = pte_offset_map(pmd, address);

                    ptl = pte_lockptr(mm, pmd);
                    spin_lock(ptl);
                    if (pte_present(*pte)) {
                        sum ++;
                    }
                    pte_unmap_unlock(pte, ptl);

                }
                
                vmas ++;
            }
            printk("sum = %lu, vmas = %lu\n", sum, vmas);
        }
    }

    return 0;
}

static void frame_usage_exit(void) {
    printk("[frame_usage] frame_usage end\n");
}

module_init(frame_usage_init);
module_exit(frame_usage_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Showing Frame Usage");
