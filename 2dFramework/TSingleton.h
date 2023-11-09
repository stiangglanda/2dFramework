#pragma once

template <class T>
class TSingleton
{
protected:
	static T* m_pSingleton;

public:
	virtual ~TSingleton()
	{
	}

	static T* Get()
	{
		if (!m_pSingleton)
		{
			m_pSingleton = new T;
		}
		return (m_pSingleton);
	}

	static void Del()
	{
		if (m_pSingleton)
		{
			delete (m_pSingleton);
			m_pSingleton = nullptr;
		}
	}
};

template <class T>
T* TSingleton<T>::m_pSingleton = nullptr;
