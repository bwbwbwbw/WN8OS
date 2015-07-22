#ifndef _IDT_H_
#define _IDT_H_

#include <interrupt.h>
#include <isr.h>
#include <arch.h>
#include <runtime/types.h>

struct idt_ptr_struct {
  u16 limit;
  u64 base;
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

struct idt_entry_struct {
  u16 base_lo;
  u16 select;
  u8 zero_1;
  u8 flags;
  u16 base_mi;
  u32 base_hi;
  u32 zero_2;
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

namespace IDT {
  extern idt_entry_t idt_entries[256];
  extern idt_ptr_t   idt_ptr;
  void init();
  void remap_pic();
  void irq_mask(u16 mask);
  void init_timer(u32 frequency);
  void set_gate(u8 id, u64 base, u16 select, u8 flags);
  void flush(idt_ptr_t * idt_ptr);
}

#endif