1) minishell> echo >
Syntax error: missing file for redirection: Success
zsh: segmentation fault (core dumped)  ./a.out

2) by 0x40343E: slice_tokens (slice_tokens.c:51)
==93981==    by 0x40194C: parse_tokens (create_ast.c:194)
==93981==    by 0x403137: main (main.c:86)
==93981== 
==93981== LEAK SUMMARY:
==93981==    definitely lost: 40 bytes in 1 blocks
==93981==    indirectly lost: 10 bytes in 2 blocks


3) echo >>outfile >>outfile >>outfile should only add one time


4) <<ola
ola
==105423== Invalid read of size 1
==105423==    at 0x484ED16: strlen (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==105423==    by 0x402C4D: ft_strjoin (ft_strjoin.c:24)
==105423==    by 0x402EB1: get_path (get_path.c:55)
==105423==    by 0x401C58: exec (execute_ast.c:10)
==105423==    by 0x40208A: execute_ast (execute_ast.c:143)
==105423==    by 0x40329F: main (main.c:89)
==105423==  Address 0x0 is not stack'd, malloc'd or (recently) free'd
==105423== 
==105423== 



./a.out
==118409== Memcheck, a memory error detector
==118409== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==118409== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==118409== Command: ./a.out
==118409== 
minishell> 
minishell> 
minishell> 
minishell> 
minishell> 
minishell> 
minishell> 
minishell> 
exit
==118409== 
==118409== HEAP SUMMARY:
==118409==     in use at exit: 204,405 bytes in 222 blocks
==118409==   total heap usage: 470 allocs, 248 frees, 241,754 bytes allocated
==118409== 
==118409== LEAK SUMMARY:
==118409==    definitely lost: 240 bytes in 1 blocks
