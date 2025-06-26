import re
import random
from dataclasses import dataclass

@dataclass
class ReturnString:
    mulStr: str = ""
    sumsStr: str = ""

def organizeExpression(string: str) -> ReturnString:
    pattern = r"[+-]?\d+(?:[*/][+-]?\d+)+"
    multiplications: list[str] = re.findall(pattern, string)
    if multiplications.__len__() == 0:
        return ReturnString("", string)
    sumsStr = string
    for mul in multiplications:
        sumsStr = sumsStr.replace(mul, "")
    multiplicationsStr = ''.join(multiplications)
    startWithPlus = multiplicationsStr.startswith("+")
    if sumsStr == "":
        return ReturnString(multiplicationsStr[(startWithPlus if 1 else 0):], "")    
    if not (sumsStr.startswith("+") or sumsStr.startswith("-")):
        sumsStr = "+"+sumsStr
    return ReturnString(multiplicationsStr[(startWithPlus if 1 else 0):], sumsStr)    



def generateExpression(size: int) -> str:
    if size <= 0:
        return ""
    operators = ['+', '-', '*', '/']
    expression = str(random.randint(1, 999))
    for _ in range(1, size):
        expression += random.choice(operators)
        expression += str(random.randint(1, 999))
    return expression


if __name__ == "__main__":    
    for _ in range(1, 10):
        string = generateExpression(10)
        result = organizeExpression(string)
        evalResult: float = round(eval(result.mulStr+result.sumsStr), 1)
        evalString: float = round(eval(string), 1)
        resultEval = round(evalResult - evalString, 1)
        if (resultEval != 0):
            result = organizeExpression(string)
            print("Test string: ",  string)
            print("Result string: ",  result)
            print("Result operation teste: ", eval(string))
            print("Result operation result: ", eval(result.mulStr+result.sumsStr))
            print()
