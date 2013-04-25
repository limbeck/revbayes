/**
 * @file
 * This file contains the Workspace function that adds types and functions
 * to the global workspace, registering them with the interpreter/compiler
 * during the process.
 *
 * @brief Function registering language objects
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @extends Frame
 * @package parser
 * @version 1.0
 * @since version 1.0 2009-09-02
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "RbException.h"
#include "UserInterface.h"
#include "Workspace.h"

/* Primitive types (alphabetic order) */
#include "RlBoolean.h"
#include "Complex.h"
#include "Integer.h"
#include "Natural.h"
#include "Probability.h"
#include "RlString.h"
#include "Real.h"
#include "RealPos.h"

#include "RlAminoAcidState.h"
//#include "RlCharacterData.h"
#include "RlClade.h"
//#include "RlContinuousCharacterState.h"
#include "RlDnaState.h"
#include "RlRnaState.h"
//#include "RlStandardState.h"
//#include "RlTaxonData.h"

/* Container types (alphabetic order) */
//#include "Matrix.h"
//#include "Set.h"
#include "Vector.h"


/* MemberObject types with auto-generated constructors (alphabetic order) */
#include "RlMcmc.h"
#include "RlModel.h"
#include "RlPowerPosterior.h"
//#include "Simulate.h"

/* Distributions with distribution constructors and distribution functions (alphabetic order) */
#include "RlBetaDistribution.h"
//#include "Dist_cat.h"
#include "RlDirichletDistribution.h"
#include "RlExponentialDistribution.h"
#include "RlGammaDistribution.h"
#include "RlGeometricBrownianMotion.h"
#include "RlLognormalDistribution.h"
//#include "Dist_multinomial.h"
#include "RlNormalDistribution.h"
#include "RlOffsetExponentialDistribution.h"
#include "RlUniformDistribution.h"
#include "RlUniformTopologyDistribution.h"

// tree priors
#include "RlConstantBirthDeathProcess.h"
#include "RlTimeDependentBirthDeathProcess.h"

// sequence models
#include "RlCharacterStateEvolutionAlongTree.h"
#include "RlRelaxedClockCharacterStateEvolution.h"

/* Moves */
#include "RlMove.h"
#include "RlScaleMove.h"
#include "RlSimplexMove.h"
#include "RlSimplexSingleElementScale.h"
#include "RlSlidingMove.h"

/* Tree Proposals */
#include "RlFixedNodeheightPruneRegraft.h"
#include "RlNearestNeighborInterchange.h"
#include "RlNearestNeighborInterchange_nonClock.h"
#include "RlNodeTimeSlideUniform.h"
#include "RlRootTimeSlide.h"
#include "RlSubtreeScale.h"
#include "RlTreeScale.h"
#include "RlWeightedNodeTimeSlide.h"


/* Monitors */
#include "RlMonitor.h"
#include "RlFileMonitor.h"
#include "RlExtendedNewickFileMonitor.h"
#include "RlScreenMonitor.h"

/// Parser functions ///

/* Basic internal functions (alphabetic order) */
#include "Func_range.h"

/* Basic arithmetic/logic templated functions */
#include "Func__and.h"
#include "Func__eq.h"
#include "Func__ge.h"
#include "Func__gt.h"
#include "Func__le.h"
#include "Func__lt.h"
#include "Func__ne.h"
#include "Func__or.h"
#include "Func__unot.h"
//#include "Func__uplus.h"

/* Builtin functions */
#include "Func_clear.h"
//#include "Func_convertToSingleElement.h"
//#include "Func_dppConFromExpNumClusters.h"
#include "Func_ls.h"
//#include "Func_normalizeVector.h"
//#include "Func_print.h"
#include "Func_quit.h"
//#include "Func_rep.h"
#include "Func_seed.h"
#include "Func_simplex.h"
//#include "Func_structure.h"
#include "Func_type.h"
//#include "Func_unique.h"
//#include "Func_size.h"
//#include "Func_sort.h"
#include "Func_Source.h"

/* Builtin templated functions */
//#include "Func_set.h"
//#include "Func_transpose.h"
#include "Func_vector.h"
#include "Func_rlvector.h"
//#include "Func_rlvector.h"

