//-------------------------------------------------------------------------------//
//                         QUIMERA ENGINE : LICENSE                              //
//-------------------------------------------------------------------------------//
// This file is part of Quimera Engine.                                          //
// Quimera Engine is free software: you can redistribute it and/or modify        //
// it under the terms of the Lesser GNU General Public License as published by   //
// the Free Software Foundation, either version 3 of the License, or             //
// (at your option) any later version.                                           //
//                                                                               //
// Quimera Engine is distributed in the hope that it will be useful,             //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// Lesser GNU General Public License for more details.                           //
//                                                                               //
// You should have received a copy of the Lesser GNU General Public License      //
// along with Quimera Engine. If not, see <http://www.gnu.org/licenses/>.        //
//                                                                               //
// This license doesn't force you to put any kind of banner or logo telling      //
// that you are using Quimera Engine in your project but we would appreciate     //
// if you do so or, at least, if you let us know about that.                     //
//                                                                               //
// Enjoy!                                                                        //
//                                                                               //
// Kinesis Team                                                                  //
//-------------------------------------------------------------------------------//

#ifndef __QFIXEDARRAY__
#define __QFIXEDARRAY__

#include "DataTypesDefinitions.h"
#include "ToolsDefinitions.h"
#include "Assertions.h"
#include "QPoolAllocator.h"
#include "QAlignment.h"
#include "QComparatorDefault.h"
#include "AllocationOperators.h"
#include "EQIterationDirection.h"

using Kinesis::QuimeraEngine::Common::DataTypes::pointer_uint_q;
using Kinesis::QuimeraEngine::Common::Memory::QAlignment;

namespace Kinesis
{
namespace QuimeraEngine
{
namespace Tools
{
namespace Containers
{

/// <summary>
/// Represents a contiguous sequence of elements that can be arbitrarly accessed using a zero-based
/// index. Its size is defined when it is constructed and cannot be changed afterwards.
/// </summary>
/// <remarks>
/// Elements are assured to be placed contiguously in memory.<br/>
/// Elements are forced to implement assignment operator, copy constructor and destructor, all of them publicly accessible.<br/>
/// If QComparatorDefault is used as comparator, elements will be forced to implement operators "==" and "<".
/// </remarks>
/// <typeparam name="T">The type of every element in the array.</typeparam>
/// <typeparam name="AllocatorT">Optional. The type of allocator to store the elements of the array. By default, QPoolAllocator will
/// be used.</typeparam>
/// <typeparam name="ComparatorT">Optional. The type of comparator to compare elements to each other, used in search and ordering
/// algorithms. By default, QComparatorDefault will be used.</typeparam>
template <class T, class AllocatorT = Kinesis::QuimeraEngine::Common::Memory::QPoolAllocator, class ComparatorT = QComparatorDefault<T> >
class QFixedArray
{

    // INTERNAL CLASSES
    // -----------------
public:

    /// <summary>
    /// Iterator that steps once per element of an array and does not allow the modification of them.
    /// </summary>
    /// <remarks>
    /// Once an interator have been bound to an array, it cannot point to another array ever.<br/>
    /// Iterators can be invalid, this means, they may not point to an existing position of the array.<br/>
    /// The position before the first element or after the last one (end positions) are considered as valid positions.
    /// </remarks>
    class QConstArrayIterator
    {
        // CONSTRUCTORS
        // ---------------
    public:

        /// <summary>
        /// Constructor that receives the array to iterate and the position to point to. This constructor is intended to be used internally, use
        /// GetIterator method of the QFixedArray class instead.
        /// </summary>
        /// <remarks>
        /// If the array is empty, it will point to the end position (forward iteration).
        /// </remarks>
        /// <param name="pArray">[IN] The array to iterate. It must not be null.</param>
        /// <param name="uPosition">[IN] The position the iterator will point to. It must be lower than the number of elements in the array.</param>
        QConstArrayIterator(const QFixedArray* pArray, const unsigned int uPosition) : m_pArray(pArray), m_uPosition(uPosition)
        {
            QE_ASSERT_ERROR(pArray != null_q, "Invalid argument: The pointer to the array cannot be null");
            QE_ASSERT_WARNING(pArray->GetCount() > uPosition || 
                              uPosition == QFixedArray::END_POSITION_BACKWARD || 
                              uPosition == QFixedArray::END_POSITION_FORWARD, "Invalid argument: The position must be lower than the number of elements in the array");

            if(pArray == null_q || 
               (pArray->GetCount() <= uPosition && uPosition != QFixedArray::END_POSITION_BACKWARD && uPosition != QFixedArray::END_POSITION_FORWARD))
                m_uPosition = QFixedArray::END_POSITION_FORWARD;
        }


        // METHODS
        // ---------------
    public:

        /// <summary>
        /// Assignment operator that moves the iterator to the same position of other iterator.
        /// </summary>
        /// <param name="iterator">[IN] Iterator whose position will be copied. It must point to the same array as the resident iterator.</param>
        /// <returns>
        /// A reference to the resident iterator.
        /// </returns>
        QConstArrayIterator& operator=(const QConstArrayIterator &iterator)
        {
            QE_ASSERT_ERROR(m_pArray == iterator.m_pArray, "The input iterator points to a different array");
            QE_ASSERT_ERROR(iterator.IsValid(), "The input iterator is not valid");

            if(m_pArray == iterator.m_pArray)
                m_uPosition = iterator.m_uPosition;

            return *this;
        }

        /// <summary>
        /// Indirection operator that returns a reference to the array element the iterator points to.
        /// </summary>
        /// <returns>
        /// A reference to the array element the iterator points to. If the iterator is invalid or points to an end position,
        /// the result is undefined.
        /// </returns>
        const T& operator*() const
        {
            // Note: This code is a copy of the same method of QArrayIterator

            QE_ASSERT_ERROR(this->IsValid(), "The iterator is not valid, it is not possible to get the reference to the array element");

            QE_ASSERT_ERROR(m_uPosition != QFixedArray::END_POSITION_FORWARD && m_uPosition != QFixedArray::END_POSITION_BACKWARD, "The iterator points to an end position, it is not possible to get the reference to the array element");

            return *((scast_q(m_pArray->m_allocator.GetPointer(), T*)) + m_uPosition);
        }

        /// <summary>
        /// Dereferencing operator that returns a pointer to the array element the iterator points to.
        /// </summary>
        /// <returns>
        /// A pointer to the array element the iterator points to. If the iterator is invalid or points to an end position,
        /// the result is undefined.
        /// </returns>
        const T* operator->() const
        {
            // Note: This code is a copy of the same method of QArrayIterator

            QE_ASSERT_ERROR(this->IsValid(), "The iterator is not valid, it is not possible to get the pointer to the array element");

            QE_ASSERT_ERROR(m_uPosition != QFixedArray::END_POSITION_FORWARD && m_uPosition != QFixedArray::END_POSITION_BACKWARD, "The iterator points to an end position, it is not possible to get the reference to the array element");

            return scast_q(m_pArray->m_allocator.GetPointer(), T*) + m_uPosition;
        }

        /// <summary>
        /// Post-increment operator that makes the iterator step forward after the expression have been evaluated.
        /// </summary>
        /// <remarks>
        /// It is not possible to increment an iterator that already points to the position after the last element (end position).<br/>
        /// It is not possible to increment an invalid iterator.
        /// </remarks>
        /// <param name=".">[IN] Unused parameter.</param>
        /// <returns>
        /// A copy of the previous state of the iterator.
        /// </returns>
        QConstArrayIterator operator++(int)
        {
            // Note: This code is a copy of the same method of QConstArrayIterator (just replacing QArrayIterator with QConstArrayIterator)

            QE_ASSERT_ERROR(this->IsValid(), "The iterator is not valid, it cannot be incremented");

            QE_ASSERT_WARNING(m_uPosition != QFixedArray::END_POSITION_FORWARD, "The iterator points to an end position, it is not possible to increment it");

            QConstArrayIterator iteratorCopy = *this;

            if(m_uPosition == m_pArray->m_uLast)
                m_uPosition = QFixedArray::END_POSITION_FORWARD;
            else if(m_uPosition == QFixedArray::END_POSITION_BACKWARD)
                m_uPosition = m_pArray->m_uFirst;
            else if(m_uPosition != QFixedArray::END_POSITION_FORWARD)
                ++m_uPosition;

            return iteratorCopy;
        }

