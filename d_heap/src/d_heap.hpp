/*!
*  D-Heap class
*
*  Created by Ivan Semenenko on 20.12.2017.
*  Copyright © 2017 Ivan Semenenko. Source code is distributed under MIT license.
*
*/

/*-----------------------------------------------------------------------------------*/

#ifndef DHEAP_HPP_
#define DHEAP_HPP_

/*-----------------------------------------------------------------------------------*/

#include "messages.hpp"

#include <initializer_list>
#include <stdexcept>
#include <cstring>
#include <vector>

/*-----------------------------------------------------------------------------------*/

template< class T, class Compare = std::less< typename T > >
class DHeap
{
	public:

		using value_type      = T;
        using pointer_type    = T*;
        using size_type       = std::size_t;
		using reference       = T&;
        using const_reference = T const&;

        /*---------------------------------------------------------------------------*/

        explicit DHeap( size_type _dParam = 2, Compare const& _predicate = Compare() );

        DHeap( std::initializer_list< T > _list, size_type _dParam = 2, Compare const& _predicate = Compare() );

        DHeap( DHeap< T, Compare > const& _other );

        DHeap( DHeap< T, Compare > && _other );

        /*---------------------------------------------------------------------------*/

        DHeap< T, Compare >& operator = ( DHeap< T, Compare > const& _other );

        DHeap< T, Compare >& operator = ( DHeap< T, Compare > && _other );

        /*---------------------------------------------------------------------------*/

        const_reference get_min() const;

		size_type size() const;

		bool empty() const;

        /*---------------------------------------------------------------------------*/

        void insert( const_reference _value );

		void delete_min();

		void hilling();

		void clear();

        /*---------------------------------------------------------------------------*/

		~DHeap() = default;

	private:

        std::vector< value_type > m_data;

		size_type m_dParam;

		Compare m_functor;

        /*---------------------------------------------------------------------------*/

		size_type min_child( size_type _position );

		void swim( size_type _position );

		void sink( size_type _position );

        /*---------------------------------------------------------------------------*/

		void checkEmpty() const;

}; // class DHeap

/*-----------------------------------------------------------------------------------*/

template< class T, class Compare >
DHeap< T, Compare >::DHeap( size_type _dParam, Compare const& _predicate )
    : m_data(), m_dParam( _dParam ), m_functor( _predicate ) {}

/*-----------------------------------------------------------------------------------*/

template< class T, class Compare >
DHeap< T, Compare >::DHeap( std::initializer_list< T > _list, size_type _dParam, Compare const& _predicate )
    : m_data( _list ), m_dParam( _dParam ), m_functor( _predicate )
{
	hilling();
}

/*-----------------------------------------------------------------------------------*/

template< class T, class Compare >
DHeap< T, Compare >::DHeap( DHeap< T, Compare > const& _other )
    : m_data( _other.m_data ), m_dParam( _other.m_dParam ), m_functor( _other.m_functor ) {}

/*-----------------------------------------------------------------------------------*/

template< class T, class Compare >
DHeap< T, Compare >::DHeap( DHeap< T, Compare > && _other )
    : m_data( std::move( _other.m_data ) )
    , m_dParam( _other.m_dParam )
    , m_functor( std::move( _other.m_functor ) ) {}

/*-----------------------------------------------------------------------------------*/

template< class T, class Compare >
DHeap< T, Compare >& DHeap< T, Compare >::operator = ( DHeap< T, Compare > const& _other )
{
	m_data    = _other.m_data;
	m_dParam  = _other.m_dParam;
	m_functor = _other.m_functor;

	return *this;
}

/*-----------------------------------------------------------------------------------*/

template< class T, class Compare >
DHeap< T, Compare >& DHeap< T, Compare >::operator = ( DHeap< T, Compare > && _other )
{
	std::swap( m_data, _other.m_data );
	std::swap( m_dParam, _other.m_dParam );
	std::swap( m_functor, _other.m_functor );

	return *this;
}

/*-----------------------------------------------------------------------------------*/

template< class T, class Compare >
typename DHeap< T, Compare >::const_reference DHeap< T, Compare >::get_min() const
{
	checkEmpty();
	return m_data[0];
}

/*-----------------------------------------------------------------------------------*/

template< class T, class Compare >
typename DHeap< T, Compare >::size_type DHeap< T, Compare >::size() const
{
	return m_data.size();
}

/*-----------------------------------------------------------------------------------*/

template< class T, class Compare >
bool DHeap< T, Compare >::empty() const
{
	return m_data.empty();
}

/*-----------------------------------------------------------------------------------*/

template< class T, class Compare >
void DHeap< T, Compare >::insert( const_reference _value )
{
	m_data.push_back( _value );
	swim( size() - 1 );
}

/*-----------------------------------------------------------------------------------*/

template< class T, class Compare >
void DHeap< T, Compare >::delete_min()
{
	checkEmpty();

	std::swap( m_data[0], m_data[size() - 1] );
	m_data.pop_back();

	if( size() )
		sink( 0 );
}

/*-----------------------------------------------------------------------------------*/

template< class T, class Compare >
void DHeap< T, Compare >::hilling()
{
	checkEmpty();

    for( size_type i = 0; i < size(); ++i )
		swim( i );
}

/*-----------------------------------------------------------------------------------*/

template< class T, class Compare >
void DHeap< T, Compare >::clear()
{
	m_data.clear();
}

/*-----------------------------------------------------------------------------------*/

template< class T, class Compare >
typename DHeap< T, Compare >::size_type DHeap< T, Compare >::min_child( size_type _position )
{
    if( _position * m_dParam >= size() - 1 )
		return 0;
	else
	{
		size_type first_child = ( _position * m_dParam ) + 1;
        size_type last_child  = ( _position + 1 ) * m_dParam;

        if( last_child >= size() )
			last_child = size() - 1;

        pointer_type minElement = &m_data[first_child];
		size_type posMin = first_child;

        for( size_type i = first_child + 1; i <= last_child; ++i )
		{
            if( m_functor( m_data[i], *minElement ) )
			{
                minElement = &m_data[i];
				posMin = i;
			}
		}

        if( m_functor( *minElement, m_data[_position] ) )
			return posMin;
		else
			return 0;
	}
}

/*-----------------------------------------------------------------------------------*/

template< class T, class Compare >
void DHeap< T, Compare >::swim( size_type _position )
{
    size_type parent = ( _position - 1 ) / m_dParam;

	while( _position && m_functor( m_data[_position], m_data[parent] ) )
	{
		std::swap( m_data[_position], m_data[parent] );
		_position = parent; 
        parent = ( _position - 1 ) / m_dParam;
	}
}

/*-----------------------------------------------------------------------------------*/

template< class T, class Compare >
void DHeap< T, Compare >::sink( size_type _position )
{
	size_type wrong = min_child( _position );

    while( wrong && m_functor( m_data[wrong], m_data[_position] ) )
	{
		std::swap( m_data[wrong], m_data[_position] );
		_position = wrong;
		wrong = min_child( _position );
	}
}

/*-----------------------------------------------------------------------------------*/

template< class T, class Compare >
void DHeap< T, Compare >::checkEmpty() const
{
    if( m_data.empty() )
        throw DEBUG_EXCEPTION_EMPTY
}

/*-----------------------------------------------------------------------------------*/

#endif // DHEAP_HPP_

/*-----------------------------------------------------------------------------------*/
