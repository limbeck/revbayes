/**
 * @file
 * This file contains the declaration of Dist_multinomial, which is used to hold
 * parameters and functions related to a multinomial distribution.
 *
 * @brief Declaration of Dist_multinomial
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-14 12:43:32 +0100 (Mån, 14 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id: MemberObject.h 194 2009-12-14 11:43:32Z ronquist $
 */

#ifndef Dist_multinomial_H
#define Dist_multinomial_H

#include "DistributionContinuous.h"
#include "VectorNatural.h"

#include <ostream>
#include <string>

class DAGNode;
class Real;
class StochasticNode;
class VectorString;

const std::string Dist_multinomial_name = "Dist_multinomial";

class Dist_multinomial: public DistributionContinuous {

    public:
                                    Dist_multinomial(void);                                             //!< constructor

        // Basic utility functions
        Dist_multinomial*           clone(void) const;                                                  //!< Clone object
        const VectorString&         getClass(void) const;                                               //!< Get class vector
        const TypeSpec&             getTypeSpec(void) const;                                            //!< Get language type of the object

        // Member variable setup
        RbPtr<const MemberRules>    getMemberRules(void) const;                                         //!< Get member variable rules

        // Multinomial distribution functions
        double                      cdf(RbPtr<const RbLanguageObject> value);                           //!< Cumulative density
        const TypeSpec&             getVariableType(void) const;                                        //!< Get random variable type (VectorNatural)
        double                      lnPdf(RbPtr<const RbLanguageObject> value) const;                   //!< Ln probability density
        double                      pdf(RbPtr<const RbLanguageObject> value) const;                     //!< Probability density
        RbPtr<Real>                 quantile(const double p);                                           //!< Quantile
        RbPtr<RbLanguageObject>     rv(void);                                                           //!< Generate random variable
    
    private:
        static const TypeSpec       typeSpec;
        static const TypeSpec       varTypeSpec;
};

#endif

