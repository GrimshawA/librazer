Building process
==================

## Overview:
Source -> TokenStream -> AST -> Validated AST -> IR -> Code

1. Lexical parsing of the module files into token streams

2. Parsing base information
	Go through each token stream and get initial information so the real
	parse can have intra modular mutual dependency.

3. Parsing the module
	Builds a proper final AST that corresponds to the module's source code closely

4. Semantic analysis
	Inspect, patch and improve the AST.
	By the end of this process, all errors should be detected.

5. IR generation
	Go through the final AST and generate IR from it.
	Does not do any checking, this step assumes all the AST makes sense and is properly
	linked together. Its supposed to be a thin generator of a representation friendly to codegen and optimization. It doesn't have any core logic of the frontend because other
	backends could be used at any time.

6. CodeGen
	Take the IR and generate final code out of it.
