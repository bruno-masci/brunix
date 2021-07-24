#include <stdint.h>     // for uint32_t

#include <asm/mmu.h>
#include <asm/memlayout.h>
#include <asm/page.h>
#include <asm/paging.h>
#include <asm/processor.h>

#include <brunix/defs.h>
#include <brunix/string.h>  // for memset()
#include <brunix/kernel.h>  // for panic()

#include "brunix/kmalloc.h"


extern char data[];  // defined by kernel.ld
pde_t *kpgdir;  // for use in scheduler()


void * alloc_empty_page(void);
pde_t * pgdir_entry(pde_t *pgdir, const void *va);
pte_t * pgtable_entry(pte_t *pgtable, const void *va);
pte_t * pgtable_base(pde_t *pde);
void set_pgdir_entry(pde_t *pde, pte_t *pgtab);
void switchkvm(void);
pde_t * setupkvm(void);
void kvmalloc(void);
int mappages(pde_t *pgdir, void *va, uint32_t size, uint32_t pa, uint32_t perm);
pte_t * walkpgdir(pde_t *pgdir, const void *va, int alloc);

// Set up CPU's kernel segment descriptors.
// Run once on entry on each CPU.
//void
//seginit(void)
//{
//    struct cpu *c;
//
//    // Map "logical" addresses to virtual addresses using identity map.
//    // Cannot share a CODE descriptor for both kernel and user
//    // because it would have to have DPL_USR, but the CPU forbids
//    // an interrupt from CPL=0 to DPL=3.
//    c = &cpus[cpuid()];
//    c->gdt[SEG_KCODE] = SEG(STA_X|STA_R, 0, 0xffffffff, 0);
//    c->gdt[SEG_KDATA] = SEG(STA_W, 0, 0xffffffff, 0);
//    c->gdt[SEG_UCODE] = SEG(STA_X|STA_R, 0, 0xffffffff, DPL_USER);
//    c->gdt[SEG_UDATA] = SEG(STA_W, 0, 0xffffffff, DPL_USER);
//    lgdt(c->gdt, sizeof(c->gdt));
//}





// Return the address of the PTE in page table pgdir
// that corresponds to virtual address va.  If alloc!=0,
// create any required page table pages.
//static
pde_t * pgdir_entry(pde_t *pgdir, const void *va) {
    return &pgdir[PDX(va)];
}

pte_t * pgtable_entry(pte_t *pgtable, const void *va) {
    return &pgtable[PTX(va)];
}

pte_t * pgtable_base(pde_t *pde) {
    return (pde_val(pde) & PTE_P) ?
           (pte_t *) PHYS_TO_VIRT(PTE_ADDR(pde_val(pde))) :
           NULL;
}

void set_pgdir_entry(pde_t *pde, pte_t *pgtab) {
    // The permissions here are overly generous, but they can
    // be further restricted by the permissions in the page table
    // entries, if necessary.
//    printk("pgtab=%x - pgtab=%x - V2P pgtab=%x\n", pgtab, VIRT_TO_PHYS(pgtab));
    pde_set(pde, VIRT_TO_PHYS(pgtab) | PTE_P | PTE_W | PTE_U);
}

void * alloc_empty_page(void) {
    void *ptr = NULL;

    if((ptr = kmalloc()))
        memset(ptr, 0, PAGE_SIZE);

    return ptr;
}

pte_t * walkpgdir(pde_t *pgdir, const void *va, int alloc) {
    if (va < (void *) KERN_BASE)
        panic("va < KERN_BASE");

    if (pgdir < (pde_t *) KERN_BASE)
        panic("pgdir < KERN_BASE");

    pde_t *pde = pgdir_entry(pgdir, va);
    pte_t *pgtab = pgtable_base(pde);
//    printk("SS pgtab=%p\n", pgtab);
    if (pde < (pde_t *) KERN_BASE)
        panic("pde < KERN_BASE");

//    printk("pde=%p pgtab=%p\n", pde, pgtab);

    if (!pgtab) {
        if(!alloc || (pgtab = (pte_t *) alloc_empty_page()) == NULL) {
            return NULL;
        }
//        printk("pgtab: %p\n", pgtab);
        set_pgdir_entry(pde, pgtab);
    }
//    printk("AFTER pde=%p pgtab=%p\n", pde, pgtab);

    if (pgtab < (pte_t *) KERN_BASE)
        panic("pgtab < KERN_BASE");

    return pgtable_entry(pgtab, va);
}

