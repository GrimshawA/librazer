Allocations are calculated in the following way:

1) Infer from reality model the technical types each thing should have. By lack of information everything becomes the default, like int32_t for integers and so on. Otherwise, more specialized technical types are picked up for next phases.

2)
For each entity or unit with data, we will run an analysis to see if there is a benefit to split each property from the rest of the entity, based on usage stats cost for the decision. Doing separate allocation may not be available for all concrete types!!

3) Now let's find all available storage options we have, or if we might want to even split into different storage options.

4) Add all possible storage options into the graph and generate a cost for each based on traversing logical units and asserting when and how the data is touched.

An amortized cost should be C = A * R * M
where C is our amortized cost for a given operation, M being a factor that comes from the machine model (in x86 something could cost double as much as in ARM).
R is the requirements knowledge we have, for example if we can infer that some data will usually exist in the thousands, we can average it

A is the analytical cost for the operation.

Therefore, if we can see in the logical model that a given entity is read and never written to, always in the same property, from the technical data type, we can have an analytical cost for the operation, for example:

data type is int -> cost 2
data type is big_int -> cost 5

M might mean that int reads are super fast on x86 and weight 0.9x and big_int can't be fast there for some reason and upscale it to 1.5x, yielding:

data type is int -> cost 1.7 approx
data type is big_int -> cost 7.5 approx

In the absence of additional information we couldn't usually know how many of these entities would exist so A would be 1. Otherwise if we know something like we will be reading usually from 1000 entities, our A could be effectively 1000:

Assuming int is our choice we could calculate:

C = 1.7  * 1000

Which could give us an approximation of how generically, the costs of reads to that property will be, so now we can combine that information with different containers, like:

Contiguous: 2x
Sparse: 0.5x

Meaning that we get a node for each of the ones above, each leading to the same amortized cost of read operations.

Because of this it would become fairly obvious the Contiguous is better for us here.

Of course we could for example complicate with allocation statistics:

IF we knew the cost of writes in general for the entity, we could add that info to the tree as well:

Contiguous provides a 0.6x for writes, exponentially 
Sparse provides a 1.0x for writes

So if for each choice, contiguous or sparse, we add in each our writes/reads costs in each, we could have a more informed decision:

The more writes we would have the more the Contiguous approach would become unsuitable opening way for the Sparse solution.

Each leaf in the graph combines (multiplies) with its parent until the root to form the branch cost, but sibling branches are summed instead of multiplied.