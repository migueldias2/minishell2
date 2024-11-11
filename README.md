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
