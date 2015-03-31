#include "MatrixReal.h"
#include "RateMatrix_Tamura92.h"
#include "RbException.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <string>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMatrix_Tamura92::RateMatrix_Tamura92(void) : TimeReversibleRateMatrix( 4 ){
    
    kappa = 1.0;
    
    update();
    
}


/** Destructor */
RateMatrix_Tamura92::~RateMatrix_Tamura92(void) {
    
}



/** Calculate the transition probabilities */
void RateMatrix_Tamura92::calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const {
    
    double t = rate * (startAge - endAge);
    // notation:
    double pi_A = (1-gc)/2;
    double pi_C = gc/2;
    double pi_G = gc/2;
    double pi_T = (1-gc)/2;
    
    // compute auxilliary variables
    double pi_AG = pi_A + pi_G;
    double pi_CT = pi_C + pi_T;
    
    // compute beta
    double beta = 1.0 / (2.0*pi_AG*pi_CT+2.0*kappa*((pi_A*pi_G)+(pi_C*pi_T)));
    
    // calculate the transition probabilities
    
    double xx = - beta * t;
    double aa = exp(xx);
    double bbR = exp( (1.0+pi_AG*(kappa-1.0))* xx);
    double bbY = exp( (1.0+pi_CT*(kappa-1.0))* xx);
    double oneminusa = 1 - aa;
    
    P[0][0] = (pi_A * ( pi_AG+pi_CT*aa ) + pi_G*bbR) / pi_AG;
    P[0][1] = pi_C * oneminusa;
    P[0][2] = (pi_G * ( pi_AG+pi_CT*aa ) - pi_G*bbR) / pi_AG;
    P[0][3] = pi_T * oneminusa;
    
    P[1][0] = pi_A * oneminusa;
    P[1][1] = (pi_C * ( pi_CT+pi_AG*aa ) + pi_T*bbY) / pi_CT;
    P[1][2] = pi_G * oneminusa;
    P[1][3] = (pi_T * ( pi_CT+pi_AG*aa ) - pi_T*bbY) / pi_CT;
    
    P[2][0] = (pi_A * ( pi_AG+pi_CT*aa ) - pi_A*bbR) / pi_AG;
    P[2][1] = P[0][1];
    P[2][2] = (pi_G * ( pi_AG+pi_CT*aa ) + pi_A*bbR) / pi_AG;
    P[2][3] = P[0][3];
    
    P[3][0] = P[1][0];
    P[3][1] = (pi_C * ( pi_CT+pi_AG*aa ) - pi_C*bbY) / pi_CT;
    P[3][2] = P[1][2];
    P[3][3] = (pi_T * ( pi_CT+pi_AG*aa ) + pi_C*bbY) / pi_CT;
    
}


RateMatrix_Tamura92* RateMatrix_Tamura92::clone( void ) const {
    
    return new RateMatrix_Tamura92( *this );
}


void RateMatrix_Tamura92::setKappa( double k ) {
    
    kappa = k;
    
    // set flags
    needsUpdate = true;
    
}


void RateMatrix_Tamura92::setGC(double f)
{
    
    gc = f;
    
    // set flags
    needsUpdate = true;
    
}


void RateMatrix_Tamura92::update( void ) {
    
    if ( needsUpdate )
    {
        MatrixReal &m = *theRateMatrix;
        
        // @todo: This is only needed for printing the values of the rate matrix properly to the screen. We should do this more efficiently (Sebastian).
        // We could instead only update the matrix if a print call happened and the matrix was flagged as dirty.
        
        // compute the off-diagonal values
        m[0][1] = gc/2;
        m[0][2] = kappa*gc/2;
        m[0][3] = (1-gc)/2;
        
        m[1][0] = (1-gc)/2;
        m[1][2] = gc/2;
        m[1][3] = kappa*(1-gc)/2;
        
        m[2][0] = kappa*(1-gc)/2;
        m[2][1] = gc/2;
        m[2][3] = (1-gc)/2;
        
        m[3][0] = (1-gc)/2;
        m[3][1] = kappa*gc/2;
        m[3][2] = gc/2;
        
        // set the diagonal values
        setDiagonal();
        
        // rescale
        rescaleToAverageRate( 1.0 );
        
        // clean flags
        needsUpdate = false;
    }
}


