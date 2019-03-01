Proposal for indexed lifetime objects
=====================================

The motivation is to explore possibilities when we want a given behavioral class to be always consistent in terms of state, when we know
its stateful dependencies will not necessarily live forever.

The idea is that for example:

Star: class {

}

StarDust : class {
    star: Star

    // star is ALWAYS a valid star, which means that the entire StarDust object must die if there is no longer a star
}

type: class 
{
    some_star: Star?
    
    dust: StarDust? (some_star) {

    }
    
}