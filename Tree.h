///////////////////////////////////////////////////////////////////////////////
//	Author			:	Suresh Kalavala
//	Date			:	Mar 13th 1998
///////////////////////////////////////////////////////////////////////////////

// Revisions:
// 1. Added getSafeSubTreeList() for readonly access (use it safe!)

#ifndef __TREE_H
#define __TREE_H
#include <afxtempl.h>

template<class TYPE>
class MTree {
	public:
			MTree();
			~MTree();
		// attributes
		virtual TYPE   getValue() const ;				// Retrieve node value
		virtual BOOL   getExpanded() const;				// whether this item was expanded
		virtual int	   getSubTreeCount() const ;		// Retrieve # of sub trees

		virtual MTree<TYPE>& getSubTree(int index);		// Retrive sub-tree
		virtual const MTree<TYPE>& getSafeSubTree(int index) const ;		// Retrive sub-tree
		virtual const CList< MTree<TYPE>*,MTree<TYPE>* >& getSafeSubTreeList() const; // Retrieve sub-trees list
		virtual CList< MTree<TYPE>*,MTree<TYPE>* >& getSubTreeList(); // Retrieve sub-trees list


		virtual void setValue(TYPE value);			// Set node value
		virtual void setExpanded(BOOL bFlag) ;		// set if tree is expanded
		virtual void addSubTree( TYPE value );		// Add sub tree
		virtual void addSubTree( MTree<TYPE>* pSubTree ); // Add sub tree
		virtual void removeSubTree( int index );	// Remove sub tree
		virtual void removeAll();					// Remove all sub trees
	protected:
		// operator
		// member variables
		TYPE m_value;
		BOOL m_bIsExpanded;
		CList< MTree<TYPE>*,MTree<TYPE>* > m_list;
};


//   Constructor/Destructor
template<class TYPE>
MTree<TYPE>::MTree()
{
	m_bIsExpanded = FALSE;
}

template<class TYPE>
MTree<TYPE>::~MTree()
{
	// Remvoe all sub-trees
	removeAll();
}

//	Return node value		TYPE MTree<TYPE>::getValue()
//	TYPE [return]			Node value
template<class TYPE>
TYPE MTree<TYPE>::getValue() const
{
	return m_value;
}

//	Retrieve # of sub-trees		int MTree<TYPE>::getSubTreeCount()
//	int [return]			# of sub-trees
template<class TYPE>
int	 MTree<TYPE>::getSubTreeCount() const 
{
	return m_list.GetCount();
}

//	Retrieve sub-tree		MTree<TYPE>& MTree<TYPE>::getSubTree();
//	Return value:
//		MTree<TYPE>&	 [return]			Sub-tree
//	Arguments:
//		int	index		Specify index of sub trees
//	Pre-conditions:
//		0 =< index < getSubTreeCount() 
template<class TYPE>
MTree<TYPE>& MTree<TYPE>::getSubTree(int index)
{
	return (MTree<TYPE>&) getSafeSubTree(index);
}

//	Retrieve sub-tree		const MTree<TYPE>& MTree<TYPE>::getSafeSubTree();
//
//	Return value:
//		MTree<TYPE>&	 [return]			Sub-tree
//
//	Arguments:
//		int	index		Specify index of sub-trees
//
//	Pre-conditions:
//		0 =< index < getSubTreeCount() 
template<class TYPE>
const MTree<TYPE>& MTree<TYPE>::getSafeSubTree(int index) const
{
	// Get position 
	POSITION pos = m_list.FindIndex(index);
	
	return *m_list.GetAt(pos);
}

//	Retrieve sub-tree list		const CList< MTree<TYPE>*,MTree<TYPE>* >& MTree<TYPE>::getSafeSubTreeList() const
//
//	Return value:
//		const CList< MTree<TYPE>*,MTree<TYPE>* >&	 [return]	List of sub-tree
template<class TYPE>
const CList< MTree<TYPE>*,MTree<TYPE>* >& MTree<TYPE>::getSafeSubTreeList() const
{
	return m_list;
}

//	Retrieve sub-tree list		const CList< MTree<TYPE>*,MTree<TYPE>* >& MTree<TYPE>::getSafeSubTreeList() const
//
//	Return value:
//		const CList< MTree<TYPE>*,MTree<TYPE>* >&	 [return]	List of sub-tree
template<class TYPE>
CList< MTree<TYPE>*,MTree<TYPE>* >& MTree<TYPE>::getSubTreeList() 
{
	return m_list;
}

//	Set node-value	void MTree<TYPE>::setValue(TYPE value)
//
//	Arguments:
//		TYPE	value	Node value
//
//	Post-conditions:
//		getValue() == value 
template<class TYPE>
void MTree<TYPE>::setValue(TYPE value)
{
	m_value = value;
}

//	Add sub-tree	void MTree<TYPE>::addSubTree( TYPE value ); 
//
//	Arguments:
//		TYPE	value	Node value of sub-tree which is added
//
//	Post-consitions:
//		Increment getSubTreeCount()
//		Node value of last in sub-tree list equal to 'value'
template<class TYPE>
void MTree<TYPE>::addSubTree( TYPE value )
{
	// Create sub-tree object
	MTree<TYPE>* pSubTree = new MTree<TYPE>;
	
	// Set node vlaue
	pSubTree->setValue( value );
	
	// Add new sub-tree to list
	m_list.AddTail( pSubTree );
}

// Add sub tree	void MTree<TYPE>::addSubTree( MTree<TYPE>* pSubTree )
//
//	Arguments:
//		MTree<TYPE>*	pSubTree		Object of sub-tree for adding
//									    Afterward,'this' handle pSubTree
//
//	Pre-conditions:
//		subTree object should be created as new
//
//	Post-conditions:
//		getSubTree( getSubTreeCount()-1 ) == subTree
template<class TYPE>
void MTree<TYPE>::addSubTree( MTree<TYPE>* pSubTree )
{
	// Add to list of sub-trees
	m_list.AddTail(pSubTree);
}

// Delete sub-tree	void MTree<TYPE>::removeSubTree( int index )
//
//	Arguments:
//		int		index	Index of target for deleting
//
//	How to retirieve error:
//		By CATCH CException
//
//	Pre-conditions:
//		0 <= index < getSubTreeCount()
//
//	Post-conditions:
//		Decrement getSubTreeCount()
//		Delete index'th sub-tee
template<class TYPE>
void MTree<TYPE>::removeSubTree( int index )
{
	// Get position
	POSITION pos = m_list.FindIndex(index);
	
	// Get sub-tree object
	MTree<TYPE>* pSubTree = m_list.GetAt(pos);
	
	// Remove from list
	m_list.RemoveAt(pos);
	
	// Destroy sub-tree object
	delete pSubTree;
}

// Delete all sub-trees	void MTree<TYPE>::removeAll()
//	Post-conditions:
//		getSubTreeCount() == 0
template<class TYPE>
void MTree<TYPE>::removeAll()
{
	// Retrieve # of sub-trees
	int cnt = getSubTreeCount();
	
	// Destroy each sub-trees
	for( int i=0 ; i<cnt ; i++ )
	{
		// Remvoe sub-tree which top-of list
		removeSubTree(0);
	}
}

template<class TYPE>
BOOL  MTree<TYPE>::getExpanded() const
{
	return m_bIsExpanded;
}

template<class TYPE>
void MTree<TYPE>::setExpanded(BOOL bFlag) 
{
	m_bIsExpanded = bFlag;
}

#endif