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

#ifndef CATS_CORECAT_MEMORYPOOL_HPP
#define CATS_CORECAT_MEMORYPOOL_HPP


#include <cassert>
#include <cstdlib>


namespace Cats {
namespace Corecat {

template <std::size_t SIZE = 65536>
class MemoryPoolFast {
    
private:
    
    struct Block {
        
        Block* next;
        std::size_t size;
        std::size_t free;
        
    };
    
private:
    
    Block* firstBlock;
    Block* lastBlock;
    
private:
    
    void allocateBlock(std::size_t size) {
        
        auto block = static_cast<Block*>(std::malloc(sizeof(Block) + size));
        block->next = nullptr;
        block->size = size;
        block->free = 0;
        if(lastBlock) { lastBlock->next = block; lastBlock = block; }
            else firstBlock = lastBlock = block;
        
    }
    
public:
    
    MemoryPoolFast() = default;
    MemoryPoolFast(const MemoryPoolFast& src) = delete;
    ~MemoryPoolFast() { clear(); }
    
    void* allocate(std::size_t size) {
        
        assert(size);
        if(!lastBlock || (lastBlock->size - lastBlock->free) < size)
            allocateBlock(size > (SIZE - sizeof(Block)) ? size : (SIZE - sizeof(Block)));
        void* p = reinterpret_cast<char*>(lastBlock + 1) + lastBlock->free;
        lastBlock->free += size;
        return p;
        
    }
    
    void deallocate(void* /*p*/) {}
    
    void clear() {
        
        for(auto p = firstBlock; p; ) { auto next = p->next; std::free(p); p = next; }
        firstBlock = lastBlock = nullptr;
    
    }
    
};

}
}


#endif
