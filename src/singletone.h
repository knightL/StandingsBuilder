#ifndef SINGLETONE_H_
#define SINGLETONE_H_

template <typename T>
class SingleTone
{
	static T* instance;
public:
	SingleTone()
	{
		if(instance==0)
			instance=new T();
	}
	~SingleTone()
	{
	}
	T* operator->()
	{
		return instance;
	}

	const T* operator->() const
	{
		return instance;
	}
	T& operator*()
	{
		return *instance;
	}
	const T& operator*() const
	{
		return *instance;
	}
};

template <typename T> T* SingleTone<T>::instance=0;


#endif /* SINGLETONE_H_ */
