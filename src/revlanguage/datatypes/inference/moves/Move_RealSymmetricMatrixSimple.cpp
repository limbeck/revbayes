//
//  Move_RealSymmetricMatrixSimple.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-28.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#include "Move_RealSymmetricMatrixSimple.h"


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ContinuousStochasticNode.h"
#include "Natural.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlBoolean.h"
#include "MatrixRealSymmetric.h"
#include "MatrixRealSymmetricSimpleMove.h"
#include "RealSymmetricMatrix.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Move_RealSymmetricMatrixSimple::Move_RealSymmetricMatrixSimple() : Move() {
    
}

/** Clone object */
Move_RealSymmetricMatrixSimple* Move_RealSymmetricMatrixSimple::clone(void) const {
    
	return new Move_RealSymmetricMatrixSimple(*this);
}


void Move_RealSymmetricMatrixSimple::constructInternalObject( void ) {
    // we free the memory first
    delete value;
  
    // now allocate a new wishart simple move
    double l = static_cast<const RealPos &>( lambda->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<RevBayesCore::MatrixRealSymmetric>* tmp = static_cast<const RealSymmetricMatrix &>( mat->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<RevBayesCore::MatrixRealSymmetric > *matrix = static_cast<RevBayesCore::StochasticNode<RevBayesCore::MatrixRealSymmetric > *>( tmp );
    bool t = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    value = new RevBayesCore::MatrixRealSymmetricMove(matrix, l, t, w);
        
}


/** Get class name of object */
const std::string& Move_RealSymmetricMatrixSimple::getClassType(void) {
    
    static std::string revClassType = "Move_VectorSingleElement";
    
	return revClassType;
}

/** Get class type spec describing type of object */
const TypeSpec& Move_RealSymmetricMatrixSimple::getClassTypeSpec(void) {
    
    static TypeSpec revClassTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revClassTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& Move_RealSymmetricMatrixSimple::getMemberRules(void) const {
    
    static MemberRules scalingMoveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        scalingMoveMemberRules.push_back( new ArgumentRule( "x", false, RealSymmetricMatrix::getClassTypeSpec() ) );
        scalingMoveMemberRules.push_back( new ArgumentRule( "lambda", true, RealPos::getClassTypeSpec() , new Real(1.0) ) );
        scalingMoveMemberRules.push_back( new ArgumentRule( "tune"  , true, RlBoolean::getClassTypeSpec(), new RlBoolean( true ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getMemberRules();
        scalingMoveMemberRules.insert( scalingMoveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return scalingMoveMemberRules;
}

/** Get type spec */
const TypeSpec& Move_RealSymmetricMatrixSimple::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Move_RealSymmetricMatrixSimple::printValue(std::ostream &o) const {
    
    o << "Move_RealSymmetricMatrixSimple(";
    if (mat != NULL) {
        o << mat->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Move_RealSymmetricMatrixSimple::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "x" ) {
        mat = var;
    }
    else if ( name == "lambda" ) {
        lambda = var;
    }
    else if ( name == "weight" ) {
        weight = var;
    }
    else if ( name == "tune" ) {
        tune = var;
    }
    else {
        Move::setConstMemberVariable(name, var);
    }
}