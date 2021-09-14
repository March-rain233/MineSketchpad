#pragma once
#include<iostream>
template<typename T>
class Compare {
public:
    int operator () (const T& x, const T& y)const 
    { return x < y ? -1 : x>y ? 1 : 0; }
};

template<typename T, typename compare = Compare<T>>
class Mat {
public:
    Mat(); //无参数的构造函数，创建行列都为零的Mat对象
    Mat(int h, int w); //构造函数重载，创建h行，w列的Mat对象
    Mat(int h, int w, T val); //构造函数重载，矩阵元素的值都为val;
    Mat(const T* m, int h, int w); //构造函数重载，从动态数组创建Mat对象;
    Mat(const T** m, int h, int w); //构造函数重载，从动态数组创建Mat对象;
    Mat(const Mat& m); //拷贝构造函数;
    virtual ~Mat(); //析构函数;

    int Height();//得到矩阵高度
    int Height() const;//得到矩阵高度
    int Width();//得到矩阵宽度
    int Width() const;//得到矩阵宽度

    T Min(); //得到矩阵元素的最小值
    T Min() const; //得到矩阵元素的最小值
    T Max(); //得到矩阵元素的最大值
    T Max() const; //得到矩阵元素的最大值

    T& At(int col, int row);          //获取某点的值
    T& At(int n); //获取第n位的值
    const T& At(int col, int row) const;  //获取某点的值，const重载
    const T& At(int n) const; //获取第n位
    
    void Set(int col, int row, T value); //设置元素(row,col)为某值;
    void Set(int i, T val);
    void Set(T value); //设置所有元素为同一值;

    void Flip(int code); //翻转; 根据code的值：0:左右翻转，1:上下翻转;
    void Resize(int h, int w); //缩放
    void Crop(int x1, int y1, int x2, int y2);//裁剪点(x1,y1)到点(x2,y2)
    void Rotate(int degree);//旋转，90度的整数倍
    void Transpose(); // 转置
    void Reshape(int h, int w); //在元素总数不变的情况下，将矩阵的行列变为参数给定的大小
    bool IsEmpty() const;// 判断是否为空矩阵
    bool IsSquare() const;// 判断矩阵是否为方阵
    Mat<T>  MajorDiagonal();// 求主对角线上的元素，输出一个N行1列的矩阵，N为主对角线上元素的个数
    Mat<T>  MinorDiagonal();// 求副对角线上的元素，输出一个N行1列的矩阵，N为副对角线上元素的个数
    Mat<T> Row(int n);// 返回矩阵的第n行上的元素，组出一个1行N列的矩阵输出，N为第n行上元素的个数
    Mat<T> Column(int n);// 返回矩阵的第n列上的元素，组出一个N行1列的矩阵输出，N为第n列上元素的个数
    void Cat(const Mat<T>& m, int code); // 将m与当前对象进行拼接，code代表拼接的方式
    void CopyTo(Mat<T>& m); // 将矩阵复制给m，完成深拷贝
    Mat<T> Clone(); // 从当前对象拷贝创建一个新的矩阵，完成深拷贝

    Mat<double> Normalize();//将矩阵元素的值变换到0-1范围内，以double类型的Mat对象输出。注意：在这个函数里，无法访问Mat<double>类型的对象的私有成员data，需要调用其At函数获得某个元素。

    Mat<T>& operator=(const Mat<T>& m);  //重载赋值运算符，完成对象间的拷贝；
    bool operator==(const Mat<T>& m) const;  //判断两个Mat对象是否相等
    friend Mat<T> operator+(const Mat<T>& lhs, const Mat<T>& rhs) {  //对应元素的数值相加；
        int total = lhs._height * lhs._width;
        Mat<T> res;
        res._height = lhs._height;
        res._width = lhs._weight;
        res._data = new T[total];
        for (int i = 0; i < total; ++i) {
            res._data[i] = lhs._data[i] + rhs._data[i];
        }
        return res;
    }
    friend Mat<T> operator-(const Mat<T>& lhs, const Mat<T>& rhs) {  //对应元素的数值相减；
        int total = lhs._height * lhs._width;
        Mat<T> res;
        res._height = lhs._height;
        res._width = lhs._weight;
        res._data = new T[total];
        for (int i = 0; i < total; ++i) {
            res._data[i] = lhs._data[i] - rhs._data[i];
        }
        return res;
    }
    Mat<T>& operator++();  //前置自加；
    Mat<T>& operator--();  //前置自减；
    Mat<T> operator ++(int);  //后置自加；
    Mat<T> operator --(int);  //后置自减；

