/* This work allows operator lookup to be in constant time.
   As the operator list grows, we may have to change the 'hash_modulus'
   variable and recalculate the operator array lookup indices. */

// basically, djb2 hashing algorithm, reduced by a modulus that accomodates
// any functions we need. This is probably going to be deprecated, as far as 
// function lookup is concerned.
/*static MYINT get_hash(unsigned char *in_str)
{
  unsigned MYINT hash = 5381;
  unsigned MYINT hash_modulus = 367;
  unsigned MYINT c;
  while ((c = *in_str++)) {
    hash = ((hash << 5) + hash) + c;
  }
  return hash % hash_modulus;
}*/

/* we need to populate the dictionary first with primitives */
struct primitive {
  const char *name;
  void (*function) (void);
};

static const struct primitive primitives[] = {
  {"+", addfunc},
  {"-", subfunc},
  {"*", mulfunc},
  {"/", divfunc},
  {"%", modfunc},
  {"abs", absfunc},
  {"min", minfunc},
  {"max", maxfunc},
  {"<<", lshiftfunc},
  {">>", rshiftfunc},
  /* comparisons */
  {"=", eqfunc},
  {"<>", noteqfunc},
  {"<", ltfunc},
  {">", gtfunc},
  {"<=", ltefunc},
  {">=", gtefunc},
  /* logic */
  {"and", andfunc},
  {"or", orfunc},
  {"not", notfunc},
  {"xor", xorfunc},
  /* fp -> int */
  {"round", roundfunc},
  {"ceil", ceilfunc},
  {"floor", floorfunc},
  /* higher math */
  {"pow", powerfunc},
  {"sqrt", sqrtfunc},
  {"log", logfunc},
  {"log2", log2func},
  {"log10", log10func},
  /* constants */
  {"pi", pifunc},
  {"e", efunc},
  /* trig */
  {"sin", sinefunc},
  {"cos", cosfunc},
  {"tan", tanfunc},
  /*randomness */
  {"rand", randfunc},
  /* stack operators */
  {"drop", dropfunc},
  {"dup", dupfunc},
  {"swap", swapfunc},
  {"over", overfunc},
  {"rot", rotfunc},
  {"-rot", rotnegfunc},
  {"nip", nipfunc},
  {"tuck", tuckfunc},
  {"2drop", drop2func},
  {"2dup", dup2func},
  {"2swap", swap2func},
  {"2over", over2func},
  {"2rot", rot2func},
  /* variables in memory */
  {"!", pokefunc},
  {"@", peekfunc},
  /* branching */
  {"do", dofunc},
  {"redo", redofunc},
  {"exitdo", exitdofunc},
  {"qdo", qdofunc},
  {"qredo", qredofunc},
  {"for", forfunc},
  {"next", nextfunc},
  {"exitfor", exitforfunc}, 
  {"skip", skipfunc},
  {"i", ifunc},
  {"j", jfunc},
  /* output */
  {".", showfunc},
  {".s", showstackfunc},
  {".rj", showrjfunc},
  {"cr", crfunc},
  {"s\"", stringfunc},
  {"print", printfunc},
  {"emit", emitfunc},
  {"uemit", uemitfunc},
  /* time */
  {"clock", clockfunc},
  {"rdtsc", rdtscfunc},
  {0, 0}
};
