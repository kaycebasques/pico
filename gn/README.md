# host

## Setup

```
git clone --recursive git@github.com:kaycebasques/host.git
# Shouldn't be necessary, but just in case...
git submodule update --init --recursive
source boostrap.sh
gn gen out
```

## Build

```
ninja -C out
```
