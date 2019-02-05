/** (C) 2017 Ivan Semenenko */

#ifndef BIG_INTEGER_HPP_
#define BIG_INTEGER_HPP_

/*-----------------------------------------------------------------------------------*/

#include <stdexcept>
#include <cstring>
#include <algorithm>
#include <string>

/*-----------------------------------------------------------------------------------*/

class BigInteger
{
    public:

        using size_type = std::size_t;
        using block_type = unsigned char;

        /*---------------------------------------------------------------------------*/

        BigInteger();

        BigInteger( std::string const& _str );

        BigInteger( BigInteger const& _other );

        BigInteger( BigInteger && _other );

        ~BigInteger();

        /*---------------------------------------------------------------------------*/

        size_type getDigitsCount() const noexcept;

        /*---------------------------------------------------------------------------*/

        BigInteger& operator = ( BigInteger const& _other );

        BigInteger& operator = ( BigInteger && _other );

        /*---------------------------------------------------------------------------*/

        block_type operator [] ( size_type _position ) const;

        block_type& operator [] ( size_type _position );

        operator bool() const;

        /*---------------------------------------------------------------------------*/

        friend bool operator < ( BigInteger const& _left, BigInteger const& _right );

        friend bool operator > ( BigInteger const& _left, BigInteger const& _right );

        friend bool operator == ( BigInteger const& _left, BigInteger const& _right );

        friend bool operator != ( BigInteger const& _left, BigInteger const& _right );

        friend bool operator <= ( BigInteger const& _left, BigInteger const& _right );

        friend bool operator >= ( BigInteger const& _left, BigInteger const& _right );

        friend std::istream& operator >> ( std::istream & _stream, BigInteger & _bigInt );

        friend std::ostream& operator << ( std::ostream & _stream, BigInteger const& _bigInt );

        /*---------------------------------------------------------------------------*/

        BigInteger& operator += ( BigInteger const& _other );

        BigInteger& operator += ( size_type _integer );

        friend BigInteger operator + ( BigInteger _left, BigInteger const& _right );

        friend BigInteger operator + ( BigInteger _bigInt, size_type _integer );

    private:

        void fillArray( std::string const& _string );

        size_type getIntLength ( size_type _number );

        void carryDecision( block_type * _array, size_type _finish );

        /*---------------------------------------------------------------------------*/

        void checkNumberString( std::string const& _string ) const;

        void checkRange( size_type _position ) const;

        void checkDigit( char _digit ) const;

        /*---------------------------------------------------------------------------*/

        block_type* m_pData;

        size_type m_size;

        /*---------------------------------------------------------------------------*/

}; // class BigInteger

/*-----------------------------------------------------------------------------------*/

BigInteger operator "" _b ( const char * _str );

/*-----------------------------------------------------------------------------------*/

#endif // BIG_INTEGER_HPP_

/*-----------------------------------------------------------------------------------*/
