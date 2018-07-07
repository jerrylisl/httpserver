import sys
class Solution:
    def calculate(self, s):
        """
        :type s: str
        :rtype: int
        """
        import re
        if not s: return 0
        tokens = list(filter(lambda c: c, re.split(r'(\(|\)|\+|-|\d+|\s+)', s)))
        sign = 1
        # stack of list (value, sign -1 or 1)
        stack = [[0, 1]]
        for token in tokens:
            # Notice we need to reset the sign variable to 1 when we enter and exit ()
            if token == '(':
                stack.append([0, sign])
                sign = 1
            elif token == ')':
                num, prev_sign = stack.pop()
                stack[-1][0] += num * prev_sign
                sign = 1
            elif token == '+':
                sign = 1
            elif token == '-':
                sign = -1
            elif token.isdigit():
                stack[-1][0] += int(token) * sign    
        return sum(val for val, _ in stack)  



c = Solution()
formula = sys.argv[1][sys.argv[1].find('=')+1:]
print c.calculate(formula)


