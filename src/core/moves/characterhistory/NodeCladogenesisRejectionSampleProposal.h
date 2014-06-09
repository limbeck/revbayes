//
//  NodeCladogenesisRejectionSampleProposal.h
//  rb_mlandis
//
//  Created by Michael Landis on 5/7/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__NodeCladogenesisRejectionSampleProposal__
#define __rb_mlandis__NodeCladogenesisRejectionSampleProposal__

#include "BiogeographicTreeHistoryCtmc.h"
#include "BranchHistory.h"
#include "DeterministicNode.h"
#include "DiscreteCharacterData.h"
#include "DistributionBinomial.h"
#include "DistributionPoisson.h"
#include "PathRejectionSampleProposal.h"
#include "Proposal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RateMap.h"
#include "RbException.h"
#include "StochasticNode.h"
//#include "TransitionProbability.h"
#include "TopologyNode.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>
#include <set>
#include <string>

namespace RevBayesCore {
    
    /**
     * The scaling operator.
     *
     * A scaling proposal draws a random uniform number u ~ unif(-0.5,0.5)
     * and scales the current vale by a scaling factor
     * sf = exp( lambda * u )
     * where lambda is the tuning parameter of the Proposal to influence the size of the proposals.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2009-09-08, version 1.0
     *
     */
    
    template<class charType, class treeType>
    class NodeCladogenesisRejectionSampleProposal : public Proposal {
        
    public:
        NodeCladogenesisRejectionSampleProposal( StochasticNode<AbstractCharacterData> *n, StochasticNode<treeType>* t, DeterministicNode<RateMap> *q, double l, TopologyNode* nd=NULL );                                                                //!<  constructor
        
        // Basic utility functions
        void                            assignNode(TopologyNode* nd);
        void                            assignSiteIndexSet(const std::set<size_t>& s);
        NodeCladogenesisRejectionSampleProposal*    clone(void) const;                                                                  //!< Clone object
        void                            cleanProposal(void);
        double                          doProposal(void);                                                                   //!< Perform proposal
        const std::vector<DagNode*>&    getNodes(void) const;                                                               //!< Get the vector of DAG nodes this proposal is working on
        const std::string&              getProposalName(void) const;                                                        //!< Get the name of the proposal for summary printing
        void                            printParameterSummary(std::ostream &o) const;                                       //!< Print the parameter summary
        void                            prepareProposal(void);                                                              //!< Prepare the proposal
        double                          sampleNodeCharacters(const std::set<size_t>& indexSet);   //!< Sample the characters at the node
        //        void                            sampleNodeCharacters2(const TopologyNode& node, const std::set<size_t>& indexSet);   //!< Sample the characters at the node
        double                          sampleRootCharacters(const std::set<size_t>& indexSet);
        void                            swapNode(DagNode *oldN, DagNode *newN);                                             //!< Swap the DAG nodes on which the Proposal is working on
        void                            tune(double r);                                                                     //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                            undoProposal(void);                                                                 //!< Reject the proposal
        
    protected:
        
        // parameters
        StochasticNode<AbstractCharacterData>*  ctmc;
        StochasticNode<treeType>*               tau;
        DeterministicNode<RateMap>*             qmap;
        std::vector<DagNode*>                   nodes;
        
        // dimensions
        size_t                                  numNodes;
        size_t                                  numCharacters;
        size_t                                  numStates;
        
        // proposal
        std::vector<unsigned>                   storedNodeState;
        std::vector<unsigned>                   storedBudState;
        std::vector<unsigned>                   storedTrunkState;
        std::vector<unsigned>                   storedRootState;
        int                                     monitorIndex;
        std::set<size_t>                        siteIndexSet;
        double                                  storedLnProb;
        double                                  proposedLnProb;
        
        int                                     storedCladogenesisState;
        TopologyNode*                           node;
        TopologyNode*                           storedBudNode;
        TopologyNode*                           storedTrunkNode;
        TopologyNode*                           proposedBudNode;
        TopologyNode*                           proposedTrunkNode;

        
        PathRejectionSampleProposal<charType,treeType>* nodeProposal;
        PathRejectionSampleProposal<charType,treeType>* leftProposal;
        PathRejectionSampleProposal<charType,treeType>* rightProposal;
        
