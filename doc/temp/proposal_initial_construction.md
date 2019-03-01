Constructing instances of code entities should be as declarative as possible and done within the scope of construction as much as possible.

Each code entity should be ideally as detached of the rest of the code as possible, and not only receive from the outside all dependencies it may need,
minimizing the need for mutable state as much as possible.

For this reason, all the language defaults tend towards this principle, making its user do additional syntactic work to mark things as mutable:

chat: IChatService 

For example, chat is a "instance member" of the type IChatService. By default IChatService is not nullable and it CANNOT contain a null, ever (unless the user of the language severely cheats).

So, usually, a constructed class such as:

x: class {
    x(IChatService c) {
        chat = c;
    }

    chat: IChatService
}

Would symbolize that:

1) x cannot be instanced without an IChatService dependency
2) IChatService is equivalent to C++ IChatService& which means that the dependency is NOT owned by x and is never null
3) chat is always safe to use from within x

Additionally, to prevent tampering with chat further down the lifetime of the entity, it is also default const.
This means that it is impossible to assign anything to chat, keeping it absolutely secure in any case of external or internal tampering.

In order to make it nullable, we would have to mark it with the type IChatService? both in constructor and instance.
And to mark it as mutable for further change, one would have to do:

chat: mut IChatService

In checked mode, the runtime itself will validate such assignments and ensure they are valid, in the same class of validation of bounds checking and so on.

Nevertheless, to make it a bit more flexible, while still secure to use, we could:

chat: IChatService => condition ? thisChat : thatChat;

So, chat curiously is not mutable but still can adopt multiple values? Yes, the reactive nature of these assignments means effectively that at
declaration, as long as all paths that assign to the variable are of equal safety, the binding remains valid. The mut keyword affects imperative assignment and not the declarative binding.