        /// <summary>
        /// Post-decrement operator that makes the iterator step backward after the expression have been evaluated.
        /// </summary>
        /// <remarks>
        /// It is not possible to decrement an iterator that already points to the position before the first element (end position).<br/>
        /// It is not possible to decrement an invalid iterator.
        /// </remarks>
        /// <param name=".">[IN] Unused parameter.</param>
        /// <returns>
        /// A copy of the previous state of the iterator.
        /// </returns>
        QConstArrayIterator operator--(int)
        {
            // Note: This code is a copy of the same method of QConstArrayIterator (just replacing QArrayIterator with QConstArrayIterator)

            QE_ASSERT_ERROR(this->IsValid(), "The iterator is not valid, it cannot be decremented");

            QE_ASSERT_WARNING(m_uPosition != QFixedArray::END_POSITION_BACKWARD, "The iterator points to an end position, it is not possible to decrement it");

            QConstArrayIterator iteratorCopy = *this;

            if(m_uPosition == m_pArray->m_uFirst)
                m_uPosition = QFixedArray::END_POSITION_BACKWARD;
            else if(m_uPosition == QFixedArray::END_POSITION_FORWARD)
                m_uPosition = m_pArray->m_uLast;
            else if(m_uPosition != QFixedArray::END_POSITION_BACKWARD)
                --m_uPosition;

            return iteratorCopy;
        }

        /// <summary>
        /// Pre-increment operator that makes the iterator step forward before the expression have been evaluated.
        /// </summary>
        /// <remarks>
        /// It is not possible to increment an iterator that already points to the position after the last element (end position).<br/>
        /// It is not possible to increment an invalid iterator.
        /// </remarks>
        /// <returns>
        /// A reference to the iterator.
        /// </returns>
        QConstArrayIterator& operator++()
        {
            // Note: This code is a copy of the same method of QArrayIterator

            QE_ASSERT_ERROR(this->IsValid(), "The iterator is not valid, it cannot be incremented");

            QE_ASSERT_WARNING(m_uPosition != QFixedArray::END_POSITION_FORWARD, "The iterator points to an end position, it is not possible to increment it");

            if(m_uPosition == m_pArray->m_uLast)
                m_uPosition = QFixedArray::END_POSITION_FORWARD;
            else if(m_uPosition == QFixedArray::END_POSITION_BACKWARD)
                m_uPosition = m_pArray->m_uFirst;
            else if(m_uPosition != QFixedArray::END_POSITION_FORWARD)
                ++m_uPosition;

            return *this;
        }

        /// <summary>
        /// Pre-decrement operator that makes the iterator step backward before the expression have been evaluated.
        /// </summary>
        /// <remarks>
        /// It is not possible to decrement an iterator that already points to the position before the first element (end position).<br/>
        /// It is not possible to decrement an invalid iterator.
        /// </remarks>
        /// <returns>
        /// A reference to the iterator.
        /// </returns>
        QConstArrayIterator& operator--()
        {
            // Note: This code is a copy of the same method of QArrayIterator

            QE_ASSERT_ERROR(this->IsValid(), "The iterator is not valid, it cannot be decremented");

            QE_ASSERT_WARNING(m_uPosition != QFixedArray::END_POSITION_BACKWARD, "The iterator points to an end position, it is not possible to decrement it");

            if(m_uPosition == m_pArray->m_uFirst)
                m_uPosition = QFixedArray::END_POSITION_BACKWARD;
            else if(m_uPosition == QFixedArray::END_POSITION_FORWARD)
                m_uPosition = m_pArray->m_uLast;
            else if(m_uPosition != QFixedArray::END_POSITION_BACKWARD)
                --m_uPosition;

            return *this;
        }
        
        /// <summary>
        /// Equality operator that checks if both iterators are the same.
        /// </summary>
        /// <remarks>
        /// An iterator must point to the same position of the same array to be considered equal.
        /// </remarks>
        /// <param name="iterator">[IN] The other iterator to compare to.</param>
        /// <returns>
        /// True if they are pointing to the same position of the same array; False otherwise.
        /// </returns>
        bool operator==(const QConstArrayIterator &iterator) const
        {
            QE_ASSERT_ERROR(this->IsValid(), "The iterator is not valid");
            QE_ASSERT_ERROR(iterator.IsValid(), "The input iterator is not valid");
            QE_ASSERT_ERROR(m_pArray == iterator.m_pArray, "Iterators point to different arrays");

            return m_uPosition == iterator.m_uPosition && m_pArray == iterator.m_pArray;
        }

        /// <summary>
        /// Inequality operator that checks if both iterators are different.
        /// </summary>
        /// <remarks>
        /// An iterator that points to a different position or to a different array is considered distinct.
        /// </remarks>
        /// <param name="iterator">[IN] The other iterator to compare to.</param>
        /// <returns>
        /// True if they are pointing to the a different position or a different array; False otherwise.
        /// </returns>
        bool operator!=(const QConstArrayIterator &iterator) const
        {
            QE_ASSERT_ERROR(this->IsValid(), "The iterator is not valid");
            QE_ASSERT_ERROR(iterator.IsValid(), "The input iterator is not valid");
            QE_ASSERT_ERROR(m_pArray == iterator.m_pArray, "Iterators point to different arrays");

            return m_uPosition != iterator.m_uPosition || m_pArray != iterator.m_pArray;
        }

        /// <summary>
        /// Greater than operator that checks whether resident iterator points to a more posterior position than the input iterator.
        /// </summary>
        /// <remarks>
        /// If iterators point to different arrays or they are not valid, the result is undefined.
        /// </remarks>
        /// <param name="iterator">[IN] The other iterator to compare to.</param>
        /// <returns>
        /// True if the resident iterator points to a more posterior position than the input iterator; False otherwise.
        /// </returns>
        bool operator>(const QConstArrayIterator &iterator) const
        {
            QE_ASSERT_ERROR(this->IsValid(), "The iterator is not valid");
            QE_ASSERT_ERROR(iterator.IsValid(), "The input iterator is not valid");
            QE_ASSERT_ERROR(m_pArray == iterator.m_pArray, "Iterators point to different arrays");

            return ((m_uPosition == QFixedArray::END_POSITION_FORWARD  && iterator.m_uPosition != QFixedArray::END_POSITION_FORWARD)  ||
                    (m_uPosition != QFixedArray::END_POSITION_BACKWARD && iterator.m_uPosition == QFixedArray::END_POSITION_BACKWARD) ||
                    m_uPosition > iterator.m_uPosition) &&
                    m_pArray == iterator.m_pArray;
        }

        /// <summary>
        /// Lower than operator that checks whether resident iterator points to a more anterior position than the input iterator.
        /// </summary>
        /// <remarks>
        /// If iterators point to different arrays or they are not valid, the result is undefined.
        /// </remarks>
        /// <param name="iterator">[IN] The other iterator to compare to.</param>
        /// <returns>
        /// True if the resident iterator points to a more anterior position than the input iterator; False otherwise.
        /// </returns>
        bool operator<(const QConstArrayIterator &iterator) const
        {
            QE_ASSERT_ERROR(this->IsValid(), "The iterator is not valid");
            QE_ASSERT_ERROR(iterator.IsValid(), "The input iterator is not valid");
            QE_ASSERT_ERROR(m_pArray == iterator.m_pArray, "Iterators point to different arrays");

            return ((m_uPosition != QFixedArray::END_POSITION_FORWARD  && iterator.m_uPosition == QFixedArray::END_POSITION_FORWARD)  ||
                    (m_uPosition == QFixedArray::END_POSITION_BACKWARD && iterator.m_uPosition != QFixedArray::END_POSITION_BACKWARD) ||
                    m_uPosition < iterator.m_uPosition) &&
                    m_pArray == iterator.m_pArray;
        }

