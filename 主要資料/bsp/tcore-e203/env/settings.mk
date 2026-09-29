# Describes the CPU on this board to the rest of the SDK.
# Bob: changed from imac to imc to align with e203 arch
# RISCV_ARCH := rv32imfdc
RISCV_ARCH := rv32imac_zicsr #zicsr新增如 csrr，csrw的中斷與特權模式的指令，常見於 trap handler 裡
RISCV_ABI  := ilp32
