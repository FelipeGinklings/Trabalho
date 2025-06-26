import re
import random
from dataclasses import dataclass
from typing import Iterator


@dataclass
class MiniExpression:
    first: str = ""
    operation: str = ""
    second: str = ""
    hasExtra: bool = False
    operation2: str = ""
    third: str = ""


def organizeExpression(string: str) -> tuple[str, str]:
    pattern = r"[+-]?\d+(?:[*/][+-]?\d+)+"
    multiplications: list[str] = re.findall(pattern, string)
    if multiplications.__len__() == 0:
        return "", string
    sumsStr = string
    for mul in multiplications:
        sumsStr = sumsStr.replace(mul, "")
    multiplicationsStr = "".join(multiplications)
    startWithPlus = multiplicationsStr.startswith("+")
    if sumsStr == "":
        return multiplicationsStr[(startWithPlus if 1 else 0) :], ""
    if not (sumsStr.startswith("+") or sumsStr.startswith("-")):
        sumsStr = "+" + sumsStr
    return multiplicationsStr[(startWithPlus if 1 else 0) :], sumsStr


def matchToString(match: re.Match[str], string: str) -> str:
    return string[match.start() : match.end()]


def createMiniExpression(
    match: re.Match[str],
    numbers: Iterator[re.Match[str]],
    expression: str,
    miniExpressionsList: list[MiniExpression],
) -> (
    tuple[MiniExpression, re.Match[str]]
    | tuple[MiniExpression, None]
    | tuple[None, re.Match[str]]
    | tuple[None, None]
):
    first = matchToString(match, expression)
    if match.end() == expression.__len__():
        lastOperation = miniExpressionsList.pop()
        miniExpressionsList[-1].operation2 = lastOperation.operation
        miniExpressionsList[-1].third = first
        miniExpressionsList[-1].hasExtra = True
        return None, None
    operation = expression[match.end()]
    if operation in "+-":
        lastOperation = miniExpressionsList.pop()
        miniExpressionsList[-1].operation2 = lastOperation.operation
        miniExpressionsList[-1].third = first
        miniExpressionsList[-1].hasExtra = True
        return None, match
    newMatch = next(numbers)
    second = matchToString(newMatch, expression)
    return MiniExpression(first, operation, second), newMatch


def createMiniExpressions(mulStr: str, sumsStr: str):
    miniExpressionsList: list[MiniExpression] = []
    numbers = re.finditer(r"-?\d+", mulStr)
    if mulStr.__len__():
        for match in numbers:
            miniExpression, lastMatch = createMiniExpression(
                match, numbers, mulStr, miniExpressionsList
            )
            if miniExpression == None and lastMatch:
                if mulStr[lastMatch.end()] != "-":
                    miniExpressionsList.append(
                        MiniExpression("", mulStr[lastMatch.end()], "")
                    )
            elif miniExpression and lastMatch == None:
                miniExpressionsList.append(miniExpression)
                break
            elif miniExpression and lastMatch:
                miniExpressionsList.append(miniExpression)
                if lastMatch.end() != mulStr.__len__():
                    if mulStr[lastMatch.end()] != "-":
                        miniExpressionsList.append(
                            MiniExpression("", mulStr[lastMatch.end()], "")
                        )
    result = ""
    for miniExpression in miniExpressionsList:
        result += (
            miniExpression.first + miniExpression.operation + miniExpression.second
        )
        if miniExpression.hasExtra:
            result += miniExpression.operation2 + miniExpression.third

    assert mulStr == result, f"Esperado: {mulStr}, mas obteve: {result}"


def generateExpression(size: int) -> str:
    if size <= 0:
        return ""
    operators = ["+", "-", "*", "/"]
    expression = str(random.randint(1, 999))
    for _ in range(1, size):
        expression += random.choice(operators)
        expression += str(random.randint(1, 999))
    return expression


if __name__ == "__main__":
    for size in range(2, 10):
        for _ in range(1, 100):
            string = generateExpression(size)
            # string = "461*788/354"
            mulStr, sumsStr = organizeExpression(string)
            evalResult: float = round(eval(mulStr + sumsStr), 1)
            evalString: float = round(eval(string), 1)
            resultEval = round(evalResult - evalString, 1)
            assert resultEval == 0, f"Esperado: 0, mas obteve: {resultEval}"
            createMiniExpressions(mulStr, sumsStr)
