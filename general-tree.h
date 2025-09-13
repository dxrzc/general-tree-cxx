#pragma once

#include <stdexcept>
#include <utility>

template<typename T>
class general_tree
{
private:
	struct private_node
	{
		T m_data;
		private_node* m_right_sibling;
		private_node* m_left_child;
		private_node* m_parent;

		template<typename... Args>
		private_node(Args&&...args) :
			m_right_sibling(nullptr),
			m_left_child(nullptr),
			m_parent(nullptr),
			m_data(std::forward<Args>(args)...)
		{}
	};

	private_node* m_root;

public:
	/**
	* @brief Public node interface
	*/
	class node
	{
	private:
		private_node* m_node;
		friend class general_tree;

	public:
		node(private_node* node = nullptr) noexcept
			: m_node(node) {}

		bool operator==(const node& other) const noexcept
		{
			return m_node == other.m_node;
		}

		/**
		 * @brief Retrieves the left child of the current node.
		 * @return The left child node, or a null node if the current node has no left child.
		 */
		[[nodiscard]] node left_child() const noexcept
		{
			return m_node->m_left_child;
		}

		/**
		 * @brief Retrieves the parent of the current node.
		 * @return The parent node, or a null node if the current node has no parent.
		 */
		[[nodiscard]] node parent() const noexcept
		{
			return m_node->m_parent;
		}

		/**
		 * @brief Retrieves the right sibling of the current node.
		 * @return The right sibling node, or a null node if the current node has no right sibling.
		 */
		[[nodiscard]] node right_sibling() const noexcept
		{
			return m_node->m_right_sibling;
		}

		/**
		* @brief Accesses the data stored in the given node.		
		* @return const T& Reference to the data stored in the node.
		* @throws std::invalid_argument If the given node is null.
		*/
		[[nodiscard]] const T& data() const
		{
			if (!m_node)
				throw std::invalid_argument("Cannot get data from null node");
			return m_node->m_data;
		}
		
		/**
		* @brief Accesses the data stored in the given node.		
		* @return T& Reference to the data stored in the node.
		* @throws std::invalid_argument If the given node is null.
		*/
		[[nodiscard]] T& data()
		{
			return const_cast<T&>(static_cast<const general_tree<T>::node&>(*this).data());
		}

		/*
		* @brief Checks if the node is the root of the tree.
		* @return true if the node is the root, false otherwise.
		*/
		bool is_root() const noexcept
		{
			return m_node->m_parent == nullptr;
		}

		/**
		 * @brief Checks if the node is a leaf in the tree.
		 * @return true if the node is a leaf, false otherwise.
		 */
		bool is_leaf() const noexcept
		{
			return m_node->m_left_child == nullptr;
		}

		/**
		 * @brief Checks if the node has a right sibling.
		 * @return true if the node has a right sibling, false otherwise.
		 */
		bool has_right_sibling() const noexcept
		{
			return m_node->m_right_sibling != nullptr;
		}

		/**
		 * @brief Checks if the node has a left child.
		 * @return true if the node has a left child, false otherwise.
		 */
		bool has_left_child() const noexcept
		{
			return m_node->m_left_child != nullptr;
		}

		/**
		 * @brief Checks if the node is null.
		 * @return true if the node is null, false otherwise.
		 */
		bool is_null() const noexcept
		{
			return m_node == nullptr;
		}
	};

	general_tree() noexcept : m_root(nullptr) {}

	template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
	general_tree(U&& root_value) : general_tree()
	{
		m_root = new private_node(std::forward<U>(root_value));
	}

	/**
	 * @brief Creates and emplaces the root node of the tree with the given arguments.
	 * @tparam Args Variadic template parameters representing the types of arguments to be forwarded to the root node constructor.
	 * @param args Arguments to forward to the root node constructor.
	 * @return node A handle to the newly created root node.
	 * @throws std::runtime_error If a root node already exists.
	*/
	template<typename ...Args>
	node emplace_root(Args&& ...args)
	{
		if (m_root != nullptr)
			throw std::runtime_error("Root already exists");
		m_root = new private_node(std::forward<Args>(args)...);
		return m_root;
	}

