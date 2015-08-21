#ifndef _AtomicCounter_INCLUDED
#define _AtomicCounter_INCLUDED

template <typename T>
class AtomicCounter
	/// This class implements a simple counter, which
	/// provides atomic operations that are safe to
	/// use in a multithreaded environment.
	///
	/// Typical usage of AtomicCounter is for implementing
	/// reference counting and similar things.
	///
	/// On some platforms, the implementation of AtomicCounter
	/// is based on atomic primitives specific to the platform
	/// (such as InterlockedIncrement, etc. on Windows), and
	/// thus very efficient. On platforms that do not support
	/// atomic primitives, operations are guarded by a FastMutex.
	///
	/// The following platforms currently have atomic
	/// primitives:
	///   - GCC 4.1+ (Intel platforms only)
{
public:
	typedef T ValueType; /// The underlying integer type.

	AtomicCounter();
		/// Creates a new AtomicCounter and initializes it to zero.

	explicit AtomicCounter(ValueType initialValue);
		/// Creates a new AtomicCounter and initializes it with
		/// the given value.

	AtomicCounter(const AtomicCounter& counter);
		/// Creates the counter by copying another one.

	~AtomicCounter();
		/// Destroys the AtomicCounter.

	AtomicCounter& operator = (const AtomicCounter& counter);
		/// Assigns the value of another AtomicCounter.

	AtomicCounter& operator = (ValueType value);
		/// Assigns a value to the counter.

	operator ValueType () const;
		/// Returns the value of the counter.

	ValueType value() const
	{
		return _counter;
	}
		/// Returns the value of the counter.

	ValueType operator ++ () // prefix
	{
		return __sync_add_and_fetch(&_counter, 1);
	}
		/// Increments the counter and returns the result.

	ValueType operator ++ (int) // postfix
	{
		return __sync_fetch_and_add(&_counter, 1);
	}
		/// Increments the counter and returns the previous value.
		
	ValueType operator -- () // prefix
	{
		return __sync_sub_and_fetch(&_counter, 1);
	}
		/// Decrements the counter and returns the result.
		
	ValueType operator -- (int) // postfix
	{
		return __sync_fetch_and_sub(&_counter, 1);
	}
		/// Decrements the counter and returns the previous value.
		
	bool operator ! () const;
		/// Returns true if the counter is zero, false otherwise.

private:
//#elif defined(POCO_HAVE_GCC_ATOMICS)
	typedef T ImplType;

	ImplType _counter;
};


//
// inlines
//


//#if POCO_OS == POCO_OS_WINDOWS_NT

//#elif POCO_OS == POCO_OS_MAC_OS_X

//#elif defined(POCO_HAVE_GCC_ATOMICS)
//
// GCC 4.1+ atomic builtins.
//

template <typename T>
inline bool AtomicCounter<T>::operator ! () const
{
	return _counter == 0;
}

template <typename T>
AtomicCounter<T>::AtomicCounter():
	_counter(0)
{
}

template <typename T>
AtomicCounter<T>::AtomicCounter(AtomicCounter::ValueType initialValue):
	_counter(initialValue)
{
}

template <typename T>
AtomicCounter<T>::AtomicCounter(const AtomicCounter& counter):
	_counter(counter.value())
{
}

template <typename T>
AtomicCounter<T>::~AtomicCounter()
{
}

template <typename T>
AtomicCounter<T>& AtomicCounter<T>::operator = (const AtomicCounter<T>& counter)
{
	__sync_lock_test_and_set(&_counter, counter.value());
	return *this;
}

template <typename T>
AtomicCounter<T>& AtomicCounter<T>::operator = (AtomicCounter<T>::ValueType value)
{
	__sync_lock_test_and_set(&_counter, value);
	return *this;
}
//#else
//#endif // POCO_OS


#endif // _AtomicCounter_INCLUDED
