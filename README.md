# HCL

Generate colors from the HCL color space.

## Usage

```bash
git clone https://github.com/liu-congcong/HCL
cd HCL
gcc -lm hcl.c -o hcl
```

```bash
hcl -h
Generate colors from HCL color space (https://github.com/liu-congcong/HCL)
Usage:
    hcl -n colors [-min 15.0] [-max 375.0] [-c 100.0] [-l 65.0]
Options:
    -n/--n: <int> number of colors [>0]
    -min/--min-hue: <float> min value of hue (dominant wavelength)
    -max/--max-hue: <float> max value of hue (dominant wavelength)
    -c/--chroma: <float> colorfulness [0.0-100.0]
    -l/--luminance: <float> brightness [0.0-100.0]
```

```bash
hcl -n 3
#F8766D
#00BA38
#619CFF
```
