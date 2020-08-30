#include "my_hfile/World.h"

Blocks::Blocks(int x, int z) :blocks_x(x), blocks_z(z)
{
	std::strstream* turner = new std::strstream;
	turner->clear();
	*turner << blocks_x, * turner >> blocks_x_str;
	turner->clear();
	*turner << blocks_z, * turner >> blocks_z_str;

}

Blocks::~Blocks()
{
}

void Blocks::CreateBlocks_null(std::string world_name)
{
	std::fstream now_blocks;

	std::string* blocks_filename = new std::string(blocks_x_str + "  " + blocks_z_str + ".dat");
	std::string* blocks_filepath = new std::string("Saves/" + world_name + "/" + *blocks_filename);
	delete(blocks_filename);

	int posx = 0, posy = 0, posz = 0, block = 0;
	now_blocks.open(*blocks_filepath, std::ios::out | std::ios::binary);
	if (now_blocks.fail())
	{
		exit(-1);
	}
	while (posx >= 0 && posx < 16)
	{
		while (posz >= 0 && posz < 16)
		{
			while (posy >= 0 && posy < 16)
			{

				block = rand() % 4;
				now_blocks.write((char*)&block, sizeof(block));
				posy++;
			}
			posy = 0;
			posz++;
		}
		posz = 0;
		posx++;
	}
	now_blocks.close();

}

void Blocks::CreateBlocks_normal(std::string world_name)
{
	std::fstream now_blocks;

	std::string* blocks_filename = new std::string(blocks_x_str + "  " + blocks_z_str + ".dat");
	std::string* blocks_filepath = new std::string("Saves/" + world_name + "/" + *blocks_filename);
	delete(blocks_filename);

	int posx = 0, posy = 0, posz = 0, block = 0, lasty_block = 0;
	now_blocks.open(*blocks_filepath, std::ios::out | std::ios::binary);
	if (now_blocks.fail())
	{
		exit(-1);
	}

	//矿物层
	while (posx >= 0 && posx < 16)
	{
		while (posz >= 0 && posz < 16)
		{
			while (posy >= 0 && posy < 16)
			{
				switch (posy)
				{
				case 0:
					block = 1;
					break;
				default:
					if (lasty_block)
					{
						block = rand() % 4;
					}

					break;
				}
				lasty_block = block;
				now_blocks.write((char*)&block, sizeof(block));
				posy++;
			}
			posy = 0;
			posz++;
		}
		posz = 0;
		posx++;
	}
	now_blocks.close();

}

void  Blocks::CreateBlocks_flat(std::string world_name)
{
	std::fstream now_blocks;
	std::string* blocks_filename = new std::string(blocks_x_str + "  " + blocks_z_str + ".dat");
	std::string* blocks_filepath = new std::string("Saves/" + world_name + "/" + *blocks_filename);
	delete(blocks_filename);

	int posx = 0, posy = 0, posz = 0, block = 0, lasty_block = 0;
	now_blocks.open(blocks_filepath->c_str(), std::ios::out | std::ios::binary);
	if (now_blocks.fail())
	{
		exit(-1);
	}

	while (posx >= 0 && posx < 16)
	{
		while (posz >= 0 && posz < 16)
		{
			while (posy >= 0 && posy < 16)
			{
				switch (posy)
				{
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
					block = (int)BlockTextureID::stone;
					break;
				case 7:
				case 8:
				case 9:
					block = (int)BlockTextureID::dirt;
					break;
				case 10:
					block = (int)BlockTextureID::grass;
					break;
				default:
					block = 0;

					break;
				}
				now_blocks.write((char*)&block, sizeof(block));
				posy++;
			}
			posy = 0;
			posz++;
		}
		posz = 0;
		posx++;
	}
	now_blocks.close();

}

void Blocks::LaodinBlocks()
{


}

int Blocks::operator!=(Blocks& blocks2)
{
	if (this->blocks_x == blocks2.blocks_x && this->blocks_z == blocks2.blocks_z)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}

World::World(std::string world_name, World_type typein) :typeID(typein)
{
	this->nowworld_name = world_name;
	CreatWorld();

}

void World::CreatWorld()
{
	system((std::string("md Saves\\") + this->nowworld_name).c_str());
	int* seed = new int;
	std::string* seed_str = new std::string;
	std::strstream* seed_strstream = new std::strstream;
	if (0)
	{
		*seed_strstream << seed_str->c_str() << std::ends;
		*seed_strstream >> *seed;

	}
	else
	{
		*seed = time(NULL);
		srand(*seed);
	}
	delete(seed_str);
	delete(seed_strstream);

	std::fstream* a = new std::fstream;
	a->open("Saves/" + this->nowworld_name + "/" + "seed.txt", std::ios::out);
	a->write((char*)seed, sizeof(int));
	delete(seed);
	a->close();

	int blocks_x = -1, blocks_z = -1;
	std::cerr << "创建世界中：" << '[';
	while (blocks_x < 2)
	{
		while (blocks_z < 2)
		{
			switch (typeID)
			{
			case World_type::null:
				Blocks(blocks_x, blocks_z).CreateBlocks_null(this->nowworld_name);
				break;
			case World_type::normal:
				Blocks(blocks_x, blocks_z).CreateBlocks_normal(this->nowworld_name);
				break;
			case World_type::flat:
				Blocks(blocks_x, blocks_z).CreateBlocks_flat(this->nowworld_name);
				break;
			default:
				break;
			}

			blocks_z++;
			std::cerr << '*';
		}
		blocks_z = -1;
		blocks_x++;
	}
	std::cerr << ']' << std::endl;
}

void LaodinWorld()
{


}
