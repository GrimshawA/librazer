Domain modelling
==================

This proposal aims to target three fronts:
    1. Domain modelling
    2. Domain data flow
    3. Repository uniformization

Motivation:

All applications have a data domain they have to deal with. In essence, all business applications will have certain "concepts" that get mapped in
software, so that the software, either triggered by time or external triggers(network, user input) can mutate the state of the domain in some way and eventually reflect those changes into certain output channels( uI, terminal, logs, network..).

Usually, programmers model the code in an ad-hoc way based on each case and end up repeating a lot of the solution in order to apply it to each domain type, and also duplicate a lot of effort in order to replicate this domain modelling along multiple layers (database, ui, etc).

This proposal offers to address this by providing an unified way to interact with the software domain based on "cascading subrepositories",
which is a way of referencing data that references data, with optional transformations and extra semantics along the way, so that each consumer or mutator has the exact information he needs and is able to follow a proper typed channel when commiting the change back to the repository.


Modelling an entity:

User: domain_object {

}

Account: domain_object {
    owner: User
    id: Number
}

Since these entities are not traditional objects, the domain_object trait is an extremely loose definition of where the data will live and the technical type it will have. Therefore, there is an immense potential for optimization based on how the domain objects are being used in code. We call these concepts "Dematerialized" because they have well defined semantics in an abstract realm, but in the final computation, there are multiple ways to materialize their correctness, effectively detaching the technical achievement from its semantics.

The idea is that a module can define the domain globally to its other entities, for example:

domain: domain_object {
    accounts: Account[]
}

So now every entity can reference "domain" and interact with it:

test: class {
    myId: 101
    myAccount: domain.accounts where id == myId 
}

Here, myAccount will remain a valid account, obtained by filtering the domain accounts, which we can read and write to.
By not concretizing myAccount to be of a specific type, it remains dematerialized: The compiler will have to ensure all its dependant operations remain valid but provides no guarantees on how. myAccount may not even have storage within test and be always computed when necessary, as well as it may have any internal type the compiler sees fit.

The dematerialization gives a lot of wiggle room to the compiler to select the more efficient solution to the problem, as for example, when tracking a list of something, it could done in at least these ways:
    1. Acessing the source data every time, no local duplication or refs
    2. Ref to a central container
    3. Duplicated local container with automatic data commits and updates
    4. List of references of any given type (vector or linked list, etc)


Sync
====

In order to maintain integrity, a transactional model is provided when writing changes to the domain. Also, all the changes propagate automatically through the domain graph so that all consumers have the latest data and are notified of such changes.

Serialization
=============

Of course, this data will mostly not live only in memory, and not necessarily all at the same time. Therefore, the system has ways to serialize explicitly and implicitly the data to other means, in its totality or not.

This is also a main vector of advantages for the proposal. For example, let's say we have SQLite storage of all our domain:
We could simply define an utility for persistence, like SQLiteStorage which would handle a domain or subdomain.

We could instantiate a new SQLiteStorage(domain, ...options...) and have FULL serialization immediately, without additional hassle and without introducing coupling into the existing architecture (no other entity needs to become aware of this persistence and propagate actions to it).

By observing the domain, our storage could just listen for changes on everything and push it to the open database immediately or in a cached fashion.
Additionally, we would have an automatic mapping between database schema and the domain tree, as each domain_object and its relations are naturally mapped to any relational database. The same info used for code optimization could specify indexing in the database and also the types and names are immediately available.

This way, there would be a natural schema for our application that is specified only once and accessed everywhere with the same conventions.

domain_object_view
===================

Nevertheless, it is possible we want to interpret information slightly differently, like having a database field be a specific integer type instead of it being picked by the utility.

For this, we have object views, which are basically an alias type that looks to the same underlying entity but maps the source fields into destination fields, either by just changing types or even doing other arithmetic operations etc.

For example, the SQLiteStorage could choose to observe Account via a DatabaseAccountView which would be the same in every way but have an additional field computed from the others (so that we see it in the database but not anywhere else.)

DatabaseAccountView: domain_object_view(Account)
{
    import Account.*;

    maskedBalance: formatted(balance)
}

Globality
===========
Of course, accessing the domain globally allows for the highest decoupling in architecture, but it is also possible to pass down nested entities subdomains of subdomains which reduce coupling to a global variable "domain". It also possible to pass parent domains around instead of ever having a global one (at each programmer's discretion)