/*
 *
 * MIT License
 *
 * Copyright (c) 2016-2017 The Cats Project
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef CATS_CORECAT_ARRAYVIEW_HPP
#define CATS_CORECAT_ARRAYVIEW_HPP


#include <cstddef>

#include <iterator>
#include <memory>


namespace Cats {
namespace Corecat {

template <typename T>
class ArrayView {
    
public:
    
    using Type = T;
    
    using Iterator = Type*;
    using ConstIterator = const Type*;
    using ReverseIterator = std::reverse_iterator<Iterator>;
    using ConstReverseIterator = std::reverse_iterator<ConstIterator>;
    
private:
    
    Type* data;
    std::size_t size;
    
public:
    
    ArrayView(const ArrayView& src) noexcept : data(src.data), size(src.size) {}
    
    ArrayView(std::nullptr_t = nullptr) noexcept : data(nullptr), size(0) {}
    ArrayView(Type& t) noexcept : data(std::addressof(t)), size(1) {}
    ArrayView(Type* data_, std::size_t size_) noexcept : data(data_), size(size_) {}
    template <std::size_t SIZE>
    ArrayView(Type (&array)[SIZE]) noexcept : data(array), size(SIZE) {}
    
    ArrayView& operator =(const ArrayView& src) noexcept { data = src.data, size = src.size; return *this; }
    
    operator ArrayView<const Type>() noexcept { return {data, size}; }
    
    Type& operator [](std::size_t index) const noexcept { return data[index]; }
    
    Type* getData() const noexcept { return data; }
    std::size_t getSize() const noexcept { return size; }
    
    Iterator begin() const noexcept { return data; }
    Iterator end() const noexcept { return data + size; }
    
    ConstIterator cbegin() const noexcept { return begin(); }
    ConstIterator cend() const noexcept { return end(); }
    
    ReverseIterator rbegin() const noexcept { return ReverseIterator(end()); }
    ReverseIterator rend() const noexcept { return ReverseIterator(begin()); }
    
    ConstReverseIterator crbegin() const noexcept { return ConstReverseIterator(cend()); }
    ConstReverseIterator crend() const noexcept { return ConstReverseIterator(cbegin()); }
    
};

}
}


#endif
