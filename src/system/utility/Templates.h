#ifndef TEMPLATES_H_
#define TEMPLATES_H_

#include <map>
#include <vector>

template<typename T1, typename T2> void clearMap(std::map<T1, T2>* map) {
	typename std::map<T1, T2>::const_iterator i;
	for (i = map->begin(); i != map->end(); ++i) {
		delete i->second;
	}
	map->clear();
}

template<typename T> void clearVector(std::vector<T>* vector) {
	for (unsigned int i = 0; i < vector->size(); i++) {
		delete (*vector)[i];
	}
	vector->clear();
}

template<typename T> size_t getStructSize(const T& t) {
	return sizeof(t) / sizeof(*t);
}

#endif /* TEMPLATES_H_ */
