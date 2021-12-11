#ifndef _Region
#define _Region

#include <unordered_map>
#include <stack>
#include "CClass.h"

class Region
{
public:
	using ID_t = int;
	friend class GameObject;

private:
	std::unordered_map< ID_t, std::unordered_map< ID_t, CGameObject > > active;
	std::stack< CGameObject > inactive;
};

#endif