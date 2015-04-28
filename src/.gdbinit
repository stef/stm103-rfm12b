set remote hardware-breakpoint-limit 6
set remote hardware-watchpoint-limit 4
set mem inaccessible-by-default off
set disassemble-next-line on
set output-radix 16

define hook-step
  mon cortex_m maskisr on
end

define hookpost-step
  mon cortex_m maskisr off
end

define inject
  delete breakpoints
  symbol-file test.elf
  restore test.bin binary 0x20000000
  break test
  jump test
  delete breakpoints
end

target extended-remote /dev/ttyACM0
mon swdp_scan
attach 1
