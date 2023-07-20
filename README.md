# cNV

**cNV** is a nv library that depends on [cJSON](https://github.com/DaveGamble/cJSON/tree/master).


## Feature

- Supports memory cache
- Support replace and sync to fs

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

**cNV** is licensed under the Apache license, check the [LICENSE](./LICENSE) file.

## Contribute

Anyone is welcome to contribute. Simply fork this repository, make your changes in an own branch and create a pull-request for your change. Please do only one change per pull-request.

You found a bug? Please fill out an issue and include any data to reproduce the bug.

## Examples

The mock nv data is as follows, it is just a demo, the actual data structure is more complex than this.

```json
{
	"name":	"xiaomi",
	"sex":	"male",
	"age":	30,
	"height":	120
}
```

## Build with Meson

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
