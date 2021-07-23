#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H

#include <typeindex>

class Object;

class Component {
public:
	bool enabled = true;
	Object* object;
	std::type_index type = typeid(Component);

	virtual ~Component() { };
};

#endif