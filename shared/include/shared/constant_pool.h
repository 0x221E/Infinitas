#pragma once

#include <vector>
#include "logger.h"
#include "objects/base_object.h"

namespace shared{

    class ConstantPool
    { 
    public:
        ConstantPool(std::vector<Types>&& constantPool);
        ConstantPool();

        ConstantPool(const ConstantPool& constantPool);
        ConstantPool(ConstantPool&& constantPool);
        
        ConstantPool& operator=(ConstantPool&& other);

        void InsertConstant(Types&& constant);
        Types GetConstant(std::size_t index);
        
        std::size_t Size() const noexcept;
        
        [[nodiscard]] std::vector<Types> MoveConstants() noexcept;
        void InsertConstants(std::vector<Types>&& constants);
        void ReplaceConstants(std::vector<Types>&& constants);

    private:
        std::vector<Types> m_Storage;        
    };

}
