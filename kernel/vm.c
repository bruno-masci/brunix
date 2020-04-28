#include <stdint.h>     // for uint32_t

#include <arch/x86/mmu.h>
#include <arch/x86/memlayout.h>
#include <arch/x86/types.h>
#include <arch/x86/processor.h>

#include <brunix/defs.h>
#include <brunix/string.h>  // for memset()
#include <brunix/kernel.h>  // for panic()

#include "kalloc.h"


extern char data[];  // defined by kernel.ld
pde_t *kpgdir;  // for use in scheduler()

// Set up CPU's kernel segment descriptors.
// Run once on entry on each CPU.
/*void
seginit(void)
{
  struct cpu *c;
*/
  // Map "logical" addresses to virtual addresses using identity map.
  // Cannot share a CODE descriptor for both kernel and user
  // because it would have to have DPL_USR, but the CPU forbids
  // an interrupt from CPL=0 to DPL=3.
/*  c = &cpus[cpuid()];
  c->gdt[SEG_KCODE] = SEG(STA_X|STA_R, 0, 0xffffffff, 0);
  c->gdt[SEG_KDATA] = SEG(STA_W, 0, 0xffffffff, 0);
  c->gdt[SEG_UCODE] = SEG(STA_X|STA_R, 0, 0xffffffff, DPL_USER);
  c->gdt[SEG_UDATA] = SEG(STA_W, 0, 0xffffffff, DPL_USER);
  lgdt(c->gdt, sizeof(c->gdt));
}*/

// Return the address of the PTE in page table pgdir
// that corresponds to virtual address va.  If alloc!=0,
// create any required page table pages.
PRIVATE pte_t *
walkpgdir(pde_t *pgdir, const void *va, int alloc)
{
  pde_t *pde;
  pte_t *pgtab;

  pde = &pgdir[PDX(va)];
  if(*pde & PTE_P){
    pgtab = (pte_t*)PHYS_TO_VIRT(PTE_ADDR(*pde));
  } else {
    if(!alloc || (pgtab = (pte_t*)kalloc()) == 0)
      return 0;
    // Make sure all those PTE_P bits are zero.
    memset(pgtab, 0, PAGE_SIZE);
    // The permissions here are overly generous, but they can
    // be further restricted by the permissions in the page table
    // entries, if necessary.
    *pde = VIRT_TO_PHYS(pgtab) | PTE_P | PTE_W | PTE_U;
  }
  return &pgtab[PTX(va)];
}

// Create PTEs for virtual addresses starting at va that refer to
// physical addresses starting at pa. va and size might not
// be page-aligned.
PRIVATE int
mappages(pde_t *pgdir, void *va, uint32_t size, physaddr_t pa, int perm)
{
  char *a, *last;
  pte_t *pte;

  a = (char *) PGROUNDDOWN((uint32_t) va);
  last = (char *) PGROUNDDOWN(((uint32_t) va) + size - 1);
  for(;;){
    if((pte = walkpgdir(pgdir, a, 1)) == 0)
      return -1;
    if(*pte & PTE_P)
      panic("remap");
    *pte = pa | perm | PTE_P;   // pa: 4-KiB aligned physical address.
    if(a == last)
      break;
    a += PAGE_SIZE;
    pa += PAGE_SIZE;
  }
  return 0;
}

// There is one page table per process, plus one that's used when
// a CPU is not running any process (kpgdir). The kernel uses the
// current process's page table during system calls and interrupts;
// page protection bits prevent user code from using the kernel's
// mappings.
//
// setupkvm() and exec() set up every page table like this:
//
//   0..KERNBASE: user memory (text+data+stack+heap), mapped to
//                phys memory allocated by the kernel
//   KERNBASE..KERNBASE+EXT_MEM_BASE: mapped to 0..EXT_MEM_BASE (for I/O space)
//   KERNBASE+EXT_MEM_BASE..data: mapped to EXT_MEM_BASE..VIRT_TO_PHYS(data)
//                for the kernel's instructions and r/o data
//   data..KERNBASE+PHYSTOP: mapped to VIRT_TO_PHYS(data)..PHYSTOP,
//                                  rw data + free physical memory
//   0xfe000000..0: mapped direct (devices such as ioapic)
//
// The kernel allocates physical memory for its heap and for user memory
// between VIRT_TO_PHYS(kernel_end) and the end of physical memory (PHYSTOP)
// (directly addressable from end..PHYS_TO_VIRT(PHYSTOP)).

// This table defines the kernel's mappings, which are present in
// every process's page table.
PRIVATE struct kmap {
  void *virt;
  physaddr_t phys_start;
  physaddr_t phys_end;
  int perm;
} kmap[] = {
 { (void *) KERN_BASE, 0,                       EXT_MEM_BASE,       PTE_W }, // I/O space
 { (void *) KERN_LINK, VIRT_TO_PHYS(KERN_LINK), VIRT_TO_PHYS(data), 0 },     // kern text+rodata
 { (void *) data,      VIRT_TO_PHYS(data),      PHYSTOP,            PTE_W }, // kern data+memory
 { (void *) DEVSPACE,  DEVSPACE,                0,                  PTE_W }, // more devices
};

// Set up kernel part of a page table.
pde_t*
setupkvm(void)
{
  pde_t *pgdir;
  struct kmap *k;

  if ((pgdir = ((pde_t *) kalloc())) == 0)
      return 0;
  memset(pgdir, 0, PAGE_SIZE);
  if (PHYS_TO_VIRT(PHYSTOP) > ((void *) DEVSPACE))
      panic("PHYSTOP too high");
  for(k = kmap; k < &kmap[NELEM(kmap)]; k++)
    if(mappages(pgdir, k->virt, k->phys_end - k->phys_start,
                (uint32_t)k->phys_start, k->perm) < 0) {
//      freevm(pgdir);
      return 0;
    }
  return pgdir;
}

// Switch h/w page table register to the kernel-only page table,
// for when no process is running.
void
switchkvm(void)
{
  lcr3(VIRT_TO_PHYS(kpgdir));   // switch to the kernel page table
}

// Allocate one page table for the machine for the kernel address
// space for scheduler processes.
void
kvmalloc(void)
{
    kpgdir = setupkvm();
    switchkvm();
}
