# elfector
a primitive elf code injector virus

## Use instructions:
1. Assemble the shellcode with `build.sh` in `shellcode/`.
2. cd into `src/`.
3. `make` elfector.
4. Run with `./elfector [binary]` where `binary` is the name of the executable to elfect.
5. Execute `binary` and "elfected!" should be printed before the original program does anything!
