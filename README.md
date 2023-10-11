# cNV

**cNV** is a C nv library that depends on [cJSON](https://github.com/DaveGamble/cJSON/tree/master).


## Feature

- Supports multiple data types
```c
typedef enum {
    NV_DATA_U8 = 0,          ///< U8
    NV_DATA_S8,              ///< S8
    NV_DATA_U16,             ///< U16
    NV_DATA_S16,             ///< S16
    NV_DATA_U32,             ///< U32
    NV_DATA_S32,             ///< S32
    NV_DATA_U64,             ///< U64
    NV_DATA_S64,             ///< S64
    NV_DATA_FLOAT,           ///< float
    NV_DATA_DOUBLE,          ///< double
    NV_DATA_STR,             ///< only string
    NV_DATA_STRING_ARRAY,    ///< string array
    NV_DATA_INT_ARRAY,       ///< int array
    NV_DATA_FLOAT_ARRAY,     ///< float array
    NV_DATA_DOUBLE_ARRAY,    ///< double array
} nv_data_type_t;
```
- Supports Key-value pair
- Supports replacement, addition, query, deletion, etc.
- Supports operations on different files

## Download

Clone the repo, install dependencies, and serve:

```shell
sudo apt-get install -y cmake clang gcc
git clone --recursive  git@github.com:Junbo-Zheng/cNV.git
```

## Build and Run

```shell
$ cmake -H. -Bbuild
$ cd --build build # add `-j <n>` with cmake >= 3.12
$ ./build/cNV
```

## Licensing

**cNV** is under the Apache license, check the [LICENSE](./LICENSE) file.

## Contribute

Anyone is welcome to contribute. Simply fork this repository, make your changes in an own branch and create a pull-request for your change. Please do only one change per pull-request.

You found a bug? Please fill out an issue and include any data to reproduce the bug.

## Examples

The mock nv data is as follows, it is just a demo, the actual data structure is more complex than this.

Refer to [test.c](./test.c) source file for examples.

```json
{
  "age":  30,
  "height":   175,
  "high": 140,
  "id":   88,
  "name": "Bob",
  "temp_float":   36.0999984741211,
  "temp_double":  36.2,
  "IP":   "192.168.0.1",
  "MAC":  "11-22-33-44-55-66",
  "score_str":    ["100", "150"],
  "score_int":    [100, 150],
  "score_float":  [1.1000000238418579, 1.2000000476837158],
  "score_double": [2.1, 2.2]
}
```

## Build with Meson on Ubuntu

Install **meson** build dependencies

``` shell
sudo apt-get install python3 python3-pip python3-setuptools \
python3-wheel ninja-build

pip3 install meson
```

Build and Run

``` shell
mkdir builddir
meson setup builddir
meson compile -C builddir
./builddir/cNV-meson
```
