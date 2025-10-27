#pragma once

#include <vector>
#include "objects/base_object.h"

namespace shared{
    
    /**
     * @todo The underlying data type may be inefficient for a constants pool.
     * std::vector AS IS do not allow the compilation process to determine whether a value already exists.
     * Either switch to a unordered_map with custom hashing and value-based lookup logic, or implement own container through vector.
     */
    class ConstantPool
    { 
    public:
        ConstantPool();
        ConstantPool(std::vector<Types>&& constantPool);

        ~ConstantPool() = default;

        ConstantPool(ConstantPool&& constantPool);
        ConstantPool(const ConstantPool& constantPool);
        
        ConstantPool& operator=(ConstantPool&& other);
        ConstantPool& operator=(const ConstantPool& other);

    public:
        /**
         * @brief Inserts a constant to the pool.
         * @argument Runtime object.
         */
        void InsertConstant(Types&& constant);
        
        /**
         * @brief Fetches a constant.
         * @argument Index of the constant.
         * @returns Runtime object.
         */
        Types GetConstant(std::size_t index);
        
        /**
         * @brief Fetches the size of the Constant Pool.
         * @returns Size of the pool.
         */ 
        std::size_t Size() const noexcept;
        
        /**
         * @brief Moves the pool.
         * @returns Contents of constant pool.
         */
        [[nodiscard]] std::vector<Types> MoveConstants() noexcept;

        /**
         * @brief Inserts supplied vector of objects to the end of the pool.
         * @argument Object vector.
         */
        void InsertConstants(std::vector<Types>&& constants);

        /**
         * @brief Replaces the storage.
         * @argument Object vector.
         */
        void ReplaceConstants(std::vector<Types>&& constants);

    private:
        /** View class comments for potential data-types to consider here.*/
        std::vector<Types> m_Storage;        

    private:
        /**
         * @brief Clones the storage.
         * @returns Cloned object vector.
         */
        std::vector<Types> CloneStorage() const;
    };

}