        /// <summary>
        /// Greater than or equal to operator that checks whether resident iterator points to a more posterior position than the
        /// input iterator or to the same position.
        /// </summary>
        /// <remarks>
        /// If iterators point to different arrays or they are not valid, the result is undefined.
        /// </remarks>
        /// <param name="iterator">[IN] The other iterator to compare to.</param>
        /// <returns>
        /// True if the resident iterator points to a more posterior position than the input iterator or to the same position; False otherwise.
        /// </returns>
        bool operator>=(const QConstArrayIterator &iterator) const
        {
            QE_ASSERT_ERROR(this->IsValid(), "The iterator is not valid");
            QE_ASSERT_ERROR(iterator.IsValid(), "The input iterator is not valid");
            QE_ASSERT_ERROR(m_pArray == iterator.m_pArray, "Iterators point to different arrays");

            return ((m_uPosition == QFixedArray::END_POSITION_FORWARD  && iterator.m_uPosition != QFixedArray::END_POSITION_FORWARD)  ||
                    (m_uPosition != QFixedArray::END_POSITION_BACKWARD && iterator.m_uPosition == QFixedArray::END_POSITION_BACKWARD) ||
                    m_uPosition >= iterator.m_uPosition) &&
                    m_pArray == iterator.m_pArray;
        }

        /// <summary>
        /// Lower than or equal to operator that checks whether resident iterator points to a more anterior position than the input
        /// iterator or to the same position.
        /// </summary>
        /// <remarks>
        /// If iterators point to different arrays or they are not valid, the result is undefined.
        /// </remarks>
        /// <param name="iterator">[IN] The other iterator to compare to.</param>
        /// <returns>
        /// True if the resident iterator points to a more anterior position than the input iterator or to the same position; False otherwise.
        /// </returns>
        bool operator<=(const QConstArrayIterator &iterator) const
        {
            QE_ASSERT_ERROR(this->IsValid(), "The iterator is not valid");
            QE_ASSERT_ERROR(iterator.IsValid(), "The input iterator is not valid");
            QE_ASSERT_ERROR(m_pArray == iterator.m_pArray, "Iterators point to different arrays");

            return ((m_uPosition != QFixedArray::END_POSITION_FORWARD  && iterator.m_uPosition == QFixedArray::END_POSITION_FORWARD)  ||
                    (m_uPosition == QFixedArray::END_POSITION_BACKWARD && iterator.m_uPosition != QFixedArray::END_POSITION_BACKWARD) ||
                    m_uPosition <= iterator.m_uPosition) &&
                    m_pArray == iterator.m_pArray;
        }

        /// <summary>
        /// Indicates whether the iterator is pointing to one of the ends of the array.
        /// </summary>
        /// <remarks>
        /// The position immediately before the first element and the position immediately after the last element are cosidered end
        /// positions; therefore, this method can be used for both forward and backard iteration.<br/>
        /// An invalid iterator is not considered as an end position.
        /// </remarks>
        /// <returns>
        /// True if the iterator is pointing to an end position; False otherwise.
        /// </returns>
        bool IsEnd() const
        {
            QE_ASSERT_ERROR(this->IsValid(), "The input iterator is not valid");

            return m_uPosition == QFixedArray::END_POSITION_BACKWARD || m_uPosition == QFixedArray::END_POSITION_FORWARD;
        }

        /// <summary>
        /// Indicates whether the iterator is pointing to one of the ends of the array, distinguishing which of them.
        /// </summary>
        /// <remarks>
        /// The position immediately before the first element and the position immediately after the last element are cosidered end
        /// positions; therefore, this method can be used for both forward and backard iteration.<br/>
        /// An invalid iterator is not considered as an end position.
        /// </remarks>
        /// <param name="eIterationDirection">[IN] The iteration direction used to identify which of the end positions is checked.</param>
        /// <returns>
        /// True if the iterator is pointing to the position after the last element when iterating forward or if it is
        /// pointing to the position immediately before the first position when iterating backward; False otherwise.
        /// </returns>
        bool IsEnd(const EQIterationDirection &eIterationDirection) const
        {
            QE_ASSERT_ERROR(this->IsValid(), "The input iterator is not valid");

            return (eIterationDirection == EQIterationDirection::E_Backward && m_uPosition == QFixedArray::END_POSITION_BACKWARD) ||
                   (eIterationDirection == EQIterationDirection::E_Forward  && m_uPosition == QFixedArray::END_POSITION_FORWARD);
        }

        /// <summary>
        /// Makes the iterator point to the first position.
        /// </summary>
        /// <remarks>
        /// If the array is empty, the iterator will point to the end position (forward iteration).
        /// </remarks>
        void MoveFirst()
        {
            // Note: This code is a copy of the same method of QArrayIterator

            QE_ASSERT_WARNING(!m_pArray->IsEmpty(), "The array is empty, there is no first position");

            m_uPosition = m_pArray->m_uFirst == QFixedArray::END_POSITION_BACKWARD ? QFixedArray::END_POSITION_FORWARD : m_pArray->m_uFirst;
        }

        /// <summary>
        /// Makes the iterator point to the last position.
        /// </summary>
        /// <remarks>
        /// If the array is empty, the iterator will point to the end position (forward iteration).
        /// </remarks>
        void MoveLast()
        {
            // Note: This code is a copy of the same method of QArrayIterator

            QE_ASSERT_WARNING(!m_pArray->IsEmpty(), "The array is empty, there is no last position");

            m_uPosition = m_pArray->m_uLast;
        }
        
        /// <summary>
        /// Increments the position the iterator points to.
        /// </summary>
        /// <remarks>
        /// It is recommended to use pre-increment or post-increment operators instead if the intention is to advance just one position.<br/>
        /// It is not possible to increment an iterator that already points to the position after the last element (end position).<br/>
        /// It is not possible to increment an invalid iterator.
        /// </remarks>
        /// <param name="uIncrement">[IN] The amount of positions to move the iterator forward. If the new position is out of bounds, the iterator 
        /// will point to the position after the last element (end position).</param>
        /// <returns>
        /// A reference to the iterator.
        /// </returns>
        QConstArrayIterator& MoveForward(const pointer_uint_q uIncrement)
        {
            // Note: This code is a copy of the same method of QArrayIterator

            QE_ASSERT_ERROR(this->IsValid(), "The iterator is not valid, it cannot be decremented");

            QE_ASSERT_WARNING(m_uPosition != QFixedArray::END_POSITION_FORWARD, "The iterator points to an end position, it is not possible to increment it");

            if(uIncrement != 0)
            {
                if(m_uPosition == m_pArray->m_uLast)
                    m_uPosition = QFixedArray::END_POSITION_FORWARD;
                else if(m_uPosition == QFixedArray::END_POSITION_BACKWARD)
                {
                    const pointer_uint_q& ARRAY_COUNT = m_pArray->GetCount();

                    if(uIncrement < ARRAY_COUNT)
                        m_uPosition = m_pArray->m_uFirst + uIncrement - 1U;
                    else if(uIncrement == ARRAY_COUNT)
                        m_uPosition = QFixedArray::END_POSITION_FORWARD;
                }
                else if(m_uPosition != QFixedArray::END_POSITION_FORWARD)
                {
                    if((m_pArray->GetCount() - m_uPosition) <= uIncrement)
                        m_uPosition = QFixedArray::END_POSITION_FORWARD;
                    else
                        m_uPosition += uIncrement;
                }
            }

            return *this;
        }

        /// <summary>
        /// Decrements the position the iterator points to.
        /// </summary>
        /// <remarks>
        /// It is recommended to use pre-decrement or post-decrement operators instead if the intention is to move just one position back.<br/>
        /// It is not possible to decrement an iterator that already points to the position before the first element (end position).<br/>
        /// It is not possible to decrement an invalid iterator.
        /// </remarks>
        /// <param name="uDecrement">[IN] The amount of positions to move the iterator backward. If the new position is out of bounds, the iterator 
        /// will point to the position before the first element (end position).</param>
        /// <returns>
        /// A reference to the iterator.
        /// </returns>
        QConstArrayIterator& MoveBackward(const pointer_uint_q uDecrement)
        {
            // Note: This code is a copy of the same method of QArrayIterator

            QE_ASSERT_ERROR(this->IsValid(), "The iterator is not valid, it cannot be decremented");

            QE_ASSERT_WARNING(m_uPosition != QFixedArray::END_POSITION_BACKWARD, "The iterator points to an end position, it is not possible to decrement it");

            if(uDecrement != 0)
            {
                if(m_uPosition == m_pArray->m_uFirst)
                    m_uPosition = QFixedArray::END_POSITION_BACKWARD;
                else if(m_uPosition == QFixedArray::END_POSITION_FORWARD)
                {
                    const pointer_uint_q& ARRAY_COUNT = m_pArray->GetCount();

                    if(uDecrement < ARRAY_COUNT)
                        m_uPosition = m_pArray->m_uLast - uDecrement + 1U;
                    else if(uDecrement == ARRAY_COUNT)
                        m_uPosition = QFixedArray::END_POSITION_BACKWARD;
                }
                else if(m_uPosition != QFixedArray::END_POSITION_BACKWARD)
                {
                    if(m_uPosition < uDecrement)
                        m_uPosition = QFixedArray::END_POSITION_BACKWARD;
                    else
                        m_uPosition -= uDecrement;
                }
            }

            return *this;
        }

