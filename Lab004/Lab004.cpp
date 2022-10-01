// Lab004.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <iterator>
#include <vector>
#include <list>
#include <random>

template <typename T>
struct Comparator
{
    virtual bool operator()(T const&, T const&) const = 0;
};

struct IntComparator final : Comparator<int>
{
    bool operator()(int const& lha, int const& rha) const override
    {
        return lha < rha;
    }
};

template <class RandomAccessIterator>
void hoaraQuickSort(RandomAccessIterator first, RandomAccessIterator last, 
    Comparator<typename std::iterator_traits<RandomAccessIterator>::value_type>& comp)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    
    auto i = first, j = last - 1;
    value_type x = *(first + (last - first) / 2);
    do {
        while (comp(*i, x))
            ++i;
        while (comp(x, *j))
            --j;

        if (i <= j) {
            if (comp(*j, *i))
                std::swap(*i, *j);
            ++i;
            --j;
        }
    } while (i <= j);

    if (i < last - 1)
        hoaraQuickSort(i, last, comp);
    if (first < j)
        hoaraQuickSort(first, j + 1, comp);
}

template <typename T>
class PriorityQueue
{
private:
    std::vector<T> _tree;
    Comparator<T>& _comp;

public:
    PriorityQueue(Comparator<T>& comp) : _comp(comp)
    {
        _tree.push_back(static_cast<T>(0));
    }
    
    void push(T val)
    {
        _tree.push_back(val);
        sift_up(_tree.size() - 1);
    }

    T peek()
    {
        if (_tree.size() > 1)
            return _tree[1];
    }

    void poll()
    {
        if (_tree.size() > 1) {
            _tree[1] = _tree.back();
            _tree.pop_back();
            sift_down(1);
        }
    }

    void sift_up(int v) 
    {
        if (v == 1)
            return;

        if (_comp(_tree[v / 2], _tree[v])) {
            std::swap(_tree[v], _tree[v / 2]);
            sift_up(v / 2);
        }
    }

    void sift_down(int v) 
    {
        if (v * 2 >= _tree.size())
            return;

        int max_idx;
        if (v * 2 + 1 == _tree.size())
            max_idx = v * 2;
        else if (!_comp(_tree[v * 2], _tree[v * 2 + 1]))
            max_idx = v * 2;
        else
            max_idx = v * 2 + 1;

        if (_comp(_tree[v], _tree[max_idx])) {
            std::swap(_tree[v], _tree[max_idx]);
            sift_down(max_idx);
        }
    }

    bool is_empty()
    {
        return _tree.size() == 1;
    }

    void free()
    {
        _tree.clear();
    }
};

int main()
{
#pragma region Test 1
    std::mt19937 gen(static_cast<unsigned int>(time(nullptr)));
    std::uniform_int_distribution<int> dist_n(10000, 20000);
    std::uniform_int_distribution<int> dist(-100000, 100000);

    size_t n = dist_n(gen);
    std::vector<int> arr;
    std::generate_n(std::back_inserter(arr), n, [&dist, &gen]() { return dist(gen); });

    IntComparator comparator;
    hoaraQuickSort(arr.begin(), arr.end(), comparator);

    bool sorted = true;
    for (auto i = arr.begin() + 1; i != arr.end(); ++i)
        if (*(i - 1) > *i)
            sorted = false;

    std::cout << (sorted ? "Array is sorted\n" : "Array isn't sorted\n");
#pragma endregion
#pragma region Test 2
    size_t n2 = dist_n(gen);
    std::vector<int> arr2;
    std::generate_n(std::back_inserter(arr2), n2, [&dist, &gen]() { return dist(gen); });

    PriorityQueue<int> pq(comparator);

    for (auto i : arr2)
        pq.push(i);

    for (auto& i : arr2) {
        i = pq.peek();
        pq.poll();
    }

    pq.free();

    sorted = true;
    for (auto i = arr.begin() + 1; i != arr.end(); ++i)
        if (*(i - 1) > *i)
            sorted = false;

    std::cout << (sorted ? "Array 2 is sorted\n" : "Array 2 isn't sorted\n");
#pragma endregion
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
