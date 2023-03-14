#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define white_x 95.047
#define white_y 100.0
#define white_z 108.883
#define gamma 2.4
#define eps (216.0 / 24389.0)
#define kappa (24389.0 / 27.0)
#define white_1x_15y_3z (white_x + 15.0 * white_y + 3.0 * white_z)
#define u (4.0 * white_x / white_1x_15y_3z)
#define v (9.0 * white_y / white_1x_15y_3z)

int calculate_xyz(double *xyz, double luminance, double chroma, double hue)
{
    /* xyz = x, y, z */
    *(xyz + 1) = luminance > eps * kappa ? pow((luminance + 16.0) / 116.0, 3.0) : luminance / kappa;
    double A = 1.0 / 3.0 * (52.0 * luminance / (chroma * cos(hue) + 13.0 * luminance * u) - 1.0);
    double B = -5.0 * (*(xyz + 1));
    *xyz = ((*(xyz + 1)) * (39.0 * luminance / (chroma * sin(hue) + 13.0 * luminance * v) - 5.0) - B) / (A + 1.0 / 3.0);
    *(xyz + 2) = (*xyz) * A + B;
    return 0;
}

unsigned char scale(double x)
{
    return (unsigned char)fmin(fmax(round(x), 0), 255);
}

int xyz2rgb(double *xyz, unsigned char *rgb)
{
    double x;
    x = 3.2404542 * (*xyz) - 1.5371385 * (*(xyz + 1)) - 0.4985314 * (*(xyz + 2));
    *rgb = scale(255.0 * (x > 0.0031308 ? 1.055 * pow(x, (1.0 / gamma)) - 0.055 : 12.92 * x));
    x = -0.9692660 * (*xyz) + 1.8760108 * (*(xyz + 1)) + 0.0415560 * (*(xyz + 2));
    *(rgb + 1) = scale(255.0 * (x > 0.0031308 ? 1.055 * pow(x, (1.0 / gamma)) - 0.055 : 12.92 * x));
    x = 0.0556434 * (*xyz) - 0.2040259 * (*(xyz + 1)) + 1.0572252 * (*(xyz + 2));
    *(rgb + 2) = scale(255.0 * (x > 0.0031308 ? 1.055 * pow(x, (1.0 / gamma)) - 0.055 : 12.92 * x));
    return 0;
}

int main(int argc, char *argv[])
{
    int n = 0;
    double min_hue = 15.0;
    double max_hue = 375.0;
    double chroma = 100.0;
    double luminance = 65.0;
    unsigned char rgb[3];
    double xyz[3];
    bool help = false;

    for (int i = 0; i < argc; i++)
    {
        if (argc == 1 || !(strncasecmp(argv[1], "-h", 2) || !strncasecmp(argv[1], "--h", 3)))
            help = true;
        else if (!strncasecmp(argv[i], "-n", 2) || !strncasecmp(argv[i], "--n", 3))
            n = atoi(argv[i + 1]);
        else if (!strncasecmp(argv[i], "-mi", 3) || !strncasecmp(argv[i], "--mi", 4))
            min_hue = atof(argv[i + 1]);
        else if (!strncasecmp(argv[i], "-ma", 3) || !strncasecmp(argv[i], "--ma", 4))
            max_hue = atof(argv[i + 1]);
        else if (!strncasecmp(argv[i], "-c", 2) || !strncasecmp(argv[i], "--c", 3))
            chroma = atof(argv[i + 1]);
        else if (!strncasecmp(argv[i], "-l", 2) || !strncasecmp(argv[i], "--l", 3))
            luminance = atof(argv[i + 1]);
    }

    if (help || n < 1 || chroma < 0.0 || chroma > 100.0 || luminance < 0.0 || luminance > 100.0)
    {
        puts("\nGenerate colors from HCL color space (https://github.com/liu-congcong/HCL)");
        puts("Usage:");
        printf("    %s -n colors [-min 15.0] [-max 375.0] [-c 100.0] [-l 65.0]\n", argv[0]);
        puts("Options:");
        puts("    -n/--n: <int> number of colors [>0]");
        puts("    -min/--min-hue: <float> min value of hue (dominant wavelength)");
        puts("    -max/--max-hue: <float> max value of hue (dominant wavelength)");
        puts("    -c/--chroma: <float> colorfulness [0.0-100.0]");
        puts("    -l/--luminance: <float> brightness [0.0-100.0]\n");
        exit(EXIT_SUCCESS);
    }

    if (!fmod(max_hue - min_hue, 360))
        max_hue -= 360.0 / n;
    double hue_step = (max_hue - min_hue) / (n > 1 ? (n - 1) : 1);
    for (int i = 0; i < n; i++)
    {
        double hue = fmin(fmax(min_hue + i * hue_step, 0.0), 360.0) * M_PI / 180.0;
        calculate_xyz(xyz, luminance, chroma, hue);
        xyz2rgb(xyz, rgb);
        printf("#%02X%02X%02X\n", rgb[0], rgb[1], rgb[2]);
        fflush(stdout);
    }
    return 0;
}