/* Constructor functions */
//#include "ConstructorTaxonData.h"

/* Phylogeny functions */
//#include "Func_concatenate.h"
//#include "Func_distance.h"
#include "Func_mapTree.h"
//#include "Func_nj.h"
#include "Func_readCharacterData.h"
#include "Func_readTrace.h"
#include "Func_readTrees.h"
#include "Func_readTreeTrace.h"
#include "Func_writeFasta.h"
#include "RlTmrcaStatistic.h"
#include "RlTreeAssemblyFunction.h"

#include "RlF81RateMatrixFunction.h"
#include "Func_gtr.h"
#include "RlHkyRateMatrixFunction.h"
#include "RlJcRateMatrixFunction.h"


/* Inference functions */
#include "OptimalBurninFunction.h"


/* Distribution functions */
#include "DistributionFunctionCdf.h"
#include "DistributionFunctionPdf.h"
#include "DistributionFunctionQuantile.h"
#include "DistributionFunctionRv.h"


/// Inference Functions ///

/* Basic arithmetic/logic templated functions */
#include "Func_add.h"
#include "Func_div.h"
#include "Func_mult.h"
#include "Func_sub.h"
#include "Func_uminus.h"

/* Math functions */
#include "Func_abs.h"
//#include "Func_cos.h"
#include "Func_exp.h"
#include "Func_ln.h"
#include "Func_log.h"
#include "Func_mean.h"
#include "Func_power.h"
//#include "Func_sin.h"
#include "Func_sqrt.h"


#include <sstream>
#include <vector>
#include <set>
#include <cstdlib>

