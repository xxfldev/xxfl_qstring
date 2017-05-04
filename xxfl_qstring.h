#pragma once
#include <algorithm>
#include <cstring>

#if defined(_MSC_VER)
#include <intrin.h>
#endif

namespace xxfl {

template<bool _is_64bits>
class qstring_base
{
protected:
    typedef uint32_t _int_type_t;

    static _int_type_t swap_byte(_int_type_t v)
    {
#if defined(_MSC_VER)
        return _byteswap_ulong(v);
#else
        return (_int_type_t)__builtin_bswap32(v);
#endif
    }

    static int32_t convert_diff(_int_type_t d) { return (int32_t)d; }
};

template<>
class qstring_base<true>
{
protected:
    typedef uint64_t _int_type_t;

    static _int_type_t swap_byte(_int_type_t v)
    {
#if defined(_MSC_VER)
        return _byteswap_uint64(v);
#else
        return (_int_type_t)__builtin_bswap64(v);
#endif
    }

    static int32_t convert_diff(_int_type_t d) { return (int32_t)(d >> 32); }
};

template<bool _is_64bits>
class qstring : public qstring_base<_is_64bits>
{
protected:
    typedef qstring_base<_is_64bits> _base;

    using typename _base::_int_type_t;
    using _base::swap_byte;
    using _base::convert_diff;

    uint32_t _length;
    uint32_t _capacity;
    _int_type_t* _pdata;

public:
    qstring() : _length(0), _capacity(0), _pdata(nullptr) {}

    qstring(const char* s, size_t length) : _length((uint32_t)length)
    {
        if (_length > 0)
        {
            _capacity = (_length + sizeof(_int_type_t)) / sizeof(_int_type_t);
            _pdata = new _int_type_t[_capacity];
            _pdata[_capacity - 1] = 0;

            std::memcpy(_pdata, s, _length);
        }
        else
        {
            _capacity = 0;
            _pdata = nullptr;
        }
    }

    qstring(const qstring& qstr) : _length(qstr._length)
    {
        if (_length > 0)
        {
            _capacity = qstr._capacity;
            _pdata = new _int_type_t[_capacity];

            for (uint32_t i = 0; i < _capacity; ++i)
            {
                _pdata[i] = qstr._pdata[i];
            }
        }
        else
        {
            _capacity = 0;
            _pdata = nullptr;
        }
    }

    qstring(qstring&& qstr)
    {
        _length = qstr._length;
        _capacity = qstr._capacity;
        _pdata = qstr._pdata;

        qstr._length = 0;
        qstr._capacity = 0;
        qstr._pdata = nullptr;
    }

    ~qstring()
    {
        delete _pdata;
    }

    size_t size() const { return _length; }

    const char* c_str() const { return (_pdata != nullptr)? (char*)_pdata : (char*)&_pdata; }

    void clear()
    {
        if (_pdata != nullptr)
        {
            _length = 0;
            _capacity = 0;
            delete _pdata;
            _pdata = nullptr;
        }
    }

    qstring& operator = (const qstring& qstr)
    {
        clear();

        _length = qstr._length;

        if (_length > 0)
        {
            _capacity = qstr._capacity;
            _pdata = new _int_type_t[_capacity];

            for (uint32_t i = 0; i < _capacity; ++i)
            {
                _pdata[i] = qstr._pdata[i];
            }
        }

        return *this;
    }

    qstring& operator = (qstring&& qstr)
    {
        delete _pdata;

        _length = qstr._length;
        _capacity = qstr._capacity;
        _pdata = qstr._pdata;

        qstr._length = 0;
        qstr._capacity = 0;
        qstr._pdata = nullptr;

        return *this;
    }

    int32_t compare(const qstring& qstr) const
    {
        uint32_t capacity_min = std::min(_capacity, qstr._capacity);

        for (uint32_t i = 0; i < capacity_min; ++i)
        {
            _int_type_t diff = swap_byte(_pdata[i]) - swap_byte(qstr._pdata[i]);
            if (diff != 0)
            {
                return convert_diff(diff);
            }
        }

        return (int32_t)(_length - qstr._length);
    }
};

template<bool _is_64bits>
inline bool operator < (const xxfl::qstring<_is_64bits>& x, const xxfl::qstring<_is_64bits>& y)
{
    return x.compare(y) < 0;
}

} // xxfl
