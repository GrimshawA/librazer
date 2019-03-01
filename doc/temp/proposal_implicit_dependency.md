implicit dependencies
=====================

In order to seriously avoid singletons, without losing the convenience of their apparently simple usage, this proposal 
aims to solve this by adding the concept of implicit dependencies in a code entity.

Basically, in other languages, you would see:

function f() {
    FileSystem::GetCurrentDirectory();
}

where FileSystem is a static class (in that GetCurrentDirectory is static or acessible from a static context).

So, to counter this, the language still should allow for static methods and such usage, but will benefit its user if he choses not to
make said FileSystem static at all.

If the language detects this dependency to NOT be a singleton and still trying to be accessed statically "FileSystem.X", then
FileSystem will automatically become a member instance of the entity.

Essentially, with FileSystem.X being accessed, the class Y would have an hidden member FileSystem __fs and all global accesses
would be translated into __fs.X. The burden of passing over the ref to the FileSystem would be on whoever instantiates Y,
which should be helped with other features, including IOC utilities.

new Y() {
    FileSystem = myFileSystem
}