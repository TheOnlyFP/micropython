MEMORY
{
    LR_IROM1 (rwx) : ORIGIN = 0x07fc0000, LENGTH = 0xC0
    LR_IROM2 (rwx) : ORIGIN = 0x07fc0000 + 0xC0, LENGTH = 0x50
    LR_TRNG_ZI (rwx) : ORIGIN = 0x07fc0000 + 0xC0 + 0x50, LENGTH = (1024)
    LR_IROM3 (rwx) : ORIGIN = (0x07fc0000 + 0xC0 + 0x50 + (1024)), LENGTH = ((((((((0x07FD4400) + (((((0x0000050A) + (( (8 * 3) + (8 + 3)) * (262))) + ((8) * (262))) + (6)) - ((((0x0000050A) + (((5) + (((1))) + (3)) * ((((251)) | 0x01) + 4 + 7))) + ((8) * ((((251)) | 0x01) + 4 + 7))) + (6)))) & (0xFFFFC00)) - ((0) + (2200) + ( ((((((((200 + 4) + (244 + 4) + (128 + 4) + (28 + 4)) * ((1)))) + 3) / 4) * 4) + 12) + (((((1024) + 3) / 4) * 4) + 12) + (((((( 256 * (((1)) + 1) + 80 * ((1)) + 96 * (2 * ((1)) + 1) + 256 + 256 * ((1)) )) + 3) / 4) * 4) + 12) ))))+3) & (~(3))) - (0x07fc0000 + 0xC0 + 0x50 + (1024)))
    LR_RETAINED_RAM0 (rw) : ORIGIN = (((((((0x07FD4400) + (((((0x0000050A) + (( (8 * 3) + (8 + 3)) * (262))) + ((8) * (262))) + (6)) - ((((0x0000050A) + (((5) + (((1))) + (3)) * ((((251)) | 0x01) + 4 + 7))) + ((8) * ((((251)) | 0x01) + 4 + 7))) + (6)))) & (0xFFFFC00)) - ((0) + (2200) + ( ((((((((200 + 4) + (244 + 4) + (128 + 4) + (28 + 4)) * ((1)))) + 3) / 4) * 4) + 12) + (((((1024) + 3) / 4) * 4) + 12) + (((((( 256 * (((1)) + 1) + 80 * ((1)) + 96 * (2 * ((1)) + 1) + 256 + 256 * ((1)) )) + 3) / 4) * 4) + 12) ))))+3) & (~(3))), LENGTH = (((((0) + (2200) + ( ((((((((200 + 4) + (244 + 4) + (128 + 4) + (28 + 4)) * ((1)))) + 3) / 4) * 4) + 12) + (((((1024) + 3) / 4) * 4) + 12) + (((((( 256 * (((1)) + 1) + 80 * ((1)) + 96 * (2 * ((1)) + 1) + 256 + 256 * ((1)) )) + 3) / 4) * 4) + 12) )))+3) & (~(3)))
}
__ER_IROM3_BASE__ = ORIGIN(LR_IROM3);
__ER_IROM3_LENGTH__ = LENGTH(LR_IROM3);
ENTRY(Reset_Handler)
SECTIONS
{
    ER_IROM1 :
    {
        KEEP(*(.vectors))
        . = 0xC0;
    } > LR_IROM1
    ER_IROM2 :
    {
        KEEP(*(.patch_table))
        . = 0x50;
    } > LR_IROM2
    LR_TRNG_ZI :
    {
        *(trng_buffer_area_zi)
    } > LR_TRNG_ZI
    ASSERT(SIZEOF(LR_TRNG_ZI) <= (1024), "LR_TRNG_ZI to small!")
    ER_IROM3 :
    {
        KEEP(*(.isr_impl))
        KEEP(*(.text.disp_heaplog))
        *(.text*)
        . = ALIGN(4);
        KEEP(*(.init))
        KEEP(*(.fini))
        *(.rodata*)
    } > LR_IROM3
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > LR_IROM3
    __exidx_start = .;
    .ARM.exidx :
    {
       *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > LR_IROM3
    __exidx_end = .;
    .copy.table :
    {
        . = ALIGN(4);
        __copy_table_start__ = .;
        LONG (__etext)
        LONG (__data_start__)
        LONG (__data_end__ - __data_start__)
        __copy_table_end__ = .;
    } > LR_IROM3
    .zero.table :
    {
        . = ALIGN(4);
        __zero_table_start__ = .;
        LONG (__bss_start__)
        LONG (__bss_end__ - __bss_start__)
        LONG (__heap_mem_area_not_ret_start__)
        LONG (__heap_mem_area_not_ret_end__ - __heap_mem_area_not_ret_start__)
        LONG (__ret_data_start__)
        LONG (__ret_data_end__ - __ret_data_start__)
        __zero_table_end__ = .;
    } > LR_IROM3
    __etext = ALIGN(4);
    .data : AT (__etext)
    {
        __data_start__ = .;
        *(vtable)
        *(.data*)
        . = ALIGN(4);
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);
        . = ALIGN(4);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);
        . = ALIGN(4);
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);
        KEEP(*(.jcr*))
        . = ALIGN(4);
        __data_end__ = .;
    } > LR_IROM3
    __code_area_end__ = .;
    .bss :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > LR_IROM3
    ER_PRODTEST (NOLOAD):
    {
        . = ALIGN(4);
        *(prodtest_uninit)
    } > LR_IROM3
    ER_ZI (NOLOAD) :
    {
        . = ALIGN(4);
        __heap_mem_area_not_ret_start__ = .;
        *jump_table.o(heap_mem_area_not_ret)
        . = ALIGN(4);
        __heap_mem_area_not_ret_end__ = .;
    } > LR_IROM3
    .heap (NOLOAD):
    {
        __end__ = .;
        PROVIDE(end = .);
        *(.heap*)
        __HeapLimit = .;
    } > LR_IROM3
    .stack_dummy (COPY):
    {
       KEEP(*(.stack*))
    } > LR_IROM3
    __StackTop = ORIGIN(LR_IROM3) + LENGTH(LR_IROM3);
    __StackLimit = __StackTop - SIZEOF(.stack_dummy);
    PROVIDE(__stack = __StackTop);
    ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")
    RET_DATA_UNINIT (((((((0x07FD4400) + (((((0x0000050A) + (( (8 * 3) + (8 + 3)) * (262))) + ((8) * (262))) + (6)) - ((((0x0000050A) + (((5) + (((1))) + (3)) * ((((251)) | 0x01) + 4 + 7))) + ((8) * ((((251)) | 0x01) + 4 + 7))) + (6)))) & (0xFFFFC00)) - ((0) + (2200) + ( ((((((((200 + 4) + (244 + 4) + (128 + 4) + (28 + 4)) * ((1)))) + 3) / 4) * 4) + 12) + (((((1024) + 3) / 4) * 4) + 12) + (((((( 256 * (((1)) + 1) + 80 * ((1)) + 96 * (2 * ((1)) + 1) + 256 + 256 * ((1)) )) + 3) / 4) * 4) + 12) ))))+3) & (~(3))) (NOLOAD) :
    {
        . = ALIGN(4);
        __ret_data_start__ = .;
        __retention_mem_area_uninit_start__ = .;
        *(retention_mem_area_uninit)
        . = ALIGN(4);
        __retention_mem_area_uninit_end__ = .;
    } > LR_RETAINED_RAM0
    ASSERT(SIZEOF(RET_DATA_UNINIT) <= (0), "RET_DATA_UNINIT to small!")
    RET_DATA (NOLOAD) :
    {
        . = ALIGN(4);
        *(retention_mem_area0)
        . = ALIGN(4);
        __ret_data_end__ = .;
    } > LR_RETAINED_RAM0
    ASSERT(SIZEOF(RET_DATA) <= (2200), "RET_DATA to small!")
    RET_HEAP (NOLOAD):
    {
        . = ALIGN(4);
        *jump_table.o(heap_env_area)
        *jump_table.o(heap_db_area)
        *jump_table.o(heap_msg_area)
    } > LR_RETAINED_RAM0
    ASSERT(SIZEOF(RET_HEAP) <= ( ((((((((200 + 4) + (244 + 4) + (128 + 4) + (28 + 4)) * ((1)))) + 3) / 4) * 4) + 12) + (((((1024) + 3) / 4) * 4) + 12) + (((((( 256 * (((1)) + 1) + 80 * ((1)) + 96 * (2 * ((1)) + 1) + 256 + 256 * ((1)) )) + 3) / 4) * 4) + 12) ), "RET_HEAP to small!")
}