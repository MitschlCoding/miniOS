; interrupts.asm - Interrupt Service Routine (ISR) stubs for exceptions 0-31

; Declare the C handler function as external so NASM knows it exists elsewhere.
extern isrHandler

; --- Macros for creating ISR stubs ---

; Macro for ISRs that DON'T push an error code onto the stack
; %1: Interrupt number
%macro ISR_NO_ERR_STUB 1
  global isr%1         ; Make the label globally visible for the linker
  isr%1:
    cli                ; Disable interrupts first
    push byte 0        ; Push a dummy error code (0) to make the stack frame consistent
    push byte %1       ; Push the interrupt number
    jmp isr_common_stub ; Jump to the common handler code
%endmacro

; Macro for ISRs that DO push an error code onto the stack (CPU does this)
; %1: Interrupt number
%macro ISR_ERR_STUB 1
  global isr%1         ; Make the label globally visible
  isr%1:
    cli                ; Disable interrupts first
    ; Error code is already pushed by the CPU
    push byte %1       ; Push the interrupt number
    jmp isr_common_stub ; Jump to the common handler code
%endmacro

; --- Generate the specific ISR stubs using the macros ---
; Exceptions 0-31

ISR_NO_ERR_STUB 0   ; 0: Divide By Zero Exception
ISR_NO_ERR_STUB 1   ; 1: Debug Exception
ISR_NO_ERR_STUB 2   ; 2: Non Maskable Interrupt Exception
ISR_NO_ERR_STUB 3   ; 3: Breakpoint Exception
ISR_NO_ERR_STUB 4   ; 4: Into Detected Overflow Exception
ISR_NO_ERR_STUB 5   ; 5: Out of Bounds Exception
ISR_NO_ERR_STUB 6   ; 6: Invalid Opcode Exception
ISR_NO_ERR_STUB 7   ; 7: No Coprocessor Exception

ISR_ERR_STUB    8   ; 8: Double Fault Exception (pushes an error code)

ISR_NO_ERR_STUB 9   ; 9: Coprocessor Segment Overrun Exception

ISR_ERR_STUB    10  ; 10: Bad TSS Exception (pushes an error code)
ISR_ERR_STUB    11  ; 11: Segment Not Present Exception (pushes an error code)
ISR_ERR_STUB    12  ; 12: Stack Fault Exception (pushes an error code)
ISR_ERR_STUB    13  ; 13: General Protection Fault Exception (pushes an error code)
ISR_ERR_STUB    14  ; 14: Page Fault Exception (pushes an error code)

ISR_NO_ERR_STUB 15  ; 15: Reserved Exception

ISR_NO_ERR_STUB 16  ; 16: Floating Point Exception (Math Fault)

ISR_ERR_STUB    17  ; 17: Alignment Check Exception (pushes an error code)

ISR_NO_ERR_STUB 18  ; 18: Machine Check Exception
ISR_NO_ERR_STUB 19  ; 19: SIMD Floating Point Exception
ISR_NO_ERR_STUB 20  ; 20: Virtualization Exception
ISR_ERR_STUB    21  ; 21: Control Protection Exception (pushes an error code)
ISR_NO_ERR_STUB 22  ; 22: Reserved
ISR_NO_ERR_STUB 23  ; 23: Reserved
ISR_NO_ERR_STUB 24  ; 24: Reserved
ISR_NO_ERR_STUB 25  ; 25: Reserved
ISR_NO_ERR_STUB 26  ; 26: Reserved
ISR_NO_ERR_STUB 27  ; 27: Reserved
ISR_NO_ERR_STUB 28  ; 28: Reserved
ISR_ERR_STUB    29  ; 29: Reserved (Hypervisor Injection / AMD SVM)
ISR_ERR_STUB    30  ; 30: Reserved (VMM Communication / AMD SVM Security)
ISR_NO_ERR_STUB 31  ; 31: Reserved (Security / AMD SEV)
ISR_NO_ERR_STUB 33  ; Keyboard Interrupt (IRQ 1 -> INT 33)

; --- The common stub called by all ISRs ---
; This part does the bulk of the work to save state and call the C handler
section .text          ; Ensure code goes into the text section
align 4                ; Align to 4 bytes for potentially better performance
isr_common_stub:
    ; 1. Save general purpose registers
    pusha              ; Pushes EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI (in that order)

    ; 2. Save data segment registers (by pushing AX/EAX)
    mov ax, ds         ; Get current data segment selector
    push eax           ; Push it onto the stack (use 32-bit push for alignment)

    ; 3. Load kernel data segments into DS, ES, FS, GS
    ; Assuming your kernel data segment selector is 0x10 from your GDT
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; 4. Prepare argument and call the C handler
    mov eax, esp       ; Get current stack pointer (points to saved registers/segments)
    push eax           ; Push the pointer as the first argument to isr_handler
    call isrHandler
    add esp, 4         ; Clean up the pushed pointer argument from the stack

    ; 5. Restore data segment registers
    pop eax            ; Pop the original data segment selector we saved into EAX
    mov ds, ax         ; Restore DS, ES, FS, GS
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; 6. Restore general purpose registers
    popa               ; Pops EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX

    ; 7. Clean up pushed interrupt number and error code
    add esp, 8         ; Removes the pushed Interrupt Number and Error Code (4 bytes each)

    ; 8. Return from interrupt
    iret               ; Pops CS, EIP, EFLAGS, SS, ESP