// This table defines the kernel's mappings, which are present in
// every process's page table.
static struct kmap {
    void *virt;
    phys_addr_t phys_start;
    phys_addr_t phys_end;
    uint32_t perm;
} kmap[] = {
        { (void*)KERN_BASE, 0,             EXT_MEM_BASE,    PTE_W}, // I/O space
        { (void*)KERN_LINK, VIRT_TO_PHYS(KERN_LINK), VIRT_TO_PHYS(data), 0},     // kern text+rodata
        { (void*)data,     VIRT_TO_PHYS(data),     PHYSTOP,   PTE_W}, // kern data+memory
        { (void*)DEVSPACE, DEVSPACE,      0,         PTE_W}, // more devices
};

// Create PTEs for virtual addresses starting at va that refer to
// physical addresses starting at pa. va and size might not
// be page-aligned.
//static
int
mappages(pde_t *pgdir, void *va, uint32_t size, uint32_t pa, uint32_t perm)
{
    char *a, *last = (char*)PGROUNDDOWN(((uint32_t)va) + size - 1);
    pte_t *pte;

    printk("Mapping va=%p - %p (%d bytes) to phys_addr=%p\n", va, (((uint32_t)va) + size), size, pa);

    for (a = (char*)PGROUNDDOWN((uint32_t)va); a < last; a += PAGE_SIZE, pa += PAGE_SIZE) {

//printk("a=%p last=%p\n", a, last);
        if((pte = walkpgdir(pgdir, a, 1)) == 0) {
printk("FAILED1");
            return -1;
        }

        if(pte_val(pte) & PTE_P)
            panic("remap");
//        *pte = pa | perm | PTE_P;
        pte_set(pte, pa | perm | PTE_P);

//        if(a == last)
//            break;

    }

    return 0;
}

// Set up kernel part of a page table.
pde_t*
setupkvm(void)
{
    pde_t *pgdir;
    struct kmap *k;

    if((pgdir = (pde_t*) alloc_empty_page()) == NULL)
        return NULL;

    if (PHYS_TO_VIRT(PHYSTOP) > (void *) DEVSPACE)
        panic("PHYSTOP too high");

    for(int i=0; i < 4; i++) {
        k = &kmap[i];
//    for(k = kmap; k < &kmap[NELEM(kmap)]; k++) {
//        printk(" k=%p!\n", k);
        if(mappages(pgdir, k->virt, k->phys_end - k->phys_start,
                    (uint32_t)k->phys_start, k->perm) < 0) {
//            freevm(pgdir);
            return 0;
        }
    }
    return pgdir;
}

// Allocate one page table for the machine for the kernel address
// space for scheduler processes.
void
kvmalloc(void)
{
    /*
     * Physical page 0 is special; it's not touched by Linux since BIOS
     * and SMM (for laptops with [34]86/SL chips) may need it.  It is read
     * and write protected to detect null pointer references in the
     * kernel.
     */
#if 0
    memset((void *) 0, 0, PAGE_SIZE);
#endif

    kpgdir = setupkvm();
    switchkvm();
}

// Switch h/w page table register to the kernel-only page table,
// for when no process is running.
void
switchkvm(void)
{
//    invalidate(); //TODO hace falta??
    lcr3(VIRT_TO_PHYS(kpgdir));   // switch to the kernel page table
}

// There is one page table per process, plus one that's used when
// a CPU is not running any process (kpgdir). The kernel uses the
// current process's page table during system calls and interrupts;
// page protection bits prevent user code from using the kernel's
// mappings.
//
// setupkvm() and exec() set up every page table like this:
//
//   0..KERN_BASE: user memory (text+data+stack+heap), mapped to
//                phys memory allocated by the kernel
//   KERN_BASE..KERN_BASE+EXTMEM: mapped to 0..EXTMEM (for I/O space)
//   KERN_BASE+EXTMEM..data: mapped to EXTMEM..VIRT_TO_PHYS(data)
//                for the kernel's instructions and r/o data
//   data..KERN_BASE+PHYSTOP: mapped to VIRT_TO_PHYS(data)..PHYSTOP,
//                                  rw data + free physical memory
//   0xfe000000..0: mapped direct (devices such as ioapic)
//
// The kernel allocates physical memory for its heap and for user memory
// between VIRT_TO_PHYS(end) and the end of physical memory (PHYSTOP)
// (directly addressable from end..PHYS_TO_VIRT(PHYSTOP)).