        /// <summary>
        /// Checks whether the iterator is valid or not.
        /// </summary>
        /// <remarks>
        /// An iterator is considered invalid when it points to an unexisting position (an array may have been shortened while the iterator
        /// was pointing to its last position). If the array to iterate have been destroyed, there is no way for the iterator to realize that so
        /// its behavior is undefined and this method will not detect that situation.<br/>
        /// The position before the first element or after the last one (end positions) are considered as valid positions.
        /// </remarks>
        /// <returns>
        /// True if the iterator is valid; False otherwise.
        /// </returns>
        bool IsValid() const
        {
            return m_pArray != null_q && ((m_uPosition >= m_pArray->m_uFirst && m_uPosition <= m_pArray->m_uLast) ||
                                          m_uPosition == QFixedArray::END_POSITION_BACKWARD ||
                                          m_uPosition == QFixedArray::END_POSITION_FORWARD);
        }


        // ATTRIBUTES
        // ---------------
    protected:

        /// <summary>
        /// The array the iterator points to.
        /// </summary>
        const QFixedArray* m_pArray;

        /// <summary>
        /// The current iteration position regarding the first element. It is zero-based.
        /// </summary>
        pointer_uint_q m_uPosition;

    }; // QConstArrayIterator


    /// <summary>
    /// Iterator that steps once per element of an array.
    /// </summary>
    /// <remarks>
    /// Once an interator have been bound to an array, it cannot point to another array ever.<br/>
    /// Iterators can be invalid, this means, they may not point to an existing position of the array.<br/>
    /// The position before the first element or after the last one (end positions) are considered as valid positions.
    /// </remarks>
    class QArrayIterator : public QFixedArray::QConstArrayIterator
    {
        using QFixedArray::QConstArrayIterator::m_uPosition;
        using QFixedArray::QConstArrayIterator::m_pArray;
        
        
        // CONSTRUCTORS
        // ---------------
    public:

        /// <summary>
        /// Constructor that receives the array to iterate and the position to point to. This constructor is intended to be used internally, use
        /// GetIterator method of the QFixedArray class instead.
        /// </summary>
        /// <remarks>
        /// If the array is empty, it will point to the end position (forward iteration).
        /// </remarks>
        /// <param name="pArray">[IN] The array to iterate. It must not be null.</param>
        /// <param name="uPosition">[IN] The position the iterator will point to. It must be lower than the number of elements in the array.</param>
        QArrayIterator(const QFixedArray* pArray, const unsigned int uPosition) : QConstArrayIterator(pArray, uPosition)
        {
        }


        // METHODS
        // ---------------
    public:

        /// <summary>
        /// Assignment operator that moves the iterator to the same position of other iterator.
        /// </summary>
        /// <param name="iterator">[IN] Iterator whose position will be copied. It must point to the same array as the resident iterator.</param>
        /// <returns>
        /// A reference to the resident iterator.
        /// </returns>
        QArrayIterator& operator=(const QArrayIterator &iterator)
        {
            QConstArrayIterator::operator=(iterator);
            return *this;
        }

        /// <summary>
        /// Indirection operator that returns a reference to the array element the iterator points to.
        /// </summary>
        /// <returns>
        /// A reference to the array element the iterator points to. If the iterator is invalid or points to an end position,
        /// the result is undefined.
        /// </returns>
        T& operator*() const
        {
            // Note: This code is a copy of the same method of QConstArrayIterator

            QE_ASSERT_ERROR(this->IsValid(), "The iterator is not valid, it is not possible to get the reference to the array element");

            QE_ASSERT_ERROR(m_uPosition != QFixedArray::END_POSITION_FORWARD && m_uPosition != QFixedArray::END_POSITION_BACKWARD, "The iterator points to an end position, it is not possible to get the reference to the array element");

            return *((scast_q(m_pArray->m_allocator.GetPointer(), T*)) + m_uPosition);
        }

        /// <summary>
        /// Dereferencing operator that returns a pointer to the array element the iterator points to.
        /// </summary>
        /// <returns>
        /// A pointer to the array element the iterator points to. If the iterator is invalid or points to an end position,
        /// the result is undefined.
        /// </returns>
        T* operator->() const
        {
            // Note: This code is a copy of the same method of QConstArrayIterator

            QE_ASSERT_ERROR(this->IsValid(), "The iterator is not valid, it is not possible to get the pointer to the array element");

            QE_ASSERT_ERROR(m_uPosition != QFixedArray::END_POSITION_FORWARD && m_uPosition != QFixedArray::END_POSITION_BACKWARD, "The iterator points to an end position, it is not possible to get the reference to the array element");

            return scast_q(m_pArray->m_allocator.GetPointer(), T*) + m_uPosition;
        }
        
        /// <summary>
        /// Post-increment operator that makes the iterator step forward after the expression have been evaluated.
        /// </summary>
        /// <remarks>
        /// It is not possible to increment an iterator that already points to the position after the last element (end position).<br/>
        /// It is not possible to increment an invalid iterator.
        /// </remarks>
        /// <param name=".">[IN] Unused parameter.</param>
        /// <returns>
        /// A copy of the previous state of the iterator.
        /// </returns>
        QArrayIterator operator++(int)
        {
            // Note: This code is a copy of the same method of QConstArrayIterator (just replacing QConstArrayIterator with QArrayIterator)

            QE_ASSERT_ERROR(this->IsValid(), "The iterator is not valid, it cannot be incremented");

            QE_ASSERT_WARNING(m_uPosition != QFixedArray::END_POSITION_FORWARD, "The iterator points to an end position, it is not possible to increment it");

            QArrayIterator iteratorCopy = *this;

            if(m_uPosition == m_pArray->m_uLast)
                m_uPosition = QFixedArray::END_POSITION_FORWARD;
            else if(m_uPosition == QFixedArray::END_POSITION_BACKWARD)
                m_uPosition = m_pArray->m_uFirst;
            else if(m_uPosition != QFixedArray::END_POSITION_FORWARD)
                ++m_uPosition;

            return iteratorCopy;
        }

        /// <summary>
        /// Post-decrement operator that makes the iterator step backward after the expression have been evaluated.
        /// </summary>
        /// <remarks>
        /// It is not possible to decrement an iterator that already points to the position before the first element (end position).<br/>
        /// It is not possible to decrement an invalid iterator.
        /// </remarks>
        /// <param name=".">[IN] Unused parameter.</param>
        /// <returns>
        /// A copy of the previous state of the iterator.
        /// </returns>
        QArrayIterator operator--(int)
        {
            // Note: This code is a copy of the same method of QConstArrayIterator (just replacing QConstArrayIterator with QArrayIterator)

            QE_ASSERT_ERROR(this->IsValid(), "The iterator is not valid, it cannot be decremented");

            QE_ASSERT_WARNING(m_uPosition != QFixedArray::END_POSITION_BACKWARD, "The iterator points to an end position, it is not possible to decrement it");

            QArrayIterator iteratorCopy = *this;

            if(m_uPosition == m_pArray->m_uFirst)
                m_uPosition = QFixedArray::END_POSITION_BACKWARD;
            else if(m_uPosition == QFixedArray::END_POSITION_FORWARD)
                m_uPosition = m_pArray->m_uLast;
            else if(m_uPosition != QFixedArray::END_POSITION_BACKWARD)
                --m_uPosition;

            return iteratorCopy;
        }

