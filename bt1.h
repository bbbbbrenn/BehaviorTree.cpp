#pragma once
#include<stdlib.h>
#include<vector>
class t_node {
public:
	t_node *control;
	void *object;
	void *blackboard;
	t_node(void*blackboard) :blackboard(blackboard) {}
	virtual void running()
	{
		if (this->control)
			this->control->running();
	}
	virtual void success()
	{
		if (this->control)
			this->control->success();
	}
	virtual void fail()
	{
		if (this->control)
			this->control->fail();
	}
	virtual void enter(void*object) = 0;
	virtual void tick(void*object) = 0;
	virtual void exit(void*object) = 0;
};
class t_branch :public t_node {
public:
	char is_running;
	int  running_idx;
	std::vector<t_node*>child_list;
	t_branch(const std::vector<t_node*>&nodes) :child_list(nodes), t_node(nullptr)
	{
		for (auto node : nodes)
		{
			node->control = this;
		}
	}
	void running()override
	{
		this->is_running = 1;
		this->control->running();
	}
	void success()override
	{
		this->is_running = 0;
		auto running_node = this->child_list[this->running_idx];
		running_node->exit(this->object);
	}
	void fail()override
	{
		this->is_running = 0;
		this->child_list[this->running_idx]->exit(this->object);
	}
	void enter(void*object)override
	{
		if (!this->is_running)
		{
			this->object = object;
			this->running_idx = 0;
		}
	}
	void tick(void*object)override
	{
		if (this->running_idx < (int)this->child_list.size())
		{
			this->run(object);
		}
	}
	void exit(void*object)override
	{

	}
public:
	void run(void*object)
	{
		auto running_node = this->child_list[this->running_idx];
		if (!this->is_running)
		{
			running_node->enter(object);
		}
		running_node->tick(object);
	}
};
class t_sequence :public t_branch {
public:
	t_sequence(const std::vector<t_node*>&nodes) :t_branch(nodes) {}
	void success()override
	{
		t_branch::success();
		this->running_idx++;
		if (this->running_idx < (int)this->child_list.size())
		{
			this->run(this->object);
		}
		else
		{
			this->control->success();
		}
	}
	void fail()override
	{
		t_branch::fail();
		this->control->fail();
	}
};
class t_random :public t_branch {
public:
	t_random(const std::vector<t_node*>&nodes) :t_branch(nodes) {}
	void enter(void*object)override
	{
		t_branch::enter(object);
		this->running_idx = rand() % (int)this->child_list.size();
#ifndef DEBUG
		printf(">>> random_enter running_idx %d\n", this->running_idx);
#endif
	}
	void success()override
	{
		t_branch::success();
		this->control->success();
	}
	void fail()override
	{
		t_branch::fail();
		this->control->fail();
	}
};
class t_priority :public t_branch {
public:
	t_priority(const std::vector<t_node*>&nodes) :t_branch(nodes) {}
	void success()override
	{
		t_branch::success();
		this->control->success();
	}
	void fail()override
	{
		t_branch::fail();
		this->running_idx++;
		if (this->running_idx < (int)this->child_list.size())
		{
			this->run(this->object);
		}
		else
		{
			this->control->fail();
		}
	}
};
class t_behavior_tree :public t_node {
public:
	t_branch*root;
	char started;
	t_behavior_tree(t_branch*_branch) :root(_branch), started(0), t_node(nullptr)
	{
		this->root->control = this;
	}
	void tick(void*object)override
	{
		if (this->started)
		{
			t_node::running();
		}
		else
		{
			this->started = 1;
			this->object = object;
			this->root->enter(object);
			this->root->tick(object);
		}
	}
	void enter(void*object)override {}
	void exit(void*object)override {}
	void running()override
	{
		t_node::running();
		this->started = 0;
	}
	void success()override
	{
		this->root->exit(this->object);
		this->started = 0;
		t_node::success();
	}
	void fail()override
	{
		this->root->exit(this->object);
		this->started = 0;
		t_node::fail();
	}
};