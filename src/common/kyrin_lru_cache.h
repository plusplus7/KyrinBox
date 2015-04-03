#ifndef KYRINBOX_SRC_COMMON_KYRIN_LRU_CACHE_H_
#define KYRINBOX_SRC_COMMON_KYRIN_LRU_CACHE_H_

#include <map>
#include <list>
#include <utility>

namespace kyrin {
namespace common {

/* Not thread safe */

template<class key_t, class value_t>
class KyrinLruCache {
    public:
        typedef typename std::list< std::pair<key_t, value_t> >::iterator list_iterator_t;
        typedef typename std::map<key_t, list_iterator_t>::iterator map_iterator_t;

        KyrinLruCache(int capacity) {
            m_capacity = capacity; 
            m_map.clear();
            m_data.clear();
        }

        bool get(key_t &key, value_t &value) {
            map_iterator_t finder = m_map.find(key);
            if (finder == m_map.end()) 
                return false;
            else {
                m_data.splice(m_data.begin(), m_data, finder->second);
                value = finder->second->second;
                return true;
            }
        }                    

        void set(key_t &key, value_t &value) {
            map_iterator_t finder = m_map.find(key);
            if (finder != m_map.end()) {
                m_data.erase(finder->second);
                m_map.erase(finder);
            } else {
                if (__builtin_expect(m_map.size() == m_capacity, 1)) {
                    list_iterator_t last = m_data.end();
                    last--;
                    m_map.erase(last->first);
                    m_data.pop_back();
                }
            }
            m_data.push_front(std::make_pair(key, value));
            m_map[key] = m_data.begin();
        }
    private:
        int m_capacity;
        std::map<key_t, list_iterator_t> m_map;
        std::list< std::pair<key_t, value_t> > m_data;
};

} /* kyrin */
} /* common */

#endif /* end of include guard: KYRINBOX_SRC_COMMON_KYRIN_LRU_CACHE_H_ */