        /// <summary>
        /// Pre-increment operator that makes the iterator step forward before the expression have been evaluated.
        /// </summary>
        /// <remarks>
        /// It is not possible to increment an iterator that already points to the position after the last element (end position).<br/>
        /// It is not possible to increment an invalid iterator.
        /// </remarks>
        /// <returns>
        /// A reference to the iterator.
        /// </returns>
        QArrayIterator& operator++()
        {
            // Note: This code is a copy of the same method of QConstArrayIterator

            QE_ASSERT_ERROR(this->IsValid(), "The iterator is not valid, it cannot be incremented");

            QE_ASSERT_WARNING(m_uPosition != QFixedArray::END_POSITION_FORWARD, "The iterator points to an end position, it is not possible to increment it");

            if(m_uPosition == m_pArray->m_uLast)
                m_uPosition = QFixedArray::END_POSITION_FORWARD;
            else if(m_uPosition == QFixedArray::END_POSITION_BACKWARD)
                m_uPosition = m_pArray->m_uFirst;
            else if(m_uPosition != QFixedArray::END_POSITION_FORWARD)
                ++m_uPosition;

            return *this;
        }

        /// <summary>
        /// Pre-decrement operator that makes the iterator step backward before the expression have been evaluated.
        /// </summary>
        /// <remarks>
        /// It is not possible to decrement an iterator that already points to the position before the first element (end position).<br/>
        /// It is not possible to decrement an invalid iterator.
        /// </remarks>
        /// <returns>
        /// A reference to the iterator.
        /// </returns>
        QArrayIterator& operator--()
        {
            // Note: This code is a copy of the same method of QConstArrayIterator

            QE_ASSERT_ERROR(this->IsValid(), "The iterator is not valid, it cannot be decremented");

            QE_ASSERT_WARNING(m_uPosition != QFixedArray::END_POSITION_BACKWARD, "The iterator points to an end position, it is not possible to decrement it");

            if(m_uPosition == m_pArray->m_uFirst)
                m_uPosition = QFixedArray::END_POSITION_BACKWARD;
            else if(m_uPosition == QFixedArray::END_POSITION_FORWARD)
                m_uPosition = m_pArray->m_uLast;
            else if(m_uPosition != QFixedArray::END_POSITION_BACKWARD)
                --m_uPosition;

            return *this;
        }
        
        /// <summary>
        /// Makes the iterator point to the first position.
        /// </summary>
        /// <remarks>
        /// If the array is empty, the iterator will point to the end position (forward iteration).
        /// </remarks>
        void MoveFirst()
        {
            // Note: This code is a copy of the same method of QConstArrayIterator

            QE_ASSERT_WARNING(!m_pArray->IsEmpty(), "The array is empty, there is no first position");

            m_uPosition = m_pArray->m_uFirst == QFixedArray::END_POSITION_BACKWARD ? QFixedArray::END_POSITION_FORWARD : m_pArray->m_uFirst;
        }

        /// <summary>
        /// Makes the iterator point to the last position.
        /// </summary>
        /// <remarks>
        /// If the array is empty, the iterator will point to the end position (forward iteration).
        /// </remarks>
        void MoveLast()
        {
            // Note: This code is a copy of the same method of QConstArrayIterator

            QE_ASSERT_WARNING(!m_pArray->IsEmpty(), "The array is empty, there is no last position");

            m_uPosition = m_pArray->m_uLast;
        }
        
        /// <summary>
        /// Increments the position the iterator points to.
        /// </summary>
        /// <remarks>
        /// It is recommended to use pre-increment or post-increment operators instead if the intention is to advance just one position.<br/>
        /// It is not possible to increment an iterator that already points to the position after the last element (end position).<br/>
        /// It is not possible to increment an invalid iterator.
        /// </remarks>
        /// <param name="uIncrement">[IN] The amount of positions to move the iterator forward. If the new position is out of bounds, the iterator 
        /// will point to the position after the last element (end position).</param>
        /// <returns>
        /// A reference to the iterator.
        /// </returns>
        QArrayIterator& MoveForward(const pointer_uint_q uIncrement)
        {
            // Note: This code is a copy of the same method of QConstArrayIterator

            QE_ASSERT_ERROR(this->IsValid(), "The iterator is not valid, it cannot be decremented");

            QE_ASSERT_WARNING(m_uPosition != QFixedArray::END_POSITION_FORWARD, "The iterator points to an end position, it is not possible to increment it");

            if(uIncrement != 0)
            {
                if(m_uPosition == m_pArray->m_uLast)
                    m_uPosition = QFixedArray::END_POSITION_FORWARD;
                else if(m_uPosition == QFixedArray::END_POSITION_BACKWARD)
                {
                    const pointer_uint_q& ARRAY_COUNT = m_pArray->GetCount();

                    if(uIncrement < ARRAY_COUNT)
                        m_uPosition = m_pArray->m_uFirst + uIncrement - 1U;
                    else if(uIncrement == ARRAY_COUNT)
                        m_uPosition = QFixedArray::END_POSITION_FORWARD;
                }
                else if(m_uPosition != QFixedArray::END_POSITION_FORWARD)
                {
                    if((m_pArray->GetCount() - m_uPosition) <= uIncrement)
                        m_uPosition = QFixedArray::END_POSITION_FORWARD;
                    else
                        m_uPosition += uIncrement;
                }
            }

            return *this;
        }

        /// <summary>
        /// Decrements the position the iterator points to.
        /// </summary>
        /// <remarks>
        /// It is recommended to use pre-decrement or post-decrement operators instead if the intention is to move just one position back.<br/>
        /// It is not possible to decrement an iterator that already points to the position before the first element (end position).<br/>
        /// It is not possible to decrement an invalid iterator.
        /// </remarks>
        /// <param name="uDecrement">[IN] The amount of positions to move the iterator backward. If the new position is out of bounds, the iterator 
        /// will point to the position before the first element (end position).</param>
        /// <returns>
        /// A reference to the iterator.
        /// </returns>
        QArrayIterator& MoveBackward(const pointer_uint_q uDecrement)
        {
            // Note: This code is a copy of the same method of QConstArrayIterator

            QE_ASSERT_ERROR(this->IsValid(), "The iterator is not valid, it cannot be decremented");

            QE_ASSERT_WARNING(m_uPosition != QFixedArray::END_POSITION_BACKWARD, "The iterator points to an end position, it is not possible to decrement it");

            if(uDecrement != 0)
            {
                if(m_uPosition == m_pArray->m_uFirst)
                    m_uPosition = QFixedArray::END_POSITION_BACKWARD;
                else if(m_uPosition == QFixedArray::END_POSITION_FORWARD)
                {
                    const pointer_uint_q& ARRAY_COUNT = m_pArray->GetCount();

                    if(uDecrement < ARRAY_COUNT)
                        m_uPosition = m_pArray->m_uLast - uDecrement + 1U;
                    else if(uDecrement == ARRAY_COUNT)
                        m_uPosition = QFixedArray::END_POSITION_BACKWARD;
                }
                else if(m_uPosition != QFixedArray::END_POSITION_BACKWARD)
                {
                    if(m_uPosition < uDecrement)
                        m_uPosition = QFixedArray::END_POSITION_BACKWARD;
                    else
                        m_uPosition -= uDecrement;
                }
            }

            return *this;
        }

    }; // QArrayIterator


    // TYPEDEFS
    // --------------
public:

    typedef typename QFixedArray::QArrayIterator Iterator;
    typedef typename QFixedArray::QConstArrayIterator ConstIterator;


    // CONSTANTS
    // ---------------
protected:

    /// <summary>
    /// Constant to symbolize the end of the sequence near the last element.
    /// </summary>
    static const pointer_uint_q END_POSITION_FORWARD;

    /// <summary>
    /// Constant to symbolize the end of the sequence near the first element.
    /// </summary>
    static const pointer_uint_q END_POSITION_BACKWARD = -2;
    
    /// <summary>
    /// The default capacity for an array when it is not specified by the user.
    /// </summary>
    static const pointer_uint_q DEFAULT_CAPACITY = 1;

public:

    /// <summary>
    /// Constant to symbolize an invalid index returned when an element is not found.
    /// </summary>
    static const pointer_uint_q ELEMENT_NOT_FOUND = -1;


