/**
 * @file stack_container.h
 * @author 0x221E
 *
 * Implementation of Stack Container of the OH Virtual Machine.
 *
 * Runtime objects are stored as ptr's in this definition.
 * 
 * @todo Add optimization for memory allocations.
 * @todo Check for redundant code, determine whether the logical decrement operation is necessary.
 */

#pragma once

#include <vector>

#include <shared/logger.h>
#include <shared/constant_pool.h>

namespace vm
{
    /**
     * @brief Logical wrapper around std::vector<ObjectPtr>.
     */
    class StackContainer
    {
    public:
        StackContainer(std::size_t capacity = 30);
        StackContainer(const StackContainer& other);

        StackContainer& operator=(const StackContainer& other);

        StackContainer& operator=(StackContainer&& other) = delete;
        StackContainer(StackContainer&& other) = delete;

    public:
        /**
         * @brief Pushes the value onto the stack by moving the argument. 
         * @param ObjectPtr&& Object pointer to be pushed.
         * 
         * @warning This function increments the size counter.
         */
        void Push(Types&& value);

        /**
         * @brief Returns a copy of the object at the top of the stack.
         */
        const Types& Back() const;

        /**
         * @brief Returns a reference to the object at the top of the stack.
         */
        Types& ReferenceBack();

        /**
         * @brief Pops the constant reference at the top of the stack.
         * Decrements the size counter.
         */
        void Pop();

        /**
         * @brief Retrieves the constant reference at the top of the stack by moving.
         * Decrements the size counter.
         */
        [[nodiscard]] Types PopMove();

        /**
         * @brief Retrieves the size counter.
         */
        std::size_t Size() const;

    private:
        std::vector<Types> m_Storage;
        std::size_t m_Size;

    private:
        std::vector<Types> CloneStorage() const;
    };

}
