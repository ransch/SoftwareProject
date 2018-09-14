#include "Shared.h"

/**
 * This variable bundles the variables that should be shared by
 * the different components of the program during its lifetime.
 */
extern SharedBundle bundle;

void initBundle() {
	bundle.puzzle = 0;
	bundle.activity = createActivity();
}

void destroyBundle() {
	destroyActivity(bundle.activity);

	if (bundle.puzzle != 0) {
		destroyPuzzle(bundle.puzzle);
	}
}