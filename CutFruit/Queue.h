#pragma once
#include "stdafx.h"
template<class T>

class QUEUE1
{
public:
	QUEUE1(int size = 5);
	QUEUE1(const QUEUE1<T> &OtherQueue);
	const QUEUE1<T> &operator =(const QUEUE1<T> &OtherQueue);
	~QUEUE1();

	void clear();
	bool IsEmpty() const;
	bool IsFull() const;
	bool push_queue(const T& value);
	bool pop_queue();
	bool pop_queue(T &data);
	// void print() const;
	int GetSize() const;
	int GetContainer() const;
	bool GetFront(T &data) const;
	bool GetRear(T &data) const;
	const T& GetAt(int i) const;

private:
	T *m_pT;
	int m_iFront;
	int m_iRear;
	int m_iSize;
	int m_iContainer;
};

template <class T>
QUEUE1<T>::QUEUE1(const QUEUE1<T> &OtherQueue) {
	m_iFront = OtherQueue.m_iFront; m_iRear = OtherQueue.m_iRear;
	m_iSize = OtherQueue.m_iSize; OtherQueue.m_iContainer = m_iContainer;
	m_pT = new T[m_iContainer];
}

template <class T>
QUEUE1<T>::QUEUE1(int size) {
	m_iFront = 0; m_iRear = 0; m_iSize = 0; m_iContainer = size;
	m_pT = new T[m_iContainer];
}
template <class T>
QUEUE1<T>::~QUEUE1() {
	if (m_pT != NULL)
		delete[] m_pT;
}

template <class T>
const QUEUE1<T> &QUEUE1<T>::operator =(const QUEUE1<T> &OtherQueue) {
	T *mNew = new T[OtherQueue.m_iContainer];
	m_iFront = OtherQueue.m_iFront; m_iRear = OtherQueue.m_iRear;
	m_iSize = OtherQueue.m_iSize; OtherQueue.m_iContainer = m_iContainer;

	for (int i = m_iFront, j = i + 1; j % m_iContainer != m_iRear; ++i, ++j)
		mNew[j % m_iContainer] = OtherQueue.m_pT[j % m_iContainer];
	//how to get index?  (oldindex + 1) % m_iContainer
	//range:0 to m_iContainer - 1
	if (m_pT != NULL) delete m_pT;
	m_pT = mNew;
}

template <class T>
void QUEUE1<T>::clear() {
	m_iSize = m_iFront = m_iRear = 0;
}

template <class T>
bool QUEUE1<T>::IsEmpty() const {
	return (m_iSize == 0);
}

template <class T>
bool QUEUE1<T>::IsFull() const {
	return (m_iSize == m_iContainer - 1);
}

template <class T>
bool QUEUE1<T>::push_queue(const T& value) {
	if (IsFull()) pop_queue();
	m_iRear = (m_iRear + 1) % m_iContainer;
	m_pT[m_iRear] = value; m_iSize++;
	return TRUE;
}

template <class T>
bool QUEUE1<T>::pop_queue() {
	if (IsEmpty()) return FALSE;
	m_iFront = (m_iFront + 1) % m_iContainer;
	m_iSize--;
	return TRUE;
}

template <class T>
bool QUEUE1<T>::pop_queue(T &data) {
	if (IsEmpty()) return FALSE;
	m_iFront = (m_iFront + 1) % m_iContainer;
	data = m_pT[m_iFront]; m_iSize--;
	return TRUE;
}

template <class T>
int QUEUE1<T>::GetSize() const {
	return m_iSize;
}

template  <class T>
int QUEUE1<T>::GetContainer() const {
	return m_iContainer;
}

template <class T>
bool QUEUE1<T>::GetFront(T &data) const {
	if (IsEmpty()) return FALSE;
	data = m_pT[(m_iFront + 1) % m_iContainer];
	return TRUE;
}

template <class T>
bool QUEUE1<T>::GetRear(T &data) const {
	if (IsEmpty()) return FALSE;
	data = m_pT[m_iRear];
	return TRUE;
}

template <class T>
const T& QUEUE1<T>::GetAt(int i) const
{
	return m_pT[(m_iFront + i + 1) % m_iContainer];
}