#ifndef CONCURRENT_MAP_H
#define CONCURRENT_MAP_H

#include <unordered_map>
#include <shared_mutex>
#include <mutex>
#include <vector>
#include <typeinfo>
#include <iostream>

template <typename Key, typename Value>
class ConcurrentMap {};

template <typename Value>
class ConcurrentMap<std::string, Value> {
public:
    using key_type = std::string;
    using value_type = Value;
    using map_type = std::unordered_map<key_type, value_type>;
    using iterator_type = typename map_type::iterator;
    using pair_type = std::pair<key_type, value_type>;
    using mutex_type = std::shared_mutex;

    ConcurrentMap(int bucket_num = 1) : umaps_num(bucket_num)
    {
        umaps = new map_type[umaps_num];
        mutexs = new mutex_type[umaps_num];
    }

    ~ConcurrentMap()
    {
        if (umaps != nullptr) {
            delete []umaps;
        }
        if (mutexs != nullptr) {
            delete []mutexs;
        }
    }

    ConcurrentMap(ConcurrentMap &o) = delete;
    ConcurrentMap& operator=(ConcurrentMap &o) = delete;

    inline iterator_type find(const key_type &k)
    {
        auto i = getIndex(k);
        std::shared_lock<mutex_type> lock(mutexs[i]);
        return umaps[i].find(k);
    }

    inline std::pair<iterator_type, bool> insert(pair_type &&pair)
    {
        auto i = getIndex(pair.first);
        std::unique_lock<mutex_type> lock(mutexs[i]);
        return umaps[i].insert(pair);
    }

    inline iterator_type end() noexcept
    {
        return umaps[0].end();
    }

    inline size_t erase(const key_type &k)
    {
        auto i = getIndex(k);
        std::unique_lock<mutex_type> lock(mutexs[i]);
        return umaps[i].erase(k);
    }

    // for debug
    std::vector<int> mapInfo()
    {
        std::vector<int> ret;
        for (int i = 0; i < umaps_num; i++) {
            ret.push_back(umaps[i].size());
        }
        return ret;
    }

private:
    map_type *umaps;
    mutex_type *mutexs;  // matching to umaps
    int umaps_num;

    int getIndex(const key_type &k)
    {
        if (umaps_num == 1) {
            return 0;
        }

        uint64_t sum = 0;
        for (int i = 0; i < k.length(); i++) {
            sum += k[i];
        }
        return sum % umaps_num;
    }
};

template <typename Value>
class ConcurrentMap<int, Value> {
public:
    using key_type = int;
    using value_type = Value;
    using map_type = std::unordered_map<key_type, value_type>;
    using iterator_type = typename map_type::iterator;
    using pair_type = std::pair<key_type, value_type>;
    using mutex_type = std::shared_mutex;

    ConcurrentMap(int bucket_num = 1) : umaps_num(bucket_num)
    {
        umaps = new map_type[umaps_num];
        mutexs = new mutex_type[umaps_num];
    }

    ~ConcurrentMap()
    {
        if (umaps != nullptr) {
            delete []umaps;
        }
        if (mutexs != nullptr) {
            delete []mutexs;
        }
    }

    ConcurrentMap(ConcurrentMap &o) = delete;
    ConcurrentMap& operator=(ConcurrentMap &o) = delete;

    inline iterator_type find(const key_type &k)
    {
        auto i = getIndex(k);
        std::shared_lock<mutex_type> lock(mutexs[i]);
        return umaps[i].find(k);
    }

    inline std::pair<iterator_type, bool> insert(pair_type &&pair)
    {
        auto i = getIndex(pair.first);
        std::unique_lock<mutex_type> lock(mutexs[i]);
        return umaps[i].insert(pair);
    }

    inline iterator_type end() noexcept
    {
        return umaps[0].end();
    }

    inline size_t erase(const key_type &k)
    {
        auto i = getIndex(k);
        std::unique_lock<mutex_type> lock(mutexs[i]);
        return umaps[i].erase(k);
    }

    // for debug
    std::vector<int> mapInfo()
    {
        std::vector<int> ret;
        for (int i = 0; i < umaps_num; i++) {
            ret.push_back(umaps[i].size());
        }
        return ret;
    }

private:
    map_type *umaps;
    mutex_type *mutexs;  // matching to umaps
    int umaps_num;

    int getIndex(const key_type &k)
    {
        if (umaps_num == 1) {
            return 0;
        }

        return k % umaps_num;
    }
};

#endif