#include "SquareMatrix.hpp"
#include <gtest/gtest.h>
#include <iostream>
#include <type_traits>

//
// Testing focuses on sng::matrix4, as actually, it is the only use of
// SquareMatrix In case of using types other than int and size other then 4,
// further testing is needed.
//

TEST(Matrix4BasicTests, checkIdentity)
{
    // GIVEN
    sng::matrix4 mat{};
    // WHEN
    mat = sng::matrix4::identity();
    // THEN
    EXPECT_EQ(mat(0, 1), 0);
    EXPECT_EQ(mat(1, 1), 1);
}

TEST(Matrix4BasicTests, checkCallOperator)
{
    // GIVEN
    sng::matrix4 mat{};
    EXPECT_EQ(mat(0, 1), 0);
    // WHEN
    mat(0, 1) = 1;
    // THEN
    EXPECT_EQ(mat(0, 1), 1);
    // GIVEN
    const sng::matrix4 MAT = mat;
    // WHEN
    const int element = MAT(0, 1);
    // THEN
    // check that const callable return const element
    EXPECT_TRUE(
        std::is_const_v<std::remove_reference<decltype(MAT(0, 1))>::type>);
    EXPECT_EQ(element, 1);
}

class Matrix4MultiplicationTests : public ::testing::Test
{
   protected:
    void SetUp() override
    {
        random_mat1 = sng::matrix4(
            {{{1, 2, 3, 4}, {3, 2, 1, 4}, {4, 1, 2, 3}, {6, 7, 8, 9}}});
    }
    sng::matrix4 random_mat1{};
};

TEST_F(Matrix4MultiplicationTests, checkZeroMultiplication)
{
    // GIVEN
    sng::matrix4 zero_mat{};
    // WHEN
    sng::matrix4 result_zero_left = zero_mat * random_mat1;
    sng::matrix4 result_zero_right = random_mat1 * zero_mat;
    // THEN
    EXPECT_EQ(result_zero_left, zero_mat);
    EXPECT_EQ(result_zero_right, zero_mat);
}

TEST_F(Matrix4MultiplicationTests, checkIdentityMultiplication)
{
    // GIVEN
    sng::matrix4 identity_mat{sng::matrix4::identity()};
    // WHEN
    sng::matrix4 result_identity_left = identity_mat * random_mat1;
    sng::matrix4 result_identity_right = random_mat1 * identity_mat;
    // THEN
    EXPECT_EQ(result_identity_left, random_mat1);
    EXPECT_EQ(result_identity_right, random_mat1);
}

TEST_F(Matrix4MultiplicationTests, checkRandomMultiplication)
{
    // GIVEN
    sng::matrix4 random_mat2 = sng::matrix4(
        {{{4, 5, 6, 7}, {6, 5, 4, 7}, {7, 4, 6, 5}, {0, 7, 8, 9}}});
    // WHEN
    sng::matrix4 result_random_left = random_mat1 * random_mat2;
    sng::matrix4 result_random_right = random_mat2 * random_mat1;
    // THEN
    EXPECT_EQ(result_random_left, sng::matrix4({{{37, 55, 64, 72},
                                                 {31, 57, 64, 76},
                                                 {36, 54, 64, 72},
                                                 {122, 160, 184, 212}}}));
    EXPECT_EQ(result_random_right, sng::matrix4({{{85, 73, 85, 117},
                                                  {79, 75, 87, 119},
                                                  {73, 63, 77, 107},
                                                  {107, 85, 95, 133}}}));
}