/** Initialize global workspace */
void RevLanguage::Workspace::initializeGlobalWorkspace(void) {

    try {
        /* Add types: add a dummy variable which we use for type checking, conversion checking and other tasks. */

//        /* Add special abstract types that do not correspond directly to classes */
//        addType( new RbAbstract( TypeSpec(RbVoid_name) ) );
//
//        /* Add abstract types */
//        addType( new RbAbstract( RbObject::getClassTypeSpec() ) );
//        addType( new RbAbstract( RbLanguageObject::getClassTypeSpec() ) );
//        addType( new RbAbstract( RbInternal::getClassTypeSpec() ) );
//        addType( new RbAbstract( MemberObject::getClassTypeSpec() ) );
////        addType( new RbAbstract( Move::getClassTypeSpec() ) );
////        addType( new RbAbstract( Distribution::getClassTypeSpec() ) );
//
//        /* Add primitive types (alphabetic order) */
//        addType( new RlAminoAcidState()                 );
        addType( new RlBoolean()                      );
        addType( new Complex()                        );
//        addType( new RlContinuousCharacterState()       );
        addType( new Integer()                        );
        addType( new Natural()                        );
        addType( new Probability()                    );
////        addType( new RateMatrix()                     );
        addType( new RlString()                       );
        addType( new Real()                           );
        addType( new RealPos()                        );
//        addType( new RlRnaState()                       );
//        addType( new RlStandardState()                  );
////        addType( new TransitionProbabilityMatrix()    );
//        

        /* Add container types (alphabetic order) */
////        addType( new Matrix<Complex>()              );
////        addType( new Matrix<Real>()                 );
////        addTypeWithConstructor( "set",         new Set<Integer>()          );
////        addTypeWithConstructor( "set",         new Set<Natural>()          );
////        addTypeWithConstructor( "set",         new Set<NucleotideState>()  );
////        addTypeWithConstructor( "set",         new Set<Real>()             );
////        addTypeWithConstructor( "set",         new Set<RealPos>()          );
////        addType( RbPtr<MemberObject>( new Vector( RbObject::getClassTypeSpec() ) )  );
        addType( new Vector<RlBoolean>()          );
        addType( new Vector<Integer>()            );
        addType( new Vector<Natural>()            );
        addType( new Vector<Real>()               );
        addType( new Vector<RealPos>()            );
        addType( new Vector<RlString>()           );
//
//        /* Add MemberObject types without auto-generated constructors (alphabetic order) */
//        addType( new Simplex()                      );
//        addType( new RlTopology()                   );
//        addType( new TopologyNode()                 );
//
//        /* Add MemberObject types with auto-generated constructors (alphabetic order) */
        addTypeWithConstructor( "clade",            new Clade() );
        addTypeWithConstructor( "mcmc",             new Mcmc()  );
        addTypeWithConstructor( "model",            new Model() );
        addTypeWithConstructor( "powerPosterior",   new PowerPosterior()  );
//        addTypeWithConstructor( "simulate",      RbPtr<MemberObject>( new Simulate() )         );
//
        
        
        //////////////////
        /* Add monitors */
        //////////////////
        
        /* File monitor */
        addTypeWithConstructor("extNewickmonitor",    new ExtendedNewickFileMonitor());
        
        /* File monitor */
        addTypeWithConstructor("filemonitor",    new FileMonitor());
        
        /* Screen monitor */
        addTypeWithConstructor("screenmonitor",    new ScreenMonitor());
        
        
        
        ///////////////
        /* Add moves */
        ///////////////
        
        
        addTypeWithConstructor("mScale",    new ScaleMove() );
        addTypeWithConstructor("mSimplex",  new SimplexMove() );
        addTypeWithConstructor("mSimplexElementScale",  new SimplexSingleElementScale() );
        addTypeWithConstructor("mSlide",    new SlidingMove() );
        
        /* Tree Proposals */
        addTypeWithConstructor("mFNPR",                 new FixedNodeheightPruneRegraft() );
        addTypeWithConstructor("mNodeTimeSlideUniform", new NodeTimeSlideUniform() );
        addTypeWithConstructor("mRootTimeSlide",        new RootTimeSlide() );
        addTypeWithConstructor("mSubtreeScale",         new SubtreeScale() );
        addTypeWithConstructor("mTreeScale",            new TreeScale() );
        addTypeWithConstructor("mNNI",                  new NearestNeighborInterchange() );
        addTypeWithConstructor("mNNI",                  new NearestNeighborInterchange_nonClock() );

//
//        /* Add phylogenetic types with auto-generated constructors (alphabetic order) */
//
//
        ///////////////////////
        /* Add Distributions */
        ///////////////////////
        
        // Pure statistical distributions
        
        // beta distribution
        addDistribution( "beta", new BetaDistribution() );

        
        // dirichlet distribution
        addDistribution( "dirichlet", new DirichletDistribution() );
        

        // gamma distribution
        addDistribution( "gamma", new GammaDistribution() );
        
        
        // geometric Brownian motion (BM)
        addDistribution( "geomBM", new GeometricBrownianMotion() );
        

//        // logistic distribution
//        addDistribution( "logistic", new LogisticDistribution() );
//        
//        // multinomial distribution
//        addDistribution( "multinomial", new MultinomialDistribution() );
//        

        // exponential distribution
        addDistribution( "exponential", new ExponentialDistribution() );
        
        // exponential distribution
        addDistribution( "offsetExponential", new OffsetExponentialDistribution() );
        
        // normal distribution
        addDistribution( "lnorm", new LognormalDistribution() );
        
        // normal distribution
        addDistribution( "norm", new NormalDistribution() );
        
        // uniform distributin
        addDistribution( "unif", new UniformDistribution() );
        
        
        // Phylogenetic distributions
        
        // constant rate birth-death process distribution
        addDistribution( "cBDP", new ConstantBirthDeathProcess() );
        
        // time-dependent rate birth-death process distribution
        addDistribution( "tdBDP", new TimeDependentBirthDeathProcess() );
        addDistribution( "time-dependentBDP", new TimeDependentBirthDeathProcess() );
        
        // character state evolution model
        addDistribution( "charStateModel", new CharacterStateEvolutionAlongTree<DnaState,TimeTree>() );
        addDistribution( "charStateModel", new CharacterStateEvolutionAlongTree<DnaState,BranchLengthTree>() );
        
        // relaxed (Time) clock-tree
        addDistribution( "charStateModelRelaxedClock", new RelaxedClockCharacterStateEvolution<DnaState,TimeTree>() );
        
        
        // uniform topology distribution
        addDistribution( "uniformTopology", new UniformTopologyDistribution() );
        
        
        
//        
//        /* Now we have added all primitive and complex data types and can start type checking */
        Workspace::globalWorkspace().typesInitialized = true;
        Workspace::userWorkspace().typesInitialized   = true;

        ///////////////////////////////
        // Add parser functions here //
        ///////////////////////////////

        /* Add basic internal functions (alphabetic order) */
        addFunction( "_range",    new Func_range()       );

        
        /* Add basic unary arithmetic templated functions */
//        addFunction( "_uplus",    new Func__uplus <         Integer,        Integer >() );
//        addFunction( "_uplus",    new Func__uplus <            Real,           Real >() );
////        addFunction( "_uplus",    new Func__uplus <    Matrix<Real>,   Matrix<Real> >() );
        
        /* Add basic logical functions */
        addFunction( "_and",      new Func__and()  );
        addFunction( "_unot",     new Func__unot() );
        addFunction( "_or",       new Func__or()   );
        
        /* Add basic logic templated functions */
        addFunction( "_eq",       new Func__eq<             Integer,        Integer >()             );
        addFunction( "_eq",       new Func__eq<                Real,           Real >()             );
        addFunction( "_eq",       new Func__eq<             Integer,           Real >()             );
        addFunction( "_eq",       new Func__eq<                Real,        Integer >()             );
        addFunction( "_eq",       new Func__eq<           RlBoolean,      RlBoolean >()             );
        addFunction( "_eq",       new Func__eq<           RlString,       RlString  >()             );
        addFunction( "_ge",       new Func__ge<             Integer,        Integer >()             );
        addFunction( "_ge",       new Func__ge<                Real,           Real >()             );
        addFunction( "_ge",       new Func__ge<             Integer,           Real >()             );
        addFunction( "_ge",       new Func__ge<                Real,        Integer >()             );
        addFunction( "_ge",       new Func__ge<           RlBoolean,      RlBoolean >()             );
        addFunction( "_gt",       new Func__gt<             Integer,        Integer >()             );
        addFunction( "_gt",       new Func__gt<                Real,           Real >()             );
        addFunction( "_gt",       new Func__gt<           RlBoolean,      RlBoolean >()             );
        addFunction( "_le",       new Func__le<             Integer,        Integer >()             );
        addFunction( "_le",       new Func__le<                Real,           Real >()             );
        addFunction( "_le",       new Func__le<             Integer,           Real >()             );
        addFunction( "_le",       new Func__le<                Real,        Integer >()             );
        addFunction( "_le",       new Func__le<           RlBoolean,      RlBoolean >()             );
        addFunction( "_lt",       new Func__lt<             Integer,        Integer >()             );
        addFunction( "_lt",       new Func__lt<                Real,           Real >()             );
        addFunction( "_lt",       new Func__lt<           RlBoolean,      RlBoolean >()             );
        addFunction( "_ne",       new Func__ne<             Integer,        Integer >()             );
        addFunction( "_ne",       new Func__ne<                Real,           Real >()             );
        addFunction( "_ne",       new Func__ne<             Integer,           Real >()             );
        addFunction( "_ne",       new Func__ne<                Real,        Integer >()             );
        addFunction( "_ne",       new Func__ne<           RlBoolean,      RlBoolean >()             );
        
        
        /* Add builtin functions (alphabetical order) */
        addFunction( "clear",                    new Func_clear()                    );
//        addFunction( "dppConFromExpNumClusters", new Func_dppConFromExpNumClusters() );
        addFunction( "ls",                       new Func_ls()                       );
////        addFunction( "normalize",                new Func_normalizeVector()          );
//        addFunction( "print",                    new Func_print()                    );
        addFunction( "q",                        new Func_quit()                     );
        addFunction( "quit",                     new Func_quit()                     );
//        addFunction( "rep",                      new Func_rep<Natural>()             );
//        addFunction( "rep",                      new Func_rep<Integer>()             );
//        addFunction( "rep",                      new Func_rep<Real>()                );
//        addFunction( "rep",                      new Func_rep<RealPos>()             );
//        addFunction( "rep",                      new Func_rep<Vector<Real> >()     );
        addFunction( "seed",                     new Func_seed()                     );
        addFunction( "simplex",                  new Func_simplex()                  );
////        addFunction( "simplex",                  new Func_simplex<Vector>() );
//        addFunction( "structure",                new Func_structure()                );
        addFunction( "type",                     new Func_type()                     );
////        addFunction( "unique",                   new Func_unique<Vector>()  );
////        addFunction( "size",                     new Func_size<DagNodeContainer>()   );
////        addFunction( "size",                     new Func_size<Vector>()    );
////        addFunction( "sort",                     new Func_sort<Vector>()    );
//        
//
//        
//        
        
        
        /////////////////////////////////////
        // Add distribution functions here //
        /////////////////////////////////////
        
        // normal distribution
        addFunction("dbeta", new DistributionFunctionPdf<RealPos>( new BetaDistribution() ) );
        addFunction("pbeta", new DistributionFunctionCdf( new BetaDistribution() ) );
        addFunction("qbeta", new DistributionFunctionQuantile( new BetaDistribution() ) );
        addFunction("rbeta", new DistributionFunctionRv<RealPos>( new BetaDistribution() ) );
        
        // exponential distribution
        addFunction("dexponential", new DistributionFunctionPdf<RealPos>( new ExponentialDistribution() ) );
        addFunction("pexponential", new DistributionFunctionCdf( new ExponentialDistribution() ) );
        addFunction("qexponential", new DistributionFunctionQuantile( new ExponentialDistribution() ) );
        addFunction("rexponential", new DistributionFunctionRv<RealPos>( new ExponentialDistribution() ) );
        
        // gamma distribution
        addFunction("dgamma", new DistributionFunctionPdf<RealPos>( new GammaDistribution() ) );
        addFunction("pgamma", new DistributionFunctionCdf( new GammaDistribution() ) );
        addFunction("qgamma", new DistributionFunctionQuantile( new GammaDistribution() ) );
        addFunction("rgamma", new DistributionFunctionRv<RealPos>( new GammaDistribution() ) );
        
        // log-normal distribution
        addFunction("dlnorm", new DistributionFunctionPdf<RealPos>( new LognormalDistribution() ) );
        addFunction("plnorm", new DistributionFunctionCdf( new LognormalDistribution() ) );
        addFunction("qlnorm", new DistributionFunctionQuantile( new LognormalDistribution() ) );
        addFunction("rlnorm", new DistributionFunctionRv<RealPos>( new LognormalDistribution() ) );
        
        // normal distribution
        addFunction("dnorm", new DistributionFunctionPdf<Real>( new NormalDistribution() ) );
        addFunction("pnorm", new DistributionFunctionCdf( new NormalDistribution() ) );
        addFunction("qnorm", new DistributionFunctionQuantile( new NormalDistribution() ) );
        addFunction("rnorm", new DistributionFunctionRv<Real>( new NormalDistribution() ) );
        
        // normal distribution
        addFunction("dunif", new DistributionFunctionPdf<Real>( new UniformDistribution() ) );
        addFunction("punif", new DistributionFunctionCdf( new BetaDistribution() ) );
        addFunction("qunif", new DistributionFunctionQuantile( new BetaDistribution() ) );
        addFunction("runif", new DistributionFunctionRv<Real>( new UniformDistribution() ) );
        
        
        //////////////////////////////////
        // Add inference functions here //
        //////////////////////////////////
        

        /* Add basic unary arithmetic and logical templated functions */

        // unary minus ( e.g. -a )
        addFunction( "_uminus",   new Func_uminus<Integer, Integer>() );
        addFunction( "_uminus",   new Func_uminus<Natural, Integer>() );
        addFunction( "_uminus",   new Func_uminus<Real, Real>() );
        addFunction( "_uminus",   new Func_uminus<RealPos, Real>() );

        
        
        /* Add basic arithmetic templated functions */
        
        // addition (e.g. a+b )
        addFunction( "_add",      new Func_add<Natural, Natural, Natural>(  ) );
        addFunction( "_add",      new Func_add<Integer, Integer, Integer>(  ) );
        addFunction( "_add",      new Func_add<Real, Real, Real>(  ) );
        addFunction( "_add",      new Func_add<RealPos, RealPos, RealPos>(  ) );
        
        
        // division
        addFunction( "_div",      new Func_div<Natural, Natural, RealPos>(  ) );
        addFunction( "_div",      new Func_div<Integer, Integer, Real>(  ) );
        addFunction( "_div",      new Func_div<Real, Real, Real>(  ) );
        addFunction( "_div",      new Func_div<RealPos, RealPos, RealPos>(  ) );

        // multiplication
        addFunction( "_mul",      new Func_mult<Natural, Natural, Natural>(  ) );
        addFunction( "_mul",      new Func_mult<Integer, Integer, Integer>(  ) );
        addFunction( "_mul",      new Func_mult<Real, Real, Real>(  ) );
        addFunction( "_mul",      new Func_mult<RealPos, RealPos, RealPos>(  ) );
        
        // subtraction
        addFunction( "_sub",      new Func_sub<Integer, Integer, Integer>(  ) );
        addFunction( "_sub",      new Func_sub<Real, Real, Real>(  ) );


        addFunction( "_exp",      new Func_power() );
//
//        /* Add basic logic templated functions */
///*        ArgumentRules ltNaturalFuncArgRules;
//        ltNaturalFuncArgRules.push_back( new ConstArgumentRule("first", Natural::getClassTypeSpec() ) );
//        ltNaturalFuncArgRules.push_back( new ConstArgumentRule("second", Natural::getClassTypeSpec() ) );
//        RlBoolean* funcLTNaturalRetVar = new RlBoolean();
//        addFunction( "_lt",      new ParserFunction( new Func__lt<int, int, bool>(), "<", ltNaturalFuncArgRules, funcLTNaturalRetVar ) );
//        
//        ArgumentRules ltIntFuncArgRules;
//        ltIntFuncArgRules.push_back( new ConstArgumentRule("first", Integer::getClassTypeSpec() ) );
//        ltIntFuncArgRules.push_back( new ConstArgumentRule("second", Integer::getClassTypeSpec() ) );
//        RlBoolean* funcLTIntRetVar = new RlBoolean();
//        addFunction( "_lt",      new ParserFunction( new Func__lt<int, int, bool>(), "<", ltIntFuncArgRules, funcLTIntRetVar ) );
//        
//        ArgumentRules ltRealPosFuncArgRules;
//        ltRealPosFuncArgRules.push_back( new ConstArgumentRule("first", RealPos::getClassTypeSpec() ) );
//        ltRealPosFuncArgRules.push_back( new ConstArgumentRule("second", RealPos::getClassTypeSpec() ) );
//        RlBoolean* funcLTRealPosRetVar = new RlBoolean();
//        addFunction( "_lt",      new ParserFunction( new Func__lt<double, double, bool>(), "<", ltRealPosFuncArgRules, funcLTRealPosRetVar ) );
//        
//        ArgumentRules ltRealFuncArgRules;
//        ltRealFuncArgRules.push_back( new ConstArgumentRule("first", Real::getClassTypeSpec() ) );
//        ltRealFuncArgRules.push_back( new ConstArgumentRule("second", Real::getClassTypeSpec() ) );
//        RlBoolean* funcLTRealRetVar = new RlBoolean();
//        addFunction( "_lt",      new ParserFunction( new Func__lt<double, double, bool>(), "<", ltRealFuncArgRules, funcLTRealRetVar ) );
//*/		
//        
//
        /* Add math functions (alphabetical order) */
		
		// absolute function
        addFunction( "abs",         new Func_abs()  );
        
        // cos function
//        addFunction( "cos",       new Func_cos()  );
		
        // exponential function
        addFunction( "exp",         new Func_exp() );
        
        // natural log function
        addFunction( "ln",          new Func_ln()  );
        
        // log function
		addFunction( "log",         new Func_log()  );	
        
        // mean function
		addFunction( "mean",        new Func_mean()  );
		
		
//        addFunction( "mean",      new Func_mean()  );
        addFunction( "power",     new Func_power() );


        // sin function
//        addFunction( "sin",       new Func_sin() );

		// square root function
        addFunction( "sqrt",      new Func_sqrt()  );
        
        
        
		// gtr function
        addFunction( "F81",      new F81RateMatrixFunction() );
        
		// JC function
        addFunction( "HKY",      new HkyRateMatrixFunction() );
        
		// gtr function
        addFunction( "gtr",      new Func_gtr() );
        
		// JC function
        addFunction( "JC",       new JcRateMatrixFunction() );
		
        
        /* Add phylogeny-related functions (alphabetical order) */
//        addFunction( "concatenate",                 new Func_concatenate()                 );
//        addFunction( "distances",                   new Func_distance()                    );
        //        addFunction( "nj",                          new Func_nj()                          );
        addFunction( "readTrace",                   new Func_readTrace()                   );
        addFunction( "mapTree",                     new Func_mapTree<BranchLengthTree>()   );
        addFunction( "mapTree",                     new Func_mapTree<TimeTree>()           );
        addFunction( "readCharacterData",           new Func_readCharacterData()           );
        addFunction( "readTrees",                   new Func_readTrees()                   );
        addFunction( "readTreeTrace",               new Func_readTreeTrace()               );
        addFunction( "writeFasta",                  new Func_writeFasta()                  );
        
        addFunction( "tmrca",                       new TmrcaStatistic()                   );
        addFunction( "treeAssembly",                new TreeAssemblyFunction()             );
//        addFunction( "ctmmtp",                      new Func_CtmmTransitionProbabilities() );
//        addFunction( "ctmmTransitionProbabilities", new Func_CtmmTransitionProbabilities() );
//
//        /* Add builtin templated functions */
////        addFunction( "transpose", new Func_transpose<       Matrix<Real>                                                    >() );
//        addFunction( "set",       new Func_set<RlBoolean>() );
//        addFunction( "set",       new Func_set<Integer>() );
//        addFunction( "set",       new Func_set<Natural>() );
//        addFunction( "set",       new Func_set<Real>() );
//        addFunction( "set",       new Func_set<RealPos>() );
////        addFunction( "set",       new Func_set<Complex>() );
//        addFunction( "set",       new Func_set<string>() );
//        addFunction( "set",       new Func_set<RlDnaState>() );
        addFunction( "v",         new Func_rlvector<Monitor>() );
        addFunction( "v",         new Func_rlvector<Move>() );
        addFunction( "v",         new Func_vector<Natural>() );
        addFunction( "v",         new Func_vector<Integer>() );
        addFunction( "v",         new Func_vector<Real>() );
        addFunction( "v",         new Func_vector<RealPos>() );
        addFunction( "v",         new Func_vector<RlBoolean>() );
        addFunction( "v",         new Func_vector<Clade>() );
//        addFunction( "v",         new Func_vector<Vector<Integer> > () );
//        addFunction( "v",         new Func_vector<Vector<Natural> >() );
//        
//        addFunction( "v",         new Func_rlvector<ParserMonitor>() );
//        addFunction( "v",         new Func_rlvector<ParserMove>() );
////        addFunction( "v",         new Func_vector<          Vector<Natural>,              Matrix<Natural>                 >() );
////        addFunction( "v",         new Func_vector<          Vector<Real>,                 Matrix<Real>                    >() );
////        addFunction( "v",         new Func_vector<          Vector<RealPos>,              Matrix<RealPos>                 >() );
////        addFunction( "v",         new Func_vector<          Vector<Complex>,              Matrix<Complex>                 >() );
        
        /////////////////////////////////////////
        // Add RevLanguage only functions here //
        /////////////////////////////////////////
        
        addFunction( "source",         new Func_Source() );
        
        // inference function
//        addFunction( "beca",           new BecaFunction() );
        addFunction( "estimateBurnin", new OptimalBurninFunction() );
    }
    catch(RbException& rbException) {

        RBOUT("Caught an exception while initializing the workspace\n");
        std::ostringstream msg;
        rbException.print(msg);
        msg << std::endl;
        RBOUT(msg.str());

        RBOUT("Please report this bug to the RevBayes Development Core Team");

        RBOUT("Press any character to exit the program.");
        getchar();
        exit(1);
    }
}


