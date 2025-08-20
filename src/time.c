
#include "time.h"
#include "outb.h"

#define PIT_CH0_PORT  0x40
#define PIT_CMD_PORT  0x43
#define PIT_INPUT_HZ  1193182u

#define PIC1_DATA     0x21  // master PIC IMR (mask) register

static volatile uint64_t g_ticks = 0;
static uint32_t          g_hz    = 0;  // actual tick rate after programming

static inline void sti(void) { __asm__ volatile("sti"); }
static inline void hlt(void) { __asm__ volatile("hlt"); }

// ----------------------- small helpers --------------------------------------

static uint32_t gcd_u32(uint32_t a, uint32_t b) {
    while (b) { uint32_t t = a % b; a = b; b = t; }
    return a;
}

// Choose a PIT divisor approximately equal to PIT_INPUT_HZ / hz, rounded
static uint16_t pit_divisor_for(uint32_t hz) {
    if (hz == 0) hz = 1000;
    uint32_t div = (PIT_INPUT_HZ + hz/2) / hz; // round to nearest
    if (div < 1)      div = 1;
    if (div > 0xFFFF) div = 0xFFFF;
    return (uint16_t)div;
}

// ceil((ms * hz) / 1000) without 128-bit math
static uint64_t ceil_mul_div_u64(uint64_t ms, uint32_t hz, uint32_t c /*=1000*/) {
    if (hz == 0) return 0;
    uint32_t g  = gcd_u32(hz, c);
    uint32_t b1 = hz / g; // reduced numerator
    uint32_t c1 = c  / g; // reduced denominator

    uint64_t q = ms / c1;
    uint64_t r = ms % c1;

    // ceil((r*b1)/c1) = (r*b1 + c1 - 1) / c1
    return q * b1 + ( (r * b1 + c1 - 1) / c1 );
}

// ----------------------- public API -----------------------------------------

void pit_init(uint32_t hz_requested) {
    // 1) Program PIT: channel 0, access lobyte/hibyte, mode 2 (rate generator)
    outb(PIT_CMD_PORT, 0x34);
    uint16_t div = pit_divisor_for(hz_requested);
    outb(PIT_CH0_PORT, (uint8_t)(div & 0xFF));
    outb(PIT_CH0_PORT, (uint8_t)(div >> 8));

    // Compute the actual tick rate we achieved with the chosen divisor
    g_hz = (uint32_t)(PIT_INPUT_HZ / div);
    if (g_hz == 0) g_hz = 1; // shouldn't happen, but avoid div-by-zero
}

void timer_irq(void) {
    // Called from your isrHandler on vector 32 (IRQ0 after remap)
    g_ticks++;
}

uint64_t timer_ticks(void) {
    return g_ticks;
}

// ms = floor(ticks * 1000 / g_hz), computed safely in 64-bit
uint64_t timer_ms(void) {
    if (!g_hz) return 0;

    // Reduce the fraction 1000/g_hz to keep intermediates small
    uint32_t g  = gcd_u32(g_hz, 1000u);
    uint32_t b1 = 1000u / g;   // ≤ 1000
    uint32_t c1 = g_hz  / g;   // typically small (e.g., if g_hz=1000 -> c1=1)

    uint64_t t = g_ticks;
    uint64_t q = t / c1;
    uint64_t r = t % c1;

    // q*b1 + (r*b1)/c1; r*b1 fits in 64-bit easily
    return q * b1 + (r * b1) / c1;
}

// Simple blocking sleep: halts the CPU between ticks (requires interrupts on)
void timer_sleep_ms(uint64_t ms) {
    if (!g_hz) return;
    uint64_t end = g_ticks + ceil_mul_div_u64(ms, g_hz, 1000u);
    sti(); // ensure we wake from HLT on timer IRQs
    while (g_ticks < end) {
        hlt();
    }
}