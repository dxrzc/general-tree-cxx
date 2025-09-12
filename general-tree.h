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
	};

	general_tree() noexcept : m_root(nullptr) {}

	template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
	general_tree(U&& root_value) : general_tree()
	{
		m_root = new private_node(std::forward<U>(root_value));
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