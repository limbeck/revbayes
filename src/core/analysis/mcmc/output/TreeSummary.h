#ifndef TreeSummary_H
#define TreeSummary_H

#include "AncestralStateTrace.h"
#include "Clade.h"
#include "ConditionalClade.h"
#include "NewickConverter.h"
#include "RlUserInterface.h"
#include "Sample.h"
#include "Tree.h"
#include "TraceTree.h"
#include "TreeUtilities.h"

#include <algorithm>
#include <map>
#include <string>

namespace RevBayesCore {

    /*
     * This functor is used as a comparator to identify equivalence classes
     * of unrooted splits, and rooted clades with or without sampled mrcas
     */
    struct CladeComparator
    {
        CladeComparator(bool r = true, const Clade& c = Clade() ) : clade(c), rooted(r) {};

        bool operator()(const Clade& a, const Clade& b) const;
        bool operator()(const Sample<Clade>& s) const;

        Clade clade;
        bool rooted;
    };
    
    struct AnnotationReport
    {
        
        AnnotationReport() :
            ages(true),
            cc_ages(false),
            ccp(true),
            tree_ages(false),
            hpd(0.95),
            map_parameters(false),
            mean(true),
            posterior(true),
            sa(true) {}
        
        bool ages;
        bool cc_ages;
        bool ccp;
        bool tree_ages;
        double hpd;
        bool map_parameters;
        bool mean;
        bool posterior;
        bool sa;
    };

    class TreeSummary : public Cloneable {
        
    public:
        
        TreeSummary(void);
        TreeSummary(const TraceTree &t);
        
        TreeSummary*                                                            clone(void) const;

        Tree*                                                                   ancestralStateTree(const Tree &inputTree, std::vector<AncestralStateTrace> &ancestralstate_traces, int burnin, std::string summary_stat, int site, bool verbose);
        void                                                                    annotateTree(Tree &inputTree, AnnotationReport report, bool verbose );
        double                                                                  cladeProbability(const Clade &c, bool verbose);
        Tree*                                                                   cladoAncestralStateTree(const Tree &inputTree, std::vector<AncestralStateTrace> &ancestralstate_traces, int burnin, std::string summary_stat, int site, bool verbose);
        int                                                                     getBurnin(void) const;
        std::vector<Tree>                                                       getUniqueTrees(double ci=0.95, bool verbose=true);
        int                                                                     getTopologyFrequency(const Tree &t, bool verbose);
        const TraceTree&                                                        getTreeTrace(void) const;
        bool                                                                    isTreeContainedInCredibleInterval(const Tree &t, double size, bool verbose);
        Tree*                                                                   mapTree(AnnotationReport report, bool verbose);
        Tree*                                                                   mccTree(AnnotationReport report, bool verbose);
        Tree*                                                                   mrTree(AnnotationReport report, double cutoff, bool verbose);
        void                                                                    printTreeSummary(std::ostream& o, double ci=0.95, bool verbose=true);
        void                                                                    printCladeSummary(std::ostream& o, double minP=0.05, bool verbose=true);
        void                                                                    setBurnin(int b);
        int                                                                     size(bool post = false) const;

    private:

        void                                                                    enforceNonnegativeBranchLengths(TopologyNode& tree) const;
        Clade                                                                   fillConditionalClades(const TopologyNode &n, std::map<Clade, std::set<Clade, CladeComparator>, CladeComparator> &cc);
        const Sample<Clade>&                                                    findCladeSample(const Clade &n) const;
        TopologyNode*                                                           findParentNode(TopologyNode&, const Clade &, std::vector<TopologyNode*>&, RbBitSet& ) const;
        std::string                                                             getSiteState( const std::string &site_sample, size_t site );
        void                                                                    mapContinuous(Tree &inputTree, const std::string &n, size_t paramIndex, double hpd = 0.95, bool np=true ) const;
        void                                                                    mapDiscrete(Tree &inputTree, const std::string &n, size_t paramIndex, size_t num = 3, bool np=true ) const;
        void                                                                    mapParameters(Tree &inputTree) const;
        void                                                                    summarize(bool verbose);

        size_t                                                                  burnin;
        bool                                                                    clock;
        bool                                                                    rooted;
        bool                                                                    summarized;
        TraceTree                                                               trace;
        bool                                                                    use_tree_trace;

        std::vector<Sample<Clade> >                                             cladeSamples;
        std::map<Taxon, Sample<Taxon> >                                         sampledAncestorSamples;
        std::vector<Sample<std::string> >                                       treeSamples;

        std::map<Clade, std::vector<double>, CladeComparator >                                    cladeAges;
        std::map<Clade, std::map<Clade, std::vector<double>, CladeComparator >, CladeComparator > conditionalCladeAges;
        std::map<std::string, std::map<Clade, std::vector<double>, CladeComparator > >            treeCladeAges;
    };
    

} //end namespace RevBayesCore


#endif
