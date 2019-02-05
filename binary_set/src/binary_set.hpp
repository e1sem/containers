/** (C) 2016 Ivan Semenenko */

#ifndef BINARY_SET_HPP_
#define BINARY_SET_HPP_

/*-----------------------------------------------------------------------------------*/

#include <string>

/*-----------------------------------------------------------------------------------*/

class BinarySet
{

    public:

        /*---------------------------------------------------------------------------*/

        using block_type = unsigned int;
        using size_type = std::size_t;
        using bit_type = bool;

        /*---------------------------------------------------------------------------*/

        explicit BinarySet( size_type _size );

        BinarySet( BinarySet const& _other );

        BinarySet( BinarySet && _other );

        ~BinarySet();

        /*---------------------------------------------------------------------------*/

        BinarySet& operator = ( BinarySet const& _other );

        BinarySet& operator = (BinarySet && _other );

        /*---------------------------------------------------------------------------*/

        size_type size() const noexcept;

        bool is_empty() const noexcept;

        void clear() noexcept;

        /*---------------------------------------------------------------------------*/

        bool has_key( size_type _index ) const;

        void insert_key( size_type _index );

        void remove_key( size_type _index );

        void flip_key( size_type _index );

        /*---------------------------------------------------------------------------*/

        void set_bits() noexcept;

        void reset_bits() noexcept;

        void flip_bits() noexcept;    

        /*---------------------------------------------------------------------------*/

        /*
        bit_type operator [] ( size_type _index ) const;

        bit_type const& operator [] ( size_type _index );
        */

        /*---------------------------------------------------------------------------*/

        /*
        std::string to_string() const noexcept;

        unsigned int to_uint() const noexcept;

        unsigned long to_ulong() const noexcept;
        */

        /*---------------------------------------------------------------------------*/

        friend BinarySet BinarySetUnite( BinarySet const& _left, BinarySet const& _right );

        friend BinarySet BinarySetIntersect( BinarySet const& _left, BinarySet const& _right );

        friend BinarySet BinarySetDifference( BinarySet const& _left, BinarySet const& _right );

        friend BinarySet BinarySetSymmDiff( BinarySet const& _left, BinarySet const& _right );

    private:

        size_type get_cell( size_type _size ) const noexcept;

        size_type get_pos( size_type _index ) const noexcept;

        void copy_class( BinarySet const& _other );

        /*---------------------------------------------------------------------------*/

        inline void checkKeyRange( size_type _index ) const;

        inline void checkInitialSize( size_type _size ) const;

        /*---------------------------------------------------------------------------*/

        block_type* m_pBitVector;

        size_type m_size;

}; // class BinarySet

/*-----------------------------------------------------------------------------------*/

#endif // BINARY_SET_HPP_

/*-----------------------------------------------------------------------------------*/
