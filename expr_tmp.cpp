#include <iostream>

template <typename Derived>
class VectorBase
{
public:
    Derived &derived() { return *static_cast<Derived *const>(this); }
    const Derived &derived() const { return *static_cast<const Derived *const>(this); }
    double operator[](int i) const { return derived().operator[](i); }
    int size() const { return derived().size(); }
};

class Vector : public VectorBase<Vector>
{
private:
    double *_data;
    int _size;

public:
    Vector(int size) : _size(size) { _data = new double[size]; }

    double &operator[](int i) { return _data[i]; }
    double operator[](int i) const { return _data[i]; }

    int size() const { return _size; }

    template <typename RightType>
    Vector(const VectorBase<RightType> &right)
    {
        const RightType &right_derived = right.derived();
        int size = right_derived.size();
        _data = new double[size];
        for (int i = 0; i < size; ++i)
        {
            _data[i] = right_derived[i];
        }
    }

    template <typename RightType>
    Vector &operator=(const VectorBase<RightType> &right)
    {
        const RightType &right_derived = right.derived();
        int size = right_derived.size();
        _data = new double[size];
        for (int i = 0; i < size; ++i)
        {
            _data[i] = right_derived[i];
        }
        return *this;
    }

    void show()
    {
        std::cout << "[";
        for (int i = 0; i < _size - 1; ++i)
        {
            std::cout << _data[i] << ", ";
        }
        std::cout << _data[_size - 1] << "]" << std::endl;
    }
};

template <typename LeftType, typename RightType>
class Add : public VectorBase<Add<LeftType, RightType>>
{
private:
    const LeftType &_left;
    const RightType &_right;

public:
    Add(const LeftType &left, const RightType &right) : _left(left), _right(right) {}

    int size() const { return _left.size(); }
    double operator[](int i) const { return _left[i] + _right[i]; }
};

template <typename LeftType, typename RightType>
Add<LeftType, RightType> operator+(const VectorBase<LeftType> &left, const VectorBase<RightType> &right)
{
    return Add<LeftType, RightType>(left.derived(), right.derived());
}

int main()
{
    Vector x = Vector(3);
    Vector y = Vector(3);
    Vector z = Vector(3);

    for (int i = 0; i < 3; ++i)
    {
        x[i] = i;
        y[i] = 2 * i;
    }

    x.show();
    y.show();
    z.show();

    z = x + y;

    x.show();
    y.show();
    z.show();
}