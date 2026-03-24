/**
 * colormatch does a quick'n'dirty set of comparisons to find the palette indexes
 * from the global_palette which most closely match the red, green, and blue color
 * channel values provided as command line arguments (decimal or hex with 0x prefix
 * permitted).
 *
 * The output lists the palette offset and similarity metric (lower is more similar)
 * for each palette entry in ascending order by similarity metric.
 */

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "../src/colors.h"
#include "../src/gfx/global_palette.h"

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Error: wrong number of arguments\nUsage: %s r g b\n", argv[0]);
    }

    int r = strtol(argv[1], NULL, 0);
    int g = strtol(argv[2], NULL, 0);
    int b = strtol(argv[3], NULL, 0);

    std::array<double, std::size(global_palette) / 2> geomean_deltas;
    for (size_t i = 0; i < sizeof(global_palette); i+= 2) {
        uint16_t c = global_palette[i] | (global_palette[i + 1] << 8);
        uint8_t cr = (c >> 10) << 3;
        uint8_t cg = (c >> 5) << 3;
        uint8_t cb = c << 3;

        // Add one to results of subtraction so deltas are never zero (because zero
        // delta in one component would cause the entire geomean to be zero).
        double dm = (abs(cr - r) + 1) * (abs(cg - g) + 1) * (abs(cb - b) + 1);
        geomean_deltas[i / 2] = std::cbrt(dm);
    }

    std::array<uint8_t, std::size(global_palette) / 2> indices;
    for (size_t i = 0; i < indices.size(); i++) {
        indices[i] = i;
    }

    auto comparator = [&geomean_deltas](uint8_t a, uint8_t b) {
        return geomean_deltas[a] < geomean_deltas[b];
    };
    std::sort(indices.begin(), indices.end(), comparator);

    for (auto i = indices.cbegin(); i != indices.cend(); i++) {
        printf("%d\t%f\n", *i - C_WEB_COLORS, geomean_deltas[*i]);
    }

    return 0;
}