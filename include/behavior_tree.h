#ifndef _behavior_tree
#define _behavior_tree

#include <iostream>
#include <vector>
#include <functional>
#include <memory>

class BehaviorTree
{
public:
	enum class Result { FAIL, RUNNING, SUCCESS };

private:
	class Node
	{
	public:
		virtual Result run() = 0;
	};

public:
	using NodePtr = std::unique_ptr< Node >;
	using Leaf = Node;

private:
	class NonLeaf : public Node
	{
	public:
		void add_child( NodePtr& node ) { children.push_back( std::move( node ) ); }
		void add_child( NodePtr&& node ) { children.push_back( std::move( node ) ); }

		virtual Result run() override = 0;

		template < typename ... Args >
		void add_children( Args&& ... args ) { ( children.push_back( std::move( args ) ), ... ); }

	protected:
		std::vector< NodePtr > children;
	};

public:
	class Selector : public NonLeaf
	{
	public:
		Result run() override
		{
			for ( size_t i = prev_running_pos; i < children.size(); ++i )
			{
				switch ( children[ i ]->run() )
				{
				case Result::RUNNING:
					prev_running_pos = i;
					return Result::RUNNING;

				case Result::SUCCESS:
					prev_running_pos = 0u;
					return Result::SUCCESS;

				case Result::FAIL:
					break;
				}
			}

			prev_running_pos = 0u;
			return Result::FAIL;
		}

	private:
		size_t prev_running_pos;
	};

	class Sequence : public NonLeaf
	{
	public:
		Result run() override
		{
			for ( size_t i = prev_running_pos; i < children.size(); ++i )
			{
				switch ( children[ i ]->run() )
				{
				case Result::RUNNING:
					prev_running_pos = i;
					return Result::RUNNING;

				case Result::FAIL:
					prev_running_pos = 0u;
					return Result::FAIL;

				case Result::SUCCESS:
					break;
				}
			}

			prev_running_pos = 0u;
			return Result::SUCCESS;
		}

	private:
		size_t prev_running_pos;
	};

	void run() { root->run(); }

	BehaviorTree( std::unique_ptr< Selector >& root ) : root{ std::move( root ) } {}
	BehaviorTree( std::unique_ptr< Sequence >& root ) : root{ std::move( root ) } {}
	BehaviorTree( NodePtr& root ) : root{ std::move( root ) } {}
	BehaviorTree( NodePtr&& root ) : root{ std::move( root ) } {}

private:
	NodePtr root;
};

#endif