    friend Mat<T> operator+(Mat<T>& m, T num) { //所有元素加上同一数值;
        int total = m._height * m._width;
        Mat<T> res;
        res._height = m._height;
        res._width = m._weight;
        res._data = new T[total];
        for (int i = 0; i < total; ++i) {
            res._data[i] = m._data[i] + num;
        }
        return res;
    }
    friend Mat<T> operator-(Mat<T>& m, T num) { //所有元素减去同一数值;
        int total = m._height * m._width;
        Mat<T> res;
        res._height = m._height;
        res._width = m._weight;
        res._data = new T[total];
        for (int i = 0; i < total; ++i) {
            res._data[i] = m._data[i] - num;
        }
        return res;
    }
    friend Mat<T> operator*(Mat<T>& m, T num) { //所有元素乘上同一数值;
        int total = m._height * m._width;
        Mat<T> res;
        res._height = m._height;
        res._width = m._weight;
        res._data = new T[total];
        for (int i = 0; i < total; ++i) {
            res._data[i] = m._data[i] * num;
        }
        return res;
    }
    friend Mat<T> operator/(Mat<T>& m, T num) { //所有元素除以同一数值;
        int total = m._height * m._width;
        Mat<T> res;
        res._height = m._height;
        res._width = m._weight;
        res._data = new T[total];
        for (int i = 0; i < total; ++i) {
            res._data[i] = m._data[i] / num;
        }
        return res;
    }
    //另外，用友元函数再写出一个T类型的数和一个Mat对象的加,减,乘,除

    friend Mat<T> operator*(const Mat<T>& m1, const Mat<T>& m2) {
        if (m1.Width() != m2.Height()) {
            throw std::exception("无法相乘");
        }
        Mat<T> res(m1.Height(), m2.Width());
        for (int i = 0; i < m1.Height(); ++i) {
            for (int j = 0; j < m2.Width(); ++j) {
                T temp = 0;
                for (int k = 0; k < m1.Width(); ++k) {
                    temp += m1.At(k, i) * m2.At(j, k);
                }
                res.Set(j, i, temp);
            }
        }
        return res;
    }

    Mat<bool> gray2bw(T t); //以给定阈值t进行二值化，返回结果对象
    T* GetBits();
    const T* GetBits() const;//获取存储的数组

    friend void Swap(Mat<T>& a, Mat<T>& b) {//使用友元函数交换两个Mat对象
        std::swap(a._height, b._height);
        std::swap(a._width, b._width);
        std::swap(a._data, b._data);
    }

    T* operator[](int i);
    const T* operator[](int i) const;
private:
    T* _data;//数据
    int _height;//高度
    int _width;//宽度
};

template<typename T, typename compare>
inline Mat<T, compare>::Mat() {
    _data = nullptr;
    _height = _width = 0;
}

template<typename T, typename compare>
inline Mat<T, compare>::Mat(int h, int w) {
    _height = h;
    _width = w;
    _data = new T[_height * _width];
}

template<typename T, typename compare>
inline Mat<T, compare>::Mat(int h, int w, T val) {
    _height = h;
    _width = w;
    int total = _height * _width;
    _data = new T [total];
    for (int i = 0; i < total; ++i) {
        _data[i] = val;
    }
}


template<typename T, typename compare>
inline Mat<T, compare>::Mat(const T* m, int h, int w) {
    _height = h;
    _width = w;
    int total = _height * _width;
    _data = new T[total];
    for (int i = 0; i < total; ++i) {
        _data[i] = m[i];
    }
}

template<typename T, typename compare>
inline Mat<T, compare>::Mat(const T** m, int h, int w) {
    _height = h;
    _width = w;
    int total = _height * _width;
    _data = new T [total];
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            _data[i * w + j] = m[i][j];
        }
    }
}

