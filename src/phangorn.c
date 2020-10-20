/*
 * phangorn.c
 *
 * (c) 2008-2020  Klaus Schliep (klaus.schliep@gmail.com)
 *
 *
 * This code may be distributed under the GNU GPL
 *
 */

# define USE_RINTERNALS

#include <Rmath.h>
#include <R.h>
#include <Rinternals.h>


/*
void countCycle(int *M, int *l, int *m, int *res){
    int j, i, tmp;
    res[0]=0L;
    for (i=0; i<*l; i++) {
        tmp = 0;
        if(M[i] != M[i + (*m -1) * *l])tmp++;
        for (j=1; j<*m; j++) {
            if(M[i + (j-1)* *l] != M[i + j * *l])tmp++;
        }
        if(tmp>2L)res[0]+=tmp;
    }
}


void countCycle2(int *M, int *l, int *m, int *res){
    int j, i, tmp;
    for (i=0; i<*l; i++) {
        tmp = 0L;
        if(M[i] != M[i + (*m -1) * *l])tmp=1L;
        for (j=1; j<*m; j++) {
            if(M[i + (j-1L)* *l] != M[i + j * *l])tmp++;
        }
        res[i]=tmp;
    }
}
*/

// C++
SEXP rowMax(SEXP sdat, SEXP sn, SEXP sk){
    int i, h, n=INTEGER(sn)[0], k=INTEGER(sk)[0];
    double x, *res, *dat;
    SEXP result;
    PROTECT(result = allocVector(REALSXP, n));
    res = REAL(result);
    PROTECT(sdat = coerceVector(sdat, REALSXP));
    dat = REAL(sdat);
    for(i = 0; i < n; i++){
        x = dat[i];
        for(h = 1; h< k; h++) {if(dat[i + h*n] > x) x=dat[i + h*n];}
        res[i] = x;
        }
    UNPROTECT(2);
    return(result);
}


void getdP(double *eva, double *ev, double *evi, int m, double el, double w, double *result){
    int i, j, h;
    double  res; //tmp[m],
    double *tmp;
    tmp = (double *) malloc(m * sizeof(double));
    for(i = 0; i < m; i++) tmp[i] = (eva[i] * w  * el) * exp(eva[i] * w * el);
    for(i = 0; i < m; i++){
        for(j = 0; j < m; j++){
            res = 0.0;
            for(h = 0; h < m; h++)    res += ev[i + h*m] * tmp[h] * evi[h + j*m];
            result[i+j*m] = res;
        }
    }
    free(tmp);
}


void getdP2(double *eva, double *ev, double *evi, int m, double el, double w, double *result){
    int i, j, h;
    double  res; //tmp[m],
    double *tmp;
    tmp = (double *) malloc(m * sizeof(double));
    for(i = 0; i < m; i++) tmp[i] = (eva[i] * w) * exp(eva[i] * w * el);
    for(i = 0; i < m; i++){
        for(j = 0; j < m; j++){
            res = 0.0;
            for(h = 0; h < m; h++)    res += ev[i + h*m] * tmp[h] * evi[h + j*m];
            result[i+j*m] = res;
        }
    }
    free(tmp);
}


void getd2P(double *eva, double *ev, double *evi, int m, double el, double w, double *result){
    int i, j, h;
    double res; //tmp[m],
    double *tmp;
    tmp = (double *) malloc(m * sizeof(double));
    for(i = 0; i < m; i++) tmp[i] = (eva[i] * w * el) * (eva[i] * w * el) * exp(eva[i] * w * el);
    for(i = 0; i < m; i++){
        for(j = 0; j < m; j++){
            res = 0.0;
            for(h = 0; h < m; h++)    res += ev[i + h*m] * tmp[h] * evi[h + j*m];
            result[i+j*m] = res;
        }
    }
    free(tmp);
}


void getd2P2(double *eva, double *ev, double *evi, int m, double el, double w, double *result){
    int i, j, h;
    double  res; //tmp[m],
    double *tmp;
    tmp = (double *) malloc(m * sizeof(double));
    for(i = 0; i < m; i++) tmp[i] = (eva[i] * w) * (eva[i] * w) * exp(eva[i] * w * el);
    for(i = 0; i < m; i++){
        for(j = 0; j < m; j++){
            res = 0.0;
            for(h = 0; h < m; h++)    res += ev[i + h*m] * tmp[h] * evi[h + j*m];
            result[i+j*m] = res;
        }
    }
    free(tmp);
}


