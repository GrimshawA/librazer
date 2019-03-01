Allocation interception is a technique to defer creation and destruction of any object to a third party.

Usually, when requesting to create an object X, it will often contain sub objects within itself.

For example:

X: class
{
    sub: Y {

    }
}

The language rules mandate that 'sub' is tied exactly to the lifetime of X, however, while Y lifetime is already managed explicitly,
its allocation and deallocation do not need to be a default "new". For this reason, we use interceptors of indirect allocations.


function alloc_y() {
   return ypool.alloc(); 
}

function release_y() {
    ypool.release();
}

// Install interception on X for objects of type Y (highly provisional API)
std::alloc_intercept<Y>(X, alloc_y, release_y);

new X();

It would be ideal to apply pattern matching and provide templated alloc for an entire set of objects / traits / hierarchies