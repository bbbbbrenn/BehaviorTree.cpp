#include"bt2.h"
#include<stdio.h>

////////////////
class t_looking :public t_node
{
public:
	void enter()override
	{
		printf("looking_enter %p\n", this);
	}
	e_status tick()override
	{
		printf("looking_tick %p\n", this);
		return e_status_success;
	}
	void exit()override
	{
		printf("looking_exit %p\n", this);
	}
};

class t_running :public t_node
{
	std::string dir;
public:
	t_running(const std::string&dir) :dir(dir) {}
	void enter()override
	{
		printf("running_enter %p %s\n", this, dir.c_str());
	}
	e_status tick()override
	{
		printf("running_tick %p %s\n", this, dir.c_str());
		return e_status_success;
	}
	void exit()override
	{
		printf("running_exit %p %s\n", this, dir.c_str());
	}
};
class t_standing :public t_node
{
	int count;
public:
	t_standing(int count) :count(count) {}
	void enter()override
	{
		printf("standing_enter %p\n", this);
	}
	e_status tick()override
	{
		count++;
		printf("standing_tick %p, stanging_count %d\n", this, count);
		if (count > 4)
			return e_status_success;
		else if (count >= 3 && count <= 4)
			return e_status_failure;
		else
			return e_status_running;
	}
	void exit()override
	{
		printf("standing_exit %p\n", this);
	}
};
int main(int argc, const char*argv[])
{
	t_behavior_tree bt
	(
		new t_sequence
		({
			new t_selector
			({
				new t_standing(0),
				new t_looking(),
			}),
			new t_random
			({
				new t_running("SOUTH"),
				new t_running("WEST"),
				new t_running("EAST"),
				new t_running("NORTH"),
			}),
			})
			);
	int ctx = 1;
	for (int tick = 0; tick < 10; ++tick)
	{
		printf(">>> behaviourtree_tick %d\n", tick);
		bt.tick();
	}
	getchar();
	return 0;
}