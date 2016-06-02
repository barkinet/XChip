#ifndef _XCHIP_UTILITY_VECTOR_H_
#define _XCHIP_UTILITY_VECTOR_H_
#include <cstring>
#include "Alloc.h"
#include "Assert.h"
#include "Log.h"
#include <type_traits>
#include <initializer_list>




namespace xchip { namespace utility {


template<class TYPE>
class Vector
{
	static_assert(std::is_pod<TYPE>::value, "Vector only support pod types");
public:
	Vector(const Vector&) = delete;
	Vector& operator=(const Vector&) = delete;

	Vector() = default;
	Vector(Vector&& other);
	~Vector();
	Vector& operator=(Vector&& other);

	bool empty() const;
	size_t size() const;
	size_t capacity() const;

	const TYPE* cbegin() const;
	const TYPE* cend() const;
	const TYPE* begin() const;
	const TYPE* end() const;
	const TYPE* data() const;
	const TYPE& operator[](size_t offset) const;


	bool initialize();
	bool initialize(const TYPE* data, const size_t size);
	bool initialize(const TYPE* begin, const TYPE* end); 
	bool initialize(std::initializer_list<TYPE> list);
	template<size_t sz>
	bool initialize(TYPE(&data)[sz]);
	bool initialize(const Vector& other);
	void initialize(Vector&& other);
	bool push_back(const TYPE& type);
	template<class ...Args>
	bool emplace_back(Args&& ...args);


	void clear();
	bool resize(const size_t size);
	bool reserve(const size_t size);

	TYPE* begin();
	TYPE* end();
	TYPE* data();
	TYPE& operator[](size_t offset);

	bool copy(const Vector& other);
	void swap(Vector& other);
	void free();


private:
	TYPE* _data = nullptr;
	size_t _size = 0;
};






template<class TYPE>
Vector<TYPE>::Vector(Vector&& other)
	: _data(other._data),
	_size(other._size)
{
	other._data = nullptr;
	other._size = 0;
}


template<class TYPE>
Vector<TYPE>::~Vector()
{
	this->free();
}


template<class TYPE>
Vector<TYPE>& Vector<TYPE>::operator=(Vector&& other)
{
	this->swap(other);
}




template<class TYPE>
inline bool Vector<TYPE>::empty() const
{
	return _size == 1 || _size == 0;
}


template<class TYPE>
inline size_t Vector<TYPE>::size() const
{
	return _size;
}


template<class TYPE>
inline size_t Vector<TYPE>::capacity() const
{
	return arr_size(_data);
}



template<class TYPE>
inline const TYPE* Vector<TYPE>::cbegin() const
{
	return _data;
}


template<class TYPE>
inline const TYPE* Vector<TYPE>::cend() const
{
	return _data + this->size();
}




template<class TYPE>
inline const TYPE* Vector<TYPE>::begin() const
{
	return _data;
}

template<class TYPE>
inline const TYPE* Vector<TYPE>::end() const
{
	return _data + this->size();
}

template<class TYPE>
inline const TYPE* Vector<TYPE>::data() const
{
	return _data;
}


template<class TYPE>
inline const TYPE& Vector<TYPE>::operator[](size_t offset) const
{
	ASSERT_MSG( offset < arr_size(_data), "offset overflow");
	return _data[offset];
}







template<class TYPE>
inline bool Vector<TYPE>::initialize()
{
	return this->resize(1);
}



template<class TYPE>
bool Vector<TYPE>::initialize(const TYPE* data, const size_t size)
{
	if(this->resize(size ? size : 1))
	{
		if(size) 
			std::memcpy(this->_data, data, sizeof(TYPE) * size);
		
		return true;
	}

	return false;
}




template<class TYPE>
inline bool Vector<TYPE>::initialize(const TYPE* begin, const TYPE* end)
{
	return this->initialize(begin, static_cast<size_t>(end - begin));
}



template<class TYPE>
inline bool Vector<TYPE>::initialize(std::initializer_list<TYPE> list)
{
	return this->initialize(list.begin(), list.size());
}



template<class TYPE>
template<size_t sz>
inline bool Vector<TYPE>::initialize(TYPE(&data)[sz])
{
	return this->initialize(data, sz);
}



template<class TYPE>
inline bool Vector<TYPE>::initialize(const Vector& other)
{
	return this->initialize(other._data, other._size);
}



template<class TYPE>
inline void Vector<TYPE>::initialize(Vector&& other)
{
	this->swap(other);
}





template<class TYPE>
inline TYPE* Vector<TYPE>::begin()
{
	return _data;
}




template<class TYPE>
inline TYPE* Vector<TYPE>::end()
{
	return _data + this->size();
}



template<class TYPE>
inline TYPE* Vector<TYPE>::data()
{
	return _data;
}



template<class TYPE>
inline TYPE& Vector<TYPE>::operator[](size_t offset)
{
	ASSERT_MSG( offset < arr_size(_data), "offset overflow");
	return _data[offset];
}






template<class TYPE>
bool Vector<TYPE>::push_back(const TYPE& type)
{
	ASSERT_MSG(_size, "Vector not initialized!");

	if( this->capacity() > _size )
	{	
		_data[_size-1] = type;
		++_size;
		return true;
	}

	else if(this->reserve(_size + 5)) 
	{
		_data[_size-1] = type;
		++_size;
		return true;
	}

	return false;
}

template<class TYPE>
template<class ...Args>
bool Vector<TYPE>::emplace_back(Args&& ...args)
{
	ASSERT_MSG(_size, "Vector not initialized!");

	if( this->capacity() > _size )
	{
		_data[_size-1] = TYPE{ std::forward<Args>(args)... };
		++_size;
		return true;
	}

	else if(this->reserve(_size + 5)) 
	{	
		_data[_size-1] = TYPE{std::forward<Args>(args)...};
		++_size;
		return true;
	}

	return false;
}







template<class TYPE>
void Vector<TYPE>::clear()
{
	ASSERT_MSG(_size, "Vector is not initialized");
	std::memset(_data, 0, sizeof(TYPE) * _size);
	_size = 1;
}




template<class TYPE>
bool Vector<TYPE>::reserve(const size_t size)
{
	const auto _capacity = this->capacity();
	
	TYPE* buff = nullptr;

	if(_capacity == 0)
		buff = (TYPE*) alloc_arr(sizeof(TYPE) * size);
	else if(_capacity < size)
		buff = (TYPE*) realloc_arr(_data, sizeof(TYPE) * size);


	if(buff)
	{
		_data = buff;
		return true;
	}

	LogError("Failed to reserve memory for Vector");
	return false;	
}




template<class TYPE>
bool Vector<TYPE>::resize(const size_t size)
{
	// TODO: err check
	if(this->reserve(size))
	{
		_size = size;
		return true;
	}

	return false;
}






template<class TYPE>
bool Vector<TYPE>::copy(const Vector& other)
{
	if(this != &other)
	{
		Vector<TYPE> tmp;
		
		if( tmp.initialize(other._data, other._size) )
		{
			this->swap(tmp);
			return true;
		}
		
		return false;
	}

	return true;
}






template<class TYPE>
void Vector<TYPE>::swap(Vector& other)
{
	if( this != &other )
	{
		const auto dataAux = this->_data;
		const auto sizeAux = this->_size;
		this->_data = other._data;
		this->_size = other._size;
		other._data = dataAux;
		other._size = sizeAux;
	}
}





template<class TYPE>
inline void Vector<TYPE>::free()
{
	if(_data)
	{
		free_arr(_data);
		_data = nullptr;
		_size = 0;
	}
}













}}


#endif