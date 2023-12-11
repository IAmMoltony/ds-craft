
# Building DS-Craft

The following is a guide on how to build DS-Craft.

## Dependencies

- DevkitPro toolchain for Nintendo DS
- Python 3

## Building

The makefile supports these targets:

- `build` (default): builds DS-Craft
- `clean`: removes binaries and intermediate files
- `run`: runs DS-Craft

## Running

To run using the makefile, you can use the `run` target. However, you
need to supply the emulator using the `EMULATOR` variable. Example:

```bash
make run EMULATOR=/home/moltony/melonDS
```

Please note that if you're on Windows, you should use Unix forward slashes
instead of Windows backslashes. (basically replace all `\` with `/`)

