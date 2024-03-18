#include <array>

namespace sng
{
    template <typename T, std::size_t N>
    class SquareMatrix final
    {
       public:
        // Construct SquareMatrix from std::array<std::array<T, N>, N>
        explicit SquareMatrix(const std::array<std::array<T, N>, N>& data)
            : data(data)
        {
        }
        SquareMatrix() = default;
        [[nodiscard]] constexpr auto operator<=>(
            const SquareMatrix& other) const = default;
        // Access element at row index and column index
        [[nodiscard]] constexpr T& operator()(std::size_t row,
                                              std::size_t column)
        {
            return data.at(row).at(column);
        }

        // Const access element at row index and column index
        [[nodiscard]] constexpr const T& operator()(std::size_t row,
                                                    std::size_t column) const
        {
            return data.at(row).at(column);
        }

        // Getter for matrix size
        [[nodiscard]] std::size_t size() const
        {
            return N;
        }

        // Multiplication operator
        [[nodiscard]] SquareMatrix operator*(const SquareMatrix& other) const
        {
            SquareMatrix result{};
            for (std::size_t row = 0; row < N; ++row)
            {
                for (std::size_t column = 0; column < N; ++column)
                {
                    for (std::size_t index = 0; index < N; ++index)
                    {
                        result(row, column) +=
                            data.at(row).at(index) * other(index, column);
                    }
                }
            }
            return result;
        }

        // Static function to generate identity matrix
        [[nodiscard]] static consteval SquareMatrix identity()
        {
            SquareMatrix result{};
            for (std::size_t index = 0; index < N; ++index)
            {
                result(index, index) = 1;  // Diagonal elements are 1
            }
            return result;
        }

       private:
        std::array<std::array<T, N>, N> data{};
    };

    using matrix4 = SquareMatrix<int, 4>;
}  // namespace sng