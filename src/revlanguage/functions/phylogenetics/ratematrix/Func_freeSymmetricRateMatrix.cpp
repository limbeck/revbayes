#include "Func_freeSymmetricRateMatrix.h"
#include "FreeSymmetricRateMatrixFunction.h"
#include "Natural.h"
#include "RateMatrix_JC.h"
#include "Real.h"
#include "RealPos.h"
#include "RlBoolean.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TransitionProbabilityMatrix.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_freeSymmetricRateMatrix::Func_freeSymmetricRateMatrix( void ) : TypedFunction<RateMatrix>( )
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_freeSymmetricRateMatrix* Func_freeSymmetricRateMatrix::clone( void ) const
{
    
    return new Func_freeSymmetricRateMatrix( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RateGenerator >* Func_freeSymmetricRateMatrix::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* tr = static_cast<const ModelVector<RealPos> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    bool r = static_cast<const RlBoolean &>( this->args[1].getVariable()->getRevObject() ).getDagNode()->getValue();
    
    RevBayesCore::FreeSymmetricRateMatrixFunction* f = new RevBayesCore::FreeSymmetricRateMatrixFunction( tr, r );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_freeSymmetricRateMatrix::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
        argumentRules.push_back( new ArgumentRule( "transition_rates",   ModelVector<RealPos>::getClassTypeSpec(),   "The transition rates between states.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "rescaled",          RlBoolean::getClassTypeSpec(),              "Should the matrix be normalized?", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        rules_set = true;
    }
    
    return argumentRules;
}


const std::string& Func_freeSymmetricRateMatrix::getClassType(void)
{
    
    static std::string rev_type = "Func_freeSymmetricRateMatrix";
    
    return rev_type;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_freeSymmetricRateMatrix::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_freeSymmetricRateMatrix::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnFreeSymmetricRateMatrix";
    
    return f_name;
}


const TypeSpec& Func_freeSymmetricRateMatrix::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}
