#include "Halide.h"
#include "halide_image_io.h"

using namespace Halide;

int main() {

    Buffer<uint8_t> input = Halide::Tools::load_image("../images/rgb.png");

    Func repeated = BoundaryConditions::repeat_edge(input);

    Func grayscale;

    Var x, y;

    Expr r = cast<float>(repeated(x, y, 0));
    Expr g = cast<float>(repeated(x, y, 1));
    Expr b = cast<float>(repeated(x, y, 2));

    grayscale(x, y) = cast<uint8_t>(min((r + g + b) / 3.f, 255.f));

    Expr center = grayscale(x, y);

    Expr p0 = grayscale(x - 1, y - 1);
    Expr p1 = grayscale(x, y - 1);
    Expr p2 = grayscale(x + 1, y - 1);
    Expr p3 = grayscale(x + 1, y);
    Expr p4 = grayscale(x + 1, y + 1);
    Expr p5 = grayscale(x, y + 1);
    Expr p6 = grayscale(x - 1, y + 1);
    Expr p7 = grayscale(x - 1, y);

    Func lbp;

    lbp(x, y) = cast<uint8_t>(
            select(p0 >= center, 1, 0) |
            select(p1 >= center, 1 << 1, 0) |
            select(p2 >= center, 1 << 2, 0) |
            select(p3 >= center, 1 << 3, 0) |
            select(p4 >= center, 1 << 4, 0) |
            select(p5 >= center, 1 << 5, 0) |
            select(p6 >= center, 1 << 6, 0) |
            select(p7 >= center, 1 << 7, 0)
    );

    Buffer<uint8_t> output = lbp.realize({input.width(), input.height()});
    Halide::Tools::save_image(output, "../images/lbp_output.png");

    Buffer<uint8_t> gray_output = grayscale.realize({input.width(), input.height()});
    Halide::Tools::save_image(gray_output, "../images/grayscale.png");
}