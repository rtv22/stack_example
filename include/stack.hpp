#include <mutex>
#include <memory>
#include <iostream>

template <typename T>
class stack
{
public:
	stack();/*noexept*/
	stack(const stack<T> &);/*strong*/
	size_t count() const; /*noexcept*/
	void print()const;/*noexcept*/
	void push(T const &); /*strong*/
	void swap(stack<T>&); /*noexcept*/
	std::shared_ptr<T> pop(); /*strong*/
	/*T top(); strong*/
	stack<T>& operator=(stack<T> &); /*noexcept*/
	~stack();/*noexcept*/
private:
	T * array_;
	mutable std::mutex mutex_;
	size_t array_size_;
	size_t count_;
};

template <typename T> 
stack<T>::stack() : count_(0), array_size_(0), array_{ nullptr }
{}

template<class T>
stack<T>::~stack()
{
	count_ = 0;
	array_size_ = 0;
	delete[] array_;
}

template <typename T> 
stack<T>::stack(const stack<T>& copy)
{
	std::lock_guard<std::mutex> lock(copy.mutex_);
	T *tmp = new T[copy.array_size_];
	count_ = copy.count_;
	array_size_ = copy.array_size_;
	array_ = tmp
	try{
		std::copy(copy.array_, copy.array_ + count_, array_);
	}
	catch(...){
		delete[] array_;
	}
}
	

template<class T>
size_t stack<T>::count() const
{
	std::lock_guard<std::mutex> lock(mutex_);
	return count_;
}

template<typename T> 
void stack<T>::swap(stack& x) 
{
	std::lock(mutex_, x.mutex_);
	std::swap(x.array_size_, array_size_);
	std::swap(count_, x.count_);
	std::swap(x.array_, array_);
	x.mutex_.lock();
	x.mutex_.unlock();
}

/*template <typename T>
T stack<T>::top()
{
	if (empty())
	{
		throw "Stack is empty!";
	}
	return array_[count_ - 1];
} */

template <typename T>
void stack<T>::push(T const &value)
{
	std::lock_guard<std::mutex> lock(mutex_);
	if (array_size_ == count_)
	{
		array_size_ *= 2;
		T* tmp = new T[array_size_];
		try
		{
			std::copy(array_, array_ + count_, tmp);
		}
		catch(...)
		{
			   delete[];
			   throw;
		}
		delete[] array_;
		array_ = tmp;
	}
	array_[count_] = value;
	++count_;
}

template <typename T>
auto stack<T>::pop() -> std::shared_ptr<T>
{
	std::lock_guard<std::mutex> lock(mutex_);
	if (count_ == 0)
	{
		throw "Stack empty";
	}
	auto top = std::make_shared<T>(array_[count_ - 1]);;
	--count_;
	return top;
}

template <typename T>
void stack<T>::print() const
{
	std::lock_guard<std::mutex> lock(mutex_);
	for (int i = 0; i < array_size_; i++)
		std::cout << array_[i];
}

template<typename T>
stack<T>& stack<T>::operator=(stack<T> & other)
{
	std::lock_guard<std::mutex> lock(other.mutex_);
	if (this != &other)
	{
		stack<T> tmp(other);
		tmp.swap(*this);
	}
	return *this;
}
