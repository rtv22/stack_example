#include <mutex>
#include <thread>
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
	bool empty() const; /*noexcept*/
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
	T *tmp = new T[copy.array_size_];
	count_ = copy.count_;
	array_size_ = copy.array_size_;
	array_ = tmp;
	std::copy(copy.array_, copy.array_ + count_, array_);
}
	

template<class T>
size_t stack<T>::count() const
{
	std::lock_guard<std::mutex> lock(mutex_);
	return count_;
}

template <typename T>
bool stack<T>::empty() const 
{
	return (count_ == 0);
}

template<typename T> 
void stack<T>::swap(stack& x) 
{
	x.mutex_.lock();
	std::swap(x.array_size_, array_size_);
	std::swap(count_, x.count_);
	std::swap(x.array_, array_);
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
	mutex_.lock();
	if (array_size_ == count_) 
	{
		array_size_ *= 2;
		stack<T> temp(*this);
		swap(temp);
	}
	array_[count_] = value;
	++count_;
	mutex_.unlock();
}

template <typename T>
auto stack<T>::pop() -> std::shared_ptr<T>
{
	std::lock_guard<std::mutex> lock(mutex_);
	if (!empty())
		--count_;
	auto top = std::make_shared<T>(*array_);
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