	/**
	 * @brief Creates the root node of the tree
	 * @param data The value to store in the root node.
	 * @return node A handle to the newly created root node.
	 * @throws std::runtime_error If a root node already exists.
	 */
	template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
	node create_root(U&& data)
	{
		return emplace_root(std::forward<U>(data));
	}

	/**
	 * @brief Creates and emplaces a new left child node for the given destination node.
	 * @tparam Args Variadic template parameters for the new node constructor.
	 * @param destiny The node to which the new left child will be attached.
	 * @param args Arguments to forward to the new node's constructor.
	 * @return node A handle to the newly created left child node.
	 * @throws std::invalid_argument If the destination node is null.
	 */
	template<typename ...Args>
	node emplace_left_child(node destiny, Args&& ...args)
	{
		if (destiny.m_node == nullptr)
			throw std::invalid_argument("Cannot insert left child to null node");

		private_node* new_node = new private_node(std::forward<Args>(args)...);
		new_node->m_right_sibling = destiny.m_node->m_left_child;
		new_node->m_parent = destiny.m_node;

		destiny.m_node->m_left_child = new_node;
		return new_node;
	}

	/**
	 * @brief Inserts a new left child node for the given destination node, with the provided value.
	 * @param destiny The node to which the left child will be inserted.
	 * @param new_node_value The value to store in the newly created left child node.
	 * @return node A handle to the newly created left child node.
	 * @throws std::invalid_argument If the destination node is null.
	*/
	template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
	node insert_left_child(node destiny, U&& new_node_value)
	{
		return emplace_left_child(destiny, std::forward<U>(new_node_value));
	}

	/**
	 * @brief Inserts an entire subtree as the left child of the given destination node.
	 * @param destiny The node to which the tree will be inserted as a left child.
	 * @param tree The tree to insert as a left child. After insertion, this tree will be empty.
	 * @return node A handle to the newly inserted left child node, or a null node if the tree is empty.
	 * @throws std::invalid_argument If the destination node is null or if attempting to insert the tree as a child of its own root.
    */
	node insert_left_child(node destiny, general_tree<T>& tree)
	{
		if (!destiny.m_node)
			throw std::invalid_argument("Cannot insert left child to null node");

		if (destiny.m_node == tree.m_root)
			throw std::invalid_argument("Cannot insert a tree as its own child");

		if (tree.m_root)
		{
			tree.m_root->m_parent = destiny.m_node;
			tree.m_root->m_right_sibling = destiny.m_node->m_left_child;
			destiny.m_node->m_left_child = tree.m_root;
			tree.m_root = nullptr;
			return destiny.m_node->m_left_child;
		}

		return node(nullptr);
	}

	/**
	 * @brief Inserts an entire subtree as the right sibling of the given destination node.
	 * @param destiny The node to which the tree will be inserted as a right sibling.
	 * @param tree The tree to insert as a right sibling. After insertion, this tree will be empty.
	 * @return node A handle to the newly inserted right sibling node, or a null node if the tree is empty.
	 * @throws std::invalid_argument If the destination node is null, is the root (cannot have siblings),
	 *                               or if attempting to insert the tree as its own sibling.
	 */
	node insert_right_sibling(node destiny, general_tree<T>& tree)
	{
		if (destiny.m_node == nullptr)
			throw std::invalid_argument("Cannot insert right sibling to null node");

		if (destiny.m_node->m_parent == nullptr)
			throw std::invalid_argument("Cannot insert right sibling to root");

		if (destiny.m_node == tree.m_root)
			throw std::invalid_argument("Cannot insert a tree as its own sibling");

		if (tree.m_root)
		{
			tree.m_root->m_parent = destiny.m_node->m_parent;
			tree.m_root->m_right_sibling = destiny.m_node->m_right_sibling;
			destiny.m_node->m_right_sibling = tree.m_root;
			tree.m_root = nullptr;
			return destiny.m_node->m_right_sibling;
		}

		return node(nullptr);
	}

	/**
	 * @brief Returns the root node of the tree.
	 */
	[[nodiscard]] node root() const noexcept
	{
		return m_root;
	}

	/**
	 * @brief Checks whether the tree is empty.
	 */
	bool empty() const noexcept
	{
		return m_root == nullptr;
	}
};