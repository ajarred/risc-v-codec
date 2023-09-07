risc: risc.c decoder.c encoder.c
		cc -std=c99 -Wall risc.c decoder.c encoder.c -ledit -lm -o risc