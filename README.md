# Raytracer

## Design Specifications
This program reads a text file from the command line and generates an image based on parameters.

---

## Scene File Keywords (Expected)

### Camera / Image
Typical fields (examples):
- `imsize W H`
- `eye ex ey ez`
- `viewdir vx vy vz`
- `updir ux uy uz`
- `vfov degrees`
- `bkgcolor r g b`

### Lights
Supports:
- **Point light**: `w = 1`
- **Directional light**: `w = 0`

### Shapes
Potential object keywords:
- `sphere cx cy cz radius`
- `cylinder cx cy cz dx dy dz radius height`
- `cone cx cy cz dx dy dz angle height`

## How to Compile
'make all' or 'mingw32-make all' if on windows

## How to run 
`./raytracer1b.exe ___.txt`

This will generate
`___.ppm`
which can be opened with GIMP