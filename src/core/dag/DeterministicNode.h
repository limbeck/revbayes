/**
 * @file
 * This file contains the declaration of DeterministicNode, which is derived
 * from DAGNode. DeterministicNode is used for DAG nodes associated with
 * an expression (equation) that determines their value.
 *
 * @brief Declaration of DeterministicNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2010-01-12 21:51:54 +0100 (Tis, 12 Jan 2010) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id: DeterministicNode.h 221 2010-01-12 20:51:54Z Hoehna $
 */

#ifndef DeterministicNode_H
#define DeterministicNode_H

#include "VariableNode.h"

#include <string>
#include <vector>

class RbFunction;

const std::string DeterministicNode_name = "Deterministic Node";

class DeterministicNode : public VariableNode {

public:
                                            DeterministicNode(const std::string& valType);                      //!< Constructor from type
                                            DeterministicNode(RbFunction* func);                                //!< Constructor with function
                                            DeterministicNode(const DeterministicNode& x);                      //!< Copy Constructor
    virtual                                ~DeterministicNode(void);                                            //!< Destructor

    // Utility functions implemented here
    const RbLanguageObject*                 getStoredValue(void) const;                                         //!< Get stored value 
    const RbLanguageObject*                 getValue(void) const;                                               //!< Get value 
    RbLanguageObject*                       getValue(void);                                                     //!< Get value (non-const to return non-const value)
    void                                    printValue(std::ostream& o) const;                                  //!< Print value for user 
    const RbFunction*                       getFunction(void) const;
    RbFunction*                             getFunction(void);

    // Utility functions you have to override
    DeterministicNode*                      clone(void) const;                                                  //!< Clone this node
    const VectorString&                     getClass(void) const;                                               //!< Get class vector
    const TypeSpec&                         getTypeSpec(void) const;                                            //!< Get language type of the object
    void                                    printStruct(std::ostream& o) const;                                 //!< Print struct for user
    std::string                             richInfo(void) const;                                               //!< Complete info about object

    // DAG functions implemented here
    void                                    swapParentNode(RbPtr<DAGNode> oldP, RbPtr<DAGNode> newP);           //!< Swap a parent node

    // DAG function you have to override
    virtual RbPtr<DAGNode>                  cloneDAG(std::map<const DAGNode*, RbPtr<DAGNode> >& newNodes) const;//!< Clone entire graph

protected:

    // Utility function you have to override
    void                                    getAffected(std::set<StochasticNode* >& affected);            //!< Mark and get affected nodes
    void                                    keepMe(void);                                                       //!< Keep value of this and affected nodes
    void                                    restoreMe(void);                                                    //!< Restore value of this nodes
    void                                    touchMe(void);                                                      //!< Tell affected nodes value is reset
    virtual void                            update(void);                                                       //!< Update value and storedValue

    // Member variable
    bool                                    needsUpdate;                                                        //!< True when value after touch but before update; if then updated set to false
    RbPtr<RbFunction>                       function;
    
private:
    static const TypeSpec                   typeSpec;
};

#endif