// There is one page table per process, plus one that's used when
// a CPU is not running any process (kpgdir). The kernel uses the
// current process's page table during system calls and interrupts;
// page protection bits prevent user code from using the kernel's
// mappings.
//
// setupkvm() and exec() set up every page table like this:
//
//   0..KERN_BASE: user memory (text+data+stack+heap), mapped to
//                phys memory allocated by the kernel
//   KERN_BASE..KERN_BASE+EXTMEM: mapped to 0..EXTMEM (for I/O space)
//   KERN_BASE+EXTMEM..data: mapped to EXTMEM..VIRT_TO_PHYS(data)
//                for the kernel's instructions and r/o data
//   data..KERN_BASE+PHYSTOP: mapped to VIRT_TO_PHYS(data)..PHYSTOP,
//                                  rw data + free physical memory
//   0xfe000000..0: mapped direct (devices such as ioapic)
//
// The kernel allocates physical memory for its heap and for user memory
// between VIRT_TO_PHYS(end) and the end of physical memory (PHYSTOP)
// (directly addressable from end..PHYS_TO_VIRT(PHYSTOP)).




// Allocate one page table for the machine for the kernel address

//// Switch TSS and h/w page table to correspond to process p.
//void
//switchuvm(struct proc *p)
//{
//    if(p == 0)
//        panic("switchuvm: no process");
//    if(p->kstack == 0)
//        panic("switchuvm: no kstack");
//    if(p->pgdir == 0)
//        panic("switchuvm: no pgdir");
//
//    pushcli();
////    mycpu()->gdt[SEG_TSS] = SEG16(STS_T32A, &mycpu()->ts,
////                                  sizeof(mycpu()->ts)-1, 0);
////    mycpu()->gdt[SEG_TSS].s = 0;
////    mycpu()->ts.ss0 = SEG_KDATA << 3;
////    mycpu()->ts.esp0 = (uint)p->kstack + KSTACKSIZE;      TODO comentada por mi!! esta linea
//    // setting IOPL=0 in eflags *and* iomb beyond the tss segment limit
//    // forbids I/O instructions (e.g., inb and outb) from user space
////    mycpu()->ts.iomb = (ushort) 0xFFFF;
//    ltr(SEG_TSS << 3);
////    lcr3(VIRT_TO_PHYS(p->pgdir));  // switch to process's address space   TODO comentada por mi!!!!!
////    popcli();
//}

//// Load the initcode into address 0 of pgdir.
//// sz must be less than a page.
//void
//inituvm(pde_t *pgdir, char *init, uint32_t sz)
//{
//    char *mem;
//
//    if(sz >= PAGE_SIZE)
//        panic("inituvm: more than a page");
//    mem = kmalloc();
//    memset(mem, 0, PAGE_SIZE);
//    mappages(pgdir, 0, PAGE_SIZE, VIRT_TO_PHYS(mem), PTE_W|PTE_U);
//    memmove(mem, init, sz);
//}

//// Load a program segment into pgdir.  addr must be page-aligned
//// and the pages from addr to addr+sz must already be mapped.
//int
//loaduvm(pde_t *pgdir, char *addr, struct inode *ip, uint32_t offset, uint32_t sz)
//{
//    uint32_t i, pa, n;
//    pte_t *pte;
//
//    if((uint32_t) addr % PAGE_SIZE != 0)
//        panic("loaduvm: addr must be page aligned");
//    for(i = 0; i < sz; i += PAGE_SIZE){
//        if((pte = walkpgdir(pgdir, addr+i, 0)) == 0)
//            panic("loaduvm: address should exist");
//        pa = PTE_ADDR(*pte);
//        if(sz - i < PAGE_SIZE)
//            n = sz - i;
//        else
//            n = PAGE_SIZE;
//        if(readi(ip, PHYS_TO_VIRT(pa), offset+i, n) != n)
//            return -1;
//    }
//    return 0;
//}

//// Allocate page tables and physical memory to grow process from oldsz to
//// newsz, which need not be page aligned.  Returns new size or 0 on error.
//int
//allocuvm(pde_t *pgdir, uint32_t oldsz, uint32_t newsz)
//{
//    char *mem;
//    uint32_t a;
//
//    if(newsz >= KERN_BASE)
//        return 0;
//    if(newsz < oldsz)
//        return oldsz;
//
//    a = PGROUNDUP(oldsz);
//    for(; a < newsz; a += PAGE_SIZE){
//        mem = kmalloc();
//        if(mem == 0){
//            cprintf("allocuvm out of memory\n");
//            deallocuvm(pgdir, newsz, oldsz);
//            return 0;
//        }
//        memset(mem, 0, PAGE_SIZE);
//        if(mappages(pgdir, (char*)a, PAGE_SIZE, VIRT_TO_PHYS(mem), PTE_W|PTE_U) < 0){
//            cprintf("allocuvm out of memory (2)\n");
//            deallocuvm(pgdir, newsz, oldsz);
//            kfree(mem);
//            return 0;
//        }
//    }
//    return newsz;
//}

