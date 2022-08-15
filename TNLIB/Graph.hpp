#pragma once

// STL
#include<unordered_map>
#include<memory>
#include<type_traits>

// TNLIB
#include"Debug.hpp"

//template<class KeyType, class ValueType>
//class NodeGraph {
//
//private:
//
//	std::unordered_map<KeyType, ValueType> m_data;
//
//public:
//
//#pragma region constructor
//
//	NodeGraph() = default;
//	NodeGraph(const NodeGraph&) = default;
//	NodeGraph(NodeGraph&&) = default;
//
//#pragma endregion constructor
//
//public:
//
//#pragma region operator
//
//	ValueType& oeprator[](const KeyType& key) {
//		return m_data[key];
//	}
//
//#pragma endregion operator
//
//public:
//
//	ValueType& At(const KeyType& key) {
//		auto iter = m_data.find(key);
//		if (iter != m_data.end()) return m_data[key];
//		else Debug::LogError("NodeGraph::AtMethod : index out of range.");
//	}
//
//};

// 親子関係を管理する
template<class NodeValue>
class TreeNode {

private:

	ulonglong_t m_hierarchy;
	std::string m_nodeName;
	NodeValue m_value;
	TreeNode* m_parent;				// 親は絶対に一つ
	std::unordered_map<std::string, TreeNode*> m_childs;	// 子は親に対して複数かつ

public:

	TreeNode() = default;
	TreeNode(const std::string& nodeName) : m_nodeName(nodeName) {};
	TreeNode(const TreeNode&) = default;
	TreeNode(TreeNode&&) = default;

public:

	TreeNode& operator[](const std::string& childNodeName) {
		auto iter = m_childs.find(childNodeName);
		auto ret = iter != m_childs.end() ? iter->second : nullptr;
		return *ret;
	}

private:

	bool _Internal_AddrEqual(TreeNode* nodeA, TreeNode* nodeB) {
		return nodeA == nodeB;
	}

	void _Internal_SetHierarchy(ulonglong_t hierarchy) {
		m_hierarchy = hierarchy;
	}

	void _Internal2_HierarchySort( TreeNode* parent ) {
		for (auto child : parent->m_childs) {
			child.second->m_hierarchy = parent->m_hierarchy + 1;
			_Internal2_HierarchySort(child.second);
		}
	}

	void _Internal_HierarchySort() {
		TreeNode* top = nullptr;
		if (m_parent != nullptr) {
			TreeNode* preParent = nullptr;
			TreeNode* parent = m_parent;
			while (parent != nullptr) {
				preParent = parent;
				parent = parent->m_parent;
			}
			top = preParent;
		}
		else {
			top = this;
		}
		top->m_hierarchy = 0;
		_Internal2_HierarchySort(top);
	}

public:

	void SetNodeName(const std::string& nodeName) {
		m_nodeName = nodeName;
	}

	void SetValue(const NodeValue& value) {
		m_value = value;
	}

	template<class T>
	void SetParent(T parent) {

		// 引数がポインタ型ではない場合
		static_assert(
			std::is_pointer_v<T>,
			"TreeNode::SetParent Method [error : The argument is not a pointer.]"
		);

		// 引数がTreeNodeクラスではない場合
		static_assert(
			std::is_same_v<std::remove_pointer_t<std::remove_reference_t<T>>, TreeNode>,
			"TreeNode::SetParent Method [error : The argument is not a TreeNode class.]"
		);

		// 設定しようとしたのが自身の子ノードだった場合
		auto iter = m_childs.find(parent->m_nodeName);
		auto child = iter != m_childs.end() ? iter->second : nullptr;
		if (_Internal_AddrEqual(parent, child)) {
			RemoveChild(parent->m_nodeName);
			parent->m_childs.emplace(m_nodeName, this);
		}
		else {
			if (this->HasParent()) {
				parent->m_childs.emplace(m_nodeName, RemoveAndGetChild(m_nodeName));
			}
			else {
				m_parent = parent;
				parent->m_childs.emplace(m_nodeName, this);
			}
		}

		_Internal_HierarchySort();
	}

