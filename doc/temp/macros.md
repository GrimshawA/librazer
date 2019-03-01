macros

keyword macro on global_scope inits the definition of a macro

macro assert_break (cond: std::expr, msg) {
    if (!cond) {
        spit(msg);
    }
}
