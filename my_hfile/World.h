#pragma once
#ifndef __World__
#define __World__
#include "iostream"
#include "fstream"
#include "strstream"
#include "Game_Objects.h"
#include "time.h"

//extern std::string world_name;
enum class World_type
{
	null,
	normal,
	flat
};

class Blocks
{
public:
	Blocks(int, int);
	void CreateBlocks_null(std::string);
	void CreateBlocks_normal(std::string);
	void CreateBlocks_flat(std::string);
	void LaodinBlocks();
	int operator!=(Blocks& blocks2);
	~Blocks();
	std::string blocks_x_str;
	std::string blocks_z_str;
	int blocks_x;
	int blocks_z;

};

class World :public Game_Object
{
public:
	World(std::string, World_type);
	World() :typeID(World_type::null), nowworld_name(std::string()), world_seed(0) {};
	~World() {}
	inline int get_ID() { return world_seed; }
	void CreatWorld();
	void LaodWorld() {};
private:
	World_type typeID;
	std::string nowworld_name;
	int world_seed;
};

#endif // !1
