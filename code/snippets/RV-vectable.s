# ---------------------------------
#
#  V E C T O R   T A B L E 
#    
#    must be 64-byte aligned.
# ---------------------------------

.balign 64
.global _vector_table
_vector_table:                  # BASE
    j _default_handler
    j _default_handler
    j _default_handler
    # -----------------------------
    j _msw_interrupt_handler    # 3 
    # -----------------------------
    j _default_handler
    j _default_handler
    j _default_handler
    # -----------------------------
    j _mtim_interrupt_handler   # 7
    # -----------------------------
    j _default_handler
    j _default_handler
    j _default_handler
    # -----------------------------
    j _mext_interrupt_handler   # 11 
    # -----------------------------
    