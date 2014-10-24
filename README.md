Pancake
=======

Design Problem #2

You can complete this assignment by yourself or with a partner. Be sure to consult the relevant guidelines for design projects on the main class webpage. Make sure you understand what you are expected to turn in before you begin.

The Problem

You are to design and implement an algorithm to solve the pancake problem, described as follows. Bob is a waiter at a diner that is very popular with the breakfast crowd. Pancakes are by far the most popular item ordered, and customers can order a stack with any desired number of pancakes. The somewhat quirky cook makes tasty pancakes, but no two pancakes in an order are ever exactly the same size. When the pancakes are done, the cook places them in a stack on the customers plate in a completely arbitrary order, so sometimes large pancakes are atop smaller ones. Bob likes to serve a nice-looking stack to the customers, ordered from largest on the bottom to smallest on the top, so he quickly reorders the pancakes on the way out of the kitchen. He doesn't want to touch the pancakes, so the only operation he can perform is to insert a pancake turner at an arbitrary point (between pancakes) in the stack and flip all the pancakes above that point. Naturally, Bob wants to order the pancakes with the smallest number of flips, and we want to explore how the number of flips required varies as the size of the stack increases.

For this assignment, assume each pancake in a stack has a unique diameter, allowing us to identify each pancake with a unique integer ranging from 1 to n, where n is the number of pancakes in the stack. We'll further assume that 1 refers to the smallest pancake, while pancake n refers to the largest. Pancakes can be reordered only by flipping a sub-stack on top. A flip is specified by specifying the index position of the bottom of the substack to be flipped. (Thus, the operation flip(1) flips the entire stack.)

Stacks will be specified on an input line, with the left-to-right order corresponding to the stack from top to bottom. For example, consider the stack below.
1 2 5 4 3
After flip(3), this stack would be transformed to
5 2 1 4 3

Input

The input will consist of a sequence of stacks of pancakes, one stack per line. Each stack will consist of n pancakes (1 ≤ n ≤ 30), so each input line will be a permutation of the numbers from 1 through n. (Bonus points if your code can find solutions for stacks taller than 30.) The input will be terminated by end-of-file. The line of input will list the pancakes from top to bottom, and all adjacent pancakes will be separated by a space.

Output

For each stack of pancakes, your program should echo the original stack on one line, followed by a sequence of flips (the argument to the flip() function at each point, as it were) that results in the stack ending up sorted, with the smallest pancake on the top. The sequence of flips for each stack should be terminated by a 0, indicating no more flips necessary, and that should be followed by the total number of flips required for your solution in parentheses. Ideally, your code should report the solution with the smallest number of flips.

Sample Input

1 2 3 4
5 4 3 2 1
4 5 6 3 2 1
<EOF>

Sample Output

1 2 3 4
0 (0)
5 4 3 2 1
1 0 (1)
4 5 6 3 2 1
4 1 0 (2)

Test File

Your report should include results using the various stacks in this test file. It includes, in order, stacks of size 5, 10, 15, 20, 25, 30, 40, 50, 60, 70, 80, 90, 100.

Compile
==========
cd Pancake
gcc pancake.c entropy.c bfs.c -o pancake -lm

Use
==========
./pancake -f stacks/manystacks.txt
