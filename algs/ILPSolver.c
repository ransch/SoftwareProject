#include "ILPSolver.h"
#include <stdlib.h>
#include "gurobi_c.h"
#include "../Strings.h"
#include "../utils/MemAlloc.h"

Bool ILPSolver(Puzzle *p) {
	GRBenv   *env = 0;
	GRBmodel *model = 0;
	unsigned int dim = p->n * p->m;
	unsigned int dim2 = dim * dim;
	unsigned int dim3 = dim2 * dim;
	int *ind;
	double *val;
	double *lb;
	char *vtype;
	double *sol;
	int optimstatus;
	unsigned int i, j, v, ig, jg, count;
	int error = 0;
	Bool ret = FALSE;
	memAllocN(ind, int, dim);
	memAllocN(val, double, dim);
	memAllocN(lb, double, dim3);
	memAllocN(vtype, char, dim3);
	memAllocN(sol, double, dim3);

	/* Init vtype, lb */
	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			for (v = 0; v < dim; v++) {
				if (getBoardValue(p, i, j) == v + 1) {
					lb[i*dim2 + j * dim + v] = 1;
				} else
					lb[i*dim2 + j * dim + v] = 0;
				vtype[i*dim2 + j * dim + v] = GRB_BINARY;
			}
		}
	}

	/* Create environment */
	error = GRBloadenv(&env, "sudoku.log");
	if (error) goto ERROR;

	/* Disable Gurobi prints */
	error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
	if (error) goto ERROR;

	/* Create new model */
	error = GRBnewmodel(env, &model, "sudoku", dim3, 0, lb, 0, vtype, 0);
	if (error) goto ERROR;

	/* Each cell gets a value */
	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			for (v = 0; v < dim; v++) {
				ind[v] = i * dim2 + j * dim + v;
				val[v] = 1.0;
			}

			error = GRBaddconstr(model, dim, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) goto ERROR;
		}
	}

	/* Each value must appear once in each row */
	for (v = 0; v < dim; v++) {
		for (j = 0; j < dim; j++) {
			for (i = 0; i < dim; i++) {
				ind[i] = i * dim2 + j * dim + v;
				val[i] = 1.0;
			}

			error = GRBaddconstr(model, dim, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) goto ERROR;
		}
	}

	/* Each value must appear once in each column */
	for (v = 0; v < dim; v++) {
		for (i = 0; i < dim; i++) {
			for (j = 0; j < dim; j++) {
				ind[j] = i * dim2 + j * dim + v;
				val[j] = 1.0;
			}

			error = GRBaddconstr(model, dim, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) goto ERROR;
		}
	}

	/* Each value must appear once in each subgrid */
	for (v = 0; v < dim; v++) {
		for (ig = 0; ig < p->m; ig++) {
			for (jg = 0; jg < p->n; jg++) {
				count = 0;
				for (i = ig * p->n; i < (ig + 1) * p->n; i++) {
					for (j = jg * p->m; j < (jg + 1) * p->m; j++) {
						ind[count] = i * dim2 + j * dim + v;
						val[count++] = 1.0;
					}
				}

				error = GRBaddconstr(model, dim, ind, val, GRB_EQUAL, 1.0, NULL);
				if (error) goto ERROR;
			}
		}
	}

	/* Optimize model */
	error = GRBoptimize(model);
	if (error) goto ERROR;

	/* Write model to 'sudoku.lp' */
	error = GRBwrite(model, "sudoku.lp");
	if (error) goto ERROR;

	/* Capture solution information */
	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error) goto ERROR;

	if (optimstatus == GRB_OPTIMAL) {
		error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, dim3, sol);
		if (error) goto ERROR;

		for (i = 0; i < dim; i++) {
			for (j = 0; j < dim; j++) {
				for (v = 0; v < dim; v++) {
					if (sol[i * dim2 + j * dim + v]) {
						setBoardValue(p, i, j, v + 1);
					}
				}
			}
		}

		ret = TRUE;
	} else if (optimstatus == GRB_INFEASIBLE || optimstatus == GRB_UNBOUNDED || optimstatus == GRB_INF_OR_UNBD) {
		ret = FALSE;
	} else {
		printf("%s\n", errMsgGurobiUnexpected);
	}
	goto QUIT;

ERROR:
	if (error) {
		printf(errMsgGurobi, GRBgeterrormsg(env));
		ret = FALSE;
	}

QUIT:
	GRBfreemodel(model);
	GRBfreeenv(env);
	memFree(ind);
	memFree(val);
	memFree(lb);
	memFree(vtype);
	memFree(sol);
	return ret;
}