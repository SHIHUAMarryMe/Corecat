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

#ifndef CATS_CORECAT_STREAM_DATAVIEWINPUTSTREAM_HPP
#define CATS_CORECAT_STREAM_DATAVIEWINPUTSTREAM_HPP


#include <algorithm>
#include <stdexcept>

#include "InputStream.hpp"
#include "../DataView/DataView.hpp"


namespace Cats {
namespace Corecat {
namespace Stream {

template <typename T>
class DataViewInputStream : public InputStream<T> {
    
private:
    
    template <typename U>
    using DataView = Cats::Corecat::DataView::DataView<U>;
    
private:
    
    DataView<T>* dv;
    std::uint64_t offset;
    
public:
    
    DataViewInputStream(DataView<T>& dv_, std::uint64_t offset_ = 0) : dv(&dv_), offset(offset_) {
        
        if(!dv->isReadable()) throw std::invalid_argument("Not readable");
        
    }
    DataViewInputStream(const DataViewInputStream& src) = delete;
    DataViewInputStream(DataViewInputStream&& src) : dv(src.dv), offset(src.offset) { src.dv = nullptr; }
    ~DataViewInputStream() override = default;
    
    DataViewInputStream& operator =(const DataViewInputStream& src) = delete;
    DataViewInputStream& operator =(DataViewInputStream&& src) { dv = src.dv, offset = src.offset, src.dv = nullptr; return *this; }
    
    T read() override { T t; if(read(&t, 1)) return t; else throw std::runtime_error("End of stream"); }
    std::size_t read(T* buffer, std::size_t count) override {
        
        count = std::min<std::size_t>(count, dv->getSize() - offset);
        dv->read(buffer, count, offset);
        offset += count;
        return count;
        
    }
    void skip() override { skip(1); }
    void skip(std::size_t count) override {
        
        offset += std::min<std::size_t>(count, dv->getSize() - offset);
        
    }
    
};

template <typename T>
inline DataViewInputStream<T> createDataViewInputStream(Cats::Corecat::DataView::DataView<T>& dv) { return DataViewInputStream<T>(dv); }
template <typename T>
inline DataViewInputStream<T> createDataViewInputStream(Cats::Corecat::DataView::DataView<T>& dv, std::uint64_t offset) { return DataViewInputStream<T>(dv, offset); }

}
}
}


#endif