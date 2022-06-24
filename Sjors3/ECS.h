#pragma once
#ifndef ECS_H
#define ECS_H

#include <vector>
#include <map>
#include "object.h"
#include "component.h"
#include "meshrenderer.h"

class ECS {
public:
	//static void AddObject(Object &object);
	//static void RemoveObject(Object &object);
private:
	static uint64_t curId;
	//static std::map<uint64_t, Object&> objects;
};

#endif
