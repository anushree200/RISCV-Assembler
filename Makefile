$(CC) = gcc
final:
  $(CC) final.c instr_R.c instr_IS.c instr_SRAI_IS.c instr_B.c instr_U.c instr_J.c -o out
Clean:
  rm *.o out
