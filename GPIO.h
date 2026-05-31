#ifndef GPIO_H
#if PI_MOD==4
static uint32_t MMIO_BASE = 0xFE000000;
#elif PI_MOD>=2
static uint32_t MMIO_BASE = 0x3F000000;
#else
static uint32_t MMIO_BASE = 0x20000000;
#endif
enum GPIO{
	GPIO_BASE = 0x200000,
	GPPUD = (GPIO_BASE + 0x94),
	GPPUDCLK0 = (GPIO_BASE + 0x98),
	MBOX_BASE    = 0xB880,
    MBOX_READ    = (MBOX_BASE + 0x00),
    MBOX_STATUS  = (MBOX_BASE + 0x18),
    MBOX_WRITE   = (MBOX_BASE + 0x20)
	};
enum UART{
	UART0_BASE = (GPIO_BASE + 0x1000), 
	UART0_DR     = (UART0_BASE + 0x00),
    UART0_RSRECR = (UART0_BASE + 0x04),
    UART0_FR     = (UART0_BASE + 0x18),
    UART0_ILPR   = (UART0_BASE + 0x20),
    UART0_IBRD   = (UART0_BASE + 0x24),
    UART0_FBRD   = (UART0_BASE + 0x28),
    UART0_LCRH   = (UART0_BASE + 0x2C),
    UART0_CR     = (UART0_BASE + 0x30),
    UART0_IFLS   = (UART0_BASE + 0x34),
    UART0_IMSC   = (UART0_BASE + 0x38),
    UART0_RIS    = (UART0_BASE + 0x3C),
    UART0_MIS    = (UART0_BASE + 0x40),
    UART0_ICR    = (UART0_BASE + 0x44),
    UART0_DMACR  = (UART0_BASE + 0x48),
    UART0_ITCR   = (UART0_BASE + 0x80),
    UART0_ITIP   = (UART0_BASE + 0x84),
    UART0_ITOP   = (UART0_BASE + 0x88),
    UART0_TDR    = (UART0_BASE + 0x8C),
};
enum UART1{
  UART1_BASE = GPIO_BASE+0x15000,
  UART1_IRQ         = UART1_BASE,
  UART1_ENABLES     = UART1_BASE + 4,
  UART1_IO_REG   = UART1_BASE + 64,
  UART1_IER_REG  = UART1_BASE + 68,
  UART1_IIR_REG  = UART1_BASE + 72,
  UART1_LCR_REG  = UART1_BASE + 76,
  UART1_MCR_REG  = UART1_BASE + 80,
  UART1_LSR_REG  = UART1_BASE + 84,
  UART1_MSR_REG  = UART1_BASE + 88,
  UART1_SCRATCH  = UART1_BASE + 92,
  UART1_CNTL_REG = UART1_BASE + 96,
  UART1_STAT_REG = UART1_BASE + 100,
  UART1_BAUD_REG = UART1_BASE + 104,
  UART1_CLOCK  = 500000000,
};
static inline void mmio_write(uint32_t reg, uint32_t data)
{
	*(volatile uint32_t*)(MMIO_BASE + reg) = data;
}
static inline uint32_t mmio_read(uint32_t reg)
{
	return *(volatile uint32_t*)(MMIO_BASE + reg);
}
#define GPIO_H
#endif
