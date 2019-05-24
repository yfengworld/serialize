#ifndef _SERIALIZE_H_
#define _SERIALIZE_H_

#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#include <list>

class Serialize
{
public:
	Serialize()
	{
		sz = 1024;
		buffer = (char*)malloc(sz);
		wr_pos = 0;
		rd_pos = 0;
	}
	Serialize(char* buff, int len)
	{
		sz = len;
		buffer = (char*)malloc(len);
		memcpy(buffer, buff, len);
		wr_pos = len;
		rd_pos = 0;
	}

	~Serialize()
	{
		if (buffer)
		{
			free(buffer);
			buffer = nullptr;
		}

		sz = 0;
		wr_pos = 0;
		rd_pos = 0;
	}

	void reset()
	{
		wr_pos = 0;
		rd_pos = 0;
	}

	template<typename T>
	void write(T t, int len)
	{
		if (wr_pos + len > sz)
		{
			sz = sz * 2;
			buffer = (char*)realloc(buffer, sz);
		}

		memcpy(buffer + wr_pos, &t, len);
		wr_pos += len;
	}
	template<typename T>
	void write(T* t, int len)
	{
		if (wr_pos + len > sz)
		{
			sz = sz * 2;
			buffer = (char*)realloc(buffer, sz);
		}

		memcpy(buffer + wr_pos, t, len);
		wr_pos += len;
	}
	template<typename T>
	void read(T& t, int len)
	{
		memcpy(&t, buffer + rd_pos, len);
		rd_pos += len;
	}
	template<typename T>
	void read(T* t, int len)
	{
		memcpy(t, buffer + rd_pos, len);
		rd_pos += len;
	}
	template<typename T>
	Serialize& operator<<(T a)
	{
		write(a, sizeof(T));
		return *this;
	}
	template<>
	Serialize& operator<<(char* str)
	{
		int n = strlen(str);
		write(n, sizeof(n));
		write(str, n);
		return *this;
	}
	template<>
	Serialize& operator<<(std::string str)
	{
		int n = str.length();
		write(n, sizeof(n));
		write(str.c_str(), n);
		return *this;
	}
	/*Serialize& operator<<(char a)
	{
		write(a, sizeof(char));
		return *this;
	}
	Serialize& operator<<(short a)
	{
		write(a, sizeof(short));
		return *this;
	}
	Serialize& operator<<(int a)
	{
		write(a, sizeof(int));
		return *this;
	}
	Serialize& operator<<(float f)
	{
		write(f, sizeof(float));
		return *this;
	}
	Serialize& operator<<(double b)
	{
		write(b, sizeof(double));
		return *this;
	}
	Serialize& operator<<(long long b)
	{
		write(b, sizeof(long long));
		return *this;
	}
	Serialize& operator<<(const char* str)
	{
		int n = strlen(str);
		write(n, sizeof(n));
		write(str, n);
		return *this;
	}
	Serialize& operator<<(std::string str)
	{
		int n = str.length();
		write(n, sizeof(n));
		write(str.c_str(), n);
		return *this;
	}*/
	template<typename T>
	Serialize& operator<<(std::vector<T> v)
	{
		int n = v.size();
		write(n, sizeof(n));
		for (auto& d : v)
		{
			this->operator<<(d);
		}

		return *this;
	}
	template<typename T>
	Serialize& operator<<(std::list<T> l)
	{
		int n = l.size();
		write(n, sizeof(n));
		for (auto& d : l)
		{
			this->operator<<(d);
		}

		return *this;
	}
	template<typename K, typename V>
	Serialize& operator<<(std::map<K, V> m)
	{
		int n = m.size();
		write(n, sizeof(n));
		for (auto& d : m)
		{
			this->operator<<(d.first);
			this->operator<<(d.second);
		}

		return *this;
	}

	template<typename T>
	Serialize& operator>>(T& a)
	{
		read(a, sizeof(T));
		return *this;
	}
	template<>
	Serialize& operator>>(std::string& str)
	{
		int n = 0;
		read(n, sizeof(int));
		char* buff = (char*)malloc(n + 1);
		read(buff, n);
		buff[n] = '\0';
		str.assign(buff);
		free(buff);
		return *this;
	}
	template<typename T>
	Serialize& operator>>(T* buff)
	{
		int n = 0;
		read(n, sizeof(int));
		read(buff, n);
		return *this;
	}
	template<>
	Serialize& operator>>(char* buff)
	{
		int n = 0;
		read(n, sizeof(int));
		read(buff, n);
		return *this;
	}
	
	/*Serialize& operator>>(char& a)
	{
		read(a, sizeof(char));
		return *this;
	}
	Serialize& operator>>(short& a)
	{
		read(a, sizeof(short));
		return *this;
	}
	Serialize& operator>>(int& a)
	{
		read(a, sizeof(int));
		return *this;
	}
	Serialize& operator>>(float& f)
	{
		read(f, sizeof(float));
		return *this;
	}
	Serialize& operator>>(double& b)
	{
		read(b, sizeof(double));
		return *this;
	}
	Serialize& operator>>(long long& b)
	{
		read(b, sizeof(long long));
		return *this;
	}
	Serialize& operator>>(char* buff)
	{
		int n = 0;
		read(n, sizeof(int));
		read(buff, n);
		return *this;
	}
	Serialize& operator>>(std::string& str)
	{
		int n = 0;
		read(n, sizeof(int));
		char* buff = (char*)malloc(n + 1);
		read(buff, n);
		buff[n] = '\0';
		str.assign(buff);
		free(buff);
		return *this;
	}*/
	template<typename T>
	Serialize& operator>>(std::vector<T>& v)
	{
		int n = 0;
		read(n, sizeof(n));
		for (int i = 0; i < n; i++)
		{
			T t;
			this->operator>>(t);
			v.emplace_back(t);
		}

		return *this;
	}
	template<typename T>
	Serialize& operator>>(std::list<T>& l)
	{
		int n = 0;
		read(n, sizeof(n));
		for (int i = 0; i < n; i++)
		{
			T t;
			this->operator>>(t);
			l.emplace_back(t);
		}

		return *this;
	}
	template<typename K, typename V>
	Serialize& operator>>(std::map<K, V>& m)
	{
		int n = 0;
		read(n, sizeof(n));
		for (int i = 0; i < n; i++)
		{
			K k;
			this->operator>>(k);
			V v;
			this->operator>>(v);
			m.insert(std::make_pair(k, v));
		}

		return *this;
	}
private:
	char* buffer;
	int sz;
	int wr_pos;
	int rd_pos;
};

#endif 