    // CONSTRUCTORS
    // ---------------

public:

    /// <summary>
    /// Constructs a fixed array passing the number of elements to store and their initial value.
    /// </summary>
    /// <remarks>
    /// The copy constructor is called for every element of the array.
    /// </remarks>
    /// <param name="uCount">[IN] Number of elements to store. It must be greater than zero.</param>
    /// <param name="initialValue">[IN] The initial value to assign to all array elements.</param>
    QFixedArray(const pointer_uint_q uCount, const T &initialValue) :
            m_uFirst(0),
            m_uLast(uCount - 1),
            m_allocator(uCount * sizeof(T), sizeof(T), QAlignment(alignof_q(T)))
    {
        QE_ASSERT_ERROR( uCount > 0, "Zero elements array is not allowed." );
        QE_ASSERT_ERROR( this->_MultiplicationOverflows(uCount, sizeof(T)) == false, "The amount of memory requested overflows the maximum allowed by this container." );

        for(pointer_uint_q uIndex = 0; uIndex < uCount; uIndex++)
        {
            // Allocates and writes in the returned buffer the initial value
            new(m_allocator.Allocate()) T(initialValue);
        }
    }

    /// <summary>
    /// Constructor that receives an existing array and its size.
    /// </summary>
    /// <remarks>
    /// The copy constructor is called for every element of the array.
    /// </remarks>
    /// <param name="pArray">[IN] The existing array that will be copied. It must not be null.</param>
    /// <param name="uNumberOfElements">[IN] The number of elements in the input array. It must be greater than zero.</param>
    QFixedArray(const T* pArray, const pointer_uint_q uNumberOfElements) :
            m_uFirst(0),
            m_uLast(uNumberOfElements - 1),
            m_allocator(uNumberOfElements * sizeof(T), sizeof(T), QAlignment(alignof_q(T)))
    {
        QE_ASSERT_ERROR( pArray != null_q, "The argument pArray is null." );
        QE_ASSERT_ERROR( uNumberOfElements > 0, "Zero elements array is not allowed." );

        for(pointer_uint_q uIndex = 0; uIndex < uNumberOfElements; ++uIndex)
        {
            // Allocates and writes in the returned buffer a copy of the input array
            new(m_allocator.Allocate()) T(pArray[uIndex]);
        }
    }

    /// <summary>
    /// Copy constructor. Creates a fixed array from another.
    /// </summary>
    /// <remarks>
    /// The copy constructor is called for every element of the array.
    /// </remarks>
    /// <param name="fixedArray">[IN] Fixed array from which to copy.</param>
    QFixedArray(const QFixedArray &fixedArray) :
            m_uFirst(fixedArray.m_uFirst),
            m_uLast(fixedArray.m_uLast),
            m_allocator(fixedArray.GetCount() * sizeof(T), sizeof(T), QAlignment(alignof_q(T)))
    {
        for(pointer_uint_q uIndex = 0; uIndex < fixedArray.m_uLast + 1U; ++uIndex)
        {
            // Constructs a T object over the buffer returned by the allocator and initializes it with
            // the value of the origin element in the corresponding array position.
            new(m_allocator.Allocate()) T(fixedArray[uIndex]);
        }
    }

protected:

    /// <summary>
    /// Constructs an empty and invalid fixed array. It's responsability of the derived classes to initialize it properly.
    /// </summary>
    QFixedArray() :
        m_uFirst(END_POSITION_BACKWARD),
        m_uLast(END_POSITION_FORWARD),
        m_allocator(QFixedArray::DEFAULT_CAPACITY * sizeof(T), sizeof(T), QAlignment(alignof_q(T)))
    {
    }


    // DESTRUCTOR
    // ---------------
public:
    /// <summary>
    /// Destructor.
    /// </summary>
    /// <remarks>
    /// The destructor is called for every element of the array.
    /// </remarks>
    ~QFixedArray()
    {
        const pointer_uint_q ARRAY_COUNT = this->GetCount();

        for(pointer_uint_q uIndex = 0; uIndex < ARRAY_COUNT; ++uIndex)
            this->GetValue(uIndex).~T();
    }


    // METHODS
    // ---------------
public:
      
    /// <summary>
    /// Defines the assignment operator which makes a copy of the passed fixed array.
    /// </summary>
    /// <remarks>
    /// If the size of the destination array is less than the size of the origin array, it will copy
    /// only the first elements that fit in the destination array. The exceeding elements are not copied and
    /// the size of the destination array does not change.
    /// If the size of the destination array is greater than the size of the origin array, the remaining
    /// elements in the destination array does not change.
    /// Assignment operator will be called for every element of the array.
    /// </remarks>
    /// <param name="fixedArray">[IN] Fixed array from which to copy.</param>
    /// <returns>
    /// A reference to the resulting fixed array.
    /// </returns>
    QFixedArray& operator= (const QFixedArray &fixedArray)
    {
        pointer_uint_q uElementsToCopy;

        if(m_uLast > fixedArray.m_uLast)
            uElementsToCopy = fixedArray.GetCount();
        else
            uElementsToCopy = this->GetCount();

        for(pointer_uint_q uIndex = 0; uIndex < uElementsToCopy; ++uIndex)
        {
            // Destination array element value = Origin array element value, using pointers arithmetic
            this->SetValue(uIndex, fixedArray.GetValue(uIndex));
        }

        return *this;
    }

    /// <summary>
    /// Returns a reference to the element stored in the passed position.
    /// </summary>
    /// <param name="uIndex">[IN] Position of the element to access. It must be less than the array's size.</param>
    /// <returns>
    /// A reference to the element stored in the passed position.
    /// </returns>
    T& operator[] (const pointer_uint_q uIndex) const
    {
        return this->GetValue(uIndex);
    }

    /// <summary>
    /// Returns a reference to the element stored in the passed position.
    /// </summary>
    /// <param name="uIndex">[IN] Position of the element to access. It must be less than the array's size.</param>
    /// <returns>
    /// A reference to the element stored in the passed position.
    /// </returns>
    T& GetValue(const pointer_uint_q uIndex) const
    {
        QE_ASSERT_ERROR( uIndex < this->GetCount(), "Index must be less than the array's size." );
        return *((T*)m_allocator.GetPointer() + uIndex);
    }

    /// <summary>
    /// Performs a fast shallow copy of the array elements.
    /// </summary>
    /// <param name="destinationArray"> [IN/OUT] Destination array where to copy the array elements. The capacity of the two arrays has to be equal.</param>
    void Clone(QFixedArray &destinationArray) const
    {
        QE_ASSERT_ERROR( destinationArray.GetCapacity() == this->GetCapacity(), "The capacity of the two arrays has to be equal." );
        m_allocator.CopyTo(destinationArray.m_allocator);
    }

    /// <summary>
    /// Copies the passed value in the passed position of the array.
    /// </summary>
    /// <remarks>
    /// The assignment operator will be called for the element that is currently occupying that position.
    /// </remarks>
    /// <param name="uIndex">[IN] Position in the array where to copy the value. It must be less than the array's size.</param>
    /// <param name="value">[IN] Value to copy in the array.</param>
    void SetValue(const pointer_uint_q uIndex, const T& value)
    {
        QE_ASSERT_ERROR( uIndex < this->GetCount(), "Index must be less than the array's size." );
        *((T*)m_allocator.GetPointer() + uIndex) = value;
    }

    /// <summary>
    /// Gets an iterator that points to a given position in the array.
    /// </summary>
    /// <param name="uIndex">[IN] Position in the array to which the iterator will point. If it is out of bounds, the returned iterator will point 
    /// to the end position.</param>
    /// <returns>
    /// An iterator that points to the position of the element.
    /// </returns>
    QArrayIterator GetIterator(const pointer_uint_q uIndex) const 
    {
        QE_ASSERT_ERROR( uIndex < this->GetCount(), "Index must be less than the array's size." );

        return QFixedArray::QArrayIterator(this, uIndex);
    }

    /// <summary>
    /// Gets an iterator that points to the first position in the array.
    /// </summary>
    /// <returns>
    /// An iterator that points to the position of the first element. If the array is empty, the iterator will point to the end position.
    /// </returns>
    QArrayIterator GetFirst() const
    {
        return QFixedArray::QArrayIterator(this, m_uFirst);
    }

