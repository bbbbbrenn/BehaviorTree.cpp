#pragma once
#include<vector>
#include<string>
enum e_status
{
	//e_status_invalid,
	e_status_running,
	e_status_success,
	e_status_failure,
};
class t_node
{
public:
	virtual void enter() = 0;
	virtual e_status tick() = 0;
	virtual void exit() = 0;
};
class t_sequence :public t_node
{
	std::vector<t_node*>nodes;
	int curid;
	bool is_running;
public:
	t_sequence(const std::vector<t_node*>&nodes) :nodes(nodes) {}
	void enter()override
	{
		if (!is_running)
		{
			curid = 0;
		}
	}
	e_status tick()override
	{
		while (curid < (int)this->nodes.size())
		{
			auto node = nodes[curid];
			if (!is_running)
				node->enter();
			auto ret = node->tick();
			switch (ret)
			{
			case e_status_running:
				is_running = 1;
				return e_status_running;
			case e_status_success:
				is_running = 0;
				node->exit();
				curid++;
				if (curid < (int)this->nodes.size())continue;
				else return e_status_success;

			case e_status_failure:
				is_running = 0;
				node->exit();
				return e_status_failure;
			default:
				break;
			}
		}
		return e_status_success;
	}
	void exit()override
	{

	}
};
class t_selector :public t_node
{
	std::vector<t_node*>nodes;
	int curid;
	bool is_running;
public:
	t_selector(const std::vector<t_node*>&nodes) :nodes(nodes) {}
	void enter()override
	{
		if (!is_running)
		{
			curid = 0;
		}
	}
	e_status tick()override
	{
		while (curid < (int)this->nodes.size())
		{
			auto node = nodes[curid];
			if (!is_running)
				node->enter();
			auto ret = node->tick();
			switch (ret)
			{
			case e_status_running:
				is_running = 1;
				return e_status_running;
			case e_status_success:
				is_running = 0;
				node->exit();
				return e_status_success;
			case e_status_failure:
				is_running = 0;
				node->exit();
				curid++;
				if (curid < (int)nodes.size())
					continue;
				else
					return e_status_failure;
			default:
				break;
			}
		}
		return e_status_failure;
	}
	void exit()override
	{

	}
};
class t_random :public t_node
{
	std::vector<t_node*>nodes;
	int curid;
	bool is_running;
public:
	t_random(const std::vector<t_node*>&nodes) :nodes(nodes) {}
	void enter()override
	{
		if (!is_running)
		{
			curid = 0;
		}
		curid = rand() % (int)nodes.size();
#ifndef DEBUG
		printf(">>> random_enter running_idx %d\n", curid);
#endif
	}
	e_status tick()override
	{
		while (curid < (int)this->nodes.size())
		{
			auto node = nodes[curid];
			if (!is_running)
				node->enter();
			auto ret = node->tick();
			switch (ret)
			{
			case e_status_running:
				is_running = 1;
				return e_status_running;
			case e_status_success:
				is_running = 0;
				node->exit();
				return e_status_success;
			case e_status_failure:
				is_running = 0;
				node->exit();
				return e_status_failure;
			default:
				break;
			}
		}
		return e_status_failure;
	}
	void exit()override
	{

	}
};
class t_behavior_tree :public t_node
{
	t_node*root;
	char started;
public:
	t_behavior_tree(t_node*root) :root(root), started(0)
	{

	}
	void enter()override
	{

	}
	e_status tick()override
	{
		if (this->started)
		{
			return e_status_running;
		}
		else
		{
			this->started = 1;
			this->root->enter();
			auto ret = this->root->tick();
			switch (ret)
			{
			case e_status_running:
				this->started = 0;
				break;
			case e_status_success:
				root->exit();
				this->started = 0;
				break;
			case e_status_failure:
				root->exit();
				started = 0;
				break;
			default:
				break;
			}
			return ret;
		}
	}
	void exit()override
	{

	}
};