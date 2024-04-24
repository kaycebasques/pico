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

## Troubleshooting

### redundant declarations

Kudzu is pinned to an old (2020) version of FreeRTOS. If you use a 2024 version
you get a bunch of warnings/errors like this:

```
In file included from ../third_party/freertos/portable/GCC/ARM_CM0/port.c:43:
../third_party/freertos/portable/GCC/ARM_CM0/portasm.h:82:10: error: redundant redeclaration of 'ulSetInterruptMask' [-Werror=redundant-decls]
   82 | uint32_t ulSetInterruptMask( void ) __attribute__( ( naked ) ) PRIVILEGED_FUNCTION;
      |          ^~~~~~~~~~~~~~~~~~
In file included from ../third_party/freertos/include/portable.h:53,
                 from ../third_party/freertos/include/FreeRTOS.h:107,
                 from ../third_party/freertos/portable/GCC/ARM_CM0/port.c:35:
../third_party/freertos/portable/GCC/ARM_CM0/portmacro.h:109:17: note: previous declaration of 'ulSetInterruptMask' with type 'uint32_t(void)' {aka 'long unsigned int(void)'}
  109 | extern uint32_t ulSetInterruptMask( void ) /* __attribute__(( naked )) PRIVILEGED_FUNCTION */;
```

I finally figured it out by comparing what was in Kudzu's
`../third_party/freertos/portable/GCC/ARM_CM0` directory versus my own.
Kudzu only had `port.c` and `portmacro.h` whereas mine had those as well
as a few more conflicting headers.
