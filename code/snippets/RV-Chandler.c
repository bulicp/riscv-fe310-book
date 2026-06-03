/*
 * Use "interrupt" attribute to indicate that the specified 
 * function is an interrupt handler.
 * The compiler generates function entry and exit
 * sequences suitable for use in an interrupt handler
 * when this attribute is present.
*/

__attribute__((interrupt)) void interrupt_handler(void) {
    // Interrupt handling code
}