    /// <summary>
    /// Gets an iterator that points to the last position in the array.
    /// </summary>
    /// <returns>
    /// An iterator that points to the position of the last element. If the array is empty, the iterator will point to the end position.
    /// </returns>
    QArrayIterator GetLast() const
    {
        return QFixedArray::QArrayIterator(this, m_uLast);
    }
    
    /// <summary>
    /// Gets a range of elements from the array.
    /// </summary>
    /// <remarks>
    /// The array must not be empty. The copy constructor of every element of the sub-array will be called.
    /// </remarks>
    /// <param name="uFirst">[IN] The index (zero-based) of the first element of the range. It must be lower than the number of elements of the array. 
    /// It must be lower than or equal to the index of the last element.</param>
    /// <param name="uLast">[IN] The index (zero-based) of the last element of the range. It must be lower than the number of elements of the array. 
    /// It must be greater than or equal to the index of the first element.</param>
    /// <returns>
    /// The sub-array that contains the entire range [first, last].
    /// </returns>
    QFixedArray GetRange(const pointer_uint_q uFirst, const pointer_uint_q uLast) const
    {
        QE_ASSERT_ERROR(!this->IsEmpty(), "The array is empty, it is not possible to get any range of elements.");
        QE_ASSERT_ERROR(uFirst < this->GetCount(), "The first index is out of bounds.");
        QE_ASSERT_ERROR(uLast < this->GetCount(), "The last index is out of bounds.");
        QE_ASSERT_ERROR(uFirst <= uLast, "The first index must be lower than or equal to the last index.");

        return QFixedArray(scast_q(m_allocator.GetPointer(), T*) + uFirst, uLast - uFirst + 1U);
    }
    
    /// <summary>
    /// Gets a range of elements from the array.
    /// </summary>
    /// <remarks>
    /// The array must not be empty. The copy constructor of every element of the sub-array will be called.
    /// </remarks>
    /// <param name="first">[IN] The position of the first element of the range. It must not be an end position. 
    /// It must be prior to the position of the last element.</param>
    /// <param name="last">[IN] The position of the last element of the range. It must not be an end position. 
    /// It must be posterior to the position of the first element.</param>
    /// <returns>
    /// The sub-array that contains the entire range [first, last].
    /// </returns>
    QFixedArray GetRange(const typename QFixedArray::QConstArrayIterator first, const typename QFixedArray::QConstArrayIterator last) const
    {
        QE_ASSERT_ERROR(!this->IsEmpty(), "The array is empty, it is not possible to get any range of elements.");
        QE_ASSERT_ERROR(!first.IsEnd(), "The first position is an end position.");
        QE_ASSERT_ERROR(!last.IsEnd(), "The last position is an end position.");
        QE_ASSERT_ERROR(first.IsValid(), "The first position is not valid.");
        QE_ASSERT_ERROR(last.IsValid(), "The last position is not valid.");
        QE_ASSERT_ERROR(first <= last, "The first index must be lower than or equal to the last index.");

        return QFixedArray(&*first, &*last - &*first + 1U);
    }
    
    /// <summary>
    /// Swaps two elements of the array.
    /// </summary>
    /// <remarks>
    /// The array must not be empty.<br/>
    /// No assignment operator nor copy constructors are called during this operation.
    /// </remarks>
    /// <param name="uElementA">[IN] The index (zero-based) of an element. It must be lower than the number of elements of the array.</param>
    /// <param name="uElementB">[IN] The index (zero-based) of the other element. It must be lower than the number of elements of the array.</param>
    void Swap(const pointer_uint_q uElementA, const pointer_uint_q uElementB)
    {
        using Kinesis::QuimeraEngine::Common::DataTypes::u8_q;

        QE_ASSERT_ERROR(uElementA < this->GetCount(), "The first index is out of bounds.");
        QE_ASSERT_ERROR(uElementB < this->GetCount(), "The last index is out of bounds.");
        QE_ASSERT_WARNING(uElementA != uElementB, "Both elements are the same.");

        T* pElementA = &this->GetValue(uElementA);
        T* pElementB = &this->GetValue(uElementB);

        u8_q arBytes[sizeof(T)];
        memcpy(arBytes,   pElementA, sizeof(T));
        memcpy(pElementA, pElementB, sizeof(T));
        memcpy(pElementB, arBytes,   sizeof(T));
    }
    
    /// <summary>
    /// Swaps two elements of the array.
    /// </summary>
    /// <remarks>
    /// The array must not be empty.<br/>
    /// No assignment operator nor copy constructors are called during this operation.
    /// </remarks>
    /// <param name="elementA">[IN] The position of an element. It must not be an end position.</param>
    /// <param name="elementB">[IN] The position of the other element. It must not be an end position.</param>
    void Swap(const typename QFixedArray::QArrayIterator elementA, const typename QFixedArray::QArrayIterator elementB)
    {
        using Kinesis::QuimeraEngine::Common::DataTypes::u8_q;

        QE_ASSERT_ERROR(!elementA.IsEnd(), "The element A position is an end position.");
        QE_ASSERT_ERROR(!elementB.IsEnd(), "The element B position is an end position.");
        QE_ASSERT_ERROR(elementA.IsValid(), "The element A's position is not valid.");
        QE_ASSERT_ERROR(elementB.IsValid(), "The element B's position is not valid.");
        QE_ASSERT_WARNING(elementA != elementB, "Both elements are the same.");

        T* pElementA = &*elementA;
        T* pElementB = &*elementB;

        u8_q arBytes[sizeof(T)];
        memcpy(arBytes,   pElementA, sizeof(T));
        memcpy(pElementA, pElementB, sizeof(T));
        memcpy(pElementB, arBytes,   sizeof(T));
    }
    
    /// <summary>
    /// Equality operator that checks whether two arrays are equal.
    /// </summary>
    /// <remarks>
    /// Every element is compared with the element at the same position in the other array. Elements are compared using the array's comparator.
    /// </remarks>
    /// <param name="array">[IN] The array to compare to.</param>
    /// <returns>
    /// True if all the elements of both arrays are equal; False otherwise.
    /// </returns>
    bool operator==(const QFixedArray &array) const
    {
        bool bAreEqual = true;

        // If they are not the same instance and they are not both empty
        if(this != &array && !(m_uLast == QFixedArray::END_POSITION_FORWARD && array.m_uLast == QFixedArray::END_POSITION_FORWARD))
        {
            // If they have the same number of elements
            if(m_uLast == array.m_uLast)
            {
                QFixedArray::QConstArrayIterator itThis = this->GetFirst();
                QFixedArray::QConstArrayIterator itInput = array.GetFirst();

                while(bAreEqual && !itThis.IsEnd())
                {
                    bAreEqual = m_comparator.Compare(*itThis, *itInput) == 0;
                    ++itThis;
                    ++itInput;
                }
            }
            else
                bAreEqual = false;
        }

        return bAreEqual;
    }
    
    /// <summary>
    /// Inequality operator that checks whether two arrays are not equal.
    /// </summary>
    /// <remarks>
    /// Every element is compared with the element at the same position in the other array. Elements are compared using the array's comparator.
    /// </remarks>
    /// <param name="array">[IN] The array to compare to.</param>
    /// <returns>
    /// True if not all the elements of both arrays are equal; False otherwise.
    /// </returns>
    bool operator!=(const QFixedArray &array) const
    {
        return !this->operator==(array);
    }
    
    /// <summary>
    /// Checks if any of the elements in the array is equal to a given one.
    /// </summary>
    /// <remarks>
    /// Elements are compared using the array's comparator.
    /// </remarks>
    /// <param name="element">[IN] The element to be searched through the array.</param>
    /// <returns>
    /// True if the element is found; False otherwise.
    /// </returns>
    bool Contains(const T &element) const
    {
        const T* pElement = scast_q(m_allocator.GetPointer(), T*);
        pointer_uint_q uIndex = 0;
        const pointer_uint_q ARRAY_COUNT = this->GetCount();
        
        bool bElementFound = false;

        while(uIndex < ARRAY_COUNT && !bElementFound)
        {
            bElementFound = m_comparator.Compare(*pElement, element) == 0;
            ++pElement;
            ++uIndex;
        }

        return bElementFound;
    }
    