SEXP getdPM(SEXP eig, SEXP nc, SEXP el, SEXP w){
    R_len_t i, j, nel, nw;
    int m=INTEGER(nc)[0], l=0;
    double *ws=REAL(w);
    double *edgelen=REAL(el);
    double *eva, *eve, *evei;
    SEXP P, RESULT;
    nel = length(el);
    nw = length(w);
    eva = REAL(VECTOR_ELT(eig, 0));
    eve = REAL(VECTOR_ELT(eig, 1));
    evei = REAL(VECTOR_ELT(eig, 2));
    PROTECT(RESULT = allocVector(VECSXP, nel*nw));
    double *p;
    if(!isNewList(eig)) error("'dlist' must be a list");
    for(j=0; j<nel; j++){
        for(i=0; i<nw; i++){
            PROTECT(P = allocMatrix(REALSXP, m, m));
            p = REAL(P);
            getdP(eva, eve, evei, m, edgelen[j], ws[i], p);
            SET_VECTOR_ELT(RESULT, l, P);
            UNPROTECT(1); //P
            l++;
        }
    }
    UNPROTECT(1);//RESULT
    return(RESULT);
}


SEXP getdPM2(SEXP eig, SEXP nc, SEXP el, SEXP w){
    R_len_t i, j, nel, nw;
    int m=INTEGER(nc)[0], l=0;
    double *ws=REAL(w);
    double *edgelen=REAL(el);
    double *eva, *eve, *evei;
    SEXP P, RESULT;
    nel = length(el);
    nw = length(w);
    eva = REAL(VECTOR_ELT(eig, 0));
    eve = REAL(VECTOR_ELT(eig, 1));
    evei = REAL(VECTOR_ELT(eig, 2));
    PROTECT(RESULT = allocVector(VECSXP, nel*nw));
    double *p;
    if(!isNewList(eig)) error("'dlist' must be a list");
    for(j=0; j<nel; j++){
        for(i=0; i<nw; i++){
            PROTECT(P = allocMatrix(REALSXP, m, m));
            p = REAL(P);
            getdP2(eva, eve, evei, m, edgelen[j], ws[i], p);
            SET_VECTOR_ELT(RESULT, l, P);
            UNPROTECT(1); //P
            l++;
        }
    }
    UNPROTECT(1); //RESULT
    return(RESULT);
}


SEXP getd2PM(SEXP eig, SEXP nc, SEXP el, SEXP w){
    R_len_t i, j, nel, nw;
    int m=INTEGER(nc)[0], l=0;
    double *ws=REAL(w);
    double *edgelen=REAL(el);
    double *eva, *eve, *evei;
    SEXP P, RESULT;
    nel = length(el);
    nw = length(w);
    eva = REAL(VECTOR_ELT(eig, 0));
    eve = REAL(VECTOR_ELT(eig, 1));
    evei = REAL(VECTOR_ELT(eig, 2));
    PROTECT(RESULT = allocVector(VECSXP, nel*nw));
    double *p;
    if(!isNewList(eig)) error("'dlist' must be a list");
    for(j=0; j<nel; j++){
        for(i=0; i<nw; i++){
            PROTECT(P = allocMatrix(REALSXP, m, m));
            p = REAL(P);
            getd2P(eva, eve, evei, m, edgelen[j], ws[i], p);
            SET_VECTOR_ELT(RESULT, l, P);
            UNPROTECT(1); //P
            l++;
        }
    }
    UNPROTECT(1); //RESULT
    return(RESULT);
}


SEXP getd2PM2(SEXP eig, SEXP nc, SEXP el, SEXP w){
    R_len_t i, j, nel, nw;
    int m=INTEGER(nc)[0], l=0;
    double *ws=REAL(w);
    double *edgelen=REAL(el);
    double *eva, *eve, *evei;
    SEXP P, RESULT;
    nel = length(el);
    nw = length(w);
    eva = REAL(VECTOR_ELT(eig, 0));
    eve = REAL(VECTOR_ELT(eig, 1));
    evei = REAL(VECTOR_ELT(eig, 2));
    PROTECT(RESULT = allocVector(VECSXP, nel*nw));
    double *p;
    if(!isNewList(eig)) error("'dlist' must be a list");
    for(j=0; j<nel; j++){
        for(i=0; i<nw; i++){
            PROTECT(P = allocMatrix(REALSXP, m, m));
            p = REAL(P);
            getd2P2(eva, eve, evei, m, edgelen[j], ws[i], p);
            SET_VECTOR_ELT(RESULT, l, P);
            UNPROTECT(1); //P
            l++;
        }
    }
    UNPROTECT(1); //RESULT
    return(RESULT);
}


/*
void tabulate(int *x, int *n, int *nbin, int *ans){
    int i, tmp;
    for (i=0; i < *nbin; i++) ans[i]=0L;
    for (i=0; i < *n; i++) {
        tmp = x[i];
        if( (tmp>0) & (tmp<(*nbin+1L)) )
        ans[tmp-1L] ++;
    }
}
*/


