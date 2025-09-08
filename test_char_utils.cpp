#include "doctest/doctest.h"

#include <cstdlib>
#include <cstring>

#include "char_utils.h"

TEST_CASE("is_space")
{
    CHECK(is_space(' '));
    CHECK(is_space('\t'));
    CHECK(!is_space('s'));
    CHECK(!is_space('\n'));
    CHECK(!is_space('\r'));
    CHECK(!is_space('\0'));
}

TEST_CASE("is_line_end")
{
    CHECK(!is_line_end(' '));
    CHECK(!is_line_end('\t'));
    CHECK(!is_line_end('s'));
    CHECK(is_line_end('\n'));
    CHECK(is_line_end('\r'));
    CHECK(is_line_end('\0'));
}

TEST_CASE("is_digit")
{
    CHECK(is_digit('0'));
    CHECK(is_digit('1'));
    CHECK(is_digit('9'));
    CHECK(!is_digit('A'));
}

TEST_CASE("is_hex")
{
    CHECK(is_hex_digit('0'));
    CHECK(is_hex_digit('1'));
    CHECK(is_hex_digit('9'));
    CHECK(is_hex_digit('A'));
    CHECK(is_hex_digit('a'));
    CHECK(!is_hex_digit('G'));
}

TEST_CASE("digit_value")
{
    CHECK(digit_value('0') == 0);
    CHECK(digit_value('1') == 1);
    CHECK(digit_value('9') == 9);
}

TEST_CASE("hex_value")
{
    CHECK(hex_value('A') == 10);
    CHECK(hex_value('f') == 15);
}