	template<class T>
	void SetChild(T child) {

		// 引数がポインタ型ではない場合
		static_assert(
			std::is_pointer_v<T>,
			"TreeNode::SetChild Method [error : The argument is not a pointer.]"
			);

		// 引数がTreeNodeクラスではない場合
		static_assert(
			std::is_same_v<std::remove_pointer_t<std::remove_reference_t<T>>, TreeNode>,
			"TreeNode::SetChild Method [error : The argument is not a TreeNode class.]"
			);

		// 自身の現在の親ノードを、自身の子ノードにしようとしている場合
		if (_Internal_AddrEqual(m_parent, child)) {
			m_parent = child->m_parent;
			child->RemoveChild(m_nodeName);
		}
		if (child->HasParent()) {
			child->RemoveParent();
		}
		child->m_parent = this;
		m_childs.emplace(child->m_nodeName, child);

		_Internal_HierarchySort();
	}

	std::string GetNodeName() const {
		return m_nodeName;
	}

	NodeValue& GetValue() {
		return m_value;
	}

	NodeValue GetValueCopy() const {
		return m_value;
	}

	const TreeNode* GetParent() const {
		return m_parent;
	}

	TreeNode* RemoveAndGetParent() {
		auto ret = m_parent;
		m_parent = nullptr;
		return ret;
	}

	void RemoveParent() {
		m_parent = nullptr;
		m_hierarchy = 0;
	}

	const TreeNode* GetChild(const std::string& nodeName) const {
		auto iter = m_childs.find(nodeName);
		return iter != m_childs.end() ? iter->second : nullptr;
	}

	std::vector<const TreeNode*> GetChilds() const {
		std::vector<const TreeNode*> ret;
		ret.reserve(m_childs.size());
		for (auto&& child : m_childs) ret.push_back(child.second);
		return ret;
	}

	TreeNode* RemoveAndGetChild(const std::string& nodeName) {
		auto iter = m_childs.find(nodeName);
		if (iter != m_childs.end()) {
			auto ret = iter->second;
			m_childs.erase(iter);
			ret->m_parent = nullptr;
			ret->m_hierarchy = 0;
			return ret;
		}
		else {
			return nullptr;
		}
	}

	void RemoveChild(const std::string& nodeName) {
		auto iter = m_childs.find(nodeName);
		if (iter != m_childs.end()) {
			iter->second->m_parent = nullptr;
			iter->second->m_hierarchy = 0;
			m_childs.erase(iter);
		}
	}

	std::vector<const TreeNode*> GetAllParents() const {
		const TreeNode* parent = m_parent;
		std::vector<const TreeNode*> ret;
		while (parent != nullptr) {
			ret.emplace_back(parent);
			parent = parent->m_parent;
		}
		return ret;
	}

	std::vector<const TreeNode*> GetAllChilds() const {
		// TODO
	}

	const TreeNode* GetTopNode() const {
		if (m_parent != nullptr) {
			const TreeNode* preParent = nullptr;
			const TreeNode* parent = m_parent;
			while (parent != nullptr) {
				preParent = parent;
				parent = parent->m_parent;
			}
			return preParent;
		}
		else {
			return this;
		}
	}

	std::vector<const TreeNode*> GetSameHierarchyNodes(ulonglong_t hierarchy) {

	}

	std::string GetParentName() const {
		if (m_parent != nullptr) {
			return m_parent->m_nodeName;
		}
		else {
			return std::string();
		}
	}

	std::vector<std::string> GetChildNames() const {
		std::vector<std::string> ret;
		ret.reserve(m_childs.size());
		for (auto&& child : m_childs) ret.push_back(child.first);
		return ret;
	}


	bool HasParent() const {
		return m_parent != nullptr;
	}

	bool HasChild(const std::string& nodeName) const {
		return m_childs.find(nodeName) != m_childs.end();
	}

	ulonglong_t GetHierarchy() {
		return m_hierarchy;
	}

};