
function main() {
	// This is not the recommended way to allocate anything
	// but new still exists, as:

	new X();

	new X {
		depdency1: val
		depdency2: val2

		a: b + 20 // injected binding!!
	}
}