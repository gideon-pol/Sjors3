#pragma once
#ifndef UTILS_H
#define UTILS_H

template<class T, class D>
class Pair {
public:
	T first;
	D second;

	Pair(T f, D s) {
		first = f;
		second = s;
	}
};

#endif
