# dissect

First you need to install `kissat` (https://github.com/arminbiere/kissat). 
Make sure the executable is added to your path.

To compile this project, execute

```shell
mkdir cmake-build-release
cd cmake-build-release
cmake .. -D CMAKE_BUILD_TYPE=Release
make
```

You need to specify a lattice and a figure you want to split.
See the directory `fig` for examples. Run `solve.sh`:

```shell
./solve.sh LATTICE NUM_PARTS DESTINATION_DIR
```
where `LATTICE` is a file in the directory `lattices`, `NUM_PARTS` is an integer, `DESTINATION` is a folder where to save images. For example,

```shell
./solve.sh lattices/tr6 5 img/tr6_triangle_5x5_5p <fig/tr6_triangle_5x5
```