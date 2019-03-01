
[yields] void load(asset* a)
{
    auto result = yield RHI->LoadTexture(a->texture);
    
    if (!result) {
        // yada yada
    }
    else {
        // all good
    }
}

[yields] void asset_load_loop()
{
    // This for would be equivalent to IEnumerable yield in C#
    for (auto& a : assets)
    {
        load(a); // if one load is yielded, the next one continues to execute
    }

    // At this point, all coroutines should have converged, return
}