   1              		.arch armv6-m
   2              		.eabi_attribute 20, 1
   3              		.eabi_attribute 21, 1
   4              		.eabi_attribute 23, 3
   5              		.eabi_attribute 24, 1
   6              		.eabi_attribute 25, 1
   7              		.eabi_attribute 26, 1
   8              		.eabi_attribute 30, 6
   9              		.eabi_attribute 34, 0
  10              		.eabi_attribute 18, 4
  11              		.file	"usart_driver.c"
  12              		.text
  13              	.Ltext0:
  14              		.cfi_sections	.debug_frame
  15              		.align	1
  16              		.global	usart_init
  17              		.syntax unified
  18              		.code	16
  19              		.thumb_func
  20              		.fpu softvfp
  22              	usart_init:
  23              	.LFB3:
  24              		.file 1 "usart_driver.c"
   1:usart_driver.c **** /*
   2:usart_driver.c **** * usart_driver.c
   3:usart_driver.c **** * MD407 library
   4:usart_driver.c **** */
   5:usart_driver.c **** /* declarations goes in 'libMD407.h' */
   6:usart_driver.c **** #include "libMD407.h"
   7:usart_driver.c **** 
   8:usart_driver.c **** /* Define functions here */
   9:usart_driver.c **** void usart_init(int initval) {
  25              		.loc 1 9 30
  26              		.cfi_startproc
  27              		@ args = 0, pretend = 0, frame = 8
  28              		@ frame_needed = 1, uses_anonymous_args = 0
  29 0000 80B5     		push	{r7, lr}
  30              		.cfi_def_cfa_offset 8
  31              		.cfi_offset 7, -8
  32              		.cfi_offset 14, -4
  33 0002 82B0     		sub	sp, sp, #8
  34              		.cfi_def_cfa_offset 16
  35 0004 00AF     		add	r7, sp, #0
  36              		.cfi_def_cfa_register 7
  37 0006 7860     		str	r0, [r7, #4]
  10:usart_driver.c **** 	
  11:usart_driver.c **** }
  38              		.loc 1 11 1
  39 0008 C046     		nop
  40 000a BD46     		mov	sp, r7
  41 000c 02B0     		add	sp, sp, #8
  42              		@ sp needed
  43 000e 80BD     		pop	{r7, pc}
  44              		.cfi_endproc
  45              	.LFE3:
  47              		.align	1
  48              		.global	usart_deinit
  49              		.syntax unified
  50              		.code	16
  51              		.thumb_func
  52              		.fpu softvfp
  54              	usart_deinit:
  55              	.LFB4:
  12:usart_driver.c **** 
  13:usart_driver.c **** void usart_deinit(int deinitval) {
  56              		.loc 1 13 34
  57              		.cfi_startproc
  58              		@ args = 0, pretend = 0, frame = 8
  59              		@ frame_needed = 1, uses_anonymous_args = 0
  60 0010 80B5     		push	{r7, lr}
  61              		.cfi_def_cfa_offset 8
  62              		.cfi_offset 7, -8
  63              		.cfi_offset 14, -4
  64 0012 82B0     		sub	sp, sp, #8
  65              		.cfi_def_cfa_offset 16
  66 0014 00AF     		add	r7, sp, #0
  67              		.cfi_def_cfa_register 7
  68 0016 7860     		str	r0, [r7, #4]
  14:usart_driver.c **** 
  15:usart_driver.c **** }
  69              		.loc 1 15 1
  70 0018 C046     		nop
  71 001a BD46     		mov	sp, r7
  72 001c 02B0     		add	sp, sp, #8
  73              		@ sp needed
  74 001e 80BD     		pop	{r7, pc}
  75              		.cfi_endproc
  76              	.LFE4:
  78              		.align	1
  79              		.global	usart_isatty
  80              		.syntax unified
  81              		.code	16
  82              		.thumb_func
  83              		.fpu softvfp
  85              	usart_isatty:
  86              	.LFB5:
  16:usart_driver.c **** 
  17:usart_driver.c **** void usart_isatty(void) {
  87              		.loc 1 17 25
  88              		.cfi_startproc
  89              		@ args = 0, pretend = 0, frame = 0
  90              		@ frame_needed = 1, uses_anonymous_args = 0
  91 0020 80B5     		push	{r7, lr}
  92              		.cfi_def_cfa_offset 8
  93              		.cfi_offset 7, -8
  94              		.cfi_offset 14, -4
  95 0022 00AF     		add	r7, sp, #0
  96              		.cfi_def_cfa_register 7
  18:usart_driver.c **** 	
  19:usart_driver.c **** }
  97              		.loc 1 19 1
  98 0024 C046     		nop
  99 0026 BD46     		mov	sp, r7
 100              		@ sp needed
 101 0028 80BD     		pop	{r7, pc}
 102              		.cfi_endproc
 103              	.LFE5:
 105              		.align	1
 106              		.global	usart_write
 107              		.syntax unified
 108              		.code	16
 109              		.thumb_func
 110              		.fpu softvfp
 112              	usart_write:
 113              	.LFB6:
  20:usart_driver.c **** 
  21:usart_driver.c **** int usart_write(char *ptr, int len) {
 114              		.loc 1 21 37
 115              		.cfi_startproc
 116              		@ args = 0, pretend = 0, frame = 8
 117              		@ frame_needed = 1, uses_anonymous_args = 0
 118 002a 80B5     		push	{r7, lr}
 119              		.cfi_def_cfa_offset 8
 120              		.cfi_offset 7, -8
 121              		.cfi_offset 14, -4
 122 002c 82B0     		sub	sp, sp, #8
 123              		.cfi_def_cfa_offset 16
 124 002e 00AF     		add	r7, sp, #0
 125              		.cfi_def_cfa_register 7
 126 0030 7860     		str	r0, [r7, #4]
 127 0032 3960     		str	r1, [r7]
  22:usart_driver.c **** 
  23:usart_driver.c **** }
 128              		.loc 1 23 1
 129 0034 C046     		nop
 130 0036 1800     		movs	r0, r3
 131 0038 BD46     		mov	sp, r7
 132 003a 02B0     		add	sp, sp, #8
 133              		@ sp needed
 134 003c 80BD     		pop	{r7, pc}
 135              		.cfi_endproc
 136              	.LFE6:
 138              		.align	1
 139              		.global	usart_read
 140              		.syntax unified
 141              		.code	16
 142              		.thumb_func
 143              		.fpu softvfp
 145              	usart_read:
 146              	.LFB7:
  24:usart_driver.c **** 
  25:usart_driver.c **** int usart_read(char *ptr, int len) {
 147              		.loc 1 25 36
 148              		.cfi_startproc
 149              		@ args = 0, pretend = 0, frame = 8
 150              		@ frame_needed = 1, uses_anonymous_args = 0
 151 003e 80B5     		push	{r7, lr}
 152              		.cfi_def_cfa_offset 8
 153              		.cfi_offset 7, -8
 154              		.cfi_offset 14, -4
 155 0040 82B0     		sub	sp, sp, #8
 156              		.cfi_def_cfa_offset 16
 157 0042 00AF     		add	r7, sp, #0
 158              		.cfi_def_cfa_register 7
 159 0044 7860     		str	r0, [r7, #4]
 160 0046 3960     		str	r1, [r7]
  26:usart_driver.c **** 
  27:usart_driver.c **** }...
 161              		.loc 1 27 1
 162 0048 C046     		nop
 163 004a 1800     		movs	r0, r3
 164 004c BD46     		mov	sp, r7
 165 004e 02B0     		add	sp, sp, #8
 166              		@ sp needed
 167 0050 80BD     		pop	{r7, pc}
 168              		.cfi_endproc
 169              	.LFE7:
 171              	.Letext0:
 172              		.file 2 "c:\\cseapp\\codelite\\tools\\gcc-arm\\arm-none-eabi\\include\\sys\\lock.h"
 173              		.file 3 "c:\\cseapp\\codelite\\tools\\gcc-arm\\arm-none-eabi\\include\\sys\\_types.h"
 174              		.file 4 "c:\\cseapp\\codelite\\tools\\gcc-arm\\lib\\gcc\\arm-none-eabi\\9.2.1\\include\\stddef.h"
 175              		.file 5 "c:\\cseapp\\codelite\\tools\\gcc-arm\\arm-none-eabi\\include\\sys\\reent.h"
 176              		.file 6 "c:\\cseapp\\codelite\\tools\\gcc-arm\\arm-none-eabi\\include\\sys\\errno.h"
 177              		.file 7 "libMD407.h"