template<typename T, typename compare>
inline Mat<T, compare>::Mat(const Mat& m) {
    _height = m._height;
    _width = m._width;
    int total = _height * _width;
    _data = new T [total];
    for (int i = 0; i < total; ++i) {
        _data[i] = m._data[i];
    }
}

template<typename T, typename compare>
inline Mat<T, compare>::~Mat() {
    if (_data != nullptr) {
        delete[] _data;
    }
    _width = _height = 0;
}

template<typename T, typename compare>
inline int Mat<T, compare>::Height() {
    return static_cast<const Mat<T>&>(*this).Height();
}

template<typename T, typename compare>
inline int Mat<T, compare>::Height() const {
    return _height;
}

template<typename T, typename compare>
inline int Mat<T, compare>::Width() {
    return static_cast<const Mat<T>&>(*this).Width();
}

template<typename T, typename compare>
inline int Mat<T, compare>::Width() const {
    return _width;
}

template<typename T, typename compare>
inline T Mat<T, compare>::Min() {
    return static_cast<const Mat<T>&>(*this).Min();
}

template<typename T, typename compare>
inline T Mat<T, compare>::Min() const {
    T* res = &_data[0];
    int total = _width * _height;
    compare c;
    for (int i = 0; i < total; ++i) {
        if (c(res, _data[i]) == 1) {
            res = &data[i];
        }
    }
    return *res;
}

template<typename T, typename compare>
inline T Mat<T, compare>::Max() {
    return static_cast<const Mat<T>&>(*this).Max();
}

template<typename T, typename compare>
inline T Mat<T, compare>::Max() const {
    T* res = &_data[0];
    int total = _width * _height;
    compare c;
    for (int i = 0; i < total; ++i) {
        if (c(res, _data[i]) == -1) {
            res = &data[i];
        }
    }
    return *res;
}

template<typename T, typename compare>
inline T& Mat<T, compare>::At(int col, int row) {
    return _data[row * _width + col];
}

template<typename T, typename compare>
inline T& Mat<T, compare>::At(int n) {
    return _data[n];
}

template<typename T, typename compare>
inline const T& Mat<T, compare>::At(int col, int row) const {
    return _data[row * _width + col];
}

template<typename T, typename compare>
inline const T& Mat<T, compare>::At(int n) const {
    return _data[n];
}

template<typename T, typename compare>
inline void Mat<T, compare>::Set(int col, int row, T value) {
    _data[row * _width + col] = value;
}

template<typename T, typename compare>
inline void Mat<T, compare>::Set(int i, T val) {
    _data[i] = val;
}

template<typename T, typename compare>
inline void Mat<T, compare>::Set(T value) {
    int total = _width * _height;
    for (int i = 0; i < total; ++i) {
        _data[i] = value;
    }
}

template<typename T, typename compare>
inline void Mat<T, compare>::Flip(int code) {
    if (code) {
        for (int i = 0; i < _height / 2; ++i) {
            for (int j = 0; j < _width; ++j) {
                std::swap(_data[i * _width + j], _data[(_height - i - 1) * _width + j]);
            }
        }
    }
    else {
        for (int i = 0; i < _height; ++i) {
            for (int j = 0; j < _width / 2; ++j) {
                std::swap(_data[i * _width + j], _data[i * _width + _width - j - 1]);
            }
        }
    }
}

template<typename T, typename compare>
inline void Mat<T, compare>::Resize(int h, int w) {
    if (h == 0 || w == 0) {
        return;
    }
    bool wBig = w > _width;
    double hi = _height / h;
    double wi = _width / w;
    double p, q;
    p = q = 0;
    auto temp = new T[h * w];

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            temp[i * w + j] = _data[(int)p * _width + (int)q];
            q += wi;
        }
        q = 0;
        p += hi;
    }

    delete[] _data;

    _data = temp;
    _width = w;
    _height = h;
}

