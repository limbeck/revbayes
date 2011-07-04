/**
 * @file
 * This file contains the declaration of TransitionProbabilityMatrix, which is
 * class that holds a matrix of transition probabilities in RevBayes.
 *
 * @brief Implementation of TransitionProbabilityMatrix
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-29 23:23:09 +0100 (Tis, 29 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Mcmc
 * @package distributions
 *
 * $Id: Mcmc.h 211 2009-12-29 22:23:09Z ronquist $
 */

#include "Boolean.h"
#include "MatrixReal.h"
#include "MemberFunction.h"
#include "MemberNode.h"
#include "Natural.h"
#include "RbException.h"
#include "RbMathMatrix.h"
#include "RbNames.h"
#include "RbString.h"
#include "RealPos.h"
#include "ReferenceRule.h"
#include "Simplex.h"
#include "StochasticNode.h"
#include "TransitionProbabilityMatrix.h"
#include "ValueRule.h"
#include "VariableNode.h"
#include "VectorIndex.h"
#include "VectorNatural.h"
#include "VectorReal.h"
#include "VectorRealPos.h"
#include "VectorString.h"
#include "Workspace.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>



/** Constructor passes member rules and method inits to base class */
TransitionProbabilityMatrix::TransitionProbabilityMatrix(void) : MemberObject(getMemberRules()) {

    numStates = 2;
    theMatrix = new MatrixReal(numStates, numStates);
}


/** Construct rate matrix with n states */
TransitionProbabilityMatrix::TransitionProbabilityMatrix(size_t n) : MemberObject(getMemberRules()) {

    numStates = n;
    theMatrix = new MatrixReal(numStates, numStates);
}


/** Copy constructor */
TransitionProbabilityMatrix::TransitionProbabilityMatrix(const TransitionProbabilityMatrix& m) {

    numStates = m.numStates;
    theMatrix = new MatrixReal( *m.theMatrix );
}


/** Destructor */
TransitionProbabilityMatrix::~TransitionProbabilityMatrix(void) {
    
    delete theMatrix;
}


/** Index operator (const) */
const VectorReal& TransitionProbabilityMatrix::operator[]( const size_t i ) const {

    if ( i >= numStates )
        throw RbException( "Index to " + TransitionProbabilityMatrix_name + "[][] out of bounds" );
    return (*theMatrix)[i];
}


/** Index operator */
VectorReal& TransitionProbabilityMatrix::operator[]( const size_t i ) {

    if ( i >= numStates )
        throw RbException( "Index to " + TransitionProbabilityMatrix_name + "[][] out of bounds" );
    return (*theMatrix)[i];
}


/** Clone object */
TransitionProbabilityMatrix* TransitionProbabilityMatrix::clone(void) const {

    return new TransitionProbabilityMatrix(*this);
}


/** Map calls to member methods */
DAGNode* TransitionProbabilityMatrix::executeOperation(const std::string& name, ArgumentFrame& args) {

    if (name == "nstates") 
        {
        return ( new Natural((int)numStates) )->wrapIntoVariable();
        }

    return MemberObject::executeOperation( name, args );
}


/** Get class vector describing type of object */
const VectorString& TransitionProbabilityMatrix::getClass(void) const {

    static VectorString rbClass = VectorString(TransitionProbabilityMatrix_name) + MemberObject::getClass();
    return rbClass;
}


/** Get member rules */
const MemberRules& TransitionProbabilityMatrix::getMemberRules(void) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if (!rulesSet) 
        {
        rulesSet = true;
        }

    return memberRules;
}


/** Get methods */
const MethodTable& TransitionProbabilityMatrix::getMethods(void) const {

    static MethodTable   methods;
    static ArgumentRules nstatesArgRules;
    static bool          methodsSet = false;

    if ( methodsSet == false ) 
        {
        // this must be here so the parser can distinguish between different instances of a character matrix
        nstatesArgRules.push_back(         new ReferenceRule( "", MemberObject_name ) );
        
        methods.addFunction("nstates",         new MemberFunction(Natural_name, nstatesArgRules)         );
        
        // necessary call for proper inheritance
        methods.setParentTable( const_cast<MethodTable*>( &MemberObject::getMethods() ) );
        methodsSet = true;
        }

    return methods;
}


/** Print value for user */
void TransitionProbabilityMatrix::printValue(std::ostream& o) const {

    o << "Transition probability matrix:" << std::endl;
    theMatrix->printValue( o );
    o << std::endl;
}


/** Complete info */
std::string TransitionProbabilityMatrix::richInfo(void) const {

	std::ostringstream o;
    printValue( o );
    return o.str();
}


/** Wrap value into a variable */
DAGNode* TransitionProbabilityMatrix::wrapIntoVariable( void ) {
    
    MemberNode* nde = new MemberNode( this );
    return static_cast<DAGNode*>(nde);
}

