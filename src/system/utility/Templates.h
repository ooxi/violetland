#ifndef TEMPLATES_H_
#define TEMPLATES_H_

#include <map>
#include <vector>

template<typename T1, typename T2> void clearMap(std::map<T1, T2>* _map) {
	typename std::map<T1, T2>::const_iterator i;
	for (i = _map->begin(); i != _map->end(); ++i) {
		delete i->second;
	}
	_map->clear();
}

template<typename T> void clearVector(std::vector<T>* _vector) {
	for (unsigned int i = 0; i < _vector->size(); i++) {
		delete (*_vector)[i];
	}
	_vector->clear();
}

template<typename T> size_t getStructSize(const T& _t) {
	return sizeof(_t) / sizeof(*_t);
}

#endif /* TEMPLATES_H_ */
