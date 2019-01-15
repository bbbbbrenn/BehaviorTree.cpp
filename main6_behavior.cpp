#include"bt1.h"
#include<stdio.h>
////////////////
enum e_DIR
{
	DIR_S = 2,
	DIR_W = 4,
	DIR_E = 6,
	DIR_N = 8,
};

const char *DIR_NAME[9] =
{
	nullptr,nullptr, "SOUTH",
	nullptr, "WEST",nullptr,
	"EAST",nullptr, "NORTH",
};
class t_looking :public t_node
{
public:
	t_looking(void*blackboard) :t_node(blackboard) {}
	void enter(void*object)override
	{
		printf("looking_enter %p\n", this);
	}
	void tick(void*object)override
	{
		printf("looking_tick %p\n", this);
		this->control->success();
	}
	void exit(void*object)override
	{
		printf("looking_exit %p\n", this);
	}
};
class t_running :public t_node
{
public:
	t_running(void*blackboard) :t_node(blackboard) {}
	void enter(void*object)override
	{
		int dir = *(int*)this->blackboard;
		printf("running_enter %p %s\n", this, DIR_NAME[dir]);
	}
	void tick(void*object)override
	{
		int dir = *(int *)this->blackboard;
		printf("running_tick %p %s\n", this, DIR_NAME[dir]);
		this->control->success();
	}
	void exit(void*object)override
	{
		int dir = *(int *)this->blackboard;
		printf("running_exit %p %s\n", this, DIR_NAME[dir]);
	}
};
class t_standing :public t_node
{
public:
	t_standing(void*blackboard) :t_node(blackboard) {}
	void enter(void*object)override
	{
		printf("standing_enter %p\n", this);
	}
	void tick(void*object)override
	{
		int *standing_count = (int*)this->blackboard;
		*standing_count = *standing_count + 1;
		printf("standing_tick %p, stanging_count %d\n", this, *standing_count);
		if (*standing_count > 4)
			this->control->success();
		else if (*standing_count >= 3 && *standing_count <= 4)
			this->control->fail();
		else
			this->control->running();
	}
	void exit(void*object)override
	{
		printf("standing_exit %p\n", this);
	}
};
int main(int argc, const char*argv[])
{
	int standing_count = 0;
	int running_dir[4] = { DIR_S, DIR_W, DIR_E, DIR_N };

	t_behavior_tree bt
	(
		new t_sequence
		({
			new t_priority
			({
				new t_standing(&standing_count),
				new t_looking(nullptr),
			}),
			new t_random
			({
				new t_running(running_dir + 0),
				new t_running(running_dir + 1),
				new t_running(running_dir + 2),
				new t_running(running_dir + 3),
			}),
			})
			);
	int ctx = 1;
	for (int tick = 0; tick < 10; ++tick)
	{
		printf(">>> behaviourtree_tick %d\n", tick);
		bt.tick(&ctx);
	}
	getchar();
	return 0;
}