template<typename T, typename compare>
inline void Mat<T, compare>::Crop(int x1, int y1, int x2, int y2) {
    if (x1 > x2) std::swap(x1, x2);
    if (y1 > y2) std::swap(y1, y2);
    if (x1 < 0 || y1 < 0 || x2 >= _width || y2 >= _height) {
        throw new std::out_of_range("超出图片边界");
    }
    int h = fabs(y2 - y1);
    int w = fabs(x2 - x1);
    T* m;
    m = new T[h * w];
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            m[i * w + j] = _data[(x1 + i) * _width + y1 + j];
        }
    }
    delete[] _data;
    _width = w;
    _height = h;
    _data = m;
}

template<typename T, typename compare>
inline void Mat<T, compare>::Rotate(int degree) {
    for (int ctr = 0; ctr < degree / 90; ++ctr) {
        Transpose();
        for (int i = 0; i < _height; ++i) {
            std::reverse(&_data[i * _width], &_data[i * _width + _width - 1]);
        }
    }
}

template<typename T, typename compare>
inline void Mat<T, compare>::Transpose() {
    int total = _height * _width;
    for (int i = 0; i < total; ++i) {
        //先判断是否为处理过的环,当索引的后置有小于索引的值时可判断改环已被处理
        int next = i;
        do {
            next = (next % _width) * _height + next / _width;//获取轮换后序
        } while (next > i);

        //对未处理过的环进行轮换
        if (next == i) {
            int cur = i;
            int pre = (cur % _height) * _width + cur / _height;
            if (pre == cur) {
                continue;
            }

            do {
                if (pre == cur) {
                    break;
                }
                //交换元素
                std::swap(_data[cur], _data[pre]);
                cur = pre;
                pre = (cur % _height) * _width + cur / _height;//获取轮换前序
            } while (pre != i);
        }
    }
    std::swap(_width, _height);
}

template<typename T, typename compare>
inline void Mat<T, compare>::Reshape(int h, int w) {
    _height = h;
    _width = w;
}

template<typename T, typename compare>
inline bool Mat<T, compare>::IsEmpty() const {
    return _data == nullptr;
}

template<typename T, typename compare>
inline bool Mat<T, compare>::IsSquare() const {
    return _height == _width;
}

template<typename T, typename compare>
inline Mat<T> Mat<T, compare>::MajorDiagonal() {
    Mat<T, compare> res;
    res._height = _height;
    res._width = 1;
    for (int i = 0; i < _height; ++i) {
        res._data[i] = _data[i * _width + i];
    }
    return res;
}

template<typename T, typename compare>
inline Mat<T> Mat<T, compare>::MinorDiagonal() {
    Mat<T, compare> res;
    res._height = _height;
    res._width = 1;
    for (int i = 0; i < _height; ++i) {
        res._data[i] = _data[i * _width + _width - i];
    }
    return res;
}

template<typename T, typename compare>
inline Mat<T> Mat<T, compare>::Row(int n) {
    Mat<T, compare> res;
    res._height = 1;
    res._width = _width;
    for (int i = 0; i < _width; ++i) {
        res._data[i] = _data[n * _width + i];
    }
    return res;
}

template<typename T, typename compare>
inline Mat<T> Mat<T, compare>::Column(int n) {
    Mat<T, compare> res;
    res._height = _height;
    res._width = 1;
    for (int i = 0; i < _height; ++i) {
        res._data[i] = _data[i * _width + n];
    }
    return res;
}

template<typename T, typename compare>
inline void Mat<T, compare>::Cat(const Mat<T>& m, int code) {
    if (m.IsEmpty()) {
        return;
    }

    if (code == 1) {//接到右边
        int newWidth = _width + m._width;
        int newHeight = _height;
        auto temp = new T [newHeight * newWidth];

        for (int i = 0; i < newHeight; ++i) {
            for (int j = 0; j < newWidth; ++j) {
                if (j < _width) {
                    temp[i * newWidth + j] = _data[i * _width + j];
                }
                else {
                    temp[i * newWidth + j] = m._data[i * m._width + j - _width];
                }
            }
        }

        delete[] _data;
        _data = temp;
        _width = newWidth;
        _height = newHeight;
    }
    else if (code == 2) {//接到下边
        int newWidth = _width;
        int newHeight = _height + m._height;
            auto temp = new T[newHeight * newWidth];

        for (int i = 0; i < newHeight; ++i) {
            for (int j = 0; j < newWidth; ++j) {
                if (i < _height) {
                    temp[i * newWidth + j] = _data[i * _width + j];
                }
                else {
                    temp[i * newWidth + j] = m._data[i * m._width + j - _width];
                }
            }
        }

        delete[] _data;
        _data = temp;
        _width = newWidth;
        _height = newHeight;
    }
}

