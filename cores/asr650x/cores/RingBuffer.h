template <class T, size_t S>
class RingBuffer {
public:
	explicit RingBuffer()
	: _buffer()
    , _head(0)
    , _tail(0)
    , _full(false)
	{}

	// write value to buffer.
	inline void write(T item)
	{
		_buffer[_head] = item;

		if(_full)
		{
			_tail = (_tail + 1) % S;
		}

		_head = (_head + 1) % S;

		_full = _head == _tail;
	}

	// read value from buffer with pushing it forward.
	inline T read()
	{
		if(empty())
		{
			return T();
		}

		//Read data and advance the tail (we now have a free space)
		T val = _buffer[_tail];
		_full = false;
		_tail = (_tail + 1) %  S;

		return val;
	}

	// value from buffer without pushing it forward.
  	inline T peek() const
	{
		if(empty())
		{
			return T();
		}

		return _buffer[_tail];
	}

	// reset buffer pointers so the buffer is "empty".
	inline void reset()
	{
		_head = _tail;
		_full = false;
	}

	// buffer is empty.
	inline bool empty() const
	{
		//if head and tail are equal, we are empty
		return (!_full && (_head == _tail));
	}

	// buffer full, next write will overwrite data and push the tail. 
	inline bool full() const
	{
		//If tail is ahead the head by 1, we are full
		return _full;
	}

	// max number of elements that the buffer can hold.
	inline size_t capacity() const
	{
		return S; 
	}

	// the number of elements currently in the buffer.
	inline size_t size() const
	{
		size_t size(S);

		if(!_full)
		{
			if(_head >= _tail)
			{
				size = _head - _tail;
			}
			else
			{
				size = S + _head - _tail;
			}
		}

		return size;
	}

	// basically this is a indexed peek with the tail as index 0.
	inline const T& operator[](size_t index) const
	{ 
		return _buffer[(_tail + index) % S];
	} 

private:
	T _buffer[S];
	size_t _head;
	size_t _tail;
	bool _full;
};