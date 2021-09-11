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
    Mat(); //�޲����Ĺ��캯�����������ж�Ϊ���Mat����
    Mat(int h, int w); //���캯�����أ�����h�У�w�е�Mat����
    Mat(int h, int w, T val); //���캯�����أ�����Ԫ�ص�ֵ��Ϊval;
    Mat(const T* m, int h, int w); //���캯�����أ��Ӷ�̬���鴴��Mat����;
    Mat(const T** m, int h, int w); //���캯�����أ��Ӷ�̬���鴴��Mat����;
    Mat(const Mat& m); //�������캯��;
    virtual ~Mat(); //��������;

    int Height();//�õ�����߶�
    int Height() const;//�õ�����߶�
    int Width();//�õ�������
    int Width() const;//�õ�������

    T Min(); //�õ�����Ԫ�ص���Сֵ
    T Min() const; //�õ�����Ԫ�ص���Сֵ
    T Max(); //�õ�����Ԫ�ص����ֵ
    T Max() const; //�õ�����Ԫ�ص����ֵ

    T& At(int row, int col);          //��ȡĳ���ֵ
    T& At(int n); //��ȡ��nλ��ֵ
    const T& At(int row, int col) const;  //��ȡĳ���ֵ��const����
    const T& At(int n) const; //��ȡ��nλ
    
    void Set(int row, int col, T value); //����Ԫ��(row,col)Ϊĳֵ;
    void Set(int i, T val);
    void Set(T value); //��������Ԫ��Ϊͬһֵ;

    void Flip(int code); //��ת; ����code��ֵ��0:���ҷ�ת��1:���·�ת;
    void Resize(int h, int w); //����
    void Crop(int x1, int y1, int x2, int y2);//�ü���(x1,y1)����(x2,y2)
    void Rotate(int degree);//��ת��90�ȵ�������
    void Transpose(); // ת��
    void Reshape(int h, int w); //��Ԫ���������������£�����������б�Ϊ���������Ĵ�С
    bool IsEmpty() const;// �ж��Ƿ�Ϊ�վ���
    bool IsSquare() const;// �жϾ����Ƿ�Ϊ����
    Mat<T>  MajorDiagonal();// �����Խ����ϵ�Ԫ�أ����һ��N��1�еľ���NΪ���Խ�����Ԫ�صĸ���
    Mat<T>  MinorDiagonal();// �󸱶Խ����ϵ�Ԫ�أ����һ��N��1�еľ���NΪ���Խ�����Ԫ�صĸ���
    Mat<T> Row(int n);// ���ؾ���ĵ�n���ϵ�Ԫ�أ����һ��1��N�еľ��������NΪ��n����Ԫ�صĸ���
    Mat<T> Column(int n);// ���ؾ���ĵ�n���ϵ�Ԫ�أ����һ��N��1�еľ��������NΪ��n����Ԫ�صĸ���
    void Cat(const Mat<T>& m, int code); // ��m�뵱ǰ�������ƴ�ӣ�code����ƴ�ӵķ�ʽ
    void CopyTo(Mat<T>& m); // �������Ƹ�m��������
    Mat<T> Clone(); // �ӵ�ǰ���󿽱�����һ���µľ���������

    Mat<double> Normalize();//������Ԫ�ص�ֵ�任��0-1��Χ�ڣ���double���͵�Mat���������ע�⣺�����������޷�����Mat<double>���͵Ķ����˽�г�Աdata����Ҫ������At�������ĳ��Ԫ�ء�

    Mat<T>& operator=(const Mat<T>& m);  //���ظ�ֵ���������ɶ����Ŀ�����
    bool operator==(const Mat<T>& m) const;  //�ж�����Mat�����Ƿ����
    friend Mat<T> operator+(const Mat<T>& lhs, const Mat<T>& rhs) {  //��ӦԪ�ص���ֵ��ӣ�
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
    friend Mat<T> operator-(const Mat<T>& lhs, const Mat<T>& rhs) {  //��ӦԪ�ص���ֵ�����
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
    Mat<T>& operator++();  //ǰ���Լӣ�
    Mat<T>& operator--();  //ǰ���Լ���
    Mat<T> operator ++(int);  //�����Լӣ�
    Mat<T> operator --(int);  //�����Լ���

    friend Mat<T> operator+(Mat<T>& m, T num) { //����Ԫ�ؼ���ͬһ��ֵ;
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
    friend Mat<T> operator-(Mat<T>& m, T num) { //����Ԫ�ؼ�ȥͬһ��ֵ;
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
    friend Mat<T> operator*(Mat<T>& m, T num) { //����Ԫ�س���ͬһ��ֵ;
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
    friend Mat<T> operator/(Mat<T>& m, T num) { //����Ԫ�س���ͬһ��ֵ;
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
    //���⣬����Ԫ������д��һ��T���͵�����һ��Mat����ļ�,��,��,��

    friend Mat<T> operator*(const Mat<T>& m1, const Mat<T>& m2) {
        if (m1.Width() != m2.Height()) {
            throw std::exception("�޷����");
        }
        Mat<T> res(m1.Height(), m2.Width());
        for (int i = 0; i < m1.Height(); ++i) {
            for (int j = 0; j < m2.Width(); ++j) {
                T temp = 0;
                for (int k = 0; k < m1.Width(); ++k) {
                    temp += m1.At(i, k) * m2.At(k, j);
                }
                res.Set(i, j, temp);
            }
        }
        return res;
    }

    Mat<bool> gray2bw(T t); //�Ը�����ֵt���ж�ֵ�������ؽ������
    T* GetBits();
    const T* GetBits() const;//��ȡ�洢������

    friend void Swap(Mat<T>& a, Mat<T>& b) {//ʹ����Ԫ������������Mat����
        std::swap(a._height, b._height);
        std::swap(a._width, b._width);
        std::swap(a._data, b._data);
    }

    T* operator[](int i);
    const T* operator[](int i) const;
private:
    T* _data;//����
    int _height;//�߶�
    int _width;//���
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
inline T& Mat<T, compare>::At(int row, int col) {
    return _data[row * _width + col];
}

template<typename T, typename compare>
inline T& Mat<T, compare>::At(int n) {
    return _data[n];
}

template<typename T, typename compare>
inline const T& Mat<T, compare>::At(int row, int col) const {
    return _data[row * _width + col];
}

template<typename T, typename compare>
inline const T& Mat<T, compare>::At(int n) const {
    return _data[n];
}

template<typename T, typename compare>
inline void Mat<T, compare>::Set(int row, int col, T value) {
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
        throw new std::out_of_range("����ͼƬ�߽�");
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
        //���ж��Ƿ�Ϊ������Ļ�,�������ĺ�����С��������ֵʱ���жϸĻ��ѱ�����
        int next = i;
        do {
            next = (next % _width) * _height + next / _width;//��ȡ�ֻ�����
        } while (next > i);

        //��δ������Ļ������ֻ�
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
                //����Ԫ��
                std::swap(_data[cur], _data[pre]);
                cur = pre;
                pre = (cur % _height) * _width + cur / _height;//��ȡ�ֻ�ǰ��
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

    if (code == 1) {//�ӵ��ұ�
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
    else if (code == 2) {//�ӵ��±�
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