template<typename T, typename compare>
inline void Mat<T, compare>::CopyTo(Mat<T>& m) {
    if (!m.IsEmpty()) {
        delete[] m._data;
    }
    m._width = _width;
    m._height = _height;
    int total = _height * _width;
    m._data = new T[total];
    for (int i = 0; i < total; ++i) {
        m._data[i] = _data[i];
    }
}

template<typename T, typename compare>
inline Mat<T> Mat<T, compare>::Clone() {
    Mat<T, compare> m;
    m._width = _width;
    m._height = _height;
    int total = _height * _width;
    m._data = new T[total];
    for (int i = 0; i < total; ++i) {
        m._data[i] = _data[i];
    }
    return m;
}

template<typename T, typename compare>
inline Mat<double> Mat<T, compare>::Normalize() {
    Mat<double> res;
    res._width = _width;
    res._height = _height;
    int total = _width * _height;
    res._data = new double[total];
    double max = 0;
    double min = 0;

	for (int i = 0; i < total; ++i) {
		max = (double)_data[i] > max ? (double)_data[i] : max;
		min = (double)_data[i] > min ? min : (double)_data[i];
	}

	for (int i = 0; i < total; ++i) {
		res._data[i] = ((double)_data[i] - min) / (max - min);
	}
	return res;
}

template<typename T, typename compare>
inline Mat<T>& Mat<T, compare>::operator=(const Mat<T>& m) {
    if (!IsEmpty()) {
        delete[] _data;
    }
    _width = m._width;
    _height = m._height;
    int total = _width * _height;
    _data = new T[total];
    for (int i = 0; i < total; ++i) {
        _data[i] = m._data[i];
    }
}

template<typename T, typename compare>
inline bool Mat<T, compare>::operator==(const Mat<T>& m) const {
    if (m._height != _height || m._width!+ _width) {
        return false;
    }
    int total = _width * _height;
    for (int i = 0; i < total; ++i) {
        if (m._data[i] != _data[i]) {
            return false;
        }
    }
    return true;
}

template<typename T, typename compare>
inline Mat<T>& Mat<T, compare>::operator++() {
    int total = _height * _width;
    for (int i = 0; i < total; ++i) {
        _data[i] += 1;
    }
}

template<typename T, typename compare>
inline Mat<T>& Mat<T, compare>::operator--() {
    int total = _height * _width;
    for (int i = 0; i < total; ++i) {
        _data[i] -= 1;
    }
}

template<typename T, typename compare>
inline Mat<T> Mat<T, compare>::operator++(int) {
    Mat<T> res = this->Clone();
    int total = _height * _width;
    for (int i = 0; i < total; ++i) {
        _data[i] += 1;
    }
    return res;
}

template<typename T, typename compare>
inline Mat<T> Mat<T, compare>::operator--(int) {
    Mat<T> res = this->Clone();
    int total = _height * _width;
    for (int i = 0; i < total; ++i) {
        _data[i] -= 1;
    }
    return res;
}

template<typename T, typename compare>
inline Mat<bool> Mat<T, compare>::gray2bw(T t) {
    Mat<bool> m;
    m._height = _height;
    m._width = _width;
    int t = _height * _width;
    m._data = new bool[t];
    compare c;
    for (int i = 0; i < t; ++i) {
        m._data = c(_data[i], t) >= 0;
    }
    return m;
}

template<typename T, typename compare>
inline T* Mat<T, compare>::GetBits() {
    return _data;
}

template<typename T, typename compare>
inline const T* Mat<T, compare>::GetBits() const {
    return _data;
}

template<typename T, typename compare>
inline T* Mat<T, compare>::operator[](int i) {
    return &_data[i * _width];
}

template<typename T, typename compare>
inline const T* Mat<T, compare>::operator[](int i) const {
    return &_data[i * _width];
}
