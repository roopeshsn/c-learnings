# C Learnings

My learnings in the C programming language

## Problems with C

### C has too weak type safety
```
for (int i = 0; n; ++i) {
   printf("Hi C! \n");
}
```
C cannot even catch simple mistakes such as this where you used a non-boolean condition. It was supposed to say `i < n`. 

### Memory Allocation and Freeing
In C, It kind of manual allocation. Whenever you receive a pointer to some data, you never know if it is your problem to de-allocate it or not.

## Reference
1. [What is Wrong With C?](https://medium.com/codex/what-is-wrong-with-c-fca25affb127)
2. [Do We Need a Replacement for C?](https://erik-engheim.medium.com/do-we-need-a-replacement-for-c-3256a8b44814)
