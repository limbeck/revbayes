//
//  RateGenerator.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 2/17/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#include "RateGenerator.h"
#include "RbException.h"
#include "RbMathMatrix.h"
#include "TransitionProbabilityMatrix.h"
#include "TypedDagNode.h"


#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateGenerator::RateGenerator(size_t n) :
    num_states( n )
{
    ; // do nothing
}

RateGenerator::~RateGenerator(void)
{
    ; // do nothing
}


RateGenerator& RateGenerator::assign(const Assignable &m)
{
    const RateGenerator *rm = dynamic_cast<const RateGenerator*>(&m);
    if ( rm != NULL )
    {
        return operator=(*rm);
    }
    else
    {
        throw RbException("Could not assign rate matrix.");
    }
}

void RateGenerator::calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const
{
    
    
    calculateTransitionProbabilities(t, 0.0, 1.0, P);
}

size_t RateGenerator::getNumberOfStates( void ) const
{
    return num_states;
}

void RateGenerator::executeMethod(const std::string &n, const std::vector<const DagNode*> &args, RbVector<RbVector<double> >& rv) const
{

    // clear old values
    rv.clear();
    
    TransitionProbabilityMatrix P(num_states);
    
    double rate = static_cast<const TypedDagNode<double> *>( args[0] )->getValue();
    double start_age = static_cast<const TypedDagNode<double> *>( args[1] )->getValue();
    double end_age = static_cast<const TypedDagNode<double> *>( args[2] )->getValue();
    
    calculateTransitionProbabilities( start_age, end_age, rate, P);
    
    for (size_t i = 0; i < num_states; i++)
    {
        RbVector<double> v;
        for (size_t j =0; j < num_states; j++)
        {
            v.push_back(P[i][j]);
        }
        rv.push_back(v);
    }
    
}


void RateGenerator::executeMethod(const std::string &n, const std::vector<const DagNode*> &args, RbVector<double> &rv) const
{
    size_t n_states = this->getNumberOfStates();
//    rv.resize(n_states);
    rv.clear();

    size_t from_idx = static_cast<const TypedDagNode<int> *>( args[0] )->getValue()-1;

    for (size_t to_idx = 0; to_idx < n_states; to_idx++)
    {
        rv.push_back(this->getRate(from_idx, to_idx, 0.0, 1.0));
    }
}


size_t RateGenerator::size( void ) const
{
    return num_states;
}



void RateGenerator::printForUser(std::ostream &o, const std::string &sep, int l, bool left) const
{
    std::streamsize previous_precision = o.precision();
    std::ios_base::fmtflags previous_flags = o.flags();
    
    o << "[ ";
    o << std::fixed;
    o << std::setprecision(4);
    
    // print the RbMatrix with each column of equal width and each column centered on the decimal
    for (size_t i=0; i < size(); i++)
    {
        if (i == 0)
        {
            o << "[ ";
        }
        else
        {
            o << "  ";
        }
        
        for (size_t j = 0; j < size(); ++j)
        {
            if (j != 0)
            {
                o << ", ";
            }
            o << getRate( i, j, 1e-6,1.0);
        }
        o <<  " ]";
        
        if (i == size()-1)
        {
            o << " ]";
        }
        else
        {
            o << " ,\n";
        }
        
    }
    
    o.setf(previous_flags);
    o.precision(previous_precision);
    
}



void RateGenerator::printForSimpleStoring(std::ostream &o, const std::string &sep, int l, bool left) const
{
    
    // print the RbMatrix with each column of equal width and each column centered on the decimal
    for (size_t i=0; i < size(); i++)
    {
        if (i > 0)
        {
            o << sep;
        }
        for (size_t j = 0; j < size(); ++j)
        {
            if (j > 0)
            {
                o << sep;
            }
            o << getRate( i, j, 1e-6, 1.0);
        }
        
    }
    
}



void RateGenerator::printForComplexStoring(std::ostream &o, const std::string &sep, int l, bool left) const
{
    
    o << "[ ";
    
    // print the RbMatrix with each column of equal width and each column centered on the decimal
    for (size_t i=0; i < size(); i++)
    {
        o << "[ ";
        for (size_t j = 0; j < size(); ++j)
        {
            if (j != 0)
            {
                o << ", ";
            }
            o << getRate( i, j, 1e-6, 1.0);
        }
        o <<  " ]";
        
        if (i == size()-1)
        {
            o << " ]";
        }
        else
        {
            o << " ,";
        }
        
    }
    
}
