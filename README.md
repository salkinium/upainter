# µ-painter: embedded graphics and event system

I wanted to call this repository *µ-painter*, but GitHub would have named it *--painter*. That's why its now called *pain-ter*.

This is a playground to figure out how to do a 2D graphics and event system (GES) on µ-controllers in a schmart™ way, i.e. fast on ARMs and yet efficient on AVRs.

This will focus on **all** layers of the graphics and event system process:

- memory access: either direct with FSMC, or (double-)buffered
- memory format: color depts, axis swapping, bit mappings
- acceleration: typical memory access operations optimized for format, like DMA2D support, fast line rendering
- renderers: using software renderer or external graphics card like FT8XX
- graphics context: transformations, context savings, etc.
- graphics api: with a small subset of SVG 2d api for resolution independence
- event system: something roughly similar to [MicroUI from ESR]() but in C++

A full fledged GUI is not part of this playground, i.e. only GES, no Base Window System, no Window Manager, no UI Toolkit. Maybe later.

Have fun.