//// Deallocate user pages to bring the process size from oldsz to
//// newsz.  oldsz and newsz need not be page-aligned, nor does newsz
//// need to be less than oldsz.  oldsz can be larger than the actual
//// process size.  Returns the new process size.
//int
//deallocuvm(pde_t *pgdir, uint32_t oldsz, uint32_t newsz)
//{
//    pte_t *pte;
//    uint32_t a, pa;
//
//    if(newsz >= oldsz)
//        return oldsz;
//
//    a = PGROUNDUP(newsz);
//    for(; a  < oldsz; a += PAGE_SIZE){
//        pte = walkpgdir(pgdir, (char*)a, 0);
//        if(!pte)
//            a = PGADDR(PDX(a) + 1, 0, 0) - PAGE_SIZE;
//        else if((*pte & PTE_P) != 0){
//            pa = PTE_ADDR(*pte);
//            if(pa == 0)
//                panic("kfree");
//            char *v = PHYS_TO_VIRT(pa);
//            kfree(v);
//            *pte = 0;
//        }
//    }
//    return newsz;
//}


//// Clear PTE_U on a page. Used to create an inaccessible
//// page beneath the user stack.
//void
//clearpteu(pde_t *pgdir, char *uva)
//{
//    pte_t *pte;
//
//    pte = walkpgdir(pgdir, uva, 0);
//    if(pte == 0)
//        panic("clearpteu");
//    *pte &= ~PTE_U;
//}

//// Given a parent process's page table, create a copy
//// of it for a child.
//pde_t*
//copyuvm(pde_t *pgdir, uint32_t sz)
//{
//    pde_t *d;
//    pte_t *pte;
//    uint32_t pa, i, flags;
//    char *mem;
//
//    if((d = setupkvm()) == 0)
//        return 0;
//    for(i = 0; i < sz; i += PAGE_SIZE){
//        if((pte = walkpgdir(pgdir, (void *) i, 0)) == 0)
//            panic("copyuvm: pte should exist");
//        if(!(*pte & PTE_P))
//            panic("copyuvm: page not present");
//        pa = PTE_ADDR(*pte);
//        flags = PTE_FLAGS(*pte);
//        if((mem = kmalloc()) == 0)
//            goto bad;
//        memmove(mem, (char*)PHYS_TO_VIRT(pa), PAGE_SIZE);
//        if(mappages(d, (void*)i, PAGE_SIZE, VIRT_TO_PHYS(mem), flags) < 0) {
//            kfree(mem);
//            goto bad;
//        }
//    }
//    return d;
//
//    bad:
//    freevm(d);
//    return 0;
//}

////PAGEBREAK!
//// Map user virtual address to kernel address.
//char*
//uva2ka(pde_t *pgdir, char *uva)
//{
//    pte_t *pte;
//
//    pte = walkpgdir(pgdir, uva, 0);
//    if((*pte & PTE_P) == 0)
//        return 0;
//    if((*pte & PTE_U) == 0)
//        return 0;
//    return (char*)PHYS_TO_VIRT(PTE_ADDR(*pte));
//}

//// Copy len bytes from p to user address va in page table pgdir.
//// Most useful when pgdir is not the current page table.
//// uva2ka ensures this only works for PTE_U pages.
//int
//copyout(pde_t *pgdir, uint32_t va, void *p, uint32_t len)
//{
//    char *buf, *pa0;
//    uint32_t n, va0;
//
//    buf = (char*)p;
//    while(len > 0){
//        va0 = (uint32_t)PGROUNDDOWN(va);
//        pa0 = uva2ka(pgdir, (char*)va0);
//        if(pa0 == 0)
//            return -1;
//        n = PAGE_SIZE - (va - va0);
//        if(n > len)
//            n = len;
//        memmove(pa0 + (va - va0), buf, n);
//        len -= n;
//        buf += n;
//        va = va0 + PAGE_SIZE;
//    }
//    return 0;
//}
