/** (C) 2017 Ivan Semenenko */

#include "biginteger.hpp"
#include "messages.hpp"

/*-----------------------------------------------------------------------------------*/

BigInteger::BigInteger()
    :    m_pData{ nullptr }
    ,    m_size{ 0 }
{
}

/*-----------------------------------------------------------------------------------*/

BigInteger::BigInteger( std::string const& _str )
{
    checkNumberString( _str );

    m_size = _str.length();
    m_pData = new block_type[m_size];

    fillArray( _str );
}

/*-----------------------------------------------------------------------------------*/

BigInteger::BigInteger( BigInteger const& _other )
    :    m_size{ _other.m_size }
{
    m_pData = new block_type[m_size];

    std::memcpy(
            m_pData
        ,    _other.m_pData
        ,    m_size * sizeof( block_type )
    );
}

/*-----------------------------------------------------------------------------------*/

BigInteger::BigInteger( BigInteger && _other )
    :   m_pData{ nullptr }
    ,   m_size{ _other.m_size }
{
    std::swap( m_pData, _other.m_pData );
}

/*-----------------------------------------------------------------------------------*/

BigInteger::~BigInteger()
{
    delete [] m_pData;
}

/*-----------------------------------------------------------------------------------*/

BigInteger::size_type
BigInteger::getDigitsCount() const noexcept
{
    return m_size;
}

/*-----------------------------------------------------------------------------------*/

BigInteger&
BigInteger::operator = ( BigInteger const& _other )
{
    delete [] m_pData;

    m_size = _other.m_size;
    m_pData = new block_type[m_size];

    std::memcpy(
            m_pData
        ,    _other.m_pData
        ,    m_size * sizeof( char )
    );

    return *this;
}

/*-----------------------------------------------------------------------------------*/

BigInteger&
BigInteger::operator = ( BigInteger && _other )
{
    std::swap( m_size, _other.m_size );
    std::swap( m_pData, _other.m_pData );

    return *this;
}

/*-----------------------------------------------------------------------------------*/

bool
operator == ( BigInteger const& _left, BigInteger const& _right )
{
    if( _left.m_size != _right.m_size )
        return false;

    for( BigInteger::size_type count = 0; count < _left.m_size; ++count )
        if( _left.m_pData[count] != _right.m_pData[count] )
            return false;

    return true;
}

/*-----------------------------------------------------------------------------------*/

bool
operator != ( BigInteger const& _left, BigInteger const& _right )
{
    return !( _left == _right );
}

/*-----------------------------------------------------------------------------------*/

bool
operator < ( BigInteger const& _left, BigInteger const& _right )
{
    if( _left.m_size < _right.m_size )
        return true;
    if( _left.m_size > _right.m_size )
        return false;

    for( int count = _left.m_size - 1; count >= 0; --count )
        if( _left.m_pData[count] < _right.m_pData[count] )
            return true;

    return false;
}

/*-----------------------------------------------------------------------------------*/

bool
operator > ( BigInteger const& _left, BigInteger const& _right )
{
    return ( _left != _right ) && !( _left < _right );
}

/*-----------------------------------------------------------------------------------*/

bool
operator <= ( BigInteger const& _left, BigInteger const& _right )
{
    return !( _left > _right );
}

/*-----------------------------------------------------------------------------------*/

bool
operator >= ( BigInteger const& _left, BigInteger const& _right )
{
    return !( _left < _right );
}

/*-----------------------------------------------------------------------------------*/

BigInteger::block_type
BigInteger::operator [] ( size_type _position ) const
{
    return m_pData[_position + 1];
}

/*-----------------------------------------------------------------------------------*/

BigInteger::block_type&
BigInteger::operator [] ( size_type _position )
{
    return m_pData[_position + 1];
}

/*-----------------------------------------------------------------------------------*/

BigInteger::operator bool () const
{
    if( m_size == 0 )
        return false;

    for( size_type count = 0; count < m_size; ++count )
        if( m_pData[count] != '0' )
            return true;

    return false;
}

/*-----------------------------------------------------------------------------------*/

