#pragma once
#include<qvector.h>
#include<functional>
template<typename User, typename... Arg>
class MyEvent {//ÊÂ¼þ
public:
	friend User;
	void Add(std::function<void(Arg...)> p) {
		_list.push_back(p);
	}
	void Remove(std::function<void(Arg...)> p) {
		int i = 0;
		for (; i < _list.size(); ++i) {
			if (_list[i] == p) {
				break;
			}
		}
		_list.remove(i);
	}
	void operator +=(std::function<void(Arg...)> p) {
		Add(p);
	}
	void operator -=(std::function<void(Arg...)> p) {
		Remove(p);
	}
private:
	void Invoke(Arg... args) {
		if (_list.size() <= 0) {
			return;
		}
		for (auto f : _list) {
			f(args...);
		}
	}
private:
	QVector<std::function<void(Arg...)>> _list;
};
