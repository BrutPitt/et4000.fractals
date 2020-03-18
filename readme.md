# et4000.fractals
For the new generation, **et4000** was a popular VGA video card from the early 90's (late 80's) with 512KB/1MB of onboard memory and resolution until 1024x768 with 256 colors (via LUT table)

This is a my old project ('90/'91), at the time of university, that uses low level programming (ASM) to access to video memory, and FPU 387 (32 bit floating point, math coprocessor: optional in the PCs of that time).

It can be built with **Turbo C++ 2.0**, or **Borland C++ 3.x** (anyway it's a **C** project for DOS), need also **Turbo Assembler**, present in both package.
(Unfortunately I used a free, but "closed" library, as GUI. It's attached, but there are not sources: open source was not widespread.)

**To use in your personal VM is required also a VM with et4000 ROM emulator (like PCem / DosBox) if you want use resolution above the VGA/MCGA standard (320x200@256 colors)*

I published it, mostly for the technique used to speed up the rendering, used also in Fractint, that calculate the boundary of areas with same exit values, and when the area is closed the algorithm fill it with color, without compute the singular internal pixel.

It's extremely advantageous and spectacular to see with the internal "lake" of Mandelbrot.
(the compute is "relatively slow", also to appreciate the render technique... and also because run in an emulator of 386/387 ;) )
Today, with current power of calculus and parallelism of CPU, and mostly GPU, there is no more necessity to use techniques like this... but in those days processing took minutes, not seconds, to complete to draw.

I have used [**em-DosBox**](https://github.com/dreamlayers/em-dosbox), Emscripten port of DosBox, to make available a LIVE example via WebAssembly.
Is required a web-browser with webAssembly capability: currently only FireFox and Chromium-based (Chrome, Opera, new MS Edge, etc) are supported.


## [LIVE ==> et4000.fractals Mandelbrot](https://brutpitt.github.io/et4000.fractals/em-dosbox/b387.html?type=Mandel)

| contour rendering | Mandel complete |
| :---: | :---: |
| ![](https://raw.githubusercontent.com/BrutPitt/et4000.fractals/master/Mandel1.jpg) | ![](https://raw.githubusercontent.com/BrutPitt/et4000.fractals/master/Mandel2.jpg) |


## [LIVE ==> et4000.fractals JuliaCos](https://brutpitt.github.io/et4000.fractals/em-dosbox/b387.html?type=JuliaC)

| contour rendering | JuliaCos complete |
| :---: | :---: |
| ![](https://raw.githubusercontent.com/BrutPitt/et4000.fractals/master/JCos1.jpg) | ![](https://raw.githubusercontent.com/BrutPitt/et4000.fractals/master/JCos2.jpg)|



Currently I preferred "stretch" the DosBox screen to entire canvas (but it don't maintains the 1024x768 / 4:3 proportion)... In future I'll provide also a fixed un-stretchable canvas.

Can be explored also other fractal types: below the keys/buttons used inside the program.

### Keys/MouseButtons used

```
 F1/h/H  Help (in the program is in Italian)
 SPACE   toggle regular renedeing / contour rendering (only in Mandel & Julia)
 {1..8}  interpolation # points (1=No interpolation, available in regular rendering)
 F5      320x200x256  mode  Vga & Mcga
 F6      640x480x256  mode  Tseng3000 or Tseng4000 mode
 F7      800x600x256  mode  Tseng3000 or Tseng4000 & Multi-sync mode
 F8      1024x768x256 mode  Tseng4000 1Mb & Multi-sync mode
 Tab     Parameters List 
 L       Start change palette, any key rotate between present palettes.
         Esc to select and return to fractal
 S       Save file (gif) ***doesn't not work in LIVE demo!***
 F10     View current palette
         Any key to return to fractal
 Enter   go to Zoom mode... after use:
        -  Mouse:
              Right Button: resize box area
              Left  Button: starts calculating the selected area 
              Move NO buttons: move box area
        -  Keyboard :");
              PgUp/PgDn: resize box area
              +/-      : tuning resizing step
              Arrows   : move box
 C       Change exploration parameters (may not work) 
            - Enter/Arrows: validate values
            - Esc/CTRL-C  : abort
            - PgUp/PgDn   : Confirm values and exit
 +       Adds 8 to iterations number
 -       Subs 8 to iterations number
 W       Write actual parameter to file ***doesn't not work in LIVE demo!***
 Esc     Return to menu
```

**It was a personal program, never published, has just "a few" known bugs... from me ;)*