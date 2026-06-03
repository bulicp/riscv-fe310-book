unsigned int mcause_value;
    
// Inline assembly to read mcause
asm volatile(
    "csrr %0, mcause"  // Read mcause into %0
    : "=r" (mcause_value)  // Output : mcause_value
);