    /// <summary>
    /// Searches for the first element in the array equal to a given one and obtains its position index.
    /// </summary>
    /// <remarks>
    /// Elements are compared using the array's comparator.
    /// </remarks>
    /// <param name="element">[IN] The element to be searched through the array.</param>
    /// <returns>
    /// The position index (zero-based) of the first element that is equal to the input one. If the element is not found,
    /// the ELEMENT_NOT_FOUND constant will be returned.
    /// </returns>
    pointer_uint_q IndexOf(const T &element) const
    {
        const T* pElement = scast_q(m_allocator.GetPointer(), T*);
        pointer_uint_q uIndex = 0;
        const pointer_uint_q ARRAY_COUNT = this->GetCount();
        
        bool bElementFound = false;

        while(uIndex < ARRAY_COUNT && !bElementFound)
        {
            bElementFound = m_comparator.Compare(*pElement, element) == 0;
            ++pElement;
            ++uIndex;
        }

        return bElementFound ? uIndex - 1U : QFixedArray::ELEMENT_NOT_FOUND;
    }
    
    /// <summary>
    /// Searches for the first element in the array equal to a given one, starting from a concrete position, and obtains its position index.
    /// </summary>
    /// <remarks>
    /// Elements are compared using the array's comparator.
    /// </remarks>
    /// <param name="element">[IN] The element to be searched through the array.</param>
    /// <param name="uStartIndex">[IN] The start position to search from. It must be lower than the number of elements in the array.</param>
    /// <returns>
    /// The position index (zero-based) of the first element that is equal to the input one. If the element is not found,
    /// the ELEMENT_NOT_FOUND constant will be returned.
    /// </returns>
    pointer_uint_q IndexOf(const T &element, const pointer_uint_q uStartIndex) const
    {
        const pointer_uint_q ARRAY_COUNT = this->GetCount();
        pointer_uint_q uIndex = uStartIndex;

        QE_ASSERT_WARNING(uIndex < ARRAY_COUNT, "The input start index must be lower than the number of elements in the array.");

        const T* pElement = scast_q(m_allocator.GetPointer(), T*) + uStartIndex;
        
        bool bElementFound = false;

        while(uIndex < ARRAY_COUNT && !bElementFound)
        {
            bElementFound = m_comparator.Compare(*pElement, element) == 0;
            ++pElement;
            ++uIndex;
        }

        return bElementFound ? uIndex - 1U : QFixedArray::ELEMENT_NOT_FOUND;
    }
    
    /// <summary>
    /// Searches for the first element in the array equal to a given one and obtains its position.
    /// </summary>
    /// <remarks>
    /// Elements are compared using the array's comparator.
    /// </remarks>
    /// <param name="element">[IN] The element to be searched through the array.</param>
    /// <returns>
    /// The position of the first element that is equal to the input one. If the element is not found,
    /// the iterator will point to the forward end position.
    /// </returns>
    QArrayIterator PositionOf(const T &element) const
    {
        const T* pElement = scast_q(m_allocator.GetPointer(), T*);
        QFixedArray::QArrayIterator position = this->GetFirst();

        bool bElementFound = false;

        while(!position.IsEnd() && !bElementFound)
        {
            bElementFound = m_comparator.Compare(*pElement, element) == 0;
            ++pElement;
            ++position;
        }

        return bElementFound ? --position : position;
    }
    
    /// <summary>
    /// Searches for the first element in the array equal to a given one, starting from a concrete position, and obtains its position.
    /// </summary>
    /// <remarks>
    /// Elements are compared using the array's comparator.
    /// </remarks>
    /// <param name="element">[IN] The element to be searched through the array.</param>
    /// <param name="startPosition">[IN] The start position to search from. It must not point to an end position.</param>
    /// <returns>
    /// The position of the first element that is equal to the input one. If the element is not found,
    /// the iterator will point to the forward end position.
    /// </returns>
    QArrayIterator PositionOf(const T &element, const typename QFixedArray::QConstArrayIterator &startPosition) const
    {
        QE_ASSERT_WARNING(!startPosition.IsEnd(), "The input start position must not point to an end position.");
        QE_ASSERT_ERROR(startPosition.IsValid(), "The input start position must not point to an end position.");

        const T* pElement = startPosition.IsEnd() ? null_q : &*startPosition;
        QFixedArray::QArrayIterator position(this, pElement - scast_q(m_allocator.GetPointer(), const T*));

        bool bElementFound = false;

        while(!position.IsEnd() && !bElementFound)
        {
            bElementFound = m_comparator.Compare(*pElement, element) == 0;
            ++pElement;
            ++position;
        }

        return bElementFound ? --position : position;
    }

private:

    /// <summary>
    /// Checks if the multiplication of two operands overflows for the pointer_uint_q type.
    /// </summary>
    /// <param name="uOperandA">[IN] Operand to multiply.</param>
    /// <param name="uOperandB">[IN] Operand to multiply.</param>
    /// <returns>
    /// True if the result overflows for the pointer_uint_q type.
    /// </returns>
    bool _MultiplicationOverflows(pointer_uint_q uOperandA, pointer_uint_q uOperandB) const
    {
        return this->_HighestOneBitPosition(uOperandA) + this->_HighestOneBitPosition(uOperandB) > sizeof(pointer_uint_q) * 8;
    }

    /// <summary>
    /// Gets the highest one bit position of a number converted to binary format.
    /// </summary>
    /// <param name="uValue">[IN] Number to trate.</param>
    /// <returns>
    /// The highest one bit position of the number passed by parameter.
    /// </returns>
    pointer_uint_q _HighestOneBitPosition(pointer_uint_q uValue) const
    {
        pointer_uint_q position = 0;

        while(uValue != 0)
        {
            ++position;
            uValue >>= 1;
        }

        return position;
    }


    // PROPERTIES
    // ---------------
public:
       /// <summary>
    /// Returns a constant pointer to the allocator.
    /// </summary>
    /// <returns>
    /// Constant pointer to the allocator.
    /// </returns>
    const AllocatorT* GetAllocator() const
    {
        return &m_allocator;
    }

    /// <summary>
    /// Returns the number of elements in the array.
    /// </summary>
    /// <returns>
    /// The number of elements in the array.
    /// </returns>
    pointer_uint_q GetCount() const
    {
        return m_allocator.GetAllocatedBytes() / sizeof(T);
    }

    /// <summary>
    /// Returns the number of elements that can be stored in the array without a reallocation.
    /// </summary>
    /// <returns>
    /// The number of elements that can be stored in the array without a reallocation.
    /// </returns>
    pointer_uint_q GetCapacity() const
    {
        return m_allocator.GetPoolSize() / sizeof(T);
    }

    /// <summary>
    /// Checks whether the array is empty or not.
    /// </summary>
    /// <returns>
    /// True if the array is empty.
    /// </returns>
    bool IsEmpty() const
    {
        return (this->GetCount() == 0);
    }

    // ATTRIBUTES
    // ---------------

protected:

    /// <summary>
    /// The Comparator.
    /// </summary>
    ComparatorT m_comparator;

    /// <summary>
    /// Index of the first element in the sequence. If the index points to an invalid position its value is the
    /// END_POSITION_BACKWARD constant.
    /// </summary>
    pointer_uint_q m_uFirst;

       /// <summary>
    /// Index of the last element in the sequence. If the index points to an invalid position its value is the
    /// END_POSITION_FORWARD constant.
    /// </summary>
    pointer_uint_q m_uLast;
    
    /// <summary>
    /// The allocator which stores the array elements.
    /// </summary>
    AllocatorT m_allocator;

};


// ATTRIBUTE INITIALIZATION
// ----------------------------
// Note: This definition was moved here from the interior of the class because compilation failed when using GCC compiler, the reason is not yet clear
template<class T, class AllocatorT, class ComparatorT>
const pointer_uint_q QFixedArray<T, AllocatorT, ComparatorT>::END_POSITION_FORWARD = -1;


} //namespace Containers
} //namespace Tools
} //namespace QuimeraEngine
} //namespace Kinesis

#endif // __QFIXEDARRAY__

