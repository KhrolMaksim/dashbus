.PHONY: all build install clean

all: clean build install

build:
	meson setup builddir
	meson compile -C builddir

test:
	cd builddir && meson test

install:
	cd builddir && ninja install

clean:
	rm -rf builddir
