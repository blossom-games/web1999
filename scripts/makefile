
colormatch: ../src/gfx/global_palette.c colormatch.cpp
	$(CC) -lm -o $@ $^

../src/gfx/global_palette.c: ../src/gfx/convimg.yaml ../src/gfx/palette_system_winvga_web216.png
	make -C .. gfx

.PHONY: clean
clean:
	rm -f colormatch
