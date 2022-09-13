#pragma once
#include "nu_unsigned_big_int.h"


class BigInt {
public:
    BigInt() {}
    BigInt(std::vector<uint8_t> const data, bool is_negative)
        : m_uint(data),
        m_is_negative(is_negative)
    {
    }

    BigInt(std::string_view number)
    {
        if (number.starts_with("-")) {
            m_is_negative = true;
            m_uint = UnsignedBigInt(number.substr(1));
        } else if (number.starts_with('+')) {
            m_is_negative = false;
            m_uint = UnsignedBigInt(number.substr(1));
        }
        m_is_negative = false;
    }


    BigInt(ssize_t number)
    {
        m_is_negative = number < 0;
        number &= ~(static_cast<ssize_t>(1) << (sizeof(ssize_t) * 8 - 1));
        m_uint = UnsignedBigInt(static_cast<size_t>(number));
    }

    BigInt(BigInt const& other)
        : m_uint(other.m_uint),
        m_is_negative(other.m_is_negative)
    {
    }

    BigInt(BigInt const&& other)
        : m_uint(std::move(other.m_uint)),
        m_is_negative(other.m_is_negative)
    {
    }
/*
    std::string as_binary() const;
    std::string as_decimal() const;

    UnsignedBigInt operator+(UnsignedBigInt const& other) const;
    UnsignedBigInt operator-(UnsignedBigInt const& other) const;
    UnsignedBigInt operator*(UnsignedBigInt const& other) const;
    void operator+=(UnsignedBigInt const& other) {
        *this = *this + other;
    }
    void operator-=(UnsignedBigInt const& other) {
        *this = *this - other;
    }
    void operator*=(UnsignedBigInt const& other) {
        *this = *this * other;
    }
    void divmod(UnsignedBigInt const& other, UnsignedBigInt& out_div, UnsignedBigInt& out_mod) const;
    UnsignedBigInt operator/(UnsignedBigInt const& other) const;
    UnsignedBigInt operator%(UnsignedBigInt const& other) const;
    void operator/=(UnsignedBigInt const& other) {
        *this = *this / other;
    }
    void operator%=(UnsignedBigInt const& other) {
        *this = *this % other;
    }
    void operator<<=(size_t amount);
    void shift_right(size_t);
    void set_bit(size_t, bool);
    bool is_bit_set(size_t) const;
    UnsignedBigInt expmod(UnsignedBigInt const& exp, UnsignedBigInt const& mod) const;

    void operator=(UnsignedBigInt const& other);
    void operator=(UnsignedBigInt const&& other);

    std::strong_ordering operator<=>(UnsignedBigInt const& other) const;
    bool operator==(UnsignedBigInt const& other) const {
        return (*this <=> other) == std::strong_ordering::equal;
    }
    bool operator!=(UnsignedBigInt const& other) const {
        return !(*this == other);
    }*/
private:
    UnsignedBigInt m_uint;
    bool m_is_negative {false};
};