std::istream&
operator >> ( std::istream & _stream, BigInteger & _bigInt )
{
    std::string inputString;
    _stream >> inputString;

    _bigInt.checkNumberString( inputString );

    delete [] _bigInt.m_pData;

    _bigInt.m_size  = inputString.length();
    _bigInt.m_pData = new BigInteger::block_type[_bigInt.m_size];

    _bigInt.fillArray( inputString );

    return _stream;
}

/*-----------------------------------------------------------------------------------*/

std::ostream&
operator << ( std::ostream & _stream, BigInteger const& _bigInt )
{
    std::string outputString;

    for( int count = _bigInt.m_size - 1; count >= 0; --count )
        outputString.push_back( _bigInt.m_pData[count] );

    _stream << outputString;

    return _stream;
}

/*-----------------------------------------------------------------------------------*/

BigInteger&
BigInteger::operator += ( BigInteger const& _other )
{
    size_type maxSize = std::max( m_size, _other.m_size );
    block_type* array = new block_type[++maxSize]; 

    std::memset(
            array
        ,    0
        ,    maxSize * sizeof( block_type )
    );
    
    for( size_type count = 0; count < m_size; ++count )
        array[count] += ( m_pData[count] - 48 );
    
    for( size_type count = 0; count < _other.m_size; ++count )
        array[count] += ( _other.m_pData[count] - 48 );

    carryDecision( array, maxSize );  

    delete [] m_pData;

    m_pData = array;
    m_size  = maxSize;

    return *this;
}

/*-----------------------------------------------------------------------------------*/

BigInteger&
BigInteger::operator += ( size_type _integer )
{
    size_type intLength = getIntLength( _integer );
    size_type maxSize = std::max( m_size, intLength );
    block_type* array = new block_type[++maxSize];

    std::memset(
        array
        , 0
        , maxSize * sizeof( block_type )
    );
    
    for( size_type count = 0; count < m_size; ++count )
        array[count] += ( m_pData[count] - 48 );
    
    size_type index{ 0 };
    for( 
        size_type count = 10;
        index < getIntLength( _integer );
        count *= 10, ++index
    )
        array[index] += _integer % count;

    carryDecision( array, maxSize );    

    delete[] m_pData;

    m_pData = array;
    m_size = maxSize;

    return *this;
}

/*-----------------------------------------------------------------------------------*/

BigInteger
operator + ( BigInteger _left, BigInteger const& _right )
{
    return _left += _right;
}

/*-----------------------------------------------------------------------------------*/

BigInteger
operator + ( BigInteger _bigInt, BigInteger::size_type _integer )
{
    return _bigInt += _integer;
}

/*-----------------------------------------------------------------------------------*/

void
BigInteger::fillArray( std::string const& _string )
{
    for( size_type count = 0; count < _string.length(); ++count )
        m_pData[count] = _string[count];
}

/*-----------------------------------------------------------------------------------*/

BigInteger::size_type
BigInteger::getIntLength( size_type _number )
{
    size_type resultLenght = 0;

    while( _number )
    {
        _number /= 10;
        ++resultLenght;
    }

    return resultLenght;
}

/*-----------------------------------------------------------------------------------*/

void
BigInteger::carryDecision( block_type * _array, size_type _finish )
{
    for( size_type count = 0; count < _finish - 1; ++count )
    {
        if( _array[count] > 9 )
        {
            _array[count] %= 10;
            _array[count + 1] += 1;
        }

        _array[count] += 48;
    }

    _array[_finish - 1] += 48;
}

/*-----------------------------------------------------------------------------------*/

void
BigInteger::checkNumberString( std::string const& _string ) const
{
    for( size_type count = 0; count < _string.length(); ++count )
        checkDigit( _string[count] );
}

/*-----------------------------------------------------------------------------------*/

void
BigInteger::checkRange( size_type _position ) const
{
    if( _position > m_size )
        throw std::logic_error( Messages::OutOfRange );
}

/*-----------------------------------------------------------------------------------*/

void
BigInteger::checkDigit( char _digit ) const
{
    if( !isdigit( _digit ) )
        throw std::logic_error( Messages::InvalidDigit );
}

/*-----------------------------------------------------------------------------------*/

BigInteger
operator "" _b ( const char * _str )
{
    std::string inputNumber( _str, std::strlen( _str ) );
    BigInteger result( inputNumber );

    return result;
}

/*-----------------------------------------------------------------------------------*/