        TransitionProbabilityMatrix nodeTpMatrix;
        TransitionProbabilityMatrix trunkTpMatrix;
        TransitionProbabilityMatrix budTpMatrix;
        
        double                                  lambda;
        
        // flags
        bool                                    fixNodeIndex;
        bool                                    sampleNodeIndex;
        bool                                    sampleSiteIndexSet;
        bool                                    swapBudTrunk;
        bool                                    failed;
        
    };
    
}



/**
 * Constructor
 *
 * Here we simply allocate and initialize the Proposal object.
 */
template<class charType, class treeType>
RevBayesCore::NodeCladogenesisRejectionSampleProposal<charType, treeType>::NodeCladogenesisRejectionSampleProposal( StochasticNode<AbstractCharacterData> *n, StochasticNode<treeType> *t, DeterministicNode<RateMap>* q, double l, TopologyNode* nd) : Proposal(),
ctmc(n),
tau(t),
qmap(q),
numNodes(t->getValue().getNumberOfNodes()),
numCharacters(n->getValue().getNumberOfCharacters()),
numStates(static_cast<const DiscreteCharacterState&>(n->getValue().getCharacter(0,0)).getNumberOfStates()),
node(nd),
nodeTpMatrix(2),
trunkTpMatrix(2),
budTpMatrix(2),
lambda(l),
sampleNodeIndex(true),
sampleSiteIndexSet(true)

{
    
    storedCladogenesisState = 0;
    storedBudNode = NULL;
    storedTrunkNode = NULL;
    proposedTrunkNode = NULL;
    proposedBudNode = NULL;
    swapBudTrunk = false;
    failed = false;
    
    nodes.push_back(ctmc);
    nodes.push_back(tau);
    nodes.push_back(qmap);
    
    nodeProposal  = new PathRejectionSampleProposal<charType,treeType>(n,t,q,l,nd);
    leftProposal  = new PathRejectionSampleProposal<charType,treeType>(n,t,q,l,nd);
    rightProposal = new PathRejectionSampleProposal<charType,treeType>(n,t,q,l,nd);
    
    storedNodeState.resize(numCharacters,0);
    storedBudState.resize(numCharacters,0);
    storedTrunkState.resize(numCharacters,0);
    storedRootState.resize(numCharacters,0);
    
    fixNodeIndex = (node != NULL);
}


