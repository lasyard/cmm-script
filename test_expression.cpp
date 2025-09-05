#include "doctest/doctest.h"

#include "script.h"

TEST_CASE("expression")
{
    Script sc;

    SUBCASE("1 + 1")
    {
        sc.exec("r = 1 + 1");
        CHECK(sc.getVar("r") == 2);
    }

    SUBCASE("2 - 3")
    {
        sc.exec("r = 2 - 3");
        CHECK(sc.getVar("r") == -1);
    }

    SUBCASE("4 * 5")
    {
        sc.exec("r = 4 * 5");
        CHECK(sc.getVar("r") == 20);
    }

    SUBCASE("20 / 4")
    {
        sc.exec("r = 20 / 4");
        CHECK(sc.getVar("r") == 5);
    }

    SUBCASE("7 / 2")
    {
        sc.exec("r = 7 / 2");
        CHECK(sc.getVar("r") == 3);
    }

    SUBCASE("7 % 3")
    {
        sc.exec("r = 7 % 3");
        CHECK(sc.getVar("r") == 1);
    }

    SUBCASE("operator precedence")
    {
        sc.exec("r = 2 + 3 * 4");
        CHECK(sc.getVar("r") == 14);
    }

    SUBCASE("parentheses")
    {
        sc.exec("r = (2 + 3) * 4");
        CHECK(sc.getVar("r") == 20);
    }

    SUBCASE("negative numbers")
    {
        sc.exec("r = -5 + 2");
        CHECK(sc.getVar("r") == -3);
    }

    SUBCASE("multiple operations")
    {
        sc.exec("r = 1 + 2 - 3 * 4 / 2");
        CHECK(sc.getVar("r") == -3);
    }

    SUBCASE("assignment chain")
    {
        sc.exec("a = b = 5");
        CHECK(sc.getVar("a") == 5);
        CHECK(sc.getVar("b") == 5);
    }

    SUBCASE("complex expression")
    {
        sc.exec("r = (1 + 2) * (3 + 4) - 5 / (2 + 3)");
        CHECK(sc.getVar("r") == 20);
    }

    SUBCASE("greater than")
    {
        sc.exec("r = 5 > 3");
        CHECK(sc.getVar("r") == 1);
        sc.exec("r = 2 > 4");
        CHECK(sc.getVar("r") == 0);
    }

    SUBCASE("less than")
    {
        sc.exec("r = 2 < 4");
        CHECK(sc.getVar("r") == 1);
        sc.exec("r = 5 < 3");
        CHECK(sc.getVar("r") == 0);
    }

    SUBCASE("equal to")
    {
        sc.exec("r = 3 == 3");
        CHECK(sc.getVar("r") == 1);
        sc.exec("r = 3 == 4");
        CHECK(sc.getVar("r") == 0);
    }

    SUBCASE("not equal to")
    {
        sc.exec("r = 3 != 4");
        CHECK(sc.getVar("r") == 1);
        sc.exec("r = 3 != 3");
        CHECK(sc.getVar("r") == 0);
    }

    SUBCASE("greater or equal")
    {
        sc.exec("r = 5 >= 5");
        CHECK(sc.getVar("r") == 1);
        sc.exec("r = 5 >= 4");
        CHECK(sc.getVar("r") == 1);
        sc.exec("r = 4 >= 5");
        CHECK(sc.getVar("r") == 0);
    }

    SUBCASE("less or equal")
    {
        sc.exec("r = 3 <= 3");
        CHECK(sc.getVar("r") == 1);
        sc.exec("r = 2 <= 3");
        CHECK(sc.getVar("r") == 1);
        sc.exec("r = 4 <= 3");
        CHECK(sc.getVar("r") == 0);
    }

    SUBCASE("logical and")
    {
        sc.exec("r = 1 && 1");
        CHECK(sc.getVar("r") == 1);
        sc.exec("r = 1 && 0");
        CHECK(sc.getVar("r") == 0);
        sc.exec("r = 0 && 1");
        CHECK(sc.getVar("r") == 0);
        sc.exec("r = 0 && 0");
        CHECK(sc.getVar("r") == 0);
    }

    SUBCASE("logical or")
    {
        sc.exec("r = 1 || 1");
        CHECK(sc.getVar("r") == 1);
        sc.exec("r = 1 || 0");
        CHECK(sc.getVar("r") == 1);
        sc.exec("r = 0 || 1");
        CHECK(sc.getVar("r") == 1);
        sc.exec("r = 0 || 0");
        CHECK(sc.getVar("r") == 0);
    }

    SUBCASE("logical not")
    {
        sc.exec("r = !1");
        CHECK(sc.getVar("r") == 0);
        sc.exec("r = !0");
        CHECK(sc.getVar("r") == 1);
    }

    SUBCASE("logical combination")
    {
        sc.exec("r = !(1 && 0) || (0 || 1)");
        CHECK(sc.getVar("r") == 1);
        sc.exec("r = !(0 || 0) && (1 && 1)");
        CHECK(sc.getVar("r") == 1);
    }

    SUBCASE("left shift")
    {
        sc.exec("r = 1 << 2");
        CHECK(sc.getVar("r") == 4);
        sc.exec("r = 3 << 1");
        CHECK(sc.getVar("r") == 6);
        sc.exec("r = 5 << 0");
        CHECK(sc.getVar("r") == 5);
    }

    SUBCASE("right shift")
    {
        sc.exec("r = 8 >> 2");
        CHECK(sc.getVar("r") == 2);
        sc.exec("r = 6 >> 1");
        CHECK(sc.getVar("r") == 3);
        sc.exec("r = 5 >> 0");
        CHECK(sc.getVar("r") == 5);
    }

    SUBCASE("plus equals")
    {
        sc.exec("r = 5; r += 3");
        CHECK(sc.getVar("r") == 8);
        sc.exec("r += 2");
        CHECK(sc.getVar("r") == 10);
    }

    SUBCASE("minus equals")
    {
        sc.exec("r = 10; r -= 4");
        CHECK(sc.getVar("r") == 6);
        sc.exec("r -= 1");
        CHECK(sc.getVar("r") == 5);
    }

    SUBCASE("multiply equals")
    {
        sc.exec("r = 2; r *= 3");
        CHECK(sc.getVar("r") == 6);
        sc.exec("r *= 2");
        CHECK(sc.getVar("r") == 12);
    }

    SUBCASE("divide equals")
    {
        sc.exec("r = 12; r /= 3");
        CHECK(sc.getVar("r") == 4);
        sc.exec("r /= 2");
        CHECK(sc.getVar("r") == 2);
    }

    SUBCASE("modulo equals")
    {
        sc.exec("r = 10; r %= 3");
        CHECK(sc.getVar("r") == 1);
        sc.exec("r = 20; r %= 6");
        CHECK(sc.getVar("r") == 2);
    }

    SUBCASE("left shift equals")
    {
        sc.exec("r = 2; r <<= 2");
        CHECK(sc.getVar("r") == 8);
        sc.exec("r <<= 1");
        CHECK(sc.getVar("r") == 16);
    }

    SUBCASE("right shift equals")
    {
        sc.exec("r = 16; r >>= 2");
        CHECK(sc.getVar("r") == 4);
        sc.exec("r >>= 1");
        CHECK(sc.getVar("r") == 2);
    }
}
