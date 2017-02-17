librazer - SDK for the Razer programming language
=================================================

Razer is a new programming language that started as an attempt to create an expressive
scripting language, particularly oriented towards game development. 

It has since then grown into a rather general purpose programming language which tries
to be innovative in terms of language design and get out of the developer's way in terms
of ease of setup, build and deploy.

1. RazerVM

Through this library, the primary environment for the language, it will be executing inside a 
virtual machine called RazerVM. RazerVM is the official implementation of the language runtime
and will evolve over time. Other competing runtimes might emerge in the future, both official and 
non official, but this will remain the primary environment for running programs.
It runs a custom made instruction set that aims to be as fast and focused on the language's features
as possible, without overhead and with the best performance possible.

It currently has no support for JIT compilation, but it is planned for the future.

2. Compiler

The lib includes the official compiler for the language that will prepare byte code to run directly on the 
RazerVM, out of source files.

3. Utils

Additional API are available for inspecting the source trees, auto complete suggestions, documentation extraction,
loading and saving code from binary packages, exporting native code for usage in the language, and more.

The public API for using the language is under include/

Disclaimer: this is a work in progress, not all mentioned features are fully working or even implemented at all at this time. Feel free to stick around and discuss development of this language, as well as collaborate if you feel like it.