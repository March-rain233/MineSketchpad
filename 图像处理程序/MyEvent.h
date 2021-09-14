#pragma once
#include<qvector.h>
#include<functional>
template<typename User, typename... Arg>
class MyEvent {//�¼�
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
	void BlockSign(bool v) {
		_block = v;
	}
private:
	void Invoke(Arg... args) {
		if (_block) {
			return;
		}
		if (_list.size() <= 0) {
			return;
		}
		for (auto f : _list) {
			if (f) {
				f(args...);
			}
		}
	}
	void operator()(Arg... args) {
		Invoke(args...);
	}
private:
	QVector<std::function<void(Arg...)>> _list;
	bool _block = false;
};
