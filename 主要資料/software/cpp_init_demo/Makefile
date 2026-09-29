CXX=riscv32-unknown-elf-g++
AS=riscv32-unknown-elf-as
LD=riscv32-unknown-elf-ld
OBJCOPY=riscv32-unknown-elf-objcopy

CXXFLAGS = -Os -Wall -march=rv32imac -mabi=ilp32 -ffreestanding -nostdlib -fno-exceptions
LDFLAGS = -Tlinker.ld -nostdlib -lgcc -lc -lstdc++ -lsupc++ -Wl,--gc-sections

all:demo.elf

demo.elf: crt0.o main.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
%.o: %.S
	$(AS) $< -o $@

clean:
	rm -f *.o *.elf
