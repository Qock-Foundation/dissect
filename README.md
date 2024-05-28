# dissect

To compile this project, execute

```shell
cmake . -B cmake-build-release -D CMAKE_BUILD_TYPE=Release
cmake --build cmake-build-release
```

You need to specify a lattice and a figure you want to split.
See the directory `fig` for examples. Run `dissect-sat.sh`:

```shell
./dissect-sat.sh LATTICE NUM_PARTS DESTINATION_DIR
```
where `LATTICE` is a file in the directory `lattices`, `NUM_PARTS` is an integer, `DESTINATION` is a folder where to save images. For example,

```shell
./dissect-sat.sh lattices/tr6 5 img/tr6_triangle_5x5_5p <fig/tr6_triangle_5x5
```
