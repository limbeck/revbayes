#ifndef Func_quit_H
#define Func_quit_H

#include "Procedure.h"

namespace RevLanguage {
    
    class Func_quit : public Procedure {
        
    public:
        Func_quit( void );
        
        // Basic utility functions
        Func_quit*                                      clone(void) const;                                          //!< Clone object
        static const std::string&                       getClassType(void);                                         //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                     //!< Get class type spec
        std::vector<std::string>                        getFunctionNameAliases(void) const;                         //!< Get aliases for the Rev name of the function
        std::string                                     getFunctionName(void) const;                                //!< Get the primary name of the function in Rev
        const TypeSpec&                                 getTypeSpec(void) const;                                    //!< Get language type of the object
        
        // Func_quit functions
        const ArgumentRules&                            getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&                                 getReturnType(void) const;                                  //!< Get type of return val
        
        RevPtr<RevVariable>                             execute(void);                                              //!< Execute function
        
    protected:
        
        std::vector<std::string>                        getHelpAuthor(void) const;                                  //!< Get the author(s) of this function
        std::vector<std::string>                        getHelpDescription(void) const;                             //!< Get the description for this function
        std::vector<std::string>                        getHelpDetails(void) const;                                 //!< Get the more detailed description of the function
        std::string                                     getHelpExample(void) const;                                 //!< Get an executable and instructive example
        std::vector<RevBayesCore::RbHelpReference>      getHelpReferences(void) const;                              //!< Get some references/citations for this function
        std::vector<std::string>                        getHelpSeeAlso(void) const;                                 //!< Get suggested other functions
        std::string                                     getHelpTitle(void) const;                                   //!< Get the title of this help entry

    };
    
}

#endif

