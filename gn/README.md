# gn

## Setup

```
git clone --recursive git@github.com:kaycebasques/host.git
source boostrap.sh
gn gen out
```

## Build

```
source activate.sh
ninja -C out apps
```

## Run

```
./out/my_host_toolchain/obj/src/bin/app
```
