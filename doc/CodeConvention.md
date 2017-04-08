Code conventions for writing Razer code
=======================================

Braces

Despite not enforced or mandatory, it is strongly encouraged the braces follow the pattern of opening brace in the same line as the declaration.
A function declaration should be in the form:
func f() {
	
}

Optional Typing

Even though the language provides optional typing, being a hybrid typing language, please understand the dynamic typing was created with making easy to implement certain features in mind, not as a tool to use for everything. Whenever it is possible to program using static typing, please use it. Leave the dynamic typing only for situations where it doesn't harm readability, performance, debuggability or any other metric. Keep in mind static typed code will always be easier to maintain,
even if it seems like it takes a bit more code to write.