template<class charType, class treeType>
void RevBayesCore::NodeCladogenesisRejectionSampleProposal<charType, treeType>::cleanProposal( void )
{
    nodeProposal->cleanProposal();
    rightProposal->cleanProposal();
    leftProposal->cleanProposal();
    
//    std::cout << "ACCEPT\n";
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of the proposal.
 */
template<class charType, class treeType>
RevBayesCore::NodeCladogenesisRejectionSampleProposal<charType, treeType>* RevBayesCore::NodeCladogenesisRejectionSampleProposal<charType, treeType>::clone( void ) const
{
    return new NodeCladogenesisRejectionSampleProposal( *this );
}

template<class charType, class treeType>
void RevBayesCore::NodeCladogenesisRejectionSampleProposal<charType, treeType>::assignNode(TopologyNode* nd)
{
    node = nd;
    sampleNodeIndex = false;
}

template<class charType, class treeType>
void RevBayesCore::NodeCladogenesisRejectionSampleProposal<charType, treeType>::assignSiteIndexSet(const std::set<size_t>& s)
{
    siteIndexSet = s;
    sampleSiteIndexSet = false;
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
template<class charType, class treeType>
const std::string& RevBayesCore::NodeCladogenesisRejectionSampleProposal<charType, treeType>::getProposalName( void ) const
{
    static std::string name = "NodeCladogenesisRejectionSampleProposal";
    
    return name;
}


/**
 * Get the vector of nodes on which this proposal is working on.
 *
 * \return  Const reference to a vector of nodes pointer on which the proposal operates.
 */
template<class charType, class treeType>
const std::vector<RevBayesCore::DagNode*>& RevBayesCore::NodeCladogenesisRejectionSampleProposal<charType, treeType>::getNodes( void ) const
{
    
    return nodes;
}


/**
 * Perform the Proposal.
 *
 * A scaling Proposal draws a random uniform number u ~ unif(-0.5,0.5)
 * and scales the current vale by a scaling factor
 * sf = exp( lambda * u )
 * where lambda is the tuning parameter of the Proposal to influence the size of the proposals.
 *
 * \return The hastings ratio.
 */
template<class charType, class treeType>
double RevBayesCore::NodeCladogenesisRejectionSampleProposal<charType, treeType>::doProposal( void )
{
    proposedLnProb = 0.0;
    failed = false;
    
//    return RbConstants::Double::neginf;
    
    double proposedLnProbRatio = 0.0;
    
    // update node state
    proposedLnProb += sampleNodeCharacters(siteIndexSet);
    if (failed)
        return RbConstants::Double::neginf;
    
    // update 3x incident paths
    proposedLnProbRatio += nodeProposal->doProposal();
    proposedLnProbRatio += leftProposal->doProposal();
    proposedLnProbRatio += rightProposal->doProposal();

    BiogeographicTreeHistoryCtmc<charType, treeType>* p = static_cast< BiogeographicTreeHistoryCtmc<charType, treeType>* >(&ctmc->getDistribution());
//    std::cout << "after\n";
//    p->getHistory(*node).print();
//    p->getHistory(*storedTrunkNode).print();
//    p->getHistory(*storedBudNode).print();
    
    return proposedLnProbRatio;
}


/**
 *
 */
template<class charType, class treeType>
void RevBayesCore::NodeCladogenesisRejectionSampleProposal<charType, treeType>::prepareProposal( void )
{
    BiogeographicTreeHistoryCtmc<charType, treeType>* p = static_cast< BiogeographicTreeHistoryCtmc<charType, treeType>* >(&ctmc->getDistribution());
    
    storedLnProb = 0.0;
    proposedLnProb = 0.0;
    
    const treeType& tree = tau->getValue();
    if (sampleNodeIndex && !fixNodeIndex)
    {
        node = NULL;
        std::vector<TopologyNode*> nds = tree.getNodes();
        while (node == NULL || node->isTip()) {
            size_t idx = GLOBAL_RNG->uniform01() * nds.size(); //numTips + GLOBAL_RNG->uniform01() * (numNodes-numTips);
            node = nds[idx];
        };
    }
    sampleNodeIndex = true;
    
    // do bud/trunk nodes swap
    const std::vector<int>& buddingState = p->getBuddingStates();
    if ( buddingState[ node->getChild(0).getIndex() ] == 1 )
    {
        storedBudNode = &node->getChild(0);
        storedTrunkNode = &node->getChild(1);
    }
    else
    {
        storedBudNode = &node->getChild(1);
        storedTrunkNode = &node->getChild(0);
    }
    swapBudTrunk = GLOBAL_RNG->uniform01() < 0.5;
    if (swapBudTrunk)
    {
        proposedBudNode = storedTrunkNode;
        proposedTrunkNode = storedBudNode;
    }
    else
    {
        proposedBudNode = storedBudNode;
        proposedTrunkNode = storedTrunkNode;
    }
    p->setBuddingState(*proposedTrunkNode, 0);
    p->setBuddingState(*proposedBudNode, 1);
    
    
    
    if (sampleSiteIndexSet)
    {
        siteIndexSet.clear();
        siteIndexSet.insert(GLOBAL_RNG->uniform01() * numCharacters); // at least one is inserted
        for (size_t i = 0; i < numCharacters; i++)
        {
            // just resample all states for now, try something clever later
//            if (GLOBAL_RNG->uniform01() < lambda)
            {
                siteIndexSet.insert(i);
            }
        }
    }
    sampleSiteIndexSet = true;
    
    // prepare the path proposals
    nodeProposal->assignNode(node);
    nodeProposal->assignSiteIndexSet(siteIndexSet);
    nodeProposal->prepareProposal();
    
    leftProposal->assignNode(&node->getChild(0));
    leftProposal->assignSiteIndexSet(siteIndexSet);
    leftProposal->prepareProposal();
    
    rightProposal->assignNode(&node->getChild(1));
    rightProposal->assignSiteIndexSet(siteIndexSet);
    rightProposal->prepareProposal();

    
    // store node state values
    storedNodeState.clear();
    storedRootState.clear();
    storedBudState.clear();
    storedTrunkState.clear();
    
    storedNodeState.resize(numCharacters,0);
    storedBudState.resize(numCharacters,0);
    storedTrunkState.resize(numCharacters,0);
    const std::vector<CharacterEvent*>& nodeState  = p->getHistory(*node).getChildCharacters();
    const std::vector<CharacterEvent*>& budState   = p->getHistory(*storedBudNode).getParentCharacters();
    const std::vector<CharacterEvent*>& trunkState = p->getHistory(*storedTrunkNode).getParentCharacters();
    for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
    {
        storedNodeState[*it]    = nodeState[*it]->getState();
        storedBudState[*it]     = budState[*it]->getState();
        storedTrunkState[*it]   = trunkState[*it]->getState();
    }
    
    
    if (node->isRoot())
    {
        storedRootState.resize(numCharacters,0);
        const std::vector<CharacterEvent*>& rootState = p->getHistory(*node).getParentCharacters();
        for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
        {
            unsigned s = rootState[*it]->getState();
            storedRootState[*it] = s;
        }
    }
    
//    std::cout << "before\n";
//    p->getHistory(*node).print();
//    p->getHistory(*storedTrunkNode).print();
//    p->getHistory(*storedBudNode).print();
}


/**
 * Print the summary of the Proposal.
 *
 * The summary just contains the current value of the tuning parameter.
 * It is printed to the stream that it passed in.
 *
 * \param[in]     o     The stream to which we print the summary.
 */
template<class charType, class treeType>
void RevBayesCore::NodeCladogenesisRejectionSampleProposal<charType, treeType>::printParameterSummary(std::ostream &o) const
{
    o << "lambda = " << lambda;
}

template<class charType, class treeType>
double RevBayesCore::NodeCladogenesisRejectionSampleProposal<charType, treeType>::sampleNodeCharacters(const std::set<size_t>& indexSet)
{
    
    if (!node->isTip())
    {
        
        BiogeographicTreeHistoryCtmc<charType, treeType>* p = static_cast< BiogeographicTreeHistoryCtmc<charType, treeType>* >(&ctmc->getDistribution());
        const std::vector<BranchHistory*>& histories = p->getHistories();
//        const std::vector<int>& cladogenicStates = p->getCladogenicStates();
//        const std::vector<int>& buddingStates = p->getBuddingStates();
        
        // get transition probs
        const RateMap& rm = qmap->getValue();
        rm.calculateTransitionProbabilities(*node, nodeTpMatrix);
        rm.calculateTransitionProbabilities(*proposedTrunkNode, trunkTpMatrix);
        rm.calculateTransitionProbabilities(*proposedBudNode, budTpMatrix);
        
        // states for conditional sampling probs
        const std::vector<CharacterEvent*>& nodeParentState  = histories[ node->getIndex()            ]->getParentCharacters();
        const std::vector<CharacterEvent*>& trunkChildState  = histories[ storedTrunkNode->getIndex() ]->getChildCharacters();
        const std::vector<CharacterEvent*>& budChildState    = histories[ storedBudNode->getIndex()   ]->getChildCharacters();
        
        // states to update
        std::vector<CharacterEvent*> nodeChildState    = histories[ node->getIndex() ]->getChildCharacters();
        std::vector<CharacterEvent*> trunkParentState  = histories[ storedTrunkNode->getIndex() ]->getParentCharacters();
        std::vector<CharacterEvent*> budParentState    = histories[ storedBudNode->getIndex()   ]->getParentCharacters();
        
//        std::cout << "node_ch  ";
//        for (size_t i = 0; i < numCharacters; i++)
//            std::cout << nodeChildState[i]->getState();
//        std::cout << "\n";
//        
//        std::cout << "trunk_pa ";
//        for (size_t i = 0; i < numCharacters; i++)
//            std::cout << trunkParentState[i]->getState();
//        std::cout << "\n";
//        
//        std::cout << "bud_pa   ";
//        for (size_t i = 0; i < numCharacters; i++)
//            std::cout << budParentState[i]->getState();
//        std::cout << "\n\n";
        
        // sample node/trunk states
        
        
        // need these factors for f(X_T, X_B | X_N)
        // but they depend on entire cfg??? maybe take num_on ~= .5 * ( X_T^ch + pa(X_A)^ch )?
        
        int n1 = 0;
        for (size_t i = 0; i < nodeChildState.size(); i++)
            if (nodeChildState[i]->getState() == 1)
                n1++;
        
        
        double onIdxSum = 0.0;
        std::vector<size_t> onIdx(budChildState.size(), 0.0);
        
        for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
        {
            unsigned int ancS = nodeParentState[*it]->getState();
            unsigned int desS1 = trunkChildState[*it]->getState();
            unsigned int desS2 = budChildState[*it]->getState();
            
            double u = GLOBAL_RNG->uniform01();
            
            // integrate over possible peripatry outcomes for TB: 00, 10, 11 (01 possible by allopatry/jump)
            double b1 = 1.0 / n1;
            double b0 = 1.0 - b1;

            double tb00 = trunkTpMatrix[0][desS1] * budTpMatrix[0][desS2] * b0; // N=0,T=0,B=0
            double tb01 = 0.0;                                                  // N=0,T=0,B=1 (forbidden)
            double tb10 = trunkTpMatrix[1][desS1] * budTpMatrix[0][desS2] * b0; // N=1,T=1,B=0
            double tb11 = trunkTpMatrix[1][desS1] * budTpMatrix[1][desS2] * b1; // N=1,T=1,B=1
            
            double g0 = nodeTpMatrix[ancS][0] * (tb00 + tb01);
            double g1 = nodeTpMatrix[ancS][1] * (tb10 + tb11);
            
//            std::cout << b0 << " " << b1 << "\n";
//            std::cout << tb00 << " " << tb01 << " " << tb10 << " " << tb11 << "\n";
//            std::cout << g0 << " " << g1 << "\n\n";
            
//            double g0 = nodeTpMatrix[ancS][0] * trunkTpMatrix[0][desS1] * budTpMatrix[0][desS2];
//            double g1 = nodeTpMatrix[ancS][1] * trunkTpMatrix[1][desS1] * budTpMatrix[1][desS2];
            
            unsigned int s = 0;
            
            // need sample three things...
            if (u < (g1 / (g0 + g1)))
            {
                s = 1;
                n1++;
                onIdx[*it] = g1;
                onIdxSum += g1;
            }
            else
            {
                n1--;
            }
            
            nodeChildState[*it]->setState(s);
            trunkParentState[*it]->setState(s);
            budParentState[*it]->setState(0); // bud has all 0s
        }
    
        // sample X_B ...
        if (onIdx.size() > 0)
        {
            double u = onIdxSum * GLOBAL_RNG->uniform01();
            for (size_t i = 0; i < onIdx.size(); i++)
            {
                u -= onIdx[i];
                if (u < 0.0)
                {
                    budParentState[i]->setState(1);
                    break;
                }
            }
            return 0.0;
            
            
//            std::cout << "node_ch  ";
//            for (size_t i = 0; i < numCharacters; i++)
//                std::cout << nodeChildState[i]->getState();
//            std::cout << "\n";
//            
//            std::cout << "trunk_pa ";
//            for (size_t i = 0; i < numCharacters; i++)
//                std::cout << trunkParentState[i]->getState();
//            std::cout << "\n";
//            
//            std::cout << "bud_pa   ";
//            for (size_t i = 0; i < numCharacters; i++)
//                std::cout << budParentState[i]->getState();
//            std::cout << "\n----";
//            
//            std::cout << "\n";
        }
        else
        {
            // causes proposal to return neginf
            failed = true;
            return RbConstants::Double::neginf;
         
        }
    }
}

template<class charType, class treeType>
double RevBayesCore::NodeCladogenesisRejectionSampleProposal<charType, treeType>::sampleRootCharacters(const std::set<size_t>& indexSet)
{
    double lnP = 0.0;
    if (!node->isRoot())
        return 0.0;
    
    
    AbstractTreeHistoryCtmc<charType, treeType>* p = static_cast< AbstractTreeHistoryCtmc<charType, treeType>* >(&ctmc->getDistribution());
    BranchHistory* bh = &p->getHistory(*node);
    std::vector<CharacterEvent*> parentState = bh->getParentCharacters();
    
    double r0 = qmap->getValue().getSiteRate(*node,1,0);
    double r1 = qmap->getValue().getSiteRate(*node,0,1);
    unsigned n1_old = 0;
    unsigned n1_new = 0;
    
    double p1 = r1 / (r0 + r1);
    for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
    {
        unsigned s = 0;
        double u = GLOBAL_RNG->uniform01();
        if (u < p1)
        {
            s = 1;
            n1_new++;
        }
        if (parentState[*it]->getState() == 1)
        {
            n1_old++;
        }
        
        parentState[*it]->setState(s); //new CharacterEvent(*it,s,0.0);
    }
    //bh->setParentCharacters(parentState);
    
    size_t n = siteIndexSet.size();
    size_t n0_old = n - n1_old;
    size_t n0_new = n - n1_new;
    double p0 = 1.0 - p1;
    
    lnP = n1_old * log(p1) + n0_old * log(p0) - n1_new * log(p1) - n0_new * log(p0);
    return 0.0;
    return lnP;
}


/**
 * Reject the Proposal.
 *
 * Since the Proposal stores the previous value and it is the only place
 * where complex undo operations are known/implement, we need to revert
 * the value of the ctmc/DAG-node to its original value.
 */
template<class charType, class treeType>
void RevBayesCore::NodeCladogenesisRejectionSampleProposal<charType, treeType>::undoProposal( void )
{
    BiogeographicTreeHistoryCtmc<charType, treeType>* p = static_cast< BiogeographicTreeHistoryCtmc<charType, treeType>* >(&ctmc->getDistribution());
    const std::vector<BranchHistory*>& histories = p->getHistories();
    
    // restore path state
    nodeProposal->undoProposal();
    rightProposal->undoProposal();
    leftProposal->undoProposal();
    
    // restore node state
    std::vector<CharacterEvent*> nodeChildState   = histories[node->getIndex()]->getChildCharacters();
    std::vector<CharacterEvent*> budParentState   = histories[storedBudNode->getIndex()]->getParentCharacters();
    std::vector<CharacterEvent*> trunkParentState = histories[storedTrunkNode->getIndex()]->getParentCharacters();
    
    for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
    {
        nodeChildState[*it]->setState(storedNodeState[*it]);
        budParentState[*it]->setState(storedBudState[*it]);
        trunkParentState[*it]->setState(storedTrunkState[*it]);
    }
    
    // restore root state
    if (node->isRoot())
    {
        std::vector<CharacterEvent*> rootState = histories[node->getIndex()]->getParentCharacters();
        for (std::set<size_t>::iterator it = siteIndexSet.begin(); it != siteIndexSet.end(); it++)
        {
            unsigned s = storedRootState[*it];
            rootState[*it]->setState(s);
        }
        
    }
    
    p->setBuddingState(*storedTrunkNode, 0);
    p->setBuddingState(*storedBudNode, 1);

//    std::cout << "REJECT\n";
}


/**
 * Swap the current ctmc for a new one.
 *
 * \param[in]     oldN     The old ctmc that needs to be replaced.
 * \param[in]     newN     The new ctmc.
 */
template<class charType, class treeType>
void RevBayesCore::NodeCladogenesisRejectionSampleProposal<charType, treeType>::swapNode(DagNode *oldN, DagNode *newN)
{
    
    if (oldN == ctmc)
    {
        ctmc = static_cast<StochasticNode<AbstractCharacterData>* >(newN) ;
    }
    else if (oldN == tau)
    {
        tau = static_cast<StochasticNode<treeType>* >(newN);
    }
    else if (oldN == qmap)
    {
        qmap = static_cast<DeterministicNode<RateMap>* >(newN);
    }
    
    nodeProposal->swapNode(oldN, newN);
    leftProposal->swapNode(oldN, newN);
    rightProposal->swapNode(oldN, newN);
}


/**
 * Tune the Proposal to accept the desired acceptance ratio.
 */
template<class charType, class treeType>
void RevBayesCore::NodeCladogenesisRejectionSampleProposal<charType, treeType>::tune( double rate )
{
    ; // do nothing
}

#endif /* defined(__rb_mlandis__NodeCladogenesisRejectionSampleProposal__) */
