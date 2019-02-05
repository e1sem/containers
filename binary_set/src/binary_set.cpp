/** (C) 2016 Ivan Semenenko */

#include "binary_set.hpp"
#include "messages.hpp"

#include <algorithm>
#include <stdexcept>
#include <cstring>
#include <cmath>

/*-----------------------------------------------------------------------------------*/

BinarySet::BinarySet( size_type _size )
    :    m_size{ _size }
{
    checkInitialSize( _size );

    size_type cellsCount = get_cell( _size );
    m_pBitVector = new block_type[cellsCount];
    clear();
}

/*-----------------------------------------------------------------------------------*/

BinarySet::BinarySet( BinarySet const& _other )
{
    copy_class( _other );
}

/*-----------------------------------------------------------------------------------*/

BinarySet::BinarySet( BinarySet && _other )
    :    m_size{ _other.m_size }
    ,    m_pBitVector{ nullptr }
{
    std::swap( m_pBitVector, _other.m_pBitVector );
}

/*-----------------------------------------------------------------------------------*/

BinarySet::~BinarySet()
{
    delete[] m_pBitVector;
}

/*-----------------------------------------------------------------------------------*/

BinarySet&
BinarySet::operator = ( BinarySet const& _other )
{
    delete[] m_pBitVector;
    copy_class( _other );

    return *this;
}

/*-----------------------------------------------------------------------------------*/

BinarySet&
BinarySet::operator = ( BinarySet && _other )
{
    std::swap( m_size, _other.m_size );
    std::swap( m_pBitVector, _other.m_pBitVector );

    return *this;
}

/*-----------------------------------------------------------------------------------*/

BinarySet::size_type
BinarySet::size() const noexcept
{
    return m_size;
}

/*-----------------------------------------------------------------------------------*/

bool
BinarySet::is_empty() const noexcept
{
    size_type cellsCount = get_cell( m_size );
    for( auto i = 0; i < cellsCount; ++i )
        if( static_cast< bool >( m_pBitVector[i] ) )
            return false;

    return true;
}

/*-----------------------------------------------------------------------------------*/

void
BinarySet::clear() noexcept
{
    size_type memorySize = get_cell( m_size ) * sizeof( block_type );

    std::memset(
            m_pBitVector
        ,    NULL
        ,    memorySize
    );
}

/*-----------------------------------------------------------------------------------*/

bool
BinarySet::has_key( size_type _index ) const
{
    checkKeyRange( _index );
    return m_pBitVector[get_pos( _index )] & ( 1 << _index );
}

/*-----------------------------------------------------------------------------------*/

void
BinarySet::insert_key( size_type _index )
{
    checkKeyRange( _index );
    m_pBitVector[get_pos( _index )] |= ( 1 << _index );
}

/*-----------------------------------------------------------------------------------*/

void
BinarySet::remove_key( size_type _index )
{
    checkKeyRange( _index );
    m_pBitVector[get_pos( _index )] &= ~( 1 << _index );
}

/*-----------------------------------------------------------------------------------*/

void
BinarySet::flip_key( size_type _index )
{
    checkKeyRange( _index );
    m_pBitVector[get_pos( _index )] ^= ( 1 << _index );
}

/*-----------------------------------------------------------------------------------*/

void
BinarySet::set_bits() noexcept
{
    size_type memorySize = get_cell( m_size ) * sizeof( block_type );

    std::memset(
            m_pBitVector
        ,    1
        ,    memorySize
    );
}

/*-----------------------------------------------------------------------------------*/

void
BinarySet::reset_bits() noexcept
{
    clear();
}

/*-----------------------------------------------------------------------------------*/

void
BinarySet::flip_bits() noexcept
{
    // TODO: consider better solution
    size_type cellsCount = get_cell( m_size );
    for( auto i = 0; i < cellsCount; ++i )
        m_pBitVector[i] = ~m_pBitVector[i];
}

/*-----------------------------------------------------------------------------------*/

BinarySet
BinarySetUnite( BinarySet const& _left,    BinarySet const& _right )
{
    auto unite = [&](
            BinarySet const& _bigger
        ,    BinarySet const& _smaller
    ) -> BinarySet
    {
        BinarySet result( _bigger );
        BinarySet::size_type smCells = _smaller.get_cell( _smaller.size() );

        for( auto i = 0; i < smCells; ++i )
            result.m_pBitVector[i] |= _smaller.m_pBitVector[i];

        return result;
    };

    if( _left.m_size > _right.m_size )
        return unite( _left, _right );
    else
        return unite( _right, _left );
}

/*-----------------------------------------------------------------------------------*/

BinarySet
BinarySetIntersect( BinarySet const& _left,    BinarySet const& _right )
{
    BinarySet::size_type minSize = std::min( _left.m_size, _right.m_size );
    BinarySet result( minSize );

    for( auto i = 0; i < result.get_cell( minSize ); ++i )
        result.m_pBitVector[i] = 
                _left.m_pBitVector[i]
            &    _right.m_pBitVector[i]
        ;

    return result;
}

/*-----------------------------------------------------------------------------------*/

BinarySet
BinarySetDifference( BinarySet const& _left,    BinarySet const& _right )
{
    BinarySet::size_type rSize = _left.m_size;
    BinarySet result( rSize );

    for( auto i = 0; i < result.get_cell( rSize ); ++i )
        result.m_pBitVector[i] =
                _left.m_pBitVector[i]
            &    ~_right.m_pBitVector[i]
        ;

    return result;
}

/*-----------------------------------------------------------------------------------*/

BinarySet
BinarySetSymmDiff( BinarySet const& _left,    BinarySet const& _right )
{
    auto symDiff = [&](
            BinarySet const& _bigger
        ,    BinarySet const& _smaller
    ) -> BinarySet
    {
        BinarySet result( _bigger );
        BinarySet::block_type minCells = _smaller.get_cell( _smaller.m_size );

        for( auto i = 0; i < minCells; ++i )
            result.m_pBitVector[i] ^= _right.m_pBitVector[i];

        return result;
    };

    if( _left.m_size > _right.m_size )
        return symDiff( _left, _right );
    else
        return symDiff( _right, _left );
}

/*-----------------------------------------------------------------------------------*/

BinarySet::size_type
BinarySet::get_cell( size_type _size ) const noexcept
{
    return ( _size - 1 ) / ( sizeof( block_type ) * 8) + 1;
}

/*-----------------------------------------------------------------------------------*/

BinarySet::size_type
BinarySet::get_pos( size_type _index ) const noexcept
{
    size_type position = --_index;
    position /= ( sizeof( block_type ) * 8 );
    _index %= ( sizeof( block_type ) * 8 );
    return position;
}

/*-----------------------------------------------------------------------------------*/

void
BinarySet::copy_class( BinarySet const& _other )
{
    m_size = _other.m_size;
    size_type cellsCount = get_cell( m_size );
    m_pBitVector = new block_type[cellsCount];

    std::memcpy(
        m_pBitVector
        , _other.m_pBitVector
        , cellsCount * sizeof( block_type )
    );
}

/*-----------------------------------------------------------------------------------*/

void
BinarySet::checkKeyRange( size_type _index ) const
{
    if( _index < 1 || _index > m_size )
        throw std::logic_error( Messages::OutOfRange );
}

/*-----------------------------------------------------------------------------------*/

void
BinarySet::checkInitialSize( size_type _size ) const
{
    if( _size < 1 )
        throw std::logic_error( Messages::InvalidSize );
}

/*-----------------------------------------------------------------